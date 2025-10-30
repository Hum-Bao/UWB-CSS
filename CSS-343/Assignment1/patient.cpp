/*
* patient.cpp
* Implementation of a patient to be added to a COVIDPriorityQueue
* April 7, 2024
* Kyle Ricks
*/

#include "patient.h"
#include <string>
#include <utility>

/*
* Overloaded constructor.
* name is a string passed by value to be assigned to the patient.
* age is a int passed by value to be assigned to the patient.
* precondition is a bool passed by value to be assigned to the patient.
*/
Patient::Patient(string name, int age, bool precondition)
    : name_(std::move(name)), age_(age), precondition_(precondition) {}

/*
* Overloaded greater than operator.
* Returns true if Patient1 > Patient2, false if not.
*/
bool operator>(const Patient& p1, const Patient& p2) {
    if ((p1.precondition_ && p2.precondition_) ||
        (!p1.precondition_ && !p2.precondition_)) {
        return (p1.age_ > p2.age_);
    } else if (p1.precondition_ && !p2.precondition_) {
        return true;
    } else if (!p1.precondition_ && p2.precondition_) {
        return false;
    }
    return false;
}

/*
* Overloaded less than operator.
* Returns true if Patient1 < Patient2, false if not.
*/
bool operator<(const Patient& p1, const Patient& p2) {
    if ((p1.precondition_ && p2.precondition_) ||
        (!p1.precondition_ && !p2.precondition_)) {
        return (p1.age_ < p2.age_);
    } else if (p1.precondition_ && !p2.precondition_) {
        return false;
    } else if (!p1.precondition_ && p2.precondition_) {
        return true;
    }
    return false;
}

/*
* Getter method for name_
*/
std::string Patient::GetName() const {
    return name_;
}

/*
* Getter method for age_
*/
int Patient::GetAge() const {
    return age_;
}

/*
* Getter method for precondition_
*/
bool Patient::GetPrecondition() const {
    return precondition_;
}
