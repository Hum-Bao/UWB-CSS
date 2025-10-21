#ifndef FUND_H_
#define FUND_H_
#include <string>
#include <vector>
#include "transaction.h"

class Fund {
public:
    explicit Fund(std::string fund_name);

    bool Withdraw(int amount);
    bool Deposit(int amount);

    void LogTransaction(Transaction transaction);
    void PrintTransactions() const;

    std::string GetName() const;
    int GetBalance() const;
    std::vector<Transaction> GetTransactions() const;

private:
    std::string fund_name_;
    int balance_;
    std::vector<Transaction> transaction_history_;
};
#endif
