#ifndef VENDING_BANK_H_
#define VENDING_BANK_H_
#include <string>

class VendingBank {
 public:
    VendingBank();
    explicit VendingBank(int id);

    int id() const;

    void SetNewId();

    double GetBalance() const;

    void DepositMoney(double value);

    bool IsEmpty() const;

    void SetQuarterBalance();
    int GetQuarterBalance() const;

    void SetOnes();
    int GetOnes() const;

    void SetFives();
    int GetFives() const;

    int SetTens();
    int GetTens() const;

    int SetTwenties();
    int GetTwenties() const;

 private:
    // id_ is a unique identifier for the VendingBank
    // much like a serial number
    int id_;

    double balance;

    int quarters_;

    int bill_one_;

    int bill_five_;

    int bill_ten_;

    int bill_twenty_;
};
#endif
