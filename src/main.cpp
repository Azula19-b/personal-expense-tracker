#include "ExpenseManager.h"

#include <iostream>
#include <limits>

namespace {

void displayMenu() {
    std::cout << "\n=== Personal Expense Tracker ===\n"
              << "1. Add expense\n"
              << "2. View all expenses\n"
              << "3. Delete an expense\n"
              << "4. Search by category\n"
              << "5. Sort expenses\n"
              << "6. Exit\n"
              << "Choose an option: ";
}

int readMenuChoice() {
    int choice = 0;

    while (!(std::cin >> choice)) {
        std::cout << "Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

}  // namespace

int main() {
    ExpenseManager manager;
    bool running = true;

    while (running) {
        displayMenu();

        switch (readMenuChoice()) {
            case 1:
                manager.addExpenseInteractive();
                break;
            case 2:
                manager.displayAllExpenses();
                break;
            case 3:
                manager.deleteExpenseInteractive();
                break;
            case 4:
                manager.searchByCategoryInteractive();
                break;
            case 5:
                manager.sortExpensesInteractive();
                break;
            case 6:
                running = false;
                break;
            default:
                std::cout << "Invalid selection. Please choose 1-6.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
