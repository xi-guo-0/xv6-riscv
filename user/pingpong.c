#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pfds[2];
    int cfds[2];
    char ps[2] = "0";
    char cs[2] = "0";
    const int len = 1;
    if (0 != pipe(pfds)) {
        exit(1);
    }
    if (0 != pipe(cfds)) {
        exit(1);
    }

    if (0 != fork()) {
        write(cfds[1], ps, len);
        read(pfds[0], ps, len);
        printf("%d: received pong\n", getpid());
    } else {
        read(cfds[0], cs, len);
        printf("%d: received ping\n", getpid());
        write(pfds[1], cs, len);
    }
    return 0;
}
