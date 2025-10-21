#include <sys/time.h>
#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "sorts.h"
using namespace std;
// Global functions
void InitVector(vector<int>& item_vetor, int size);
void PrintVector(const vector<int>& item_vector, string name);
int Elapsed(const timeval& start, const timeval& end);
void NewInitVector(vector<int>& item_vetor, int size);
void Benchmark();

int main(int argc, char* argv[]) {
    if (argv[1] != nullptr && string(argv[1]) == "Benchmark") {
        Benchmark();
        return 0;
    }
    int size = 0;
    string sort_name = "";
    bool print_out = false;
    if ((argc != 3) && (argc != 4)) {
        cerr << "Usage: Sorter algorithm size [Print]" << endl;
        return -1;
    }
    sort_name = string(argv[1]);
    size = atoi(argv[2]);
    if (size <= 0) {
        cerr << "Vector size must be positive" << endl;
        return -1;
    }
    if (argc == 4) {
        string print_arr = string(argv[3]);
        if (print_arr == "Print") {
            print_out = true;
        } else {
            cerr << "Usage: Sorter algorithm size [Print]" << endl;
            return -1;
        }
    }
    vector<int> items(size);

    srand(1);
    InitVector(items, size);

    // NewInitVector(items, size);
    if (print_out) {
        cout << "Initial:" << endl;
        PrintVector(items, string("items"));
        cout << endl;
    }
    // get time to measure the time it takes to sort
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    //
    // PLACE YOUR CODE HERE
    // ...Determine which sort to call on the vector
    // ...The code below only looks for QuickSort
    // Other Signatures:
    // BubbleSort(items, 0, size - 1)
    // InsertionSort(items, 0, size – 1)
    // MergeSort(items, 0, size – 1)
    // IterativeMergeSort(items, 0, size – 1)
    // ShellSort(items, 0, size – 1)
    // PLACE YOUR CODE HERE; for instance:
    if (sort_name == "BubbleSort") {
        BubbleSort(items, 0, size - 1);
    } else if (sort_name == "InsertionSort") {
        InsertionSort(items, 0, size - 1);
    } else if (sort_name == "MergeSort") {
        MergeSort(items, 0, size - 1);
    } else if (sort_name == "IterativeMergeSort") {
        IterativeMergeSort(items, 0, size - 1);
    } else if (sort_name == "ShellSort") {
        ShellSort(items, 0, size - 1);
    } else if (sort_name == "QuickSort") {
        QuickSort(items, 0, size - 1);
    }
    gettimeofday(&end_time, 0);
    if (print_out) {
        cout << "Sorted:" << endl;
        PrintVector(items, string("item"));
    }
    cout << "Time (microsecs): " << Elapsed(start_time, end_time) << endl;
    return 0;
}

std::string datetime() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%d-%m-%Y %H-%M-%S", timeinfo);
    return std::string(buffer);
}

void NewInitVector(vector<int>& items, int rand_max) {
    if (rand_max < 0) {
        return;
    }
    for (int i = 0; i < rand_max; i++) {
        items[i] = i;
    }
    // auto random_device = std::random_device{};
    std::random_device rd;
    auto rng = std::default_random_engine{rd()};
    // auto rng = std::default_random_engine{random_device()};
    std::shuffle(std::begin(items), std::end(items), rng);
}

void Benchmark() {
    int sizes[] = {10,     100,    1000,    2500,    5000,
                   10000,  25000,  50000,   100000,  250000,
                   500000, 750000, 1000000, 2000000, 5000000};
    int sizes_length = end(sizes) - begin(sizes);
    string sorts[] = {"BubbleSort",         "InsertionSort", "MergeSort",
                      "IterativeMergeSort", "ShellSort",     "QuickSort"};
    int sorts_length = end(sorts) - begin(sorts);
    std::cout << sorts_length << endl;
    vector<int> items;

    struct timeval start_time, end_time;
    struct timeval file_name;
    gettimeofday(&file_name, 0);
    ofstream SortFile(datetime() + ".txt");

    for (int sort_size = 0; sort_size < sizes_length; sort_size++) {
        for (int sort_type = 0; sort_type < sorts_length; sort_type++) {
            cout << "CURRENT SORT: " << sorts[sort_type]
                 << " | SIZE: " << sizes[sort_size] << endl;

            SortFile << sorts[sort_type] << " " << sizes[sort_size] << ": "
                     << endl;

            for (int runs = 0; runs < 10; runs++) {
                if ((sorts[sort_type] == "BubbleSort" ||
                     sorts[sort_type] == "InsertionSort") &&
                    sizes[sort_size] >= 500000) {
                    break;
                }
                items = vector<int>(sizes[sort_size]);
                NewInitVector(items, sizes[sort_size]);

                if (sorts[sort_type] == "BubbleSort") {
                    gettimeofday(&start_time, 0);
                    BubbleSort(items, 0, sizes[sort_size] - 1);
                } else if (sorts[sort_type] == "InsertionSort") {
                    gettimeofday(&start_time, 0);
                    InsertionSort(items, 0, sizes[sort_size] - 1);
                } else if (sorts[sort_type] == "MergeSort") {
                    gettimeofday(&start_time, 0);
                    MergeSort(items, 0, sizes[sort_size] - 1);
                } else if (sorts[sort_type] == "IterativeMergeSort") {
                    gettimeofday(&start_time, 0);
                    IterativeMergeSort(items, 0, sizes[sort_size] - 1);
                } else if (sorts[sort_type] == "ShellSort") {
                    gettimeofday(&start_time, 0);
                    ShellSort(items, 0, sizes[sort_size] - 1);
                } else if (sorts[sort_type] == "QuickSort") {
                    gettimeofday(&start_time, 0);
                    QuickSort(items, 0, sizes[sort_size] - 1);
                }
                gettimeofday(&end_time, 0);
                SortFile << Elapsed(start_time, end_time) << endl;
                cout << "Time (microsecs): " << Elapsed(start_time, end_time)
                     << endl;
            }
        }
    }
    SortFile.close();
}

void InitVector(vector<int>& item_vector, int rand_max) {
    if (rand_max < 0) {
        return;
    }
    vector<int> pool(rand_max);
    for (int i = 0; i < rand_max; i++) {
        pool[i] = i;
    }
    int spot;
    for (int i = 0; i < rand_max; i++) {
        spot = rand() % (pool.size());
        item_vector[i] = pool[spot];
        pool.erase(pool.begin() + spot);
    }
}

void PrintVector(const vector<int>& item_vector, string name) {
    int size = item_vector.size();
    for (int i = 0; i < size; i++) {
        cout << item_vector[i] << " ";
    }
    cout << endl;
}

// Function to calculate elapsed time
// Microseconds
int Elapsed(const timeval& start, const timeval& end) {
    return (end.tv_sec - start.tv_sec) * 1000000 +
           (end.tv_usec - start.tv_usec);
}
