#!/bin/bash

JAWAC=jawac

BIN_DIR=$(dirname "$0")/../build
TEST_DIR=$(dirname "$0")/../test

if [ ! -f "$BIN_DIR/$JAWAC" ]; then
  echo "could not found $BIN_DIR/$JAWAC" &>/dev/stderr
  exit 1
fi

PASSED=
FAILED=

alias diff="diff --old-group-format=$'EXPECTED:\n%<OUTPUT:\n%>\n' \\
                       --new-group-format=$'EXPECTED:\n%<OUTPUT:\n%>\n' \\
                       --unchanged-group-format=$'\e[0;33m%=\e[0m'"

for TEST in "$TEST_DIR"/*.jawa; do
  TEST_NAME=$(basename "$TEST")
  printf "==== TEST %-20s ====\n" "$TEST_NAME"
  GOLD=$(echo $TEST | sed "s/\.jawa/.out/")
  if
    "$BIN_DIR/$JAWAC" "$TEST" 2>/dev/null | diff "$GOLD" -
  then
    echo "\e[0;32m$TEST_NAME PASSED\e[0m"
    PASSED=${PASSED}1
  else
    echo "\e[0;31m$TEST_NAME FAILED\e[0m"
    FAILED=${FAILED}1
  fi

done

printf "===================================\n"
printf "%d tests PASSED\n%d tests FAILED\n" ${#PASSED} ${#FAILED}
