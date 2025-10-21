#include "transaction.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

Transaction::Transaction() : id_(-1), action_(0), type_(0) {}

Transaction::Transaction(int id, char action, char type, std::string info)
    : id_(id), action_(action), type_(type), info_(std::move(info)) {}

int Transaction::GetID() const {
    return id_;
}

char Transaction::GetAction() const {
    return action_;
}

char Transaction::GetType() const {
    return type_;
}

std::string Transaction::GetInfo() const {
    return info_;
}

std::string Transaction::ToString() const {
    std::stringstream tostring;
    tostring << "ID: " << id_;
    tostring << ", Action: " << action_;
    tostring << ", Type: " << type_;
    tostring << ", Info: " << info_;
    return tostring.str();
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
    os << "ID: " << transaction.GetID();
    os << ", Action: " << transaction.GetAction();
    os << ", Type: " << transaction.GetType();
    os << ", Info: " << transaction.GetInfo();
    return os;
}