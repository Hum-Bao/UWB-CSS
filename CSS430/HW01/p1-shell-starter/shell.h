#ifndef SHELL_H
#define SHELL_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROMPT "osh> "

#define ASCII                                                                  \
    "                    -----                    \n             "             \
    "-------------------             \n          -------------------------   " \
    "       \n        -----------------------------        \n      "           \
    "---------------------------------      \n    "                            \
    "-------------------------------------    \n "                             \
    "  -----------=###---------*##=-----------      \n   "                     \
    "---------=###=-----------=###+----------  \n  "                           \
    "-------+####=---------------=####+-------  \n "                           \
    "------###*=---------------------=*###------ \n "                          \
    "---------+#######=-------=*######+--------- \n "                          \
    "-------+###+++++###-----###+++++###+--------\n "                          \
    "----------+==+=-------------======----------\n "                          \
    "---------==+===-------------=+=+==--------- \n "                          \
    "----------==+==----*###*----==+=+=--------- \n  "                         \
    "--------==+=+=--=#######=--======--------  \n   "                         \
    "--------+====--##*****##--=+=+==--------  \n   "                          \
    "-------==+=+=--#+++++++#=-==+=+=-------   \n    "                         \
    "-------==+==--#*+++++*#--======------    \n      "                        \
    "----==+=+=---=+++++=---=+=+==----      \n        "                        \
    "---+====-------------==+=+=--        \n  "                                \
    "+=+=+==+==+=+====+==+==+===+=======+=+=+=  \n "                           \
    "+=+===+==+==+=+=+==+==+==+=+=+=+=+=+=====+= \n              "             \
    "++++++=+=++=++===              \n"                                        \
    "    Me doing this assignment (Crying emoji)"

typedef struct {
        char** last_command;
        int length;
} History;

extern History history;

int GetInput(char** line);
char** Split(const char* line, const char* delimiter, int* length);
void RunLine(char** args, int length);
void StoreHistory(char** args, int length);
void FreeHistory();
void FreeArgs(char** args, int length);
void PrintLastCommand();
void HandlePipes(char* input);
void HandleRedirection(char** args, int* length);
int RunTests();

#endif