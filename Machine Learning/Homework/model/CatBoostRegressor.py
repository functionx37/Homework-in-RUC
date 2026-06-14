from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import numpy as np
import pandas as pd
from catboost import CatBoostRegressor


ROOT_DIR = Path(__file__).resolve().parents[1]
TRAIN_PATH = ROOT_DIR / "train" / "train.csv"
TEST_PATH = ROOT_DIR / "test" / "test.csv"
STATS_PATH = ROOT_DIR / "train" / "column_min_max_vals.csv"
SAMPLE_SUBMISSION_PATH = ROOT_DIR / "submission" / "sample_submission.csv"
SUBMISSION_PATH = ROOT_DIR / "submission" / "submission.csv"


@dataclass
class ColumnStat:
    min_value: float
    max_value: float
    row_count: float
    num_unique_values: float


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Train CatBoostRegressor and directly generate submission.csv."
    )
    parser.add_argument("--random-state", type=int, default=42)
    parser.add_argument("--iterations", type=int, default=2000)
    parser.add_argument("--learning-rate", type=float, default=0.05)
    parser.add_argument("--depth", type=int, default=8)
    parser.add_argument("--l2-leaf-reg", type=float, default=5.0)
    return parser.parse_args()


def split_csv_field(value: Any) -> list[str]:
    if value is None or (isinstance(value, float) and math.isnan(value)):
        return []
    text = str(value).strip()
    if not text:
        return []
    return [item.strip() for item in text.split(",") if item.strip()]


def parse_predicates(value: Any) -> list[tuple[str, str, str]]:
    tokens = split_csv_field(value)
    predicates: list[tuple[str, str, str]] = []
    for index in range(0, len(tokens), 3):
        chunk = tokens[index : index + 3]
        if len(chunk) == 3:
            predicates.append((chunk[0], chunk[1], chunk[2]))
    return predicates


def load_column_stats(stats_path: Path) -> dict[str, ColumnStat]:
    stats_df = pd.read_csv(stats_path)
    stats: dict[str, ColumnStat] = {}
    for row in stats_df.to_dict("records"):
        stats[str(row["name"])] = ColumnStat(
            min_value=float(row["min"]),
            max_value=float(row["max"]),
            row_count=float(row["cardinality"]),
            num_unique_values=max(float(row["num_unique_values"]), 1.0),
        )
    return stats


def alias_to_row_count(alias: str, stats: dict[str, ColumnStat]) -> float:
    key_map = {
        "t": "t.id",
        "mc": "mc.id",
        "ci": "ci.id",
        "mi": "mi.id",
        "mi_idx": "mi_idx.id",
        "mk": "mk.id",
    }
    key = key_map.get(alias)
    if key is None or key not in stats:
        return 0.0
    return stats[key].row_count


def to_numeric(value: str) -> float | None:
    try:
        return float(value)
    except ValueError:
        return None


def estimate_selectivity(
    column_name: str, operator: str, raw_value: str, stats: dict[str, ColumnStat]
) -> tuple[float, float]:
    column_stat = stats.get(column_name)
    numeric_value = to_numeric(raw_value)
    if column_stat is None or numeric_value is None:
        return 0.5, 0.5

    if column_stat.max_value > column_stat.min_value:
        normalized = (numeric_value - column_stat.min_value) / (
            column_stat.max_value - column_stat.min_value
        )
    else:
        normalized = 0.5
    normalized = float(np.clip(normalized, 0.0, 1.0))

    if operator == "=":
        selectivity = min(1.0 / column_stat.num_unique_values, 1.0)
    elif operator == "<":
        selectivity = normalized
    elif operator == ">":
        selectivity = 1.0 - normalized
    else:
        selectivity = 0.5

    return float(np.clip(selectivity, 1e-9, 1.0)), normalized


def extract_features(
    row: pd.Series, stats: dict[str, ColumnStat]
) -> dict[str, float | str]:
    features: dict[str, float | str] = {}
    tables = split_csv_field(row["Tables"])
    joins = split_csv_field(row["Join Conditions"])
    predicates = parse_predicates(row["Predicates"])

    features["table_count"] = len(tables)
    features["join_count"] = len(joins)
    features["predicate_count"] = len(predicates)
    features["table_signature"] = "|".join(sorted(tables)) or "NONE"
    features["join_signature"] = "|".join(sorted(joins)) or "NONE"
    features["query_signature"] = (
        f"{features['table_signature']}__joins={len(joins)}__preds={len(predicates)}"
    )

    base_table_rows: list[float] = []
    alias_selectivities: dict[str, list[float]] = {}
    predicate_columns: list[str] = []
    predicate_ops: list[str] = []
    selectivities: list[float] = []
    normalized_values: list[float] = []

    for table_ref in tables:
        alias = table_ref.split()[-1]
        row_count = alias_to_row_count(alias, stats)
        base_table_rows.append(row_count)
        features[f"table_present={table_ref}"] = 1.0
        features[f"table_rows={table_ref}"] = row_count

    for join in joins:
        features[f"join_present={join}"] = 1.0

    for column_name, operator, raw_value in predicates:
        alias = column_name.split(".", 1)[0]
        predicate_columns.append(column_name)
        predicate_ops.append(f"{column_name}{operator}")

        selectivity, normalized = estimate_selectivity(
            column_name=column_name,
            operator=operator,
            raw_value=raw_value,
            stats=stats,
        )
        numeric_value = to_numeric(raw_value)

        alias_selectivities.setdefault(alias, []).append(selectivity)
        selectivities.append(selectivity)
        normalized_values.append(normalized)

        features[f"predicate_present={column_name}{operator}"] = 1.0
        features[f"column_present={column_name}"] = 1.0
        features[f"selectivity={column_name}{operator}"] = selectivity
        features[f"normalized_value={column_name}{operator}"] = normalized
        if numeric_value is not None:
            features[f"raw_value={column_name}{operator}"] = numeric_value

    features["predicate_signature"] = "|".join(sorted(predicate_columns)) or "NONE"
    features["predicate_op_signature"] = "|".join(sorted(predicate_ops)) or "NONE"

    if base_table_rows:
        table_rows_array = np.asarray(base_table_rows, dtype=float)
        features["base_rows_sum"] = float(table_rows_array.sum())
        features["base_rows_mean"] = float(table_rows_array.mean())
        features["base_rows_min"] = float(table_rows_array.min())
        features["base_rows_max"] = float(table_rows_array.max())
        features["base_rows_log_sum"] = float(np.log1p(table_rows_array).sum())
    else:
        features["base_rows_sum"] = 0.0
        features["base_rows_mean"] = 0.0
        features["base_rows_min"] = 0.0
        features["base_rows_max"] = 0.0
        features["base_rows_log_sum"] = 0.0

    if selectivities:
        selectivity_array = np.asarray(selectivities, dtype=float)
        features["selectivity_product"] = float(selectivity_array.prod())
        features["selectivity_sum"] = float(selectivity_array.sum())
        features["selectivity_mean"] = float(selectivity_array.mean())
        features["selectivity_min"] = float(selectivity_array.min())
        features["selectivity_max"] = float(selectivity_array.max())
        features["selectivity_log_sum"] = float(np.log(selectivity_array).sum())
        features["normalized_value_mean"] = float(np.mean(normalized_values))
        features["estimated_rows_sum"] = max(
            1.0, features["base_rows_sum"] * features["selectivity_product"]
        )
    else:
        features["selectivity_product"] = 1.0
        features["selectivity_sum"] = 0.0
        features["selectivity_mean"] = 1.0
        features["selectivity_min"] = 1.0
        features["selectivity_max"] = 1.0
        features["selectivity_log_sum"] = 0.0
        features["normalized_value_mean"] = 0.5
        features["estimated_rows_sum"] = max(1.0, features["base_rows_sum"])

    features["estimated_rows_log"] = float(math.log1p(features["estimated_rows_sum"]))

    for table_ref in tables:
        alias = table_ref.split()[-1]
        table_rows = alias_to_row_count(alias, stats)
        local_selectivities = alias_selectivities.get(alias, [])
        table_selectivity = float(np.prod(local_selectivities)) if local_selectivities else 1.0
        features[f"alias_selectivity={alias}"] = table_selectivity
        features[f"alias_filtered_rows={alias}"] = table_rows * table_selectivity

    return features


def build_feature_frame(
    df: pd.DataFrame, stats: dict[str, ColumnStat]
) -> tuple[pd.DataFrame, list[str]]:
    feature_frame = pd.DataFrame(
        [extract_features(row, stats) for _, row in df.iterrows()]
    )
    cat_columns = list(feature_frame.select_dtypes(include=["object", "string"]).columns)
    for column in cat_columns:
        feature_frame[column] = feature_frame[column].fillna("NA").astype(str)
    return feature_frame, cat_columns


def build_model(args: argparse.Namespace) -> CatBoostRegressor:
    return CatBoostRegressor(
        loss_function="RMSE",
        eval_metric="RMSE",
        iterations=args.iterations,
        learning_rate=args.learning_rate,
        depth=args.depth,
        l2_leaf_reg=args.l2_leaf_reg,
        random_seed=args.random_state,
        allow_writing_files=False,
        verbose=False,
    )


def train_and_predict(args: argparse.Namespace) -> None:
    train_df = pd.read_csv(TRAIN_PATH)
    test_df = pd.read_csv(TEST_PATH)
    stats = load_column_stats(STATS_PATH)

    train_features, cat_columns = build_feature_frame(train_df, stats)
    test_features, _ = build_feature_frame(test_df, stats)
    test_features = test_features.reindex(columns=train_features.columns, fill_value=0.0)
    for column in cat_columns:
        test_features[column] = test_features[column].fillna("NA").astype(str)

    targets = train_df["Cardinality"].to_numpy(dtype=float)

    model = build_model(args)
    model.fit(train_features, np.log1p(targets), cat_features=cat_columns)

    prediction = np.expm1(model.predict(test_features))
    prediction = np.clip(np.rint(prediction), 1, None).astype(np.int64)

    sample_submission = pd.read_csv(SAMPLE_SUBMISSION_PATH)
    submission_df = sample_submission[["Id"]].merge(
        pd.DataFrame({"Id": test_df["Id"].astype(int), "Cardinality": prediction}),
        on="Id",
        how="left",
    )
    if submission_df["Cardinality"].isna().any():
        raise ValueError("Prediction output is missing some Id values.")

    SUBMISSION_PATH.parent.mkdir(parents=True, exist_ok=True)
    submission_df["Cardinality"] = submission_df["Cardinality"].astype(np.int64)
    submission_df.to_csv(SUBMISSION_PATH, index=False)

    print(f"Train rows: {len(train_df)}")
    print(f"Feature count: {train_features.shape[1]}")
    print(f"Saved submission to: {SUBMISSION_PATH}")


def main() -> None:
    args = parse_args()
    train_and_predict(args)


if __name__ == "__main__":
    main()
