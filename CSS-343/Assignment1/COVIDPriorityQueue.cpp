/*
* COVIDPriorityQueue.cpp
* Implementation of a priority queue for patients awaiting a COVID vaccine.
* April 7, 2024
* Kyle Ricks
*/

#include "COVIDPriorityQueue.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "patient.h"

// Default priority queue constructor
COVIDPriorityQueue::COVIDPriorityQueue() = default;

/*
* Calls the std::priority_queue empty() method.
* Refer to std::priority_queue::empty() for documentation.
*/
bool COVIDPriorityQueue::empty() const {
    return queue_.empty();
}

/*
* Calls the std::priority_queue push() method.
* Accepts a Patient passed as a contant reference to be added to the queue.
* Refer to std::priority_queue::push() for documentation.
*/
void COVIDPriorityQueue::push(Patient const& patient) {
    queue_.push(patient);
}

/*
* Calls the std::priority_queue pop() method.
* Refer to std::priority_queue::pop() for documentation.
 */
void COVIDPriorityQueue::pop() {
    queue_.pop();
}

/*
* Calls the std::priority_queue top() method.
* Refer to std::priority_queue::top() for documentation.
*/
const Patient& COVIDPriorityQueue::top() const {
    return queue_.top();
}

/*
* Calls the std::priority_queue size() method.
* Refer to std::priority_queue::size() for documentation.
*/
std::size_t COVIDPriorityQueue::size() const {
    return queue_.size();
}

/*
* Helper method for parsing patient string inputs.
* input is a string passed by value to be split via a provided delimiter.
* delimiter is the string passed by constant reference to be used for splitting
* input.
* Returns the split string in a std::vector<string> container.
*/
std::vector<string> Split(string input, string const& delimiter) {
    std::vector<string> arr;
    string token;
    while (input.find(delimiter) != string::npos) {
        token = input.substr(0, input.find(delimiter));
        arr.push_back(token);
        input.erase(0, input.find(delimiter) + delimiter.length());
    }
    if (!input.empty()) {
        arr.push_back(input);
    }
    return arr;
}

/* 
* Read a file of patients and add them to the priority queue.
* Patients should be provided using this format:
* Name, Age, Precondition (bool)
* file_name is a valid file containing patients passed as a string.
* Returns a bool of type true if the file can be read.
* Returns a bool of type false if the file cannot be read.
*/
bool COVIDPriorityQueue::ReadFile(string const& file_name) {
    std::ifstream in_file(file_name);
    if (!in_file.is_open()) {
        std::cerr << "Error: Unable to open file " << file_name << "\n";
        return false;
    }
    string temp;
    std::vector<string> split_arr;
    bool temp_bool = false;
    while (std::getline(in_file, temp)) {
        split_arr = Split(temp, ", ");
        std::transform(split_arr[2].begin(), split_arr[2].end(),
                       split_arr[2].begin(), ::tolower);
        temp_bool = (split_arr[2] == "yes");
        queue_.push(Patient(split_arr[0], std::stoi(split_arr[1]), temp_bool));
    }

    in_file.close();
    return true;
}
