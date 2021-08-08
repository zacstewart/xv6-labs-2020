#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int tx[2];
    int rx[2];
    pipe(tx);
    pipe(rx);
    char buf[5];
    int pid;

    if (fork() == 0) { // child
        pid = getpid();

        read(tx[0], buf, 4);
        fprintf(0, "%d: received %s\n", pid, buf);

        write(rx[1], "pong", 4);

        exit(0);
    } else { // parent
        pid = getpid();

        write(tx[1], "ping", 4);

        read(rx[0], buf, 4);
        fprintf(0, "%d: received %s\n", pid, buf);

        wait(0);
        exit(0);
    }

    exit(0);
}
