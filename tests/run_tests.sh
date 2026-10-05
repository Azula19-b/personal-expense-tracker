#!/bin/sh

set -eu

PROJECT_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BINARY="$PROJECT_ROOT/expense_tracker"
TEST_DIRECTORY=$(mktemp -d "${TMPDIR:-/tmp}/expense-tracker-tests.XXXXXX")
trap 'rm -rf "$TEST_DIRECTORY"' EXIT

assert_contains() {
    output=$1
    expected=$2
    description=$3

    if ! printf '%s' "$output" | grep -Fq "$expected"; then
        printf 'FAIL: %s\nExpected output to contain: %s\n' "$description" "$expected"
        exit 1
    fi
}

# Start with no data file and exercise operations on an empty list.
mkdir -p "$TEST_DIRECTORY/empty/data"
empty_output=$(
    cd "$TEST_DIRECTORY/empty"
    printf '2\n5\n8\n' | "$BINARY"
)
assert_contains "$empty_output" "No expenses found." "start without a saved file"
assert_contains "$empty_output" "There are no expenses to sort." "sort an empty list"

# Add two records while also checking invalid numeric input, searches, sorts,
# and deletion of an ID that does not exist.
mkdir -p "$TEST_DIRECTORY/full/data"
session_output=$(
    cd "$TEST_DIRECTORY/full"
    printf '1\nnot-a-number\n-10\n12.50\nFood\nBreakfast\n2026-10-01\n1\n22.75\nTravel\nTaxi\n2026-10-02\n4\nOffice\n3\n999\n5\n1\n5\n2\n5\n3\n8\n' | "$BINARY"
)
assert_contains "$session_output" "Invalid amount." "reject invalid amounts"
assert_contains "$session_output" "Expense added and saved successfully." "add expenses"
assert_contains "$session_output" "No expenses found for category search \"Office\"." "missing category search"
assert_contains "$session_output" "No expense exists with ID 999." "delete an invalid ID"
assert_contains "$session_output" "Expenses sorted successfully." "sort populated expenses"

line_count=$(wc -l < "$TEST_DIRECTORY/full/data/expenses.csv" | tr -d ' ')
if [ "$line_count" -ne 3 ]; then
    printf 'FAIL: expected a header and two saved expense rows, found %s lines\n' "$line_count"
    exit 1
fi

# Start a fresh process and confirm that both records load from disk.
restart_output=$(
    cd "$TEST_DIRECTORY/full"
    printf '2\n8\n' | "$BINARY"
)
assert_contains "$restart_output" "Breakfast" "load the first saved expense"
assert_contains "$restart_output" "Taxi" "load the second saved expense"

printf 'All expense tracker integration tests passed.\n'
