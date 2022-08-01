#include "kernel/types.h"
#include "user/user.h"
#define ARG_CMAX 16
#define ARG_LEN 32
#define BUF_LEN 512

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        exit(0);
    }
    char* command = argv[1];
    char* cmdArgv[ARG_CMAX];
    int argvPin = 0;
    cmdArgv[argvPin++] = command;
    
    for (int i = 2; i < argc; i++)
    {
        cmdArgv[argvPin++] = argv[i];
    }
    cmdArgv[argvPin] = 0;
    int savedArgvPin = argvPin;
    
    char newArg[ARG_LEN];
    char str[2];
    int j = 0;
    while (read(0, str, 1))
    {
        char ch = str[0];
        /*printf("%d: %c\n", getpid(), ch);*/
        if (ch == '\n')
        {
            newArg[j] = '\0';
            cmdArgv[argvPin] = (char*)malloc(strlen(newArg)+1);
            strcpy(cmdArgv[argvPin], newArg);
            argvPin++;
            cmdArgv[argvPin] = 0;
            j = 0;
            if (fork() == 0)
            {
                /*printf("debug: command: %s\n", command);
                printf("debug: argv: ");
                for (int k = 0; cmdArgv[k] != 0; k++) {
                    printf("%s ", cmdArgv[k]);
                }
                printf("\n");*/
                exec(command, cmdArgv);
            } else {
                wait(0);
                argvPin = savedArgvPin;
            }
        } else if (ch == ' ')
        {
            newArg[j] = '\0';
            cmdArgv[argvPin] = (char*)malloc(strlen(newArg)+1);
            strcpy(cmdArgv[argvPin], newArg);
            argvPin++;
            cmdArgv[argvPin] = 0;
            j = 0;
        } else {
            newArg[j++] = ch; 
        }
    }
    exit(0);
}