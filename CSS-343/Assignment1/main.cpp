/*
* main.cpp
* Main method for a priority queue for patients awaiting a COVID vaccine.
* April 7, 2024
* Kyle Ricks
*/
#include <cstddef>
#include <iostream>
#include "COVIDPriorityQueue.h"
#include "patient.h"

/*
* Main method for the COVIDPriorityQueue.
* Accepts a list of patients via command-line arguments.
* User is queried for the number of available vaccines,
* and the user(s) at the top of the priority queue are given
*/
int main(int argc, char* argv[]) {
    if (argv[1] != nullptr) {
        const string file_name = string(argv[1]);

        COVIDPriorityQueue covid_queue;
        covid_queue.ReadFile(file_name);
        std::cout << "How many vaccines are available: ";
        size_t num = 0;
        std::cin >> num;
        if (num > covid_queue.size()) {
            num = covid_queue.size();
        }
        for (int count = 0; count < num; count++) {
            std::cout << covid_queue.top().GetName() << " ";
            covid_queue.pop();
        }
        std::cout << "\n";
    } else {
        std::cout << "Please provide a patient file\n";
    }
    return 0;
}
