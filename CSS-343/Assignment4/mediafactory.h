#ifndef MOVIEFACTORY_H_
#define MOVIEFACTORY_H_

#include <string>

/**
 *  MediaFactory class - class used to create movies
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A movie has a director, title, release year, and stocks pertaining to their media types.
 * A movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */
class MediaFactory {
 public:
    static Media CreateMedia(int stock) { return Media(stock); }
};

#endif  // MOVIEFACTORY_H_