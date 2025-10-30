#include "transaction.h"
#include <iostream>
#include <string>

// Default constructor
Transaction::Transaction()
    : transaction_type_(-1),
      origin_client_id_(-1),
      origin_fund_id_(-1),
      destination_client_id_(-1),
      destination_fund_id_(-1),
      value_(-1) {}

// Deposit / Withdraw from single account (TYPE D/W)
Transaction::Transaction(char transaction_type, int client_id, int fund_id,
                         int value)
    : transaction_type_(transaction_type),
      origin_client_id_(client_id),
      origin_fund_id_(fund_id),
      destination_client_id_(-1),
      destination_fund_id_(-1),
      value_(value) {}

// Transfer funds from one client's funds into another client's funds
// (or different fund from same client) (TYPE T)
Transaction::Transaction(char transaction_type, int origin_client_id,
                         int origin_fund_id, int destination_client_id,
                         int destination_fund_id, int value)
    : transaction_type_(transaction_type),
      origin_client_id_(origin_client_id),
      origin_fund_id_(origin_fund_id),
      destination_client_id_(destination_client_id),
      destination_fund_id_(destination_fund_id),
      value_(value) {}

// History transaction for all accounts of client (TYPE H)
Transaction::Transaction(char transaction_type, int client_id)
    : transaction_type_(transaction_type),
      origin_client_id_(client_id),
      origin_fund_id_(-1),
      destination_client_id_(-1),
      destination_fund_id_(-1),
      value_(-1) {}

// History transaction for single fund of client (TYPE F)
Transaction::Transaction(char transaction_type, int client_id, int fund_id)
    : transaction_type_(transaction_type),
      origin_client_id_(client_id),
      origin_fund_id_(fund_id),
      destination_client_id_(-1),
      destination_fund_id_(-1),
      value_(-1) {}

// Account open transaction (TYPE O)
Transaction::Transaction(char transaction_type, int client_id,
                         std::string last_name, std::string first_name)
    : transaction_type_(transaction_type),
      origin_client_id_(client_id),
      origin_fund_id_(0),
      destination_client_id_(-1),
      destination_fund_id_(-1),
      value_(200),
      last_name_(last_name),
      first_name_(first_name) {
    // driver.addAccount(Account());
}

std::ostream& operator<<(std::ostream& os, Transaction t) {
    os << t.transaction_type_ << " " << t.origin_client_id_;
    if (t.origin_fund_id_ != -1) {
        os << " " << t.origin_fund_id_;
    }
    if (t.destination_client_id_ != -1) {
        os << " " << t.destination_client_id_;
    }
    if (t.destination_fund_id_ != -1) {
        os << " " << t.destination_fund_id_;
    }
    if (t.value_ != -1) {
        os << " " << t.value_;
    }
    if (!t.transaction_success_) {
        os << " (Failed)";
    }
    return os;
}

char Transaction::GetTransactionType() const {
    return transaction_type_;
}

int Transaction::GetOriginClientID() const {
    return origin_client_id_;
}

int Transaction::GetOriginFundID() const {
    return origin_fund_id_;
}

int Transaction::GetDestinationClientID() const {
    return destination_client_id_;
}

int Transaction::GetDestinationFundID() const {
    return destination_fund_id_;
}

int Transaction::GetValue() const {
    return value_;
}

void Transaction::SetTransactionSuccess(bool success) {
    transaction_success_ = success;
}

bool Transaction::GetTransactionSuccess() const {
    return transaction_success_;
}

std::string Transaction::GetFirstName() const {
    return first_name_;
}

std::string Transaction::GetLastName() const {
    return last_name_;
}

Transaction& Transaction::operator=(const Transaction& transaction) {
    transaction_type_ = transaction.transaction_type_;
    origin_client_id_ = transaction.origin_client_id_;
    origin_fund_id_ = transaction.origin_fund_id_;
    destination_client_id_ = transaction.destination_client_id_;
    destination_fund_id_ = transaction.destination_fund_id_;
    value_ = transaction.value_;
    first_name_ = transaction.first_name_;
    last_name_ = transaction.last_name_;
    return *this;
}
