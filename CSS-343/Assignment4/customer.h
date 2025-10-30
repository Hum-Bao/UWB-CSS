#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <string>

class Customer {
 public:
    Customer();
    Customer(int id, std::string first, std::string last);

    [[nodiscard]] int getID() const;
    [[nodiscard]] std::string getFirstName() const;
    [[nodiscard]] std::string getLastName() const;
    [[nodiscard]] std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const Customer& other);

 private:
    int id_;
    std::string first_;
    std::string last_;
};
#endif