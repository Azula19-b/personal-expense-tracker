#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "Expense.h"

#include <string>
#include <vector>

class ExpenseManager {
public:
    explicit ExpenseManager(std::string filePath = "data/expenses.csv");

    void addExpenseInteractive();
    void displayAllExpenses() const;

private:
    std::vector<Expense> expenses_;
    int nextId_ = 1;
    std::string filePath_;

    void loadExpenses();
    bool saveExpenses() const;
    static double readPositiveAmount();
    static std::string readRequiredText(const std::string& prompt);
    static std::string readValidDate();
    static bool isValidDate(const std::string& date);
};

#endif
