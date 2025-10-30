#ifndef MOVIEFACTORY_H_
#define MOVIEFACTORY_H_

#include <memory>
#include <string>
#include "classics.h"
#include "comedy.h"
#include "drama.h"

/**
 *  MovieFactory class - class used to create movies
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A movie has a director, title, release year, and stocks pertaining to their media types.
 * A movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */
class MovieFactory {
 public:
    static std::shared_ptr<Comedy> CreateComedy(char media, int stock,
                                                std::string director,
                                                std::string title, int year);

    static std::shared_ptr<Drama> CreateDrama(char media, int stock,
                                              std::string director,
                                              std::string title, int year);

    static std::shared_ptr<Classics> CreateClassics(char media, int stock,
                                                    std::string director,
                                                    std::string title,
                                                    std::string actor,
                                                    int month, int year);
};
#endif  // MOVIEFACTORY_H_
