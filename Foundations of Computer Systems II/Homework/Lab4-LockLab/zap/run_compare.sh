#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
GO_BIN="${GO_BIN:-go}"
TEST_NAME='TestBufferWriter/stop_race_with_flush'
COUNT="${COUNT:-100}"
TIMEOUT="${TIMEOUT:-5s}"
OUT_DIR="$ROOT_DIR/zap/zap-exp1/out"
CACHE_DIR="$ROOT_DIR/zap/zap-exp1/.cache"
MOD_CACHE_DIR="$ROOT_DIR/zap/zap-exp1/.modcache"

mkdir -p "$OUT_DIR"
mkdir -p "$CACHE_DIR"
mkdir -p "$MOD_CACHE_DIR"

run_one() {
  local label="$1"
  local repo="$2"
  local log_file="$OUT_DIR/${label}.log"

  echo "=== $label ==="
  echo "repo=$repo"
  echo "go=$GO_BIN"
  echo "test=$TEST_NAME"
  echo "count=$COUNT timeout=$TIMEOUT"

  (
    cd "$repo"
    GOCACHE="$CACHE_DIR" GOMODCACHE="$MOD_CACHE_DIR" \
      "$GO_BIN" test ./zapcore -run "$TEST_NAME" -count "$COUNT" -timeout "$TIMEOUT"
  ) 2>&1 | tee "$log_file"

  local status="${PIPESTATUS[0]}"
  echo "exit_code=$status" | tee -a "$log_file"
  echo
}

run_one "buggy" "$ROOT_DIR/zap/zap-buggy"
run_one "fixed" "$ROOT_DIR/zap/zap-fixed"
