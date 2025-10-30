#include "fund.h"
#include <string>
#include <vector>

Fund::Fund(std::string fund_name) : fund_name_(fund_name), balance_(0) {}

bool Fund::Withdraw(int amount) {
    if (balance_ >= amount) {
        balance_ -= amount;
        return true;
    }
    return false;
}

void Fund::LogTransaction(Transaction t) {
    transaction_history_.push_back(t);
}

void Fund::PrintTransactions() const {
    for (Transaction t : transaction_history_) {
        std::cout << "  " << t << "\n";
    }
    std::cout << std::endl;
}

std::string Fund::GetName() const {
    return fund_name_;
}

int Fund::GetBalance() const {
    return balance_;
}

std::vector<Transaction> Fund::GetTransactions() const {
    return transaction_history_;
}

bool Fund::Deposit(int amount) {
    balance_ += amount;
    return true;
}
