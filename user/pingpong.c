#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char byte;

    if(argc != 1) {
        fprintf(2, "pingpong: no argument needed\n");
        exit(1);
    }

    int parent_child[2];
    int child_parent[2];
    pipe(parent_child);
    pipe(child_parent);

    int pid = fork();
    if(pid == 0) {
        close(parent_child[1]);
        close(child_parent[0]);

        read(parent_child[0], &byte, 1);
        if(byte == 'P') printf("%d: received ping\n", getpid());
        byte = 'C';
        write(child_parent[1], &byte, 1);

        close(parent_child[0]);
        close(child_parent[1]);

        exit(0);
    }
    else{
        close(parent_child[0]);
        close(child_parent[1]);

        byte = 'P';
        write(parent_child[1], &byte, 1);
        read(child_parent[0], &byte, 1);
        if(byte == 'C') printf("%d: received pong\n", getpid());

        close(parent_child[1]);
        close(child_parent[0]);

        wait(0);
        exit(0);
    }
}