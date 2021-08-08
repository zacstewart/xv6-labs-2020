#include "kernel/types.h"
#include "user/user.h"

void
usage() {
    fprintf(1, "usage: sleep ticks\n");
}

int
main(int argc, char *argv[])
{
    int ticks;

    if (argc < 2) {
        usage();
        exit(1);
    }

    ticks = atoi(argv[1]);

    sleep(ticks);

    exit(0);
}
