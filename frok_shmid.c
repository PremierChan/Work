#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define N 100

void in_put(void);
void out_put(void);
void my_fork(void);

int shmid = 0;
char* addr = NULL;
char buf[N] = "";
pid_t child = 0;

int main(int argc, char const* argv[]) {
    if ((shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0777)) < 0) {
        perror("Shmget");
        return -1;
    }

    if ((addr = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("Shmat");
        return -1;
    }

    if ((child = fork()) < 0) {
        perror("Fork");
        return -1;
    } else if (!child) {
        my_fork();
    }

    wait(NULL);

    out_put();

    shmdt(addr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

void my_fork(void) {
    in_put();
    shmdt(addr);
    exit(0);
}

void in_put(void) {
    puts("请输入：");
    fgets(addr, N, stdin);
}

void out_put(void) {
    strncpy(buf, addr, N);
    printf("buf:%s\n", buf);
}