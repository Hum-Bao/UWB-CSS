#ifndef COVIDPRIORITYQUEUE_H_
#define COVIDPRIORITYQUEUE_H_
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "patient.h"

/*
* COVIDPriorityQueue.h
* Represents a priority queue for patients awaiting a COVID vaccine.
* April 7, 2024
* Kyle Ricks
*/
class COVIDPriorityQueue {
 public:
    //Default priority queue constructor
    COVIDPriorityQueue();

    /*
    * Calls the std::priority_queue empty() method.
    * Refer to std::priority_queue::empty() for documentation.
    */
    [[nodiscard]] bool empty() const;

    /*
    * Calls the std::priority_queue push() method.
    * Accepts a Patient passed as a contant reference to be added to the queue.
    * Refer to std::priority_queue::push() for documentation.
    */
    void push(Patient const& patient);

    /*
    * Calls the std::priority_queue pop() method.
    * Refer to std::priority_queue::pop() for documentation.
    */
    void pop();

    /*
    * Calls the std::priority_queue top() method.
    * Refer to std::priority_queue::top() for documentation.
    */
    [[nodiscard]] const Patient& top() const;

    /*
    * Calls the std::priority_queue size() method.
    * Refer to std::priority_queue::size() for documentation.
    */
    [[nodiscard]] std::size_t size() const;

    /* 
    * Read a file of patients and add them to the priority queue.
    * Patients should be provided using this format:
    * Name, Age, Precondition (bool)
    * file_name is a valid file containing patients passed as a string.
    * Returns a bool of type true if the file can be read.
    * Returns a bool of type false if the file cannot be read.
    */
    bool ReadFile(string const& file_name);

 private:
    /*
    * Priority queue container.
    * Refer to std::priority_queue for documentation.
    */
    std::priority_queue<Patient> queue_;
};
#endif
