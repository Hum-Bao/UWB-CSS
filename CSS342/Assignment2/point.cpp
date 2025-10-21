#include "point.h"

Point::Point(int x, int y)
    : x_(x), y_(y) {}

Point::Point()
    : Point(int(), int()) {}

int Point::GetX() const {
    return x_;
}

void Point::SetX(int x) {
    x_ = x;
}

void Point::IncrementX() {
    x_++;
}

void Point::DecrementX() {
    x_--;
}

int Point::GetY() const {
    return y_;
}

void Point::SetY(int y) {
    y_ = y;
}

void Point::IncrementY() {
    y_++;
}

void Point::DecrementY() {
    y_--;
}

bool Point::operator==(const Point& p1) {
    if ((x_ == p1.x_) && (y_ == p1.y_)) {
        return true;
    }
    return false;
}