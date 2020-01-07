#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#define N 100

typedef struct {
    char name[N];
    char number[N];
} * student, size;

student value;

int shmid = 0;
key_t key = 2374754094;

int main(int argc, char const* argv[]) {
    if ((shmid = shmget(key, N, IPC_CREAT | 0777)) < 0) {
        perror("Shmget");
        return 0;
    }

    if ((value = (student)shmat(shmid, NULL, 0)) == (student)-1) {
        perror("Shmat");
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        printf("输入学生%d的名字：", i + 1);
        fgets((value + i)->name, N, stdin);
        printf("输入学生%d的学号：", i + 1);
        fgets((value + i)->number, N, stdin);
    }

    shmdt(value);

    puts("正在跳转read函数");
    execl("./read", NULL);

    return 0;
}
