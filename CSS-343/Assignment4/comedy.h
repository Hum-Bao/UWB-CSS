#ifndef COMEDY_H_
#define COMEDY_H_

#include <string>
#include "movie.h"

/**
 *  Comedy Class - Used to store and manage Comedy movie data
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A comedy movie has a director, title, release year, and stocks pertaining to their media types.
 * A comedy movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */
class Comedy : public Movie {
 public:
    Comedy();
    Comedy(char media, int stock, std::string director, std::string title,
           int year);

 private:
    std::ostream& Print(std::ostream& os, const char& media) const override;
};

#endif  // COMEDY_H_
