#ifndef DRAMA_H_
#define DRAMA_H_

#include <string>
#include "movie.h"

/**
 *  Drama Class - Used to store and manage Drama movie data
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A Drama movie has a director, title, release year, and stocks pertaining to their media types.
 * A Drama movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */
class Drama : public Movie {
 public:
    Drama();
    Drama(char media, int stock, std::string director, std::string title,
          int year);

 private:
    std::ostream& Print(std::ostream& os, const char& media) const override;
};
#endif  // DRAMA_H_
