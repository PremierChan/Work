#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char const* argv[]) {
    int fd = 0;

    char buf[100] = {};

    pid_t pid = 0;

    pid = fork();

    if (pid == 0) {
        if (execl("./fifo_w", NULL, NULL) < 0) {
            perror("Exexl");
            exit(1);
        }
    }

    while ((fd = open("my_fifo", O_RDWR)) < 0)
        ;

    if (read(fd, buf, 10) < 0) {
        perror("Read");
        exit(1);
    }

    puts(buf);

    close(fd);

    if (unlink("my_fifo")) {
        perror("Unlink");
    }

    return 0;
}
