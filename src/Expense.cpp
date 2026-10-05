#include "Expense.h"

#include <utility>

Expense::Expense(int id, double amount, std::string category,
                 std::string description, std::string date)
    : id_(id),
      amount_(amount),
      category_(std::move(category)),
      description_(std::move(description)),
      date_(std::move(date)) {}

int Expense::getId() const {
    return id_;
}

double Expense::getAmount() const {
    return amount_;
}

const std::string& Expense::getCategory() const {
    return category_;
}

const std::string& Expense::getDescription() const {
    return description_;
}

const std::string& Expense::getDate() const {
    return date_;
}
