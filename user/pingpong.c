#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int p[2], q[2];
    pipe(p), pipe(q);
    
    if (fork() == 0)
    {
        close(p[1]), close(q[0]);
        char str[2];
        read(p[0], str, 1);
        close(p[0]);
        printf("%d: received ping\n", getpid());
        write(q[1], str, 1);
        close(q[1]);
    } else {
        close(p[0]);
        close(q[1]);
        write(p[1], "a", 1);
        close(p[1]);
        char str[2];
        read(q[0], str, 1);
        close(q[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}