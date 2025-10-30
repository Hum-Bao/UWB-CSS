#include <assert.h>  // assert
#include <stdio.h>   // printf
#include <stdlib.h>  //exit
#include <unistd.h>  // fork, execlp
#include <wait.h>    // wait

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        // child
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        //printf("Test\n");
        execlp("ps", "ps", "-A", NULL);
        close(fd[1]);
        assert(1 == 2 && "Should never get here!");
        exit(EXIT_FAILURE);
    } else {
        // parent
        //Close write
        close(fd[1]);
        //Duplicate read to stdin
        dup2(fd[0], STDIN_FILENO);
        int status;
        wait(&status);

        char buffer[32];
        int num = 0;
        //Read 32 bytes at a time and print
        while ((num = read(fd[0], buffer,
                           (sizeof(buffer) / sizeof(buffer[0]))) > 0)) {
            printf("%s", buffer);
        }
        close(fd[0]);

        printf("\nChild finished with status: %d\n", status);
    }
}