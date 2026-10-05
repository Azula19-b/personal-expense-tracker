#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>

class Expense {
public:
    Expense(int id, double amount, std::string category,
            std::string description, std::string date);

    int getId() const;
    double getAmount() const;
    const std::string& getCategory() const;
    const std::string& getDescription() const;
    const std::string& getDate() const;

private:
    int id_;
    double amount_;
    std::string category_;
    std::string description_;
    std::string date_;
};

#endif
