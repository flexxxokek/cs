#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <linux/limits.h>

int now = 0;

void handler(int sig)
{
    printf("handler worked\n");

    now--;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("1 argument need\n");

        exit(1);
    }

    int n = atoi(argv[1]);

    signal(SIGCHLD, handler);

    char program[PATH_MAX];

    while(1)
    {
        scanf("%s", program);

        if(now < n)
        {
            now++;

            int pid = fork();

            if(pid < 0)
            {
                now--;
                
                perror("fork");

                exit(1);
            }
            else if(pid == 0)
            {
                execl(program, program, NULL);
            }
        }
        else
        {
            printf("PROGRAM LIMIT HAS REACHED\n");
        }
    }
}