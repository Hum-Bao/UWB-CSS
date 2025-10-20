// Sudoku puzzle verifier and solver

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int grid_size;
bool is_valid = true;
bool is_complete = true;

typedef struct {
        int row;
        int col;
        int** grid;
        // int* seen_arr;
} parameters;

void* checkRows(void* params) {
    parameters* p = (parameters*)params;
    int** grid = p->grid;
    int* arr = (int*)malloc(grid_size * sizeof(int));
    int num = 0;
    for (int row = p->row; row <= grid_size; row++) {
        //Reset arr after each iteration
        for (int i = 0; i < grid_size; i++) {
            arr[i] = 0;
        }

        for (int col = p->col; col <= grid_size; col++) {
            num = grid[row][col];
            if (num == 0) {
                printf("Row %d is NOT COMPLETE\n", row);
                is_complete = false;
            } else {
                if (arr[num - 1] == 1) {
                    printf("Row %d is NOT VALID\n", row);
                    is_valid = false;
                }
                arr[num - 1] = 1;
            }
        }
    }
    free(arr);
    return NULL;
}

void* checkColumns(void* params) {
    parameters* p = (parameters*)params;
    int** grid = p->grid;
    int* arr = (int*)malloc(grid_size * sizeof(int));
    int num = 0;
    for (int col = p->col; col <= grid_size; col++) {
        //Reset arr after each iteration
        for (int i = 0; i < grid_size; i++) {
            arr[i] = 0;
        }

        for (int row = p->row; row <= grid_size; row++) {
            num = grid[row][col];
            if (num == 0) {
                printf("Col %d is NOT COMPLETE\n", col);
                is_complete = false;
            } else {
                if (arr[num - 1] == 1) {
                    printf("Col %d is NOT VALID\n", col);
                    is_valid = false;
                }
                arr[num - 1] = 1;
            }
        }
        //printf("\n");
    }
    free(arr);
    return NULL;
}

void* checkSubGrid(void* params) {
    parameters* p = (parameters*)params;
    int** grid = p->grid;
    int* arr = (int*)malloc(grid_size * sizeof(int));
    int size = (int)sqrt(grid_size);
    int num = 0;

    //Set arr to default values
    for (int i = 0; i < grid_size; i++) {
        arr[i] = 0;
    }

    // Iterate over the subgrid defined by the top-left corner at (p->row, p->col)
    for (int row = p->row; row < p->row + size; row++) {
        for (int col = p->col; col < p->col + size; col++) {
            num = grid[row][col];
            // printf("%d, ", num);
            if (num == 0) {
                printf("Subgrid starting at (%d, %d) is NOT COMPLETE\n", p->row,
                       p->col);
                is_complete = false;
            } else {
                if (arr[num - 1] == 1) {
                    printf("Subgrid starting at (%d, %d) is NOT VALID\n",
                           p->row, p->col);
                    is_valid = false;
                }
                arr[num - 1] = 1;
            }
        }
        // printf("\n");
    }
    // printf("\n");
    free(arr);
    return NULL;
}

// takes puzzle size and grid[][] representing sudoku puzzle
// and two booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int** grid) {
    grid_size = psize;
    pthread_t* grid_threads = malloc((psize + 2) * sizeof(pthread_t));
    parameters* params = malloc((psize + 2) * sizeof(parameters));

    params[0].row = 1;
    params[0].col = 1;
    params[0].grid = grid;

    params[1].row = 1;
    params[1].col = 1;
    params[1].grid = grid;

    pthread_create(&grid_threads[0], NULL, checkRows, (void*)&params[0]);
    pthread_create(&grid_threads[1], NULL, checkColumns, (void*)&params[1]);

    int index = 2;
    int size = (int)sqrt(psize);
    for (int row = 1; row <= (psize - size) + 1; row += size) {
        for (int col = 1; col <= (psize - size) + 1; col += size) {
            // printf("[%d][%d]\n", row, col);
            params[index].row = row;
            params[index].col = col;
            params[index].grid = grid;

            pthread_create(&grid_threads[index], NULL, checkSubGrid,
                           (void*)&params[index]);
            index++;
        }
    }

    for (int i = 0; i < psize + 2; i++) {
        pthread_join(grid_threads[i], NULL);
    }
    free(grid_threads);
    free(params);
}

bool validMove(int** grid, int row, int col, int num) {
    int subgrid_size = (int)sqrt(grid_size);

    // Check row and column
    for (int i = 1; i <= grid_size; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
    }

    // Check subgrid
    int start_row = (((row - 1) / subgrid_size) * subgrid_size) + 1;
    int start_col = (((col - 1) / subgrid_size) * subgrid_size) + 1;
    for (int i = 0; i < subgrid_size; i++) {
        for (int j = 0; j < subgrid_size; j++) {
            if (grid[start_row + i][start_col + j] == num) {
                return false;
            }
        }
    }
    return true;
}

// Backtracking solver function
bool solve(int** grid, int row, int col) {
    if (row > grid_size) {
        return true;
    }
    if (col > grid_size) {
        return solve(grid, row + 1, 1);
    }
    if (grid[row][col] != 0) {
        return solve(grid, row, col + 1);
    }

    for (int num = 1; num <= grid_size; num++) {
        if (validMove(grid, row, col, num)) {
            grid[row][col] = num;
            if (solve(grid, row, col + 1)) {
                return true;
            }
            // Backtrack
            grid[row][col] = 0;
        }
    }
    return false;
}

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char* filename, int*** grid) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    int psize;
    fscanf(fp, "%d", &psize);
    int** agrid = (int**)malloc((psize + 1) * sizeof(int*));
    for (int row = 1; row <= psize; row++) {
        agrid[row] = (int*)malloc((psize + 1) * sizeof(int));
        for (int col = 1; col <= psize; col++) {
            fscanf(fp, "%d", &agrid[row][col]);
        }
    }
    fclose(fp);
    *grid = agrid;
    return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int** grid) {
    printf("%d\n", psize);
    for (int row = 1; row <= psize; row++) {
        for (int col = 1; col <= psize; col++) {
            printf("%d ", grid[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int** grid) {
    for (int row = 1; row <= psize; row++) {
        free(grid[row]);
    }
    free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: ./sudoku puzzle.txt\n");
        return EXIT_FAILURE;
    }
    // grid is a 2D array
    int** grid = NULL;
    // find grid size and fill grid
    int sudokuSize = readSudokuPuzzle(argv[1], &grid);
    checkPuzzle(sudokuSize, grid);
    if (!is_complete) {
        printf("Puzzle is not complete, attempting to solve\n");
        if (solve(grid, 1, 1)) {
            is_complete = true;
            is_valid = true;
            checkPuzzle(sudokuSize, grid);
        } else {
            printf("Puzzle could not be solved\n");
        }
    }
    printf("Complete puzzle? ");
    printf(is_complete ? "true\n" : "false\n");
    if (is_complete) {
        printf("Valid puzzle? ");
        printf(is_valid ? "true\n" : "false\n");
    }
    printSudokuPuzzle(sudokuSize, grid);
    deleteSudokuPuzzle(sudokuSize, grid);
    return EXIT_SUCCESS;
}
