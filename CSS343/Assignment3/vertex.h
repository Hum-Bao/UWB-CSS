#ifndef VERTEX_H_
#define VERTEX_H_

#include <string>
#include <utility>

using std::string;

class Vertex {
    friend class Graph;

 public:
    // Default constructor
    // Accepts a string as the name of the vertex
    inline explicit Vertex(string name) : name_(std::move(name)) {}

 private:
    string name_;
    int state_ = 0;
    int path_ = 0;
    int prev_ = 0;
};
#endif
