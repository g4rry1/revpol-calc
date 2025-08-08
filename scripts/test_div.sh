#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")
TESTDIR=$(realpath "$ROOTDIR/test/div")
CALCULATOR=$(realpath "$ROOTDIR/build/src/calculator")
CHECKER="$BASEDIR/checker.py"
CORRECT="correct"
INCORRECT="incorrect"

printf '\033[96m%s\033[0m\n' 'Test div incorrect tests'
python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${INCORRECT}"

printf '\033[96m%s\033[0m\n' 'Test div correct tests'
python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${CORRECT}"
