// Simple Child class to help test the LIST 342 object
#ifndef CHILD_H_
#define CHILD_H_
#include <iostream>
#include <string>
using namespace std;
class Child {
    friend ostream& operator<<(ostream& stream, const Child& child);
    friend istream& operator>>(istream& stream, Child& child);

public:
    Child();
    Child(string first_name, string last_name, int age);
    Child(string first_name, string last_name);
    string name() const;
    int age() const;
    void set_first_name(const string&);
    void set_last_name(const string&);
    // Operator Overloads
    bool operator<(const Child&) const;
    bool operator<=(const Child&) const;
    bool operator>=(const Child&) const;
    bool operator>(const Child&) const;
    bool operator==(const Child&) const;
    bool operator!=(const Child&) const;

private:
    string first_name_;
    string last_name_;
    int age_;
};
#endif
