#include "account.h"
#include <string>

std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << account.first_name_ << " " << account.last_name_
       << " Account ID: " << account.account_id_ << "\n";
    for (Fund f : account.funds_) {
        os << "    " << f.GetName() << ": $" << f.GetBalance() << "\n";
    }
    return os;
}

Account::Account(std::string last_name, std::string first_name, int account_id)
    : last_name_(last_name), first_name_(first_name), account_id_(account_id) {}

Account::Account(const Account& account)
    : last_name_(account.last_name_),
      first_name_(account.first_name_),
      account_id_(account.account_id_),
      funds_(account.funds_) {}

std::string Account::GetFirstName() const {
    return first_name_;
}

std::string Account::GetLastName() const {
    return last_name_;
}

bool Account::Withdraw(int fund_id, int value) {
    return funds_[fund_id].Withdraw(value);
}

bool Account::Deposit(int fund_id, int value) {
    return funds_[fund_id].Deposit(value);
}

Fund Account::GetFund(int fund_id) const {
    return funds_[fund_id];
}

bool Account::TransferBetweenFunds(Transaction& transaction) {
    if (transaction.GetOriginClientID() ==
        transaction.GetDestinationClientID()) {
        if (transaction.GetOriginFundID() == 0 ||
            transaction.GetOriginFundID() == 1 &&
                (transaction.GetDestinationFundID() == 0 ||
                 transaction.GetDestinationFundID() == 1)) {
            return false;
        } else if (transaction.GetOriginFundID() == 2 ||
                   transaction.GetOriginFundID() == 3 &&
                       (transaction.GetDestinationFundID() == 2 ||
                        transaction.GetDestinationFundID() == 3)) {
            return false;
        }
    }

    int origin_fund;
    int destination_fund;

    if (transaction.GetOriginFundID() == 0) {
        origin_fund = 0;
        destination_fund = 1;
    } else if (transaction.GetOriginFundID() == 1) {
        origin_fund = 1;
        destination_fund = 0;
    } else if (transaction.GetOriginFundID() == 2) {
        origin_fund = 2;
        destination_fund = 3;
    } else if (transaction.GetOriginFundID() == 3) {
        origin_fund = 3;
        destination_fund = 2;
    } else {
        return false;
    }

    if (transaction.GetValue() > funds_[origin_fund].GetBalance()) {
        if (funds_[origin_fund].GetBalance() +
                funds_[destination_fund].GetBalance() >=
            transaction.GetValue()) {
            int amount =
                transaction.GetValue() - funds_[origin_fund].GetBalance();
            Withdraw(destination_fund, amount);
            Transaction temp =
                Transaction('W', account_id_, destination_fund, amount);
            temp.SetTransactionSuccess(true);
            LogTransaction(temp);
            transaction = Transaction(transaction.GetTransactionType(),
                                      transaction.GetOriginClientID(),
                                      transaction.GetOriginFundID(),
                                      transaction.GetDestinationClientID(),
                                      transaction.GetDestinationFundID(),
                                      transaction.GetValue() - amount);
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

bool Account::Transfer(int fund_id, int value, Account* destination_account,
                       int destination_fund) {
    if (Withdraw(fund_id, value)) {
        return destination_account->Deposit(destination_fund, value);
    }
    return false;
}

void Account::LogTransaction(Transaction transaction) {
    if (transaction.GetTransactionType() == 'T') {
        funds_[transaction.GetOriginFundID()].LogTransaction(transaction);
        funds_[transaction.GetDestinationFundID()].LogTransaction(transaction);
    } else {
        funds_[transaction.GetOriginFundID()].LogTransaction(transaction);
    }
}

void Account::PrintTransactions() const {
    for (Fund f : funds_) {
        if (f.GetTransactions().size() != 0) {
            std::cout << f.GetName() << ": $" << f.GetBalance() << "\n";
            for (Transaction t : f.GetTransactions()) {
                std::cout << "  " << t << "\n";
            }
        }
    }
    std::cout << std::endl;
}

std::string Account::GetFundName(int fund_id) const {
    return funds_[fund_id].GetName();
}

int Account::GetID() const {
    return account_id_;
}
