#include <iostream>
#ifndef TIMESPAN_H_
#define TIMESPAN_H_

class TimeSpan {
 public:
    // Constructors for the TimeSpan
    TimeSpan(double hours, double minutes, double seconds);
    TimeSpan(double minutes, double seconds);
    TimeSpan(double seconds);
    TimeSpan();

    // Getters
    int hours() const;
    int minutes() const;
    int seconds() const;

    // Setter method
    void Set_Time(int hours, int minutes, int seconds);

    // Convert time to seconds for comparisons
    int ToSeconds() const;

    // Operator overloads
    TimeSpan& operator+=(TimeSpan const& ts) &;
    TimeSpan& operator-=(TimeSpan const& ts) &;
    TimeSpan operator-();
    bool operator<=(TimeSpan const& ts) &;
    bool operator>=(TimeSpan const& ts) &;
    bool operator<(TimeSpan const& ts) &;
    bool operator>(TimeSpan const& ts) &;
    // bool operator==(TimeSpan const& ts) const;
    // bool operator!=(TimeSpan const& ts) const;
    friend bool operator==(TimeSpan const& ts1, TimeSpan const& ts2);
    friend bool operator!=(TimeSpan const& ts1, TimeSpan const& ts2);
    friend TimeSpan operator+(TimeSpan ts1, TimeSpan const& ts2);
    friend TimeSpan operator-(TimeSpan ts1, TimeSpan const& ts2);
    friend std::ostream& operator<<(std::ostream& os, const TimeSpan& ts);
    friend std::istream& operator>>(std::istream& is, TimeSpan& ts);

 private:
    // Method for formatting times that don't fall within the specified bounds
    // (-60 < seconds < 60 && -60 < minutes < 60)
    void FormatTime();
    // Values for TimeSpan
    int hours_;
    int minutes_;
    int seconds_;
};
#endif
