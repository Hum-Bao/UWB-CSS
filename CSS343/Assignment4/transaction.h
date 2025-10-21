#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include <iostream>
#include <string>
#include "movie.h"

class Transaction {
 public:
    Transaction();
    Transaction(int id, char action, char type, std::string info);

    [[nodiscard]] int GetID() const;
    [[nodiscard]] char GetAction() const;
    [[nodiscard]] char GetType() const;
    [[nodiscard]] std::string GetInfo() const;

    [[nodiscard]] std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os,
                                    const Transaction& transaction);

 private:
    int id_;
    char action_;
    char type_;
    std::string info_;
};

#endif  // TRANSACTION_H_