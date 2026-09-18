#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    int i = 0;
    char *totalArgs[MAXARG];
    char arg[512];
    int base;

    for(; i < argc -1; i++) totalArgs[i] = argv[i+1];
    base = i;

    totalArgs[i] = arg;

    int c = 0;
    for(; read(0, &arg[c], 1) != 0; ) {
        if(arg[c] == ' ') {
            if(i + 1 > MAXARG) {
                fprintf(2, "xargs: too many arguments\n");
                exit(1);
            }

            arg[c] = 0;
            totalArgs[i] = &arg[c+1];
            c++;
            continue;
        }
        if(arg[c] == '\n') {
            arg[c] = 0;
            totalArgs[i+1] = 0;

            if(fork() == 0) {
                exec(totalArgs[0], totalArgs);
                fprintf(2, "xargs: exec failed\n");
                exit(1);
            }

            wait(0);

            i = base;
            totalArgs[i] = arg;
            c = 0;
            continue;
        }
        if (c < sizeof(arg) - 1) c++;
    }

    if (c > 0) {
        arg[c] = 0;
        i++;
        totalArgs[i] = 0;

        if (fork() == 0) {
            exec(totalArgs[0], totalArgs);
            fprintf(2, "xargs: exec failed\n");
            exit(1);
        }

        wait(0);
    }

    exit(0);
}