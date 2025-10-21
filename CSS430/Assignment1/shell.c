
#include "shell.h"

History history = {NULL, 0};

char** Split(const char* line, const char* delimiter, int* count) {
    if (!line || !delimiter || !count) {
        return NULL;
    }

    //Copy line safely
    char* temp = strdup(line);
    if (!temp) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int tokens = 0;
    char* token = strtok(temp, delimiter);

    while (token) {
        tokens++;
        token = strtok(NULL, delimiter);
    }

    free(temp);

    char** result = malloc((tokens + 1) * sizeof(char*));
    if (!result) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    //Duplicate again for safe tokenization
    temp = strdup(line);
    token = strtok(temp, delimiter);
    int i = 0;
    while (token) {
        result[i] = strdup(token);
        if (!result[i]) {
            perror("Memory allocation error");
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            free(temp);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, delimiter);
        i++;
    }
    result[i] = NULL;

    free(temp);
    *count = tokens;
    return result;
}

int GetInput(char** line) {
    size_t length = 0;
    ssize_t read = getline(line, &length, stdin);

    if (read == -1) {
        printf("Reading error");
        return -1;
    }
    if ((*line)[read - 1] == '\n') {
        (*line)[read - 1] = '\0';
    }
    return 0;
}

void PrintLastCommand() {
    printf("%s", history.last_command[0]);
    for (int count = 1; count < history.length; count++) {
        printf(" %s", history.last_command[count]);
    }
    printf("\n");
}

void FreeHistory() {
    for (int count = 0; count < history.length; count++) {
        free(history.last_command[count]);
    }
    free(history.last_command);
    history.last_command = NULL;
    history.length = 0;
}

void StoreHistory(char** args, int length) {
    if (history.last_command != NULL) {
        for (int count = 0; history.last_command[count] != NULL; count++) {
            free(history.last_command[count]);
        }
        free(history.last_command);
    }
    //Allocate new history
    history.last_command = malloc((length + 1) * sizeof(char*));
    history.length = length;
    for (int i = 0; i < length; i++) {
        history.last_command[i] = strdup(args[i]);
    }
    //NULL-terminate for execvp
    history.last_command[length] = NULL;
}

void FreeArgs(char** args, int length) {
    for (int i = 0; i < length; i++) {
        free(args[i]);
    }
    free(args);
    args = NULL;
}

void HandlePipes(char* input) {
    char* cmd1 = strtok(input, "|");
    char* cmd2 = strtok(NULL, "|");

    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        //Child 1 write to pipe
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        char** args1;
        int length1;
        args1 = Split(cmd1, " ", &length1);
        if (execvp(args1[0], args1) == -1) {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        //Child 2 read from pipe
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        char** args2;
        int length2;
        args2 = Split(cmd2, " ", &length2);
        if (execvp(args2[0], args2) == -1) {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void HandleRedirection(char** args, int* length) {
    int in = -1;
    int out = -1;

    //Check for input/output redirection
    for (int i = 0; i < *length; i++) {
        if (strcmp(args[i], "<") == 0) {
            in = i;
        } else if (strcmp(args[i], ">") == 0) {
            out = i;
        }
    }

    //Input redirection
    if (in != -1) {
        if (in + 1 < *length) {
            int fd_in = open(args[in + 1], O_RDONLY);
            if (fd_in == -1) {
                perror("Input redirection failed");
                return;
            }
            //Redirect stdin to file
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);

            //Shift args to remove redirection operator and file
            for (int j = in; j < *length - 2; j++) {
                args[j] = args[j + 2];
            }

            args[*length - 2] = NULL;
            //Adjust length
            *length -= 2;
        } else {
            perror("No file specified for input redirection");
        }
    }

    //Output redirection
    if (out != -1) {
        if (out + 1 < *length) {
            int fd_out =
                open(args[out + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1) {
                perror("Output redirection failed");
                return;
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);

            //Shift args to remove redirection operator and file
            for (int j = out; j < *length - 2; j++) {
                args[j] = args[j + 2];
            }

            args[*length - 2] = NULL;
            *length -= 2;
        } else {
            perror("No file specified for output redirection");
        }
    }
}

void RunLine(char** args, int length) {
    bool should_wait = true;
    bool last_command = false;
    char** background_process = NULL;
    int background_length = 0;

    for (int count = 0; count < length; count++) {
        if (strcmp(args[count], "ascii") == 0) {
            printf("%s\n", ASCII);
            return;
        }
        if (strcmp(args[count], "!!") == 0) {
            if (history.last_command != NULL) {
                last_command = true;
                FreeArgs(args, length);
                args = malloc((history.length + 1) * sizeof(char*));
                if (!args) {
                    perror("Memory allocation error");
                    return;
                }
                for (int i = 0; i < history.length; i++) {
                    args[i] = strdup(history.last_command[i]);
                }
                args[history.length] = NULL;
                length = history.length;
            } else {
                printf("No commands in history.\n");
                return;
            }
        }

        if (!last_command) {
            StoreHistory(args, length);
        }

        if (strcmp(args[count], "&") == 0 || strcmp(args[count], ";") == 0) {
            if (strcmp(args[count], ";") == 0) {
                should_wait = true;
            } else {
                should_wait = false;
            }
            background_length = count;
            background_process =
                malloc((background_length + 1) * sizeof(char*));
            if (!background_process) {
                perror("Memory allocation error");
                return;
            }

            for (int temp = 0; temp < background_length; temp++) {
                background_process[temp] = args[temp];
            }
            background_process[background_length] = NULL;

            for (int j = count + 1; j < length; j++) {
                args[j - (count + 1)] = args[j];
            }

            // Null terminate args before '&'
            args[length - (count + 1)] = NULL;

            // Remaining args after '&'
            length -= (background_length + 1);

            if (strcmp(args[length - 1], ";") == 0) {
                free(args[length - 1]);
                args[length - 1] = NULL;
                length--;
            }

            break;
        }
    }

    if (background_process != NULL) {
        pid_t pid = fork();
        if (pid == 0) {
            if (execvp(background_process[0], background_process) == -1) {
                perror("Background execution failed");
                exit(EXIT_FAILURE);
            }
        } else {
            if (should_wait) {
                waitpid(pid, NULL, 0);
            }
            for (int count = 0; count < background_length; count++) {
                free(background_process[count]);
            }
            free(background_process);
        }
    }

    if (length > 0) {
        pid_t pid = fork();
        if (pid == 0) {
            HandleRedirection(args, &length);

            if (execvp(args[0], args) == -1) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
            // }
        } else if (pid > 0) {
            if (should_wait) {
                waitpid(pid, NULL, 0);
            }
        } else {
            perror("fork failed");
        }
    }

    FreeArgs(args, length);
}

int RunTests() {
    printf("*** Running basic tests ***\n");
    char lines[7][80] = {
        "ls",      "ls -al", "ls & whoami ;", "ls > junk.txt", "cat < junk.txt",
        "ls | wc", "ascii"};
    for (int i = 0; i < 7; i++) {
        printf("* %d. Testing %s *\n", i + 1, lines[i]);
        if (strchr(lines[i], '|')) {
            HandlePipes(lines[i]);
        } else {
            int length = 0;
            char** args = Split(lines[i], " ", &length);
            RunLine(args, length);
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "--interactive") == 0) {
        bool run = true;
        int length = 0;
        char* line = NULL;
        while (run) {
            printf("%s", PROMPT);

            int exit_code = GetInput(&line);
            if (exit_code != 0 || !line || strcmp(line, "exit") == 0) {
                free(line);
                break;
            }

            if (strchr(line, '|')) {
                HandlePipes(line);
            } else {
                char** args = Split(line, " ", &length);
                RunLine(args, length);
            }

            free(line);
            line = NULL;
            // printf("\n");
        }
    } else {
        RunTests();
    }

    FreeHistory();
    return 0;
}