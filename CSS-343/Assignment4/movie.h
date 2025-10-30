#ifndef MOVIE_H_
#define MOVIE_H_

#include <iostream>
#include <map>
#include <string>
#include "media.h"

/**
 *  Movie Class - interface class used to store and manage movie data
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A Movie has a director, title, release year, and stocks pertaining to their media types.
 * A Movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */
class Movie {
 public:
    Movie();
    Movie(char media, int stock, std::string director, std::string title,
          int year);
    virtual ~Movie() = default;

    bool UpdateStock(char media, int stock);

    bool CheckOut(char media);
    bool Return(char media);

    [[nodiscard]] bool InStock(char media) const;
    [[nodiscard]] int GetTotalStock(char media) const;
    [[nodiscard]] int GetRemainingStock(char media) const;
    [[nodiscard]] int GetBorrowedStock(char media) const;

    [[nodiscard]] std::string GetDirector() const;
    [[nodiscard]] std::string GetTitle() const;
    [[nodiscard]] int GetYear() const;

    bool operator<(const Movie& other) const;
    bool operator>(const Movie& other) const;
    bool operator==(const Movie& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& movie);

 private:
    std::map<char, Media> stock_;
    std::string director_;
    std::string title_;
    int year_;
    virtual std::ostream& Print(std::ostream& os, const char& media) const;
};

#endif  // MOVIE_H_
