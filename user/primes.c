#include "kernel/types.h"
#include "user/user.h"

void print_prime(int p) {
    printf("prime %d\n", p);
}

void f(int pfds[2]) {
    close(pfds[1]);
    int flag = 0;
    int first_num;
    int num;
    int fds[2];
    int cpid;
    if (0 < read(pfds[0], &first_num, 4)) {
        flag = 1;
        pipe(fds);
        cpid = fork();
    }
    if (flag) {
        if (cpid) {
            close(fds[0]);
            print_prime(first_num);
            while (0 < read(pfds[0], &num, 4)) {
                if (num % first_num != 0) {
                    write(fds[1], &num, 4);
                }
            }
            close(pfds[0]);
            close(fds[1]);
            do {

            } while (wait(0) != cpid);
        } else {
            f(fds);
        }
    }
}

int main(int argc, char *argv[]) {
    int first = 2;
    int last = 35;
    int fds[2];
    int cpid;
    pipe(fds);
    cpid = fork();
    if (cpid) {
        close(fds[0]);
        print_prime(first);
        for (int i = first; i <= last; ++i) {
            if (0 != i % first) {
                write(fds[1], &i, 4);
            }
        }
        close(fds[1]);
        do {

        } while (wait(0) != cpid);

    } else {
        f(fds);
    }
    return 0;
}
