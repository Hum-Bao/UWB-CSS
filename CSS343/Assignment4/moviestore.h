#ifndef MOVIE_STORE_H_
#define MOVIE_STORE_H_
#include <string>
#include "customermanager.h"
#include "inventory.h"
#include "transaction.h"
#include "transactionmanager.h"

//const char MEDIA_TYPE = 'D';

class MovieStore {
 public:
    void ReadMovies(const std::string& file);
    void ReadCustomers(const std::string& file);
    void ReadCommands(const std::string& file);

 private:
    Inventory inventory_;
    CustomerManager customers_;
    TransactionManager transactions_;
};

#endif  // MOVIE_STORE_H_