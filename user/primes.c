#include "kernel/types.h"
#include "user/user.h"

__attribute__((noreturn))
void
recurrence(int readPort)
{
    int currentPrime, i;

    if(read(readPort, &currentPrime, sizeof(int)) == 0) {
        //最后一个子进程发现 pipe 内没有元素，直接关闭 readPort 并退出
        close(readPort);
        exit(0);
    }
    printf("prime %d\n", currentPrime);

    int primePipe[2];
    pipe(primePipe);

    if(fork() == 0) {
        //子进程不需要 pipe 的写入端和父进程的 readPort
        close(primePipe[1]);
        close(readPort);

        recurrence(primePipe[0]);
    }

    //父进程不需要 pipe 的读入端
    close(primePipe[0]);
    while(read(readPort, &i, sizeof(int)) != 0) {
        if(i % currentPrime == 0)
            continue;
        write(primePipe[1], &i, sizeof(int));
    }

    //父完成读取和筛选后关闭 readPort 和 pipe 输出端
    close(readPort);
    close(primePipe[1]);

    //等待子进程退出
    wait(0);
    exit(0);
}

int
main(int argc, char *argv[])
{
    if(argc != 1) {
        fprintf(2, "primes: no argument needed\n");
        exit(1);
    }

    int i = 2;

    int firstPipe[2];
    pipe(firstPipe);

    if(fork() == 0) {
        close(firstPipe[1]);
        recurrence(firstPipe[0]);
    }

    close(firstPipe[0]);
    for(; i<=35; i++) {
        write(firstPipe[1], &i, sizeof(int));
    }

    close(firstPipe[1]);

    wait(0);
    exit(0);
}