#include "ExpenseManager.h"

#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

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

std::string escapeCsv(const std::string& value) {
    std::string escaped = "\"";
    for (char character : value) {
        if (character == '\"') {
            escaped += "\"\"";
        } else {
            escaped += character;
        }
    }
    return escaped + "\"";
}

bool parseCsvRow(const std::string& line, std::vector<std::string>& fields) {
    fields.clear();
    std::string field;
    bool insideQuotes = false;

    for (std::size_t index = 0; index < line.size(); ++index) {
        const char character = line[index];
        if (insideQuotes) {
            if (character == '\"' && index + 1 < line.size() &&
                line[index + 1] == '\"') {
                field += '\"';
                ++index;
            } else if (character == '\"') {
                insideQuotes = false;
            } else {
                field += character;
            }
        } else if (character == '\"' && field.empty()) {
            insideQuotes = true;
        } else if (character == ',') {
            fields.push_back(field);
            field.clear();
        } else {
            field += character;
        }
    }

    if (insideQuotes) {
        return false;
    }

    fields.push_back(field);
    return true;
}

bool parseInteger(const std::string& text, int& value) {
    std::istringstream stream(text);
    char extra = '\0';
    return (stream >> value) && !(stream >> extra);
}

bool parseDouble(const std::string& text, double& value) {
    std::istringstream stream(text);
    char extra = '\0';
    return (stream >> value) && !(stream >> extra);
}

}  // namespace

ExpenseManager::ExpenseManager(std::string filePath)
    : filePath_(std::move(filePath)) {
    loadExpenses();
}

void ExpenseManager::addExpenseInteractive() {
    const double amount = readPositiveAmount();
    const std::string category = readRequiredText("Category: ");
    const std::string description = readRequiredText("Description: ");
    const std::string date = readValidDate();

    expenses_.emplace_back(nextId_++, amount, category, description, date);
    if (saveExpenses()) {
        std::cout << "Expense added and saved successfully.\n";
    } else {
        std::cout << "Expense added, but the data file could not be updated.\n";
    }
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

void ExpenseManager::loadExpenses() {
    std::ifstream input(filePath_);
    if (!input) {
        return;  // A data file will be created when the first expense is added.
    }

    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty() || (lineNumber == 1 && line == "id,amount,category,description,date")) {
            continue;
        }

        std::vector<std::string> fields;
        int id = 0;
        double amount = 0.0;
        if (!parseCsvRow(line, fields) || fields.size() != 5 ||
            !parseInteger(fields[0], id) || !parseDouble(fields[1], amount) ||
            id < 1 || amount <= 0.0 || !std::isfinite(amount) ||
            trim(fields[2]).empty() || trim(fields[3]).empty() ||
            !isValidDate(fields[4])) {
            std::cerr << "Warning: skipped invalid data on line " << lineNumber
                      << " of " << filePath_ << ".\n";
            continue;
        }

        expenses_.emplace_back(id, amount, fields[2], fields[3], fields[4]);
        if (id >= nextId_ && id < std::numeric_limits<int>::max()) {
            nextId_ = id + 1;
        }
    }
}

bool ExpenseManager::saveExpenses() const {
    std::ofstream output(filePath_);
    if (!output) {
        return false;
    }

    output << "id,amount,category,description,date\n";
    output << std::setprecision(15);
    for (const Expense& expense : expenses_) {
        output << expense.getId() << ',' << expense.getAmount() << ','
               << escapeCsv(expense.getCategory()) << ','
               << escapeCsv(expense.getDescription()) << ','
               << expense.getDate() << '\n';
    }

    return output.good();
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
