#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char const* argv[]) {
    int fd = 0;

    if (mkfifo("my_fifo", 0666)) {
        if (errno == EEXIST) {
            puts("已经存在！");
        } else {
            perror("Fifo");
            exit(1);
        }
    }

    if ((fd = open("my_fifo", O_RDWR)) < 0) {
        perror("Open");
        exit(1);
    }

    if (write(fd, "HELLO", 6) < 0) {
        perror("Write");
    }

    sleep(1);

    return 0;
}
