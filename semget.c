#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

int shmid, semid;
char* buf;
// vi -t semun
union semun {
    int val;               /* Value for SETVAL */
    struct semid_ds* buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short* array; /* Array for GETALL, SETALL */
    struct seminfo* __buf;
    /* Buffer for IPC_INFO (Linux-specific) */
};
union semun mysemun;
//////////////////////////////////////

struct sembuf mysembuf;

int main(int argc, const char* argv[]) {
    key_t key;
    key = ftok("/", 'a');
    if (key == -1) {
        perror("ftok");
        return key;
    }

    // 创建共享内存对象
    shmid = shmget(key, 128, IPC_CREAT | IPC_EXCL | 0777);
    if (shmid == -1) {
        perror("shmget");
        return shmid;
    }
    // 共享内存映射到虚拟地址
    buf = shmat(shmid, NULL, 0);
    if (-1 == (int)(*buf)) {
        // 映射失败，删除共享内存对象
        shmctl(shmid, IPC_RMID, NULL);
        return -1;
    }

    // 创建信号灯集
    semid = semget(key, 2, IPC_CREAT | IPC_EXCL | 0777);
    if (semid == -1) {
        exit(-1);
    }
    // 初始化信号灯 0
    mysemun.val = 1;
    semctl(semid, 0, SETVAL, mysemun);
    // 初始化信号灯 1
    mysemun.val = 0;
    semctl(semid, 1, SETVAL, mysemun);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    } else if (pid == 0) {  //子进程
        while (1) {
            mysembuf.sem_num = 1;  // 1号灯
            mysembuf.sem_op = -1;  // P操作
            mysembuf.sem_flg = 0;  // 阻塞
            semop(semid, &mysembuf, 1);

            // 将共享内存中数据打印
            printf("%s\n", buf);

            mysembuf.sem_num = 0;  // 0号灯
            mysembuf.sem_op = 1;   // V操作
            mysembuf.sem_flg = 0;  // 阻塞
            semop(semid, &mysembuf, 1);
        }
    } else {  //父进程
        while (1) {
            mysembuf.sem_num = 0;  // 0号灯
            mysembuf.sem_op = -1;  // P操作
            mysembuf.sem_flg = 0;  // 阻塞
            semop(semid, &mysembuf, 1);

            // 把终端输入 放到共享内存中
            printf("plase input:");
            fflush(stdout);
            fgets(buf, 11, stdin);

            mysembuf.sem_num = 1;  // 1号灯
            mysembuf.sem_op = 1;   // V操作
            mysembuf.sem_flg = 0;  // 阻塞
            semop(semid, &mysembuf, 1);
        }
    }
    return 0;
}
