#ifndef BANK_H_
#define BANK_H_
#include <queue>
#include <string>
#include "bstree.h"
#include "transaction.h"

class Bank {
public:
    Bank();
    void AddToQueue(Transaction transaction);
    bool AddAccount(Account* acount);
    bool GetAccount(const int& account_id, Account* account) const;
    bool ProcessQueue();

    bool Deposit(Transaction transaction);
    bool Withdraw(Transaction transaction);
    bool Transfer(Transaction transaction);
    bool DisplayAllHistory(Transaction transaction);
    bool DisplayFundHistory(Transaction transaction);
    bool Open(Transaction transaction);

    void PrintAccounts();

private:
    BSTree accounts_;
    std::queue<Transaction> transaction_queue;
    void AccountNotFoundError(const int& account_id) const;
    void InsufficientFundsError(const Transaction& transaction,
                                const Account* account) const;
};
#endif
