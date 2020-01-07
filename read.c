#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#define N 100

typedef struct {
    char name[N];
    char number[N];
} * student, size;

student value;

int shmid = 0;
key_t key = 2374754094;

int main(int argc, char const* argv[]) {
    if ((shmid = shmget(key, 0, 0)) < 0) {
        perror("Shmget");
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        if ((value = (student)shmat(shmid, NULL, 0)) == (student)-1) {
            perror("Shmat");
            return 0;
        }
    }

    for (int i = 0; i < 3; i++) {
        printf("%d名字:%s\n", i + 1, (value + i)->name);
        printf("%d学号:%s\n", i + 1, (value + i)->number);
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
