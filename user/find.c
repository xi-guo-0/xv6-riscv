#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

int compare(const char *X, const char *Y) {
    while (*X && *Y) {
        if (*X != *Y) {
            return 0;
        }

        X++;
        Y++;
    }

    return (*Y == '\0');
}

// Function to implement `strstr()` function
const char *strstr(const char *X, const char *Y) {
    while (*X != '\0') {
        if ((*X == *Y) && compare(X, Y)) {
            return X;
        }
        X++;
    }

    return 0;
}

void basename(char const *str, char *ans) {
    const char *p = str;
    const char *q;
    while (*p) {
        ++p;
    }
    q = p - 1;
    while (str <= q && *q != '/') {
        --q;
    }
    ++q;
    while (q < p) {
        *ans = *q;
        ++q;
        ++ans;
    }
    *ans = '\0';
}


void find_str(char const *str, char const *substr) {
    char buf[512];
    basename(str, buf);
    if (compare(buf, substr)) {
        printf("%s\n", str);
    }
}

void find(char *path, char *name) {
    char buf[512];
    char subbuf[512];
    struct dirent de;
    struct stat st;
    int fd;
    char *p;

    if ((fd = open(path, 0)) < 0) {
        return;
    }

    if (fstat(fd, &st) < 0) {
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p = '/';
            ++p;
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    continue;
                }
                if (T_DIR == st.type) {
                    basename(buf, subbuf);
                    if (!compare(".", subbuf) && !compare("..", subbuf)) {
                        find_str(buf, name);
                        find(buf, name);
                    }
                } else {
                    find_str(buf, name);
                }
            }
            break;
        default:
            if (stat(buf, &st) < 0) {
                break;
            }
            find_str(path, name);
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: find dir name\n");
    } else {
        find(argv[1], argv[2]);
    }
    return 0;
}
