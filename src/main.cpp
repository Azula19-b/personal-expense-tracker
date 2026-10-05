#include "ExpenseManager.h"

#include <iostream>
#include <limits>

namespace {

void displayMenu() {
    std::cout << "\n=== Personal Expense Tracker ===\n"
              << "1. Add expense\n"
              << "2. View all expenses\n"
              << "3. Exit\n"
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
                running = false;
                break;
            default:
                std::cout << "Invalid selection. Please choose 1-3.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
