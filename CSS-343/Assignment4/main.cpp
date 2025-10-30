#include <cassert>
#include <iostream>
#include <string>
#include "classics.h"
#include "moviestore.h"

void TestClassics() {
    const Classics temp_classics;
    assert(temp_classics.GetYear() == -1);
}

int main() {
    TestClassics();
    MovieStore store = MovieStore();
    store.ReadMovies("data4movies.txt");
    store.ReadCustomers("data4customers.txt");
    store.ReadCommands("data4commands.txt");

    cout << "Done." << "\n";
}