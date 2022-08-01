#include "kernel/types.h"
#include "user/user.h"

int isPrime(int num)
{
    if (num < 2)
    {
        return 0;
    }
    for (int i = 2; i*i <= num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

void primeSieve(int* p)
{
    close(p[1]);
    int num;
    while(read(p[0], &num, sizeof(int)))
    {
        if (isPrime(num))
        {
            printf("prime %d\n", num);
            int q[2];
            pipe(q);
            if (fork() == 0)
            {
                primeSieve(q);
            } else {
                close(q[0]);
                while (read(p[0], &num, sizeof(int)))
                {
                    write(q[1], &num, sizeof(int));
                }
                close(q[1]);
                wait(0);
            }
        }
    }
    close(p[0]);
}

int main(int argc, char* argv[])
{
    int q[2];
    pipe(q);
    if (fork() == 0)
    {
        primeSieve(q);
    } else {
        close(q[0]);
        for (int i = 1; i < 36; i++)
        {
            write(q[1], &i, sizeof(int));
        }
        close(q[1]);
        wait(0);
    }
    exit(0);
}