#include "inventory.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "classics.h"
#include "movie.h"
#include "transaction.h"
#include "transactionmanager.h"

Inventory::Inventory() {
    inventory_.insert({'F', std::vector<std::shared_ptr<Movie>>()});
    inventory_.insert({'D', std::vector<std::shared_ptr<Movie>>()});
    inventory_.insert({'C', std::vector<std::shared_ptr<Movie>>()});
}

bool Inventory::Add(const char& type, const std::shared_ptr<Movie>& movie) {
    if (inventory_.find(type) != inventory_.end()) {
        inventory_.find(type)->second.push_back(movie);
        return true;
    }
    return false;
}

//Comedy contains
bool Inventory::Contains(const char& type, const std::string& title,
                         const int& year) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetTitle() == title && temp->GetYear() == year) {
                return true;
            }
        }
    }
    return false;
}

//Drama contains
bool Inventory::Contains(const char& type, const std::string& director,
                         const std::string& title) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetDirector() == director && temp->GetTitle() == title) {
                return true;
            }
        }
    }
    return false;
}

//Classics contains
bool Inventory::Contains(const char& type, const std::string& actor,
                         const int& month, const int& year) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            auto& classic_cast = dynamic_cast<Classics&>(*temp);
            if (classic_cast.GetActor() == actor &&
                classic_cast.GetMonth() == month &&
                classic_cast.GetYear() == year) {
                return true;
            }
        }
    }
    return false;
}

//Comedy
Transaction Inventory::Borrow(const char& type, const std::string& title,
                              const int& year, const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetTitle() == title && temp->GetYear() == year) {
                temp->CheckOut('D');
                const std::string info =
                    temp->GetTitle() + ", " + to_string(temp->GetYear());
                return TransactionManager::CreateTransaction(id, 'B', type,
                                                             info);
            }
        }
    }
    return {};
}

//Drama
Transaction Inventory::Borrow(const char& type, const std::string& director,
                              const string& title, const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetDirector() == director && temp->GetTitle() == title) {
                temp->CheckOut('D');
                const std::string info =
                    temp->GetDirector() + ", " + temp->GetTitle();
                return TransactionManager::CreateTransaction(id, 'B', type,
                                                             info);
            }
        }
    }
    return {};
}

//Classics
Transaction Inventory::Borrow(const char& type, const std::string& actor,
                              const int& month, const int& year,
                              const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            auto& classic_cast = dynamic_cast<Classics&>(*temp);
            if (classic_cast.GetActor() == actor &&
                classic_cast.GetMonth() == month &&
                classic_cast.GetYear() == year) {
                classic_cast.CheckOut('D');
                const std::string info = classic_cast.GetTitle() + ", " +
                                         classic_cast.GetActor() + ", " +
                                         to_string(classic_cast.GetYear());
                return TransactionManager::CreateTransaction(id, 'B', type,
                                                             info);
            }
        }
    }
    return {};
}

//Comedy
Transaction Inventory::Return(const char& type, const std::string& title,
                              const int& year, const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetTitle() == title && temp->GetYear() == year) {
                temp->Return('D');
                const string info =
                    temp->GetTitle() + ", " + to_string(temp->GetYear());
                return TransactionManager::CreateTransaction(id, 'R', type,
                                                             info);
            }
        }
    }
    return {};
}

//Drama
Transaction Inventory::Return(const char& type, const std::string& director,
                              const std::string& title, const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            if (temp->GetDirector() == director && temp->GetTitle() == title) {
                temp->Return('D');
                const std::string info =
                    temp->GetDirector() + ", " + temp->GetTitle();
                return TransactionManager::CreateTransaction(id, 'R', type,
                                                             info);
            }
        }
    }
    return {};
}

//Classics
Transaction Inventory::Return(const char& type, const std::string& actor,
                              const int& month, const int& year,
                              const int& id) {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& temp :
             inventory_.find(type)->second) {
            auto& classic_cast = dynamic_cast<Classics&>(*temp);
            if (classic_cast.GetActor() == actor &&
                classic_cast.GetMonth() == month &&
                classic_cast.GetYear() == year) {
                classic_cast.Return('D');
                const string info = classic_cast.GetTitle() + ", " +
                                    classic_cast.GetActor() + ", " +
                                    to_string(classic_cast.GetYear());
                return TransactionManager::CreateTransaction(id, 'R', type,
                                                             info);
            }
        }
    }
    return {};
}

void Inventory::Print() const {
    PrintHelper('F');
    PrintHelper('D');
    PrintHelper('C');
}

void Inventory::PrintHelper(const char& type) const {
    if (inventory_.find(type) != inventory_.end()) {
        for (const std::shared_ptr<Movie>& movie :
             inventory_.find(type)->second) {
            std::cout << *movie << "\n";
        }
    }
}

void Inventory::Sort() {
    SortHelper('F');
    SortHelper('D');
    SortHelper('C');
}

void Inventory::SortHelper(const char& type) {
    if (inventory_.find(type) != inventory_.end()) {
        std::vector<std::shared_ptr<Movie>>& movie_vector =
            inventory_.find(type)->second;
        if (type == 'C') {
            std::sort(movie_vector.begin(), movie_vector.end(),
                      [](const std::shared_ptr<Movie>& movie1,
                         const std::shared_ptr<Movie>& movie2) {
                          auto& m1 = dynamic_cast<Classics&>(*movie1);
                          auto& m2 = dynamic_cast<Classics&>(*movie2);
                          if (m1.GetYear() < m2.GetYear()) {
                              return true;
                          } else if ((m1.GetYear() == m2.GetYear()) &&
                                     (m1.GetMonth() < m2.GetMonth())) {
                              return true;
                          } else if ((m1.GetYear() == m2.GetYear()) &&
                                     (m1.GetMonth() == m2.GetMonth()) &&
                                     (m1.GetActor() < m2.GetActor())) {
                              return true;
                          } else {
                              return false;
                          }
                      });
        } else if (type == 'D') {
            std::sort(movie_vector.begin(), movie_vector.end(),
                      [](const std::shared_ptr<Movie>& m1,
                         const std::shared_ptr<Movie>& m2) {
                          if (m1->GetDirector() < m2->GetDirector()) {
                              return true;
                          } else if ((m1->GetDirector() == m2->GetDirector()) &&
                                     (m1->GetTitle() < m2->GetTitle())) {
                              return true;
                          } else {
                              return false;
                          }
                      });
        } else {
            std::sort(movie_vector.begin(), movie_vector.end(),
                      [](const std::shared_ptr<Movie>& m1,
                         const std::shared_ptr<Movie>& m2) {
                          if (m1->GetTitle() < m2->GetTitle()) {
                              return true;
                          } else if ((m1->GetTitle() == m2->GetTitle()) &&
                                     (m1->GetYear() < m2->GetYear())) {
                              return true;
                          } else {
                              return false;
                          }
                      });
        }
    }
}
