#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const* argv[]) {
    char buf[10] = {};

    pid_t fd = 0;

    if (!mkfifo("pq", 0666)) {
        if (errno == EEXIST) {
            puts("pipe is exist!");
        } else {
            perror("mkfifo");
            exit(1);
        }
    }
    fd = open("pq", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    write(fd, "hei!", 5);
    read(fd, buf, sizeof(buf));
    printf("buf:%s\n", buf);
    close(fd);

    return 0;
}
