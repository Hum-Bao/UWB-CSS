#include "classics.h"
#include <iostream>
#include <string>
#include <utility>
#include "movie.h"

Classics::Classics() : month_(-1) {}

Classics::Classics(char media, int stock, std::string director,
                   std::string title, std::string actor, int month, int year)
    : Movie(media, stock, std::move(director), std::move(title), year),
      actor_(std::move(actor)),
      month_(month) {}

std::string Classics::GetActor() const {
    return actor_;
}

int Classics::GetMonth() const {
    return month_;
}

std::ostream& Classics::Print(std::ostream& os, const char& media) const {
    return os << "C, " << GetRemainingStock(media) << ", " << GetDirector()
              << ", " << GetTitle() << ", " << GetActor() << ", " << GetMonth()
              << " " << GetYear();
}