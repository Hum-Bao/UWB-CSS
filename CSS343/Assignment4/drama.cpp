#include "drama.h"
#include <iostream>
#include <string>
#include <utility>
#include "movie.h"

Drama::Drama() = default;

Drama::Drama(char media, int stock, std::string director, std::string title,
             int year)
    : Movie(media, stock, std::move(director), std::move(title), year) {}

std::ostream& Drama::Print(std::ostream& os, const char& media) const {
    return os << "D, " << GetRemainingStock(media) << ", " << GetDirector()
              << ", " << GetTitle() << ", " << GetYear();
}
