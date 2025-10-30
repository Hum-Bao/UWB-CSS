#include <iostream>
#include "threadedBST.h"

using namespace std;

void test1() {  // size 1
    const int n = 1;
    const ThreadedBST origTree(n);   // creating a tree
    ThreadedBST newTree = origTree;  // copying original tree
    // remove all evens from newTree
    for (int i = 2; i < n + 1; i += 2) {
        newTree.remove(i);
    }
    //output
    cout << "Original Tree: " << origTree << endl;
    cout << "New Tree: " << newTree << endl << endl;
}

void test2() {  // size 4
    const int n = 4;
    ThreadedBST origTree(n);         // creating a tree
    ThreadedBST newTree = origTree;  // copying original tree
    // remove all evens from newTree
    for (int i = 2; i < n + 1; i += 2) {
        newTree.remove(i);
    }
    //output
    cout << "Original Tree: " << origTree << endl;
    cout << "New Tree: " << newTree << endl << endl;
}

int main() {

    test1();
    test2();

    return 0;
}
