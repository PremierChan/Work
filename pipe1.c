#include <stdio.h>
#include <unistd.h>

#define N 5

int main(int argc, char const* argv[]) {
    int pipefd[2] = {0, 0};

    char number1[N] = "1234";
    char number2[N] = "";

    printf("Start:number2 ->%s\n      number1 ->%s\n"
           , number2, number1);

    if (pipe(pipefd)) {
        perror("Pipe");
        return 0;
    }

    if (write(pipefd[1], (int*)&number1, N) <= -1) {
        perror("Write");
        return 0;
    }

    if (read(pipefd[0], (int*)&number2, N) <= -1) {
        perror("Read");
        return 0;
    }

    printf("End:number2 ->%s\n    number1 ->%s\n"
            , number2, number1);

    return 0;
}
