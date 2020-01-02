#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define count 3

void* pthread_one(void* arg);
void* pthread_two(void* arg);

int main(int argc, char const* argv[]) {
    int a = 1;

    pthread_t tid_one = 0;
    pthread_t tid_two = 0;

    //线程一 创建
    if (pthread_create(&tid_one, NULL, pthread_one, 'A')) {
        perror("Create_one");
        return 0;
    }

    a = pthread_join(tid_one, NULL);
    if (a) {
        perror("Join_one");
        return 0;
    }

    //线程二 创建
    if (pthread_create(&tid_two, NULL, pthread_two, 'B')) {
        perror("Create_two");
        return 0;
    }

    a = pthread_join(tid_two, NULL);
    if (a) {
        perror("Join_two");
        return 0;
    }

    puts("");
    puts("Over!");

    return 0;
}

void* pthread_one(void* arg) {
    for (int i = 0; i < count; i++) {
        printf("A");
        fflush(stdout);
        sleep(1);
    }

    pthread_exit(NULL);
}

void* pthread_two(void* arg) {
    for (int i = 0; i < count; i++) {
        printf("B");
        fflush(stdout);
        sleep(1);
    }

    pthread_exit(NULL);
}