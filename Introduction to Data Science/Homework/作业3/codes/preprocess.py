import pandas as pd

# 加载数据
file_path = "dataset/ai_job_trends_dataset.csv"
df = pd.read_csv(file_path)

# 开始数据检查与清洗
print("\n开始数据检查与清洗：\n")

# 检查缺失值
missing_values = df.isnull().sum()
total_missing = missing_values.sum()
print(f"共有 {total_missing} 处数据缺失")

# 填补缺失值
if total_missing > 0:
    numeric_columns = df.select_dtypes(include=["number"]).columns
    categorical_columns = df.select_dtypes(exclude=["number"]).columns

    # 数值型字段：用中位数填充
    if len(numeric_columns) > 0:
        df[numeric_columns] = df[numeric_columns].fillna(df[numeric_columns].median())

    # 分类型字段：用众数填充
    for col in categorical_columns:
        mode_value = df[col].mode()
        df[col].fillna(mode_value[0], inplace=True)

# 检查数据类型
expected_types = {
    "Job Title": "object",
    "Industry": "object",
    "Job Status": "object",
    "AI Impact Level": "object",
    "Median Salary (USD)": ["int64", "float64"],
    "Required Education": "object",
    "Experience Required (Years)": ["int64", "float64"],
    "Job Openings (2024)": ["int64", "float64"],
    "Projected Openings (2030)": ["int64", "float64"],
    "Remote Work Ratio (%)": ["int64", "float64"],
    "Automation Risk (%)": ["int64", "float64"],
    "Location": "object",
    "Gender Diversity (%)": ["int64", "float64"],
}
unreasonable_columns = []

for col in expected_types:
    actual_type = str(df[col].dtype)
    expected = expected_types[col]
    if isinstance(expected, list):
        is_reasonable = actual_type in expected
    else:
        is_reasonable = actual_type == expected
    if not is_reasonable:
        unreasonable_columns.append(col)
print(f"共有 {len(unreasonable_columns)} 处数据类型错误")


# 检查异常值

# 薪资不能为负数或过高
salary_outliers = df[
    (df["Median Salary (USD)"] < 0) | (df["Median Salary (USD)"] > 10000000)
]
print(f"共有 {len(salary_outliers)} 处薪资异常")

# 自动化风险、远程工作比例、性别多样性应在 0–100% 之间
automation_outliers = df[
    (df["Automation Risk (%)"] < 0) | (df["Automation Risk (%)"] > 100)
]
print(f"共有 {len(automation_outliers)} 处自动化风险异常")
remote_outliers = df[
    (df["Remote Work Ratio (%)"] < 0) | (df["Remote Work Ratio (%)"] > 100)
]
print(f"共有 {len(remote_outliers)} 处远程工作比例异常")
gender_outliers = df[
    (df["Gender Diversity (%)"] < 0) | (df["Gender Diversity (%)"] > 100)
]
print(f"共有 {len(gender_outliers)} 处性别多样性异常")

# 工作经验年数不应为负或过高
exp_outliers = df[
    (df["Experience Required (Years)"] < 0) | (df["Experience Required (Years)"] > 60)
]
print(f"共有 {len(exp_outliers)} 处工作经验记录异常")

# 职位空缺数不应为负
openings_2024_neg = df[df["Job Openings (2024)"] < 0]
print(f"共有 {len(openings_2024_neg)} 处 2024职位空缺异常")
openings_2030_neg = df[df["Projected Openings (2030)"] < 0]
print(f"共有 {len(openings_2030_neg)} 处 2030预计职位空缺异常")

# 裁剪异常值
df["Median Salary (USD)"] = df["Median Salary (USD)"].clip(lower=0, upper=10000000)
df["Automation Risk (%)"] = df["Automation Risk (%)"].clip(lower=0, upper=100)
df["Remote Work Ratio (%)"] = df["Remote Work Ratio (%)"].clip(lower=0, upper=100)
df["Gender Diversity (%)"] = df["Gender Diversity (%)"].clip(lower=0, upper=100)
df["Experience Required (Years)"] = df["Experience Required (Years)"].clip(lower=0, upper=60)
df["Job Openings (2024)"] = df["Job Openings (2024)"].clip(lower=0)
df["Projected Openings (2030)"] = df["Projected Openings (2030)"].clip(lower=0)

# 完成数据检查与清洗
df.to_csv("ai_job_trends_dataset_cleaned.csv", index=False)
print("\n数据检查与清洗已完成\n")