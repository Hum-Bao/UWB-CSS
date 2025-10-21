#include "child.h"
// Constructors
Child::Child() {
    age_ = 0;
}
Child::Child(string first_name, string last_name, int age) {
    first_name_ = first_name;
    last_name_ = last_name;
    age_ = age;
}
Child::Child(string first_name, string last_name) {
    first_name_ = first_name;
    last_name_ = last_name;
    age_ = 0;
}
string Child::name() const {
    return (first_name_ + last_name_);
}
int Child::age() const {
    return age_;
}
void Child::set_first_name(const string& name) {
    first_name_ = name;
}
void Child::set_last_name(const string& name) {
    last_name_ = name;
}
// Operator Overloads
bool Child::operator==(const Child& child) const {
    return ((first_name_ == child.first_name_) &&
            (last_name_ == child.last_name_) &&
            (age_ == child.age_));
}
bool Child::operator!=(const Child& child) const {
    return !(*this == child);
}
bool Child::operator<(const Child& child) const {
    // Compare last names, then first names, then age.
    if (last_name_ < child.last_name_) {
        return true;
    } else if (last_name_ > child.last_name_) {
        return false;
    }
    if (first_name_ < child.first_name_) {
        return true;
    } else if (first_name_ > child.first_name_) {
        return false;
    }
    return (age_ < child.age_);
}
bool Child::operator<=(const Child& child) const {
    return ((*this == child) || (*this < child));
}
bool Child::operator>(const Child& child) const {
    return !(*this <= child);
}
bool Child::operator>=(const Child& child) const {
    return ((*this == child) || (*this > child));
}
ostream& operator<<(ostream& stream, const Child& child) {
    stream << child.first_name_ << child.last_name_ << child.age_;
    return stream;
}
istream& operator>>(istream& stream, Child& child) {
    stream >> child.first_name_;
    stream >> child.last_name_;
    stream >> child.age_;
    return stream;
}
