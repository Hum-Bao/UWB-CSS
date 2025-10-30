/**
 *  Classics Class - class used to store and manage Classic movie data
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A Classics movie has a director, title, major actor, release month, release year,
 * and stocks pertaining to their media types.
 * A Classics movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */

#ifndef CLASSICS_H_
#define CLASSICS_H_

#include <string>
#include "movie.h"

class Classics : public Movie {
 public:
    Classics();
    Classics(char media, int stock, std::string director, std::string title,
             std::string actor, int month, int year);

    [[nodiscard]] std::string GetActor() const;
    [[nodiscard]] int GetMonth() const;

 private:
    std::string actor_;
    int month_;
    std::ostream& Print(std::ostream& os, const char& media) const override;
};

#endif  // CLASSICS_H_