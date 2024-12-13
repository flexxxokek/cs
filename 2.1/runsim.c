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

void* runprog(void* path)
{
    now++;

    int pid = fork();

    if(pid < 0)
    {
        printf("fork is not working\n");

        return NULL;
    }
    else if(pid == 0)
    {
        printf("executing prog %s\n", (char*) path);

        if(execl((char*) path, (char*) path, NULL) < 0)
            printf("execl error\n");
        exit(1);
    }

    waitpid(pid, NULL, 0);
    now--;

    printf("1 program ended\n");

    return NULL;
}

int main(int argc, char* argv[], char* envp[])
{
    signal(SIGCHLD, SIG_IGN);

    if(argc != 2)
    {
        printf("1 argument need\n");

        exit(1);
    }

    int n = atoi(argv[1]);

    char program[PATH_MAX];

    while(1)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        scanf("%s", program);

        if(now < n)
        {
            pthread_create(&tid, &attr, runprog, program);
        }
        else
        {
            fputs("ERROR RUNNING PROGRAMS LIMIT HAS REACHED\n", stdout);
        }
    }
}