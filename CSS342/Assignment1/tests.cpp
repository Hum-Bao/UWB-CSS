#include <iostream>
#include <sstream>
#include "time_span.h"
using namespace std;

bool CheckValues(TimeSpan time, int hours, int minutes, int seconds) {
    if ((time.hours() != hours) || (time.minutes() != minutes) ||
        (time.seconds() != seconds)) {
        return false;
    }
    return true;
}

bool TestToSeconds() {
    TimeSpan ts(1, 2, 3);
    return ts.ToSeconds() == 3723;
}

bool TestLessThan() {
    TimeSpan ts1(1, 2, 3);
    TimeSpan ts2(2, 3, 4);
    return ts1 < ts2;
}

bool TestLessThanOrEqual() {
    TimeSpan ts1(1, 2, 3);
    TimeSpan ts2(1, 2, 3);
    return ts1 <= ts2;
}

bool TestGreaterThan() {
    TimeSpan ts1(1, 2, 3);
    TimeSpan ts2(2, 3, 4);
    return ts2 > ts1;
}

bool TestGreaterThanOrEqual() {
    TimeSpan ts1(2, 3, 4);
    TimeSpan ts2(2, 3, 4);
    return ts2 >= ts1;
}

bool TestEqual() {
    TimeSpan ts1(2, 3, 4);
    TimeSpan ts2(2, 3, 4);
    return ts1 == ts2;
}

bool TestNotEqual() {
    TimeSpan ts1(2, 3, 5);
    TimeSpan ts2(2, 3, 4);
    return ts1 != ts2;
}

bool TestNegation() {
    TimeSpan ts(1, 2, 3);
    ts = -ts;
    return CheckValues(ts, -1, -2, -3);
}

bool TestLargeValues() {
    TimeSpan ts1(1000, 120, 180);
    return CheckValues(ts1, 1002, 3, 0);
}

bool TestCombineAddSubtract() {
    TimeSpan ts1(10, 30, 45);
    TimeSpan ts2(2, 15, 30);
    TimeSpan ts3 = ts1 + ts2 - ts1;
    return CheckValues(ts3, 2, 15, 30);
}

bool TestChainedOperations() {
    TimeSpan ts1(5, 10, 15);
    TimeSpan ts2(1, 2, 3);
    TimeSpan ts3 = ts1 + ts2 - ts2 + ts1;
    return CheckValues(ts3, 10, 20, 30);
}

bool TestZeros() {
    TimeSpan ts(0, 0, 0);
    return CheckValues(ts, 0, 0, 0);
}

bool TestFloatSeconds() {
    TimeSpan ts(127.86);
    return CheckValues(ts, 0, 2, 8);
}

bool TestNegativeSecond() {
    TimeSpan ts(1.5, 4, -10);
    if (!CheckValues(ts, 1, 33, 50)) {
        return false;
    }

    ts = TimeSpan(7, -3);
    if (!CheckValues(ts, 0, 6, 57)) {
        return false;
    }

    ts = TimeSpan(-190);
    if (!CheckValues(ts, 0, -3, -10)) {
        return false;
    }
    return true;
}

bool TestNegativeMinute() {
    TimeSpan ts(8, -23, 0);
    if (!CheckValues(ts, 7, 37, 0)) {
        return false;
    }

    return true;
}

bool TestNegativeHour() {
    TimeSpan ts(-3, 73, 2);
    return CheckValues(ts, -1, -46, -58);
}

bool TestAdd() {
    TimeSpan ts1, ts2(5), ts3(7, 0), ts4(4, 0, 0);
    TimeSpan add_it_up = ts1 + ts2 + ts3 + ts4;
    return CheckValues(add_it_up, 4, 7, 5);
}

bool TestSubtract() {
    TimeSpan ts1, ts2(5), ts3(7, 0), ts4(4, 0, 0);
    TimeSpan subtract_it_up = ts4 - ts3 - ts2 - ts1;
    return CheckValues(subtract_it_up, 3, 52, 55);
}

bool TestAddInPlace() {
    TimeSpan ts1(5, 6, 7);
    TimeSpan ts2(1, 1, 1);
    if ((!CheckValues(ts1, 5, 6, 7)) || (!CheckValues(ts2, 1, 1, 1))) {
        return false;
    }
    ts1 += ts2;
    if ((!CheckValues(ts1, 6, 7, 8)) || (!CheckValues(ts2, 1, 1, 1))) {
        return false;
    }
    return true;
}

bool TestSubtractInPlace() {
    TimeSpan ts1(5, 6, 7);
    TimeSpan ts2(1, 1, 1);
    if ((!CheckValues(ts1, 5, 6, 7)) || (!CheckValues(ts2, 1, 1, 1))) {
        return false;
    }
    ts1 -= ts2;
    if ((!CheckValues(ts1, 4, 5, 6)) || (!CheckValues(ts2, 1, 1, 1))) {
        return false;
    }
    return true;
}

bool TestInputStream() {
    stringstream input_stream("2 3 4");
    TimeSpan ts;
    input_stream >> ts;
    return CheckValues(ts, 2, 3, 4);
}

int main() {
    cout << "Testing TimeSpan Class" << endl;
    if (!TestToSeconds())
        cout << "Failed: TestToSeconds" << endl;

    if (!TestLessThan())
        cout << "Failed: TestLessThan" << endl;
    if (!TestLessThanOrEqual())
        cout << "Failed: TestLessThanOrEqual" << endl;

    if (!TestGreaterThan())
        cout << "Failed: TestGreaterThan" << endl;
    if (!TestGreaterThanOrEqual())
        cout << "Failed: TestGreaterThanOrEqual" << endl;

    if (!TestEqual())
        cout << "Failed: TestEqual" << endl;
    if (!TestNotEqual())
        cout << "Failed: TestNotEqual" << endl;

    if (!TestNegation())
        cout << "Failed: TestNegation" << endl;

    if (!TestLargeValues())
        cout << "Failed: TestLargeValues" << endl;

    if (!TestCombineAddSubtract())
        cout << "Failed: TestCombineAddSubtract" << endl;

    if (!TestChainedOperations())
        cout << "Failed: TestChainedOperations" << endl;

    if (!TestZeros())
        cout << "Failed: TestZeros" << endl;
    if (!TestFloatSeconds())
        cout << "Failed: TestFloatSeconds" << endl;
    if (!TestNegativeSecond())
        cout << "Failed: TestNegativeSecond" << endl;
    if (!TestNegativeMinute())
        cout << "Failed: TestNegativeMinute" << endl;
    if (!TestNegativeHour())
        cout << "Failed: TestNegativeHour" << endl;
    if (!TestAdd())
        cout << "Failed: TestAdd" << endl;
    if (!TestAddInPlace())
        cout << "Failed: TestAddInPlace" << endl;
    if (!TestSubtract())
        cout << "Failed: TestSubtract" << endl;
    if (!TestSubtractInPlace())
        cout << "Failed: TestSubtractInPlace" << endl;

    if (!TestInputStream())
        cout << "Failed: TestInputStream" << endl;
    cout << "Testing Complete" << endl;
}
