#include <iostream>
#include "child.h"
#include "list342.h"
using namespace std;
int main() {
    Child c1("Angie", "Ham", 7), c2("Pradnya", "Dhala", 8),
        c3("Bill", "Vollmann", 13), c4("Cesar", "Ruiz", 6);
    Child c5("Piqi", "Tangi", 7), c6("Russell", "Wilson", 13),
        c7("Hank", "Aaron", 3), c8("Madison", "Fife", 7);
    Child c9("Miles", "Davis", 65), c10("John", "Zorn", 4), c11;
    List342<Child> class1, class2, soccer, chess;
    int a = 1, b = -1, c = 13;
    class1.Insert(&c1);
    class1.Insert(&c2);
    class1.Insert(&c3);
    class1.Insert(&c4);
    class1.Insert(&c5);
    class1.Insert(&c6);
    class1.Insert(&c5);
    cout << "class1: " << class1 << endl;
    if (!class1.Insert(&c1)) {
        cout << "ERROR::: Duplicate" << endl;
    }
    class2.Insert(&c4);
    class2.Insert(&c5);
    class2.Insert(&c6);
    class2.Insert(&c7);
    class2.Insert(&c10);
    cout << "Class2: " << class2 << endl;
    class1.Merge(class2);
    class2.Merge(class1);
    class1.Merge(class2);
    class1.Merge(class1);
    cout << "class1 and 2 Merged: " << class1 << endl;
    class1.Remove(c4, c11);
    class1.Remove(c5, c11);
    class1.Remove(c11, c11);
    if (class1.Remove(c1, c11)) {
        cout << "Removed from class1, student " << c11 << endl;
    }
    cout << "class1: " << class1 << endl;
    soccer.Insert(&c6);
    soccer.Insert(&c4);
    soccer.Insert(&c9);
    cout << "soccer: " << soccer << endl;
    soccer += class1;
    cout << "soccer += class1 : " << soccer << endl;
    List342<Child> football = soccer;
    if (football == soccer) {
        cout << "football: " << football << endl;
    }
    if (football.Peek(c6, c11)) {
        cout << c11 << " is on the football team" << endl;
    }
    soccer.DeleteList();
    List342<int> numbers;
    numbers.Insert(&a);
    numbers.Insert(&b);
    numbers.Insert(&c);
    cout << "These are the numbers: " << numbers << endl;
    numbers.DeleteList();
}