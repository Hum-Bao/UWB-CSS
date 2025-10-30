#include "catalan.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    cout << Catalan().computeCatalanNum(stoi(argv[1])) << endl;
    return 0;
}

// Computes the Cataln number. Numbers over 20 are too large for an unsigned integer,
// so they are not accepted. Negative numbers are also not accepted.
int Catalan::computeCatalanNum(int n) {
    if (n < 0) {
        // throw std::invalid_argument("Input number outside of acceptable range (n >= 0)");
        cout << "Input number outside of acceptable range (n >= 0)" << endl;
        return -1;
    }
    if (n <= 1) {
        return 1;
    }
    if (n >= 20) {
        // throw std::overflow_error("Calculation exceeds limits of unsigned integer");
        cout << "Calculation exceeds limits of unsigned integer" << endl;
        return -1;
    }
    unsigned int val = 0;
    for (int i = 0; i < n; i++) {
        val += computeCatalanNum(i) * computeCatalanNum(n - 1 - i);
    }
    return val;
}