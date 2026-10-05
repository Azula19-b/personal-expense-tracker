# Personal Expense Tracker CLI

A menu-driven C++ application for recording everyday expenses in the terminal.
The project demonstrates object-oriented programming, file I/O, input
validation, searching, sorting, and aggregation without external libraries.

## Features

- Add an expense with an amount, category, description, and date
- View every expense in a readable table
- Delete an expense by its unique ID
- Search categories with case-insensitive, partial matching
- Sort by amount, date, or category
- Display total spending and totals grouped by category
- Reject invalid amounts, dates, IDs, blank fields, and menu choices
- Save data automatically and load it again on the next run
- Skip malformed saved rows safely instead of crashing

## Technologies used

- C++17
- C++ Standard Library
- CSV file storage
- Make
- POSIX shell for integration tests

No third-party libraries are required.

## File structure

```text
expense-tracker/
├── data/
│   ├── .gitkeep
│   └── sample_expenses.csv
├── include/
│   ├── Expense.h
│   └── ExpenseManager.h
├── src/
│   ├── Expense.cpp
│   ├── ExpenseManager.cpp
│   └── main.cpp
├── tests/
│   └── run_tests.sh
├── .gitignore
├── Makefile
└── README.md
```

`Expense` represents one record. `ExpenseManager` owns the collection,
validates user input, performs searches and calculations, and reads or writes
the CSV data file. `main.cpp` provides the menu loop.

## How to compile

From the project directory, compile with Make:

```bash
make
```

Alternatively, compile directly with any C++17-compatible compiler:

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/*.cpp -o expense_tracker
```

## How to run

```bash
./expense_tracker
```

The application uses `data/expenses.csv` for runtime data. A missing or empty
file is handled automatically, and the file is created after the first expense
is added. Runtime data is ignored by Git so personal records are not committed.

## Example usage

```text
================================
    Personal Expense Tracker
================================
1. Add expense
2. View all expenses
3. Delete an expense
4. Search by category
5. Sort expenses
6. Show total spending
7. Show spending by category
8. Exit
Choose an option: 1
Amount: $14.25
Category: Food
Description: Lunch
Date (YYYY-MM-DD): 2026-10-05
Expense added and saved successfully.
```

To try the included sample records, copy the sample file before starting the
program:

```bash
cp data/sample_expenses.csv data/expenses.csv
./expense_tracker
```

You can also generate records by choosing option `1` from the menu.

## Tests

Run the integration checks with:

```bash
make test
```

The test script uses temporary directories and covers starting without a saved
file, adding one and multiple expenses, invalid amounts, a category with no
matches, an invalid deletion ID, sorting an empty list, all three sorting
choices, and reloading saved data in a new process.

## Future improvements

- Add monthly budget limits and warnings
- Export reports for a selected date range
- Support editing an existing expense
- Add optional descending sort order
- Create charts from category and monthly totals
