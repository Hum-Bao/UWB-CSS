#include "movie.h"
#include <iostream>
#include <string>
#include <utility>
#include "mediafactory.h"

Movie::Movie() : year_(-1) {}

Movie::Movie(char media, int stock, std::string director, std::string title,
             int year)
    : director_(std::move(director)), title_(std::move(title)), year_(year) {
    if (stock_.find(media) != stock_.end()) {
        stock_.find(media)->second.UpdateStock(stock);
    } else {
        stock_.insert({media, MediaFactory::CreateMedia(stock)});
    }
}

bool Movie::UpdateStock(char media, int stock) {
    if (stock_.find(media) != stock_.end()) {
        stock_.find(media)->second.UpdateStock(stock);
        return true;
    }
    std::cerr << "Movie Media type does not exist\n";
    return false;
}

bool Movie::CheckOut(char media) {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.CheckOut();
    }
    std::cerr << "Movie Media type does not exist\n";
    return false;
}

bool Movie::Return(char media) {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.Return();
    }
    std::cerr << "Movie Media type does not exist\n";
    return false;
}

bool Movie::InStock(char media) const {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.InStock();
    }
    std::cerr << "Movie Media type does not exist\n";
    return false;
}

int Movie::GetTotalStock(char media) const {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.GetTotalStock();
    }
    std::cerr << "Movie Media type does not exist\n";
    return -1;
}

int Movie::GetRemainingStock(char media) const {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.GetRemainingStock();
    }
    std::cerr << "Movie Media type does not exist\n";
    return -1;
}

int Movie::GetBorrowedStock(char media) const {
    if (stock_.find(media) != stock_.end()) {
        return stock_.find(media)->second.GetBorrowedStock();
    }
    std::cerr << "Movie Media type does not exist\n";
    return -1;
}

std::string Movie::GetDirector() const {
    return director_;
}

std::string Movie::GetTitle() const {
    return title_;
}

int Movie::GetYear() const {
    return year_;
}

std::ostream& Movie::Print(std::ostream& os, const char& media) const {
    return os << "BASE CLASS PRINT " << media << "\n";
}

std::ostream& operator<<(std::ostream& os, const Movie& movie) {
    return movie.Print(os, 'D');
}