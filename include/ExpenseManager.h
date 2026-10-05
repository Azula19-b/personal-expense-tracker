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
    void deleteExpenseInteractive();
    void searchByCategoryInteractive() const;
    void sortExpensesInteractive();
    void displayTotalSpending() const;
    void displayCategoryTotals() const;

private:
    std::vector<Expense> expenses_;
    int nextId_ = 1;
    std::string filePath_;

    void loadExpenses();
    bool saveExpenses() const;
    static void displayExpenses(const std::vector<Expense>& expenses);
    static double readPositiveAmount();
    static int readPositiveInteger(const std::string& prompt);
    static std::string readRequiredText(const std::string& prompt);
    static std::string readValidDate();
    static bool isValidDate(const std::string& date);
};

#endif
