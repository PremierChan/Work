#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const* argv[]) {
    int flag;
    char buf[100];

    flag = fcntl(0, F_GETFL);

    flag = flag | O_NONBLOCK;

    fcntl(0, F_SETFL < flag);

    while (1) {
        fgets(buf, 100, stdin);
        printf("buf:%s\n", buf);
        sleep(1);
        puts("+++++++++++++++++");
    }

    return 0;
}
