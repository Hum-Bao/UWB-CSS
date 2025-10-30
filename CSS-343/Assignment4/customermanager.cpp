#include "customermanager.h"
#include <utility>
#include "customer.h"
#include "hashtable.h"

Customer CustomerManager::Get(int id) const {
    return list_.get(id);
}

bool CustomerManager::Insert(int id, std::string first, std::string last) {
    return list_.insert(id, Create(id, std::move(first), std::move(last)));
}

bool CustomerManager::Contains(int id) const {
    return list_.contains(id);
}

Customer CustomerManager::Create(int id, std::string first, std::string last) {
    return {id, std::move(first), std::move(last)};
}

std::ostream& operator<<(std::ostream& os, const CustomerManager& other) {
    return os << other.list_;
}
