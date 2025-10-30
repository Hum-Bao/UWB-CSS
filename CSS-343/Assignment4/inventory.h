#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "movie.h"
#include "transaction.h"

class Inventory {
 public:
    // Default constructor
    Inventory();
    // Adds movie to inventory using type as identifier
    bool Add(const char& type, const std::shared_ptr<Movie>& movie);
    // Finds movie from type and title.
    bool Contains(const char& type, const std::string& title, const int& year);
    bool Contains(const char& type, const std::string& director,
                  const std::string& title);
    bool Contains(const char& type, const std::string& actor, const int& month,
                  const int& year);

    Transaction Borrow(const char& type, const std::string& title,
                       const int& year, const int& id);
    Transaction Borrow(const char& type, const std::string& director,
                       const std::string& title, const int& id);
    Transaction Borrow(const char& type, const std::string& actor,
                       const int& month, const int& year, const int& id);

    Transaction Return(const char& type, const std::string& title,
                       const int& year, const int& id);
    Transaction Return(const char& type, const std::string& director,
                       const std::string& title, const int& id);
    Transaction Return(const char& type, const std::string& actor,
                       const int& month, const int& year, const int& id);

    void Print() const;
    void Sort();

 private:
    std::map<char, std::vector<std::shared_ptr<Movie>>> inventory_;
    using KeyValuePair = std::pair<char, std::vector<Movie>>;
    void PrintHelper(const char& type) const;
    void SortHelper(const char& type);
};
#endif  // INVENTORY_H