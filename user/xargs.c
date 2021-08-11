#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void
execute_line(char *line, char *args[], int skip)
{
    int ws = 1;
    char **a, *l;

    // the following routine splits a (char *) by whitespace
    // into a (char **) without actually copying any of the bytes.
    // The resulting "string array" args is actually just an organized
    // view into the block of byte data given as user input.
    a = &args[skip];
    l = line;
    while (*l != '\0') {
        if (*l == ' ') {
            if (!ws) {
                *l = '\0';
                a++;
            }
            ws = 1;
        } else {
            if (ws) {
                *a = l;
            }
            ws = 0;
        }
        l++;
    }

    int f = fork();
    if (f < 0) {
        fprintf(2, "xargs: fork failed\n");
        exit(1);
    } else if(f == 0) { // child
        exec(args[0], args);
    } else { // parent
        wait((int *) 0);
    }
}

int
main(int argc, char *argv[])
{
    char *args[MAXARG] = { 0 };
    char buf[512], *b;
    b = buf;

    // copy ptrs of xargs args into child proc args
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    // Read each line of stdin into the buffer and execute the command given as
    // args. This routine replaces newlines with nulls so that we can pass the
    // treat the buffer like null-terminated string that ends at the end of the
    // line in the original input.
    while (read(0, b, 1) > 0) {
        if (*b == '\n') {
            *b = '\0';
            execute_line(buf, args, argc - 1);
        }

        b++;
    }
    exit(0);
}
