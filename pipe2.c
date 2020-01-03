#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#define N 5

int main(int argc, char const* argv[]) {
    int pipefd[2] = {0, 0};

    char buf1[N * 2] = "";
    char buf2[N * 2] = "";

    pid_t pid1 = 0;
    pid_t pid2 = 0;

    if (pipe(pipefd)) {
        perror("Pipe");
        return 0;
    }

    if ((pid1 = fork()) < 0) {
        perror("Fork1");
        exit(1);
    } else if (pid1 == 0) {
        strcpy(buf1, "ABCDE");
        write(pipefd[1], buf1, N);
        exit(1);
    } else {
        if ((pid2 = fork()) < 0) {
            perror("Fork2");
            exit(1);
        } else if (pid2 == 0) {
            sleep(1);
            strcpy(buf1, "abcde");
            write(pipefd[1], buf1, N);
            exit(1);
        } else {
            wait(NULL);
            read(pipefd[0], buf2, N);
            puts(buf2);

            wait(NULL);
            read(pipefd[0], buf2, N);
            puts(buf2);
        }
    }
    return 0;
}
