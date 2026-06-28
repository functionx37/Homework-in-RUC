#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUGGY_DIR="$ROOT_DIR/netty/netty-buggy"
EXP_DIR="$ROOT_DIR/netty/netty-exp3"
OUT_DIR="$EXP_DIR/out"
FAKE_HOME="$EXP_DIR/.home"
M2_DIR="$EXP_DIR/.m2"
MVN_BIN="${MVN_BIN:-mvn}"
TIME_LIMIT="${TIME_LIMIT:-300s}"
TEST_SPEC="${TEST_SPEC:-DefaultPromiseTest#testSignalRace}"

mkdir -p "$OUT_DIR" "$FAKE_HOME" "$M2_DIR"

if ! command -v "$MVN_BIN" >/dev/null 2>&1; then
  echo "maven not found: $MVN_BIN" >&2
  exit 2
fi

if [[ -n "${JAVA_HOME:-}" ]]; then
  if [[ -x "$JAVA_HOME/bin/java" ]]; then
    export PATH="$JAVA_HOME/bin:$PATH"
  elif [[ -x "$JAVA_HOME/jre/bin/java" ]]; then
    export PATH="$JAVA_HOME/jre/bin:$PATH"
  fi
fi

if ! command -v java >/dev/null 2>&1; then
  echo "java not found; set JAVA_HOME to a JDK 8 install" >&2
  exit 2
fi

JAVA_VERSION_LINE="$(java -version 2>&1 | head -n 1)"
if [[ "$JAVA_VERSION_LINE" != *'"1.8'* && "$JAVA_VERSION_LINE" != *'"8.'* ]]; then
  echo "JDK 8 recommended, current java is: $JAVA_VERSION_LINE" >&2
fi

if ! command -v timeout >/dev/null 2>&1; then
  echo "timeout not found" >&2
  exit 2
fi

echo "repo=$BUGGY_DIR"
echo "maven=$MVN_BIN"
echo "java=$JAVA_VERSION_LINE"
echo "test=$TEST_SPEC"
echo "timeout=$TIME_LIMIT"

cd "$BUGGY_DIR"
HOME="$FAKE_HOME" \
  "$MVN_BIN" -version >/dev/null

HOME="$FAKE_HOME" \
  timeout --kill-after=10s "$TIME_LIMIT" \
  "$MVN_BIN" \
  -Dmaven.repo.local="$M2_DIR/repository" \
  -pl common -am \
  -Dtest="$TEST_SPEC" \
  test \
  2>&1 | tee "$OUT_DIR/buggy.log"
