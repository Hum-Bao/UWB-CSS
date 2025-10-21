#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#include <iostream>
#include <string>

class Transaction {
    friend std::ostream& operator<<(std::ostream& os, Transaction t);

public:
    Transaction();
    // Deposit / Withdraw from single account (TYPE D/W)
    Transaction(char transaction_type, int client_id, int fund_id, int value);
    // Transfer funds from one client's funds into another client's funds
    // (or different fund from same client) (TYPE T)
    Transaction(char transaction_type, int origin_client_id, int origin_fund_id,
                int destination_client_id, int destination_fund_id, int value);
    // History transaction for all accounts of client (TYPE H)
    Transaction(char transaction_type, int client_id);
    // History transaction for single fund of client (TYPE F)
    Transaction(char transaction_type, int client_id, int fund_id);
    // Account open transaction (TYPE O)
    Transaction(char transaction_type, int client_id, std::string last_name,
                std::string first_name);

    char GetTransactionType() const;
    int GetOriginClientID() const;
    int GetOriginFundID() const;
    int GetDestinationClientID() const;
    int GetDestinationFundID() const;
    int GetValue() const;
    void SetTransactionSuccess(bool success);
    bool GetTransactionSuccess() const;
    std::string GetFirstName() const;
    std::string GetLastName() const;

    Transaction& operator=(const Transaction& transaction);

private:
    char transaction_type_;
    int origin_client_id_;
    int origin_fund_id_;
    int destination_client_id_;
    int destination_fund_id_;
    int value_;
    bool transaction_success_;
    std::string last_name_;
    std::string first_name_;
};
#endif
