#include "ExpenseManager.h"

#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

}  // namespace

void ExpenseManager::addExpenseInteractive() {
    const double amount = readPositiveAmount();
    const std::string category = readRequiredText("Category: ");
    const std::string description = readRequiredText("Description: ");
    const std::string date = readValidDate();

    expenses_.emplace_back(nextId_++, amount, category, description, date);
    std::cout << "Expense added successfully.\n";
}

void ExpenseManager::displayAllExpenses() const {
    if (expenses_.empty()) {
        std::cout << "No expenses found.\n";
        return;
    }

    std::cout << '\n'
              << std::left << std::setw(5) << "ID"
              << std::setw(12) << "Date"
              << std::setw(18) << "Category"
              << std::right << std::setw(12) << "Amount" << "  "
              << std::left << "Description\n"
              << std::string(70, '-') << '\n';

    for (const Expense& expense : expenses_) {
        std::cout << std::left << std::setw(5) << expense.getId()
                  << std::setw(12) << expense.getDate()
                  << std::setw(18) << expense.getCategory()
                  << std::right << '$' << std::setw(10) << std::fixed
                  << std::setprecision(2) << expense.getAmount() << "  "
                  << std::left << expense.getDescription() << '\n';
    }
}

double ExpenseManager::readPositiveAmount() {
    while (true) {
        std::cout << "Amount: $";
        std::string input;
        std::getline(std::cin, input);

        std::istringstream stream(input);
        double amount = 0.0;
        char extra = '\0';

        if (stream >> amount && !(stream >> extra) && amount > 0.0 &&
            std::isfinite(amount)) {
            return amount;
        }

        std::cout << "Invalid amount. Enter a number greater than zero.\n";
    }
}

std::string ExpenseManager::readRequiredText(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        input = trim(input);

        if (!input.empty()) {
            return input;
        }

        std::cout << "This field cannot be empty.\n";
    }
}

std::string ExpenseManager::readValidDate() {
    while (true) {
        const std::string date = readRequiredText("Date (YYYY-MM-DD): ");
        if (isValidDate(date)) {
            return date;
        }

        std::cout << "Invalid date. Use YYYY-MM-DD and enter a real calendar date.\n";
    }
}

bool ExpenseManager::isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (std::size_t index = 0; index < date.size(); ++index) {
        if (index != 4 && index != 7 &&
            !std::isdigit(static_cast<unsigned char>(date[index]))) {
            return false;
        }
    }

    const int year = std::stoi(date.substr(0, 4));
    const int month = std::stoi(date.substr(5, 2));
    const int day = std::stoi(date.substr(8, 2));
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    const int daysInMonth[] = {
        31, isLeapYear(year) ? 29 : 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    return day <= daysInMonth[month - 1];
}
