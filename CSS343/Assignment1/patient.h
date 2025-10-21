#ifndef PATIENT_H_
#define PATIENT_H_
#include <iostream>
#include <string>

using std::string;

/*
* patient.h
* Represents a patient to be added to a COVIDPriorityQueue
* April 7, 2024
* Kyle Ricks
*/
class Patient {
 public:
    /*
    * Overloaded constructor.
    * name is a string passed by value to be assigned to the patient.
    * age is a int passed by value to be assigned to the patient.
    * precondition is a bool passed by value to be assigned to the patient.
    */
    Patient(string name, int age, bool precondition);

    /*
    * Overloaded greater than operator.
    * Returns true if Patient1 > Patient2, false if not.
    */
    friend bool operator>(const Patient& p1, const Patient& p2);

    /*
    * Overloaded less than operator.
    * Returns true if Patient1 < Patient2, false if not.
    */
    friend bool operator<(const Patient& p1, const Patient& p2);

    /*
    * Getter method for name_
    */
    [[nodiscard]] string GetName() const;

    /*
    * Getter method for age_
    */
    [[nodiscard]] int GetAge() const;

    /*
    * Getter method for precondition_
    */
    [[nodiscard]] bool GetPrecondition() const;

 private:
    /*
    * string representing the Patient's name
    */
    string name_;

    /*
    * int representing the Patient's age
    */
    int age_;

    /*
    * bool representing the Patient's existing preconditions
    */
    bool precondition_;
};
#endif
