#include "kernel/types.h"
#include "user/user.h"
#define LIMIT 35

int p[2];

int
is_prime(int n)
{
    if (n == 2 || n == 3) {
        return 1;
    }

    if (n <= 1 || n % 2 == 0 || n % 3 == 0) {
        return 0;
    }

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }

    return 1;
}

int
next_prime(int n) {
    // find next prime after n
    while (!is_prime(n)) {
        n++;
    }
    return n;
}

void
pipeline(int n)
{
    char buf[1];

    if (n >= LIMIT) {
        return;
    }

    int pid = fork();

    if (pid == 0) {
        if (read(p[0], buf, 1) == 0) {
            fprintf(1, "%d: read failed\n", getpid());
            exit(1);
        }
        close(p[0]);

        if (buf[0] != 'h') {
            fprintf(1, "%d: read incorrect value %c\n", getpid(), buf[0]);
            exit(1);
        }

        pipe(p);
        write(p[1], buf, 1);
        close(p[1]);

        fprintf(0, "prime %d\n", n);

        pipeline(next_prime(n + 1));
        exit(0);
    } else if (pid > 0) {
        wait((int *) 0);
        exit(0);
    } else {
        fprintf(1, "fork error\n");
        exit(1);
    }
}

int
main(int argc, char *argv[])
{
    pipe(p);
    write(p[1], "h", 1);
    close(p[1]);
    pipeline(2);
    exit(0);
}
