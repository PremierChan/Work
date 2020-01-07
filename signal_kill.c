#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void my_fork(void);
void handler_seller(int signum);
void handler_driver(int signum);

pid_t child = 0;

int main(int argc, char const* argv[]) {
    if (!(child = fork())) {
        my_fork();
    }
    if (child < -1) {
        perror("Fork");
        return 0;
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGUSR1, handler_driver);
    signal(SIGUSR2, handler_driver);
    signal(SIGTSTP, handler_driver);

    wait(NULL);

    return 0;
}

void my_fork(void) {
    while (1) {
        signal(SIGTSTP, SIG_IGN);
        signal(SIGINT, handler_seller);
        signal(SIGQUIT, handler_seller);
        signal(SIGUSR1, handler_seller);
    }
}

void handler_seller(int signum) {
    if (signum == SIGINT) {
        kill(getppid(), SIGUSR1);
    } else if (signum == SIGQUIT) {
        kill(getppid(), SIGUSR2);
    } else if (signum, SIGUSR1) {
        printf("下车！");
        exit(1);
    }
}

void handler_driver(int signum) {
    if (signum == SIGUSR1) {
        printf("发车！\n");
    } else if (signum == SIGUSR2) {
        printf("快！快！快！");
    } else if (signum == SIGTSTP) {
        kill(child, SIGUSR1);
    }
}