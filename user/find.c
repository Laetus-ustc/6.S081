#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void findThisPath(char* path, char* fileName)
{
    int fd;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) {
        char* p = path + strlen(path) - 1;
        while (p >= path && *p != '/') p--;
        p++;
        if (strcmp(p, fileName) == 0) {
            printf(path);
            printf("\n");
        }
    } else if (st.type == T_DIR) {
        char pathBuf[strlen(path) + 1 + DIRSIZ + 1];
        strcpy(pathBuf, path);
        char* p = pathBuf + strlen(path);
        *p++ = '/';
        struct dirent de;
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0) {
                continue;
            }
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            findThisPath(pathBuf, fileName);
        }
    }

    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("Please check arguments. You need input a path and a file name.\n");
        exit(1);
    }

    findThisPath(argv[1], argv[2]);
    exit(0);
}