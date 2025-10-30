#ifndef TRANSACTION_MANAGER_H_
#define TRANSACTION_MANAGER_H_

#include <map>
#include <string>
#include <vector>
#include "transaction.h"

using namespace std;

class TransactionManager {
 public:
    [[nodiscard]] std::vector<Transaction> Get(int id) const;
    bool Insert(int id, char action, char type, const std::string& info);
    bool Insert(int id, const Transaction& transaction);

    Transaction static CreateTransaction(int id, char action, char type,
                                         std::string info);

    void Print(int id) const;

 private:
    std::map<int, std::vector<Transaction>> transactions_;
};
#endif  // TRANSACTION_MANAGER_H_
