#include <unistd.h>  // pipe, fork, dup2, close, read
#include <wait.h>    // wait
#include <array>
#include <cstddef>
#include <cstdio>  // dprintf

int main() {
    std::array<int, 2> fd = std::array<int, 2>();  // file descriptors
    pipe(fd.begin());                              // create pipe
    int pid = fork();                              // fork child
    if (pid == 0) {                                // child
        close(fd[0]);                              // ain't going to read
        dup2(fd[1], 1);                            // sysout => fd[1]
        //dprintf(fd[1], "Child here! \n");          // write to pipe
        execlp("/bin/ls", "ls", "-la", NULL);
        //execlp("/bin/bash", "cat", "pipe.cpp", NULL);
        close(fd[1]);  // done writing
    } else {           // parent
        close(fd[1]);
        std::array<char, 5> buf =
            std::array<char, 5>();  // aint' going to write
        int n = 0;
        while ((n = static_cast<int>(read(fd[0], buf.begin(), 5) > 0)) != 0) {
            printf("%s", buf.begin());
        }
        close(fd[0]);  // done reading
    }
    return 0;
}
