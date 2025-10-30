#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <string>
#include "fund.h"
#include "transaction.h"

class Account {
    friend std::ostream& operator<<(std::ostream& os, const Account& account);

public:
    Account(std::string last_name, std::string first_name, int account_id);
    Account(const Account& account);

    std::string GetFirstName() const;
    std::string GetLastName() const;
    int GetID() const;

    bool Withdraw(int fund_id, int value);
    bool Deposit(int fund_id, int value);
    bool Transfer(int fund_id, int value, Account* destination_account,
                  int destination_fund);
    std::string GetFundName(int fund_id) const;
    Fund GetFund(int fund_id) const;

    void LogTransaction(Transaction transaction);
    void PrintTransactions() const;

    bool TransferBetweenFunds(Transaction& transaction);

private:
    std::string first_name_;
    std::string last_name_;
    int account_id_;
    Fund funds_[9] = {Fund("Money Market"),       Fund("Prime Money Market"),
                      Fund("Long-Term Bond"),     Fund("Short-Term Bond"),
                      Fund("500 Index Fund"),     Fund("Capital Value Fund"),
                      Fund("Growth Equity Fund"), Fund("Growth Index Fund"),
                      Fund("International Fund")};
};
#endif
