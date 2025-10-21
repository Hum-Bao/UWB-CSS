#include "customer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

Customer::Customer() : id_(-1) {}

Customer::Customer(int id, std::string first, std::string last)
    : id_(id), first_(std::move(first)), last_(std::move(last)) {}

int Customer::getID() const {
    return id_;
}

std::string Customer::getFirstName() const {
    return first_;
}

std::string Customer::getLastName() const {
    return last_;
}

std::string Customer::ToString() const {
    std::stringstream temp;
    temp << id_ << " " << first_ << " " << last_;
    return temp.str();
}

std::ostream& operator<<(std::ostream& os, const Customer& other) {
    return os << other.ToString();
}