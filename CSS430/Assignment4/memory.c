#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMSIZE 80

char memory_pool[MEMSIZE];

bool executeCommand(char* line);

bool readFile(char* file_name);

bool worstFit(char** arr) {
    int size = strtol(arr[2], NULL, 10);
    int worst_index = -1;
    int worst_length = 0;

    int index = 0;
    int length = 0;

    for (int count = 0; count < MEMSIZE; count++) {
        if (memory_pool[count] == '.') {
            length++;
        } else {
            if (length >= size && length > worst_length) {
                worst_index = index;
                worst_length = length;
            }
            index = count + 1;
            length = 0;
        }
    }

    if (length >= size && length > worst_length) {
        worst_index = index;
    }

    if (worst_index != -1) {
        memset(memory_pool + worst_index, *arr[1], size);
        return true;
    }
    return false;
}

bool bestFit(char** arr) {
    int size = strtol(arr[2], NULL, 10);
    int best_index = -1;
    int best_length = MEMSIZE + 1;

    int index = 0;
    int length = 0;

    for (int count = 0; count < MEMSIZE; count++) {
        if (memory_pool[count] == '.') {
            length++;
        } else {
            if (length >= size && length < best_length) {
                best_index = index;
                best_length = length;
            }
            index = count + 1;
            length = 0;
        }
    }

    if (length >= size && length < best_length) {
        best_index = index;
    }

    if (best_index != -1) {
        memset(memory_pool + best_index, *arr[1], size);
        return true;
    }
    return false;
}

bool firstFit(char** arr) {
    int size = strtol(arr[2], NULL, 10);
    int length = 0;
    int index = 0;
    for (int count = 0; count < MEMSIZE; count++) {
        if (memory_pool[count] == '.') {
            length++;
        } else {
            index = count + 1;
            length = 0;
        }

        if (length == size) {
            break;
        }
    }

    if (length == size) {
        memset(memory_pool + index, *arr[1], size);
        return true;
    }
    return false;
}

void compact() {
    int to_index = -1;

    for (int i = 0; i < MEMSIZE; i++) {
        if (memory_pool[i] == '.') {
            if (to_index == -1) {
                to_index = i;
            }
        } else if (to_index != -1) {

            char name = memory_pool[i];
            int from_start = i;
            int from_end = i;

            while (from_end < MEMSIZE && memory_pool[from_end] == name) {
                from_end++;
            }
            int length = from_end - from_start;

            memmove(memory_pool + to_index, memory_pool + from_start, length);

            memset(memory_pool + to_index + length, '.', from_start - to_index);

            to_index += length;
            i = from_end - 1;
        }
    }
}

void freeMemory(char** arr) {
    int start = -1;
    int end = -1;
    char name = *arr[1];
    for (int count = 0; count < MEMSIZE; count++) {
        if (memory_pool[count] == name) {
            if (start == -1) {
                start = count;
            }
            end = count;
        } else {
            if (start != -1) {
                end++;
                //printf("FREEING: %d BLOCKS FROM POS: %d - %d\n", end - start,start, end);
                memset(memory_pool + start, '.', end - start);
                start = -1;
                end = -1;
            }
        }
    }

    if (start != -1) {
        end++;
        //printf("FREEING: %d BLOCKS FROM POS: %d - %d\n", end - start, start,end);
        memset(memory_pool + start, '.', end - start);
    }
}

void split(char** arr, char* line) {
    char* temp = strtok(line, " ");
    int count = 0;
    while (temp != NULL) {
        arr[count++] = temp;
        temp = strtok(NULL, " ");
    }
}

bool executeCommand(char* line) {
    switch (line[0]) {
        case 'E': {
            return true;
        }
        case 'A': {
            char* split_command[4];
            split(split_command, line);
            switch (*split_command[3]) {
                case 'F': {
                    firstFit(split_command);
                    break;
                }
                case 'B': {
                    bestFit(split_command);
                    break;
                }
                case 'W': {
                    worstFit(split_command);
                    break;
                }
            }
            break;
        }
        case 'F': {
            char* split_command[2];
            split(split_command, line);
            freeMemory(split_command);
            break;
        }
        case 'S': {
            for (int count = 0; count < MEMSIZE; count++) {
                printf("%c", memory_pool[count]);
            }
            printf("\n");
            break;
        }
        case 'R': {
            char* split_command[2];
            split(split_command, line);
            if (readFile(split_command[1])) {
                return true;
            }
            break;
        }
        case 'C': {
            compact();
            break;
        }
    }
    return false;
}

bool readFile(char* file_name) {
    FILE* commands = fopen(file_name, "r");
    if (commands == NULL) {
        printf("File read failure\n");
        return false;
    }
    char line[16];

    while (fgets(line, sizeof(line), commands) != NULL) {
        line[strcspn(line, "\n")] = 0;
        //printf("READING LINE: %s\n", line);
        if (executeCommand(line)) {
            fclose(commands);
            return true;
        }
    }

    fclose(commands);
    return false;
}

void getLine(char** line) {
    size_t length = 0;
    ssize_t read = getline(line, &length, stdin);

    if (read == -1) {
        printf("Reading error\n");
        exit(EXIT_FAILURE);
    }
    if ((*line)[read - 1] == '\n') {
        (*line)[read - 1] = '\0';
    }
}

void read() {
    char* line = NULL;
    int exit = 0;
    while (exit == 0) {
        getLine(&line);
        exit = executeCommand(line);
        free(line);
        line = NULL;
    }
}

void initiateMemory() {
    for (int count = 0; count < MEMSIZE; count++) {
        memory_pool[count] = '.';
    }
}

int main() {
    initiateMemory();
    read();
}