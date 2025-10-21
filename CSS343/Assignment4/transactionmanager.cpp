#include "transactionmanager.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "transaction.h"

std::vector<Transaction> TransactionManager::Get(int id) const {
    if (transactions_.find(id) != transactions_.end()) {
        return transactions_.find(id)->second;
    }
    return {};
}

bool TransactionManager::Insert(int id, char action, char type,
                                const std::string& info) {
    if (transactions_.find(id) != transactions_.end()) {
        transactions_.find(id)->second.push_back(
            CreateTransaction(id, action, type, info));
    } else {
        std::vector<Transaction> vector;
        vector.push_back(CreateTransaction(id, action, type, info));
        transactions_.insert({id, vector});
    }
    return true;
}

bool TransactionManager::Insert(int id, const Transaction& transaction) {
    if (transactions_.find(id) != transactions_.end()) {
        transactions_.find(id)->second.push_back(transaction);
    } else {
        std::vector<Transaction> vector;
        vector.push_back(transaction);
        transactions_.insert({id, vector});
    }
    return true;
}

Transaction TransactionManager::CreateTransaction(int id, char action,
                                                  char type, std::string info) {
    return {id, action, type, std::move(info)};
}

void TransactionManager::Print(int id) const {
    if (transactions_.find(id) != transactions_.end()) {
        std::cout << "Transactions for: " << id << ":" << "\n";
        for (const Transaction& temp : transactions_.find(id)->second) {
            std::cout << temp << "\n";
        }
    }
}