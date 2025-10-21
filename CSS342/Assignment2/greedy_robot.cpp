#include "greedy_robot.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

GreedyRobot::GreedyRobot(int max_distance, Point robot_pos, Point treasure_pos)
    : max_distance_(max_distance), robot_pos_(robot_pos), treasure_pos_(treasure_pos) {}

// Get vector containing all possible paths
vector<string> GreedyRobot::GetPaths() const {
    return paths_;
}

// Recursively find all possible paths. loc is the current location, current_path tracks
// the current path, and n, s, e, and w track the consecutive moves in each cardinal direction.
// Each direction is tried until it reaches the max distance, upon which it removes its last move
// and returns to the method. The direction integers are not reset at this point, ensuring that
// the robot does not repeat the same path again.
void GreedyRobot::FindPaths(Point loc, string current_path, int n, int s, int e, int w) {
    if (loc == treasure_pos_) {
        paths_.push_back(current_path);
        return;
    }

    if (loc.GetY() < treasure_pos_.GetY() && n < max_distance_) {
        loc.IncrementY();
        FindPaths(loc, current_path += "N", n + 1, 0, 0, 0);
        current_path.pop_back();
        loc.DecrementY();
    }

    if (loc.GetY() > treasure_pos_.GetY() && s < max_distance_) {
        loc.DecrementY();
        FindPaths(loc, current_path += "S", 0, s + 1, 0, 0);
        current_path.pop_back();
        loc.IncrementY();
    }

    if (loc.GetX() < treasure_pos_.GetX() && e < max_distance_) {
        loc.IncrementX();
        FindPaths(loc, current_path += "E", 0, 0, e + 1, 0);
        current_path.pop_back();
        loc.DecrementX();
    }

    if (loc.GetX() > treasure_pos_.GetX() && w < max_distance_) {
        loc.DecrementX();
        FindPaths(loc, current_path += "W", 0, 0, 0, w + 1);
        current_path.pop_back();
        loc.IncrementX();
    }
}

// Method for starting the recursive calls. Also initializes the vector that
// will hold all of the paths
void GreedyRobot::StartPathfinding() {
    paths_ = vector<string>();
    FindPaths(robot_pos_, "", 0, 0, 0, 0);
}

int main(int argc, char* argv[]) {
    if (argc < 6 || argc > 6) {
        cout << "Please enter 5 digits" << endl;
        return -1;
    }
    int max_distance = stoi(argv[1]);
    Point robot_pos = Point(stoi(argv[2]), stoi(argv[3]));
    Point treasure_pos = Point(stoi(argv[4]), stoi(argv[5]));

    if (robot_pos == treasure_pos) {
        cout << "Already at treasure" << endl;
        cout << "Number of paths: 0" << endl;
        return 0;
    }

    if ((robot_pos.GetX() == treasure_pos.GetX() && abs(robot_pos.GetY() - treasure_pos.GetY()) > max_distance) || (robot_pos.GetY() == treasure_pos.GetY() && abs(robot_pos.GetX() - treasure_pos.GetX()) > max_distance)) {
        cout << "No possible paths" << endl;
        cout << "Number of paths: 0" << endl;
        return 0;
    }

    GreedyRobot gr = GreedyRobot(max_distance, robot_pos, treasure_pos);
    gr.StartPathfinding();
    int shortest_distance = abs(robot_pos.GetX() - treasure_pos.GetX()) + abs(robot_pos.GetY() - treasure_pos.GetY());
    for (string temp : gr.GetPaths()) {
        if (temp.length() == shortest_distance) {
            cout << temp << endl;
        }
    }
    cout << "Number of paths: " << gr.GetPaths().size() << endl;
}