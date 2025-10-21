#ifndef CUSTOMER_MANAGER_H_
#define CUSTOMER_MANAGER_H_

#include <map>
#include "customer.h"
#include "hashtable.h"

class CustomerManager {
 public:
    [[nodiscard]] Customer Get(int id) const;
    bool Insert(int id, std::string first, std::string last);

    [[nodiscard]] bool Contains(int id) const;

    // void Print(const int& id) const;

    friend std::ostream& operator<<(std::ostream& os,
                                    const CustomerManager& other);

 private:
    HashTable<int, Customer> list_;
    Customer static Create(int id, std::string first, std::string last);
};
#endif  // CUSTOMER_MANAGER_H_