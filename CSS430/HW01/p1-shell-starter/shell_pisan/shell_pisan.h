#ifndef SHELL_H
#define SHELL_H

#include <assert.h>   // assert
#include <fcntl.h>    // O_RDWR, O_CREAT
#include <stdbool.h>  // bool
#include <stdio.h>    // printf, getline
#include <stdlib.h>   // calloc
#include <string.h>   // strcmp
#include <unistd.h>   // execvp

#define MAXLINE 80
#define PROMPT "osh> "

#define RD 0
#define WR 1

bool equal(char* a, char* b);
int fetchline(char** line);
int interactiveShell();
int runTests();
void processLine(char* line);
int main();

#endif