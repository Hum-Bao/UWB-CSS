#include "time_span.h"
#include <cmath>
#include <iostream>

// Convert decimal values of hours to minutes, and decimal values of minutes
// to seconds. Everything is added to seconds, and then formatted to a normal
// TimeSpan using the formatTime() method.
TimeSpan::TimeSpan(double hours, double minutes, double seconds)
    : hours_(0),
      minutes_(0),
      seconds_(((int(hours) * 60) * 60) +
               ((int)std::round(minutes + (((hours - int(hours)) * 60))) * 60) +
               (int)std::round(seconds + ((minutes - int(minutes)) * 60))) {
    FormatTime();
}

TimeSpan::TimeSpan(double minutes, double seconds)
    : TimeSpan(0.0, minutes, seconds) {}

TimeSpan::TimeSpan(double seconds) : TimeSpan(0.0, 0.0, seconds) {}

TimeSpan::TimeSpan() : TimeSpan(0.0, 0.0, 0.0) {}

int TimeSpan::hours() const {
    return hours_;
}

int TimeSpan::minutes() const {
    return minutes_;
}

int TimeSpan::seconds() const {
    return seconds_;
}

// Converts seconds into minutes and minutes into hours using modulo.
void TimeSpan::FormatTime() {
    minutes_ += seconds_ / 60;
    seconds_ %= 60;
    hours_ += minutes_ / 60;
    minutes_ %= 60;

    if (hours_ > 0 && minutes_ < 0) {
        hours_--;
        minutes_ += 60;
    }
    if (minutes_ > 0 && seconds_ < 0) {
        minutes_--;
        seconds_ += 60;
    }
}

void TimeSpan::Set_Time(int hours, int minutes, int seconds) {
    hours_ = hours;
    minutes_ = minutes;
    seconds_ = seconds;
    FormatTime();
}

// Converts everything to seconds
int TimeSpan::ToSeconds() const {
    return ((hours_ * 60) * 60) + (minutes_ * 60) + seconds_;
}

TimeSpan& TimeSpan::operator+=(TimeSpan const& ts) & {
    Set_Time(hours_ += ts.hours_, minutes_ += ts.minutes_,
             seconds_ += ts.seconds_);
    return *this;
}

TimeSpan& TimeSpan::operator-=(TimeSpan const& ts) & {
    Set_Time(hours_ -= ts.hours_, minutes_ -= ts.minutes_,
             seconds_ -= ts.seconds_);
    return *this;
}

TimeSpan TimeSpan::operator-() {
    return TimeSpan(-hours_, -minutes_, -seconds_);
}

bool TimeSpan::operator<=(TimeSpan const& ts) & {
    return ToSeconds() <= ts.ToSeconds();
}

bool TimeSpan::operator>=(TimeSpan const& ts) & {
    return ToSeconds() >= ts.ToSeconds();
}

bool TimeSpan::operator<(TimeSpan const& ts) & {
    return ToSeconds() < ts.ToSeconds();
}

bool TimeSpan::operator>(TimeSpan const& ts) & {
    return ToSeconds() > ts.ToSeconds();
}

bool operator==(TimeSpan const& ts1, TimeSpan const& ts2) {
    return (ts1.hours_ == ts2.hours_) && (ts1.minutes_ == ts2.minutes_) &&
           (ts1.seconds_ == ts2.seconds_);
}

bool operator!=(TimeSpan const& ts1, TimeSpan const& ts2) {
    return !(ts1 == ts2);
}

TimeSpan operator+(TimeSpan ts1, TimeSpan const& ts2) {
    ts1 += ts2;
    return ts1;
}

TimeSpan operator-(TimeSpan ts1, TimeSpan const& ts2) {
    ts1 -= ts2;
    return ts1;
}

std::ostream& operator<<(std::ostream& os, TimeSpan const& ts) {
    return os << "Hours: " << ts.hours() << ", Minutes: " << ts.minutes()
              << ", Seconds: " << ts.seconds();
}

std::istream& operator>>(std::istream& is, TimeSpan& ts) {
    int hours, minutes, seconds;
    is >> hours >> minutes >> seconds;
    ts.Set_Time(hours, minutes, seconds);
    return is;
}
