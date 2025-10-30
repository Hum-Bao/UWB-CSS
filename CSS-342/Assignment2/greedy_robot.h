#ifndef GREEDY_ROBOT_H_
#define GREEDY_ROBOT_H_
#include <string>
#include <vector>
#include "point.h"

using std::string;
using std::vector;
class GreedyRobot {
public:
    GreedyRobot(int max_distance, Point robot_pos, Point treasure_pos);
    void FindPaths(Point robot_pos, string current_path, int n, int s, int e, int w);
    void StartPathfinding();
    vector<string> GetPaths() const;

private:
    Point robot_pos_;
    Point treasure_pos_;
    int max_distance_;
    vector<string> paths_;
};
#endif