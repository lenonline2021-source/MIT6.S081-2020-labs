#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int ticks;

    if(argc < 2) {
        fprintf(2, "sleep: need an argument\n");
        exit(1);
    }

    if(argc > 2) {
        fprintf(2, "sleep: too many arguments\n");
        exit(1);
    }

    ticks = atoi(argv[1]);
    if(sleep(ticks) == -1) {
        fprintf(2, "sleep: current process has been killed\n");
        exit(1);
    }
    exit(0);
}