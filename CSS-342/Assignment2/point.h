#ifndef POINT_H_
#define POINT_H_

class Point {
public:
    Point();
    Point(int x, int y);

    int GetX() const;
    void SetX(int x);
    void IncrementX();
    void DecrementX();

    int GetY() const;
    void SetY(int y);
    void IncrementY();
    void DecrementY();

    bool operator==(const Point& p1);

private:
    int x_;
    int y_;
};
#endif