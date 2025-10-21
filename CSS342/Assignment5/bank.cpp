#include "bank.h"
#include <iostream>

Bank::Bank() : accounts_(), transaction_queue(std::queue<Transaction>()) {}

bool Bank::GetAccount(const int& account_id, Account* account) const {
    return accounts_.Retrieve(account_id, account);
}

bool Bank::AddAccount(Account* account) {
    return accounts_.Insert(account);
}

void Bank::PrintAccounts() {
    if (accounts_.Size() != 0) {
        std::cout << "Account Balances:\n" << std::endl;
        accounts_.Display();
    }
}

void Bank::AccountNotFoundError(const int& account_id) const {
    std::cerr << "ERROR: Account " << account_id
              << " not found. Transferal refused." << std::endl;
}

void Bank::InsufficientFundsError(const Transaction& transaction,
                                  const Account* account) const {
    std::cerr << "ERROR: Not enough funds to withdraw "
              << transaction.GetValue() << " from " << account->GetFirstName()
              << " " << account->GetLastName() << " "
              << account->GetFundName(transaction.GetOriginFundID())
              << std::endl;
}

bool Bank::Deposit(Transaction transaction) {
    Account* account;
    bool status = accounts_.Retrieve(transaction.GetOriginClientID(), account);
    if (status) {
        account->Deposit(transaction.GetOriginFundID(), transaction.GetValue());
        transaction.SetTransactionSuccess(true);
        account->LogTransaction(transaction);
    } else {
        AccountNotFoundError(transaction.GetOriginClientID());
        transaction.SetTransactionSuccess(false);
    }
    return status;
}

bool Bank::Withdraw(Transaction transaction) {
    Account* account;
    bool status = accounts_.Retrieve(transaction.GetOriginClientID(), account);

    if (status) {
        if (transaction.GetValue() >
            account->GetFund(transaction.GetOriginFundID()).GetBalance()) {
            account->TransferBetweenFunds(transaction);
        }
        if (account->Withdraw(transaction.GetOriginFundID(),
                              transaction.GetValue())) {
            transaction.SetTransactionSuccess(true);
        } else {
            InsufficientFundsError(transaction, account);
            transaction.SetTransactionSuccess(false);
        }
        account->LogTransaction(transaction);
    } else {
        AccountNotFoundError(transaction.GetOriginClientID());
    }

    return status;
}

bool Bank::Transfer(Transaction transaction) {
    Account* origin_account;
    Account* destination_account;
    bool origin_status =
        accounts_.Retrieve(transaction.GetOriginClientID(), origin_account);
    bool destination_status;
    if (transaction.GetOriginClientID() ==
        transaction.GetDestinationClientID()) {
        destination_account = origin_account;
        destination_status = origin_status;
    } else {
        destination_status = accounts_.Retrieve(
            transaction.GetDestinationClientID(), destination_account);
    }
    transaction.SetTransactionSuccess(false);

    if (origin_status && destination_status) {
        if (transaction.GetValue() >
            origin_account->GetFund(transaction.GetOriginFundID())
                .GetBalance()) {
            origin_account->TransferBetweenFunds(transaction);
        }
        if (origin_account->Transfer(
                transaction.GetOriginFundID(), transaction.GetValue(),
                destination_account, transaction.GetDestinationFundID())) {
            transaction.SetTransactionSuccess(true);
        } else {
            std::cerr << "ERROR: Not enough funds to transfer "
                      << transaction.GetValue() << " from "
                      << origin_account->GetFirstName() << " "
                      << origin_account->GetLastName() << " "
                      << origin_account->GetFundName(
                             transaction.GetOriginFundID())
                      << " to " << destination_account->GetFirstName() << " "
                      << destination_account->GetLastName() << " "
                      << destination_account->GetFundName(
                             transaction.GetDestinationFundID())
                      << std::endl;
        }
    } else if (origin_status && !destination_status) {
        AccountNotFoundError(transaction.GetDestinationClientID());
    } else if (!origin_status && destination_status) {
        AccountNotFoundError(transaction.GetOriginClientID());
    } else {
        AccountNotFoundError(transaction.GetOriginClientID());
        AccountNotFoundError(transaction.GetDestinationClientID());
    }

    if (origin_account != destination_account && origin_status &&
        destination_status) {
        origin_account->LogTransaction(transaction);
        destination_account->LogTransaction(transaction);
    } else if (origin_account == destination_account && origin_status) {
        origin_account->LogTransaction(transaction);
    }

    return origin_status == destination_status && origin_status == true;
}

bool Bank::DisplayAllHistory(Transaction transaction) {
    Account* account;
    bool status = accounts_.Retrieve(transaction.GetOriginClientID(), account);
    if (status) {
        std::cout << "Transaction History for " << account->GetFirstName()
                  << " " << account->GetLastName() << " by fund." << std::endl;
        account->PrintTransactions();
        transaction.SetTransactionSuccess(true);
    } else {
        AccountNotFoundError(transaction.GetOriginClientID());
        transaction.SetTransactionSuccess(false);
    }
    return status;
}

bool Bank::DisplayFundHistory(Transaction transaction) {
    Account* account;
    bool status = accounts_.Retrieve(transaction.GetOriginClientID(), account);
    if (status) {
        std::cout
            << "Transaction History for " << account->GetFirstName() << " "
            << account->GetLastName() << " "
            << account->GetFund(transaction.GetOriginFundID()).GetName()
            << ": $"
            << account->GetFund(transaction.GetOriginFundID()).GetBalance()
            << std::endl;
        account->GetFund(transaction.GetOriginFundID()).PrintTransactions();
        transaction.SetTransactionSuccess(true);
    } else {
        AccountNotFoundError(transaction.GetOriginClientID());
        transaction.SetTransactionSuccess(false);
    }
    return status;
}

bool Bank::Open(Transaction transaction) {
    Account* account =
        new Account(transaction.GetLastName(), transaction.GetFirstName(),
                    transaction.GetOriginClientID());
    if (!AddAccount(account)) {
        std::cerr << "ERROR: Account " << transaction.GetOriginClientID()
                  << " is already open. Transaction refused." << std::endl;
        transaction.SetTransactionSuccess(false);
        delete account;
        account = nullptr;
        return false;
    }
    account->Deposit(transaction.GetOriginFundID(), transaction.GetValue());
    transaction.SetTransactionSuccess(true);
    return true;
}

void Bank::AddToQueue(Transaction transaction) {
    transaction_queue.push(transaction);
}

bool Bank::ProcessQueue() {
    while (!transaction_queue.empty()) {
        switch (transaction_queue.front().GetTransactionType()) {
            case 'O':
                Open(transaction_queue.front());
                break;
            case 'D':
                Deposit(transaction_queue.front());
                break;
            case 'W':
                Withdraw(transaction_queue.front());
                break;
            case 'T':
                Transfer(transaction_queue.front());
                break;
            case 'H':
                DisplayAllHistory(transaction_queue.front());
                break;
            case 'F':
                DisplayFundHistory(transaction_queue.front());
                break;
            default:
                std::cerr << "ERROR: Transaction type "
                          << transaction_queue.front().GetTransactionType()
                          << " does not exist." << std::endl;
        }
        transaction_queue.pop();
    }
    return true;
}
