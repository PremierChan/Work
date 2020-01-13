#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char const* argv[]) {
    int val = 0;
    int fd = -1;
    char buf[100] = {};

    if ((fd = open("/dev/input/mouse0", O_RDONLY)) < 0) {
        perror("Open");
        exit(1);
    }

    fd_set readfds, temp;

    FD_ZERO(&readfds);     //清空
    FD_SET(fd, &readfds);  //将鼠标输入放入
    FD_SET(0, &readfds);   //键盘标输入放入

    while (1) {
        struct timeval tv = {6, 0};

        temp = readfds;

        if ((val = select(fd + 1, &temp, NULL, NULL, &tv)) < 0) {
            perror("Select");
            exit(1);
        } else if (val == 0) {
            puts("超时！");
            continue;
        }

        if (FD_ISSET(0, &temp)) {
            read(0, buf, sizeof(buf));
            puts("键盘输入！");
        }

        if (FD_ISSET(fd, &temp)) {
            read(fd, buf, sizeof(buf));
            puts("鼠标操作！");
            sleep(1);
        }
    }

    close(fd);
    return 0;
}
