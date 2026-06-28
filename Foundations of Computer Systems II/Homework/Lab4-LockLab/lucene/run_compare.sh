#!/usr/bin/env bash
set -uo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
EXP_DIR="$ROOT_DIR/lucene/lucene-exp2"
OUT_DIR="$EXP_DIR/out"
FAKE_HOME="$EXP_DIR/.home"
IVY_LIB_DIR="$EXP_DIR/.ant/lib"
IVY_USER_DIR="$EXP_DIR/.ivy2"
IVY_SETTINGS_FILE="$EXP_DIR/ivysettings.xml"
BUGGY_DIR="$ROOT_DIR/lucene/lucene-buggy"
FIXED_DIR="$ROOT_DIR/lucene/lucene-fixed"
ANT_BIN="${ANT_BIN:-ant}"
TIME_LIMIT="${TIME_LIMIT:-300s}"
TEST_CLASS="${TEST_CLASS:-TestTragicIndexWriterDeadlock}"
TEST_METHOD="${TEST_METHOD:-testDeadlockStalledMerges}"

mkdir -p "$OUT_DIR" "$FAKE_HOME" "$IVY_LIB_DIR" "$IVY_USER_DIR"

if ! command -v "$ANT_BIN" >/dev/null 2>&1; then
  echo "ant not found: $ANT_BIN" >&2
  exit 2
fi

if [[ -n "${JAVA_HOME:-}" ]]; then
  export PATH="$JAVA_HOME/bin:$PATH"
fi

if ! command -v java >/dev/null 2>&1; then
  echo "java not found; set JAVA_HOME to a JDK 8 install" >&2
  exit 2
fi

JAVA_VERSION_LINE="$(java -version 2>&1 | head -n 1)"
if [[ "$JAVA_VERSION_LINE" != *'"1.8'* && "$JAVA_VERSION_LINE" != *'"8.'* ]]; then
  echo "JDK 8 required, current java is: $JAVA_VERSION_LINE" >&2
  exit 2
fi

if ! command -v timeout >/dev/null 2>&1; then
  echo "timeout not found" >&2
  exit 2
fi

bootstrap_ivy() {
  local repo="$1"

  if compgen -G "$IVY_LIB_DIR/ivy-*.jar" > /dev/null; then
    return
  fi

  (
    cd "$repo"
    "$ANT_BIN" \
      -Duser.home="$FAKE_HOME" \
      -Divy.default.ivy.user.dir="$IVY_USER_DIR" \
      -Divy_install_path="$IVY_LIB_DIR" \
      -Divysettings.xml="$IVY_SETTINGS_FILE" \
      -Divy_bootstrap_url1=https://repo1.maven.org/maven2 \
      -Divy_bootstrap_url2=https://repo.maven.apache.org/maven2 \
      ivy-bootstrap
  )
}

run_one() {
  local label="$1"
  local repo="$2"
  local log_file="$OUT_DIR/${label}.log"

  bootstrap_ivy "$repo"

  echo "=== $label ==="
  echo "repo=$repo"
  echo "ant=$ANT_BIN"
  echo "java=$JAVA_VERSION_LINE"
  echo "test=$TEST_CLASS#$TEST_METHOD"
  echo "timeout=$TIME_LIMIT"

  (
    cd "$repo/lucene/core"
    timeout --kill-after=10s "$TIME_LIMIT" \
      "$ANT_BIN" -lib "$IVY_LIB_DIR" \
      -Duser.home="$FAKE_HOME" \
      -Divy.default.ivy.user.dir="$IVY_USER_DIR" \
      -Divysettings.xml="$IVY_SETTINGS_FILE" \
      test \
      -Dtestcase="$TEST_CLASS" \
      -Dtests.method="$TEST_METHOD"
  ) 2>&1 | tee "$log_file"

  local status="${PIPESTATUS[0]}"
  echo "exit_code=$status" | tee -a "$log_file"
  echo
}

run_one "buggy" "$BUGGY_DIR"
run_one "fixed" "$FIXED_DIR"
