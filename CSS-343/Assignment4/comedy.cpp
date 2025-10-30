#include "comedy.h"
#include <iostream>
#include <string>
#include <utility>
#include "movie.h"

Comedy::Comedy() = default;

Comedy::Comedy(char media, int stock, std::string director, std::string title,
               int year)
    : Movie(media, stock, std::move(director), std::move(title), year) {}

std::ostream& Comedy::Print(std::ostream& os, const char& media) const {
    return os << "F, " << GetRemainingStock(media) << ", " << GetDirector()
              << ", " << GetTitle() << ", " << GetYear();
}
