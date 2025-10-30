#ifndef SORTS_H_
#define SORTS_H_
#include <algorithm>
#include <array>
#include <vector>
#define INSERTION_SORT_THRESHOLD 32
using std::vector;
// Global functions
void Swap(vector<int>& items, int first_index, int second_index);
void BubbleSort(vector<int>& items, int start_index, int end_index);
void InsertionSort(vector<int>& items, int start_index, int end_index);
void Merge(vector<int>& items, int start_index, int mid_index, int end_index,
           vector<int>& left_half, vector<int>& right_half);
void MergeSortHelper(vector<int>& items, int start_index, int end_index,
                     vector<int>& left_half, vector<int>& right_half);
void MergeSort(vector<int>& items, int start_index, int end_index);
void IterativeMerge(vector<int>& items, int start_index, int mid_index,
                    int end_index, vector<int>& temp);
void IterativeMergeSort(vector<int>& items, int start_index, int end_index);
void ShellSort(vector<int>& items, int start_index, int end_index);
void QuickSort(vector<int>& items, int start_index, int end_index);

void Swap(vector<int>& items, int first_index, int second_index) {
    int temp = items[first_index];
    items[first_index] = items[second_index];
    items[second_index] = temp;
}

void BubbleSort(vector<int>& items, int start_index, int end_index) {
    int count = 0;
    bool sorted = true;
    while (true) {
        if (count >= end_index) {
            if (sorted) {
                break;
            }
            count = 0;
            sorted = true;
        }
        if (items[count] > items[count + 1]) {
            Swap(items, count, count + 1);
            sorted = false;
        }
        count++;
    }
}

void InsertionSort(vector<int>& items, int start_index, int end_index) {
    for (int count = start_index + 1; count <= end_index; count++) {
        if (items[count] < items[count - 1]) {
            int counter = count;
            while (counter > start_index &&
                   items[counter] < items[counter - 1]) {
                Swap(items, counter, counter - 1);
                counter--;
            }
        }
    }
}

void Merge(vector<int>& items, int start_index, int mid_index, int end_index,
           vector<int>& left_half, vector<int>& right_half) {
    int n1 = mid_index - start_index + 1;  // size of the first half
    int n2 = end_index - mid_index;        // size of the second half

    std::copy(items.begin() + start_index, items.begin() + start_index + n1,
              left_half.begin());
    std::copy(items.begin() + mid_index + 1, items.begin() + mid_index + 1 + n2,
              right_half.begin());
    int i = 0;
    int j = 0;
    int k = start_index;

    while (i < n1 && j < n2) {
        if (left_half[i] <= right_half[j]) {
            items[k] = left_half[i];
            i++;
        } else {
            items[k] = right_half[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        items[k] = left_half[i];
        i++;
        k++;
    }

    while (j < n2) {
        items[k] = right_half[j];
        j++;
        k++;
    }
}

void MergeSortHelper(vector<int>& items, int start_index, int end_index,
                     vector<int>& left_half, vector<int>& right_half) {
    if (end_index - start_index + 1 <= INSERTION_SORT_THRESHOLD) {
        InsertionSort(items, start_index, end_index);
        return;
    }
    if (start_index < end_index) {
        int mid_index = start_index + (end_index - start_index) / 2;

        MergeSortHelper(items, start_index, mid_index, left_half, right_half);
        MergeSortHelper(items, mid_index + 1, end_index, left_half, right_half);

        Merge(items, start_index, mid_index, end_index, left_half, right_half);
    }
}

void MergeSort(vector<int>& items, int start_index, int end_index) {
    if (end_index <= 1) {
        return;
    }

    vector<int> left_half(end_index / 2);
    vector<int> right_half(end_index - end_index / 2);

    MergeSortHelper(items, start_index, end_index, left_half, right_half);
}

void IterativeMerge(vector<int>& items, int start_index, int mid_index,
                    int end_index, vector<int>& temp) {
    int i = start_index;
    int j = mid_index + 1;
    int k = start_index;

    while (i <= mid_index && j <= end_index) {
        if (items[i] <= items[j]) {
            temp[k++] = items[i++];
        } else {
            temp[k++] = items[j++];
        }
    }

    while (i <= mid_index) {
        temp[k++] = items[i++];
    }

    while (j <= end_index) {
        temp[k++] = items[j++];
    }

    for (int idx = start_index; idx <= end_index; ++idx) {
        items[idx] = temp[idx];
    }
}

void IterativeMergeSort(vector<int>& items, int start_index, int end_index) {
    if (end_index - start_index + 1 <= 1) {
        return;
    }
    if (end_index - start_index + 1 <= INSERTION_SORT_THRESHOLD) {
        InsertionSort(items, start_index, end_index);
        return;
    }
    vector<int> temp(end_index - start_index + 1);

    for (int current_size = 1; current_size <= (end_index - start_index);
         current_size *= 2) {
        for (int idx = start_index; idx < end_index; idx += 2 * current_size) {
            int mid_index = std::min(idx + current_size - 1, end_index);
            int end_of_merge = std::min(idx + 2 * current_size - 1, end_index);
            IterativeMerge(items, idx, mid_index, end_of_merge, temp);
        }
    }
}

void ShellSort(vector<int>& items, int start_index, int end_index) {
    for (int gap = end_index / 2; gap > start_index; gap /= 2) {
        for (int outer = gap; outer <= end_index; outer += 1) {
            int temp = items[outer];
            int inner;
            for (inner = outer; inner >= gap && items[inner - gap] > temp;
                 inner -= gap) {
                items[inner] = items[inner - gap];
            }
            items[inner] = temp;
        }
    }
}

void QuickSort(vector<int>& items, int start_index, int end_index) {
    if (start_index >= end_index) {
        return;
    }
    int left = start_index;
    int right = end_index;
    int pivot = items[(start_index + end_index) / 2];
    while (left <= right) {
        while (items[left] < pivot) {
            left++;
        }
        while (items[right] > pivot) {
            right--;
        }
        if (left <= right) {
            Swap(items, left, right);
            left++;
            right--;
        }
    }
    QuickSort(items, start_index, right);
    QuickSort(items, left, end_index);
}
#endif
