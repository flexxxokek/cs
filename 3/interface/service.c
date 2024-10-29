#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>

#include <linux/limits.h>

struct msgbuff
{
    char src [PATH_MAX];
    char dst [PATH_MAX];
};

int main(int argc, char* argv[], char* envp[])
{
    if(argc == 3)
    {
        umask(0);

        key_t key = ftok("/tmp/new", 0);

        if(key < 0)
        {
            printf("ftok error\n");

            exit(0);
        }

        struct msgbuff msg;

        int msqid = msgget(key, 0666 | IPC_CREAT);

        if(msqid < 0)
        {
            printf("msgget error\n");

            exit(0);
        }

        // printf("argv[1]: %s | argv[2]: %s\n", argv[1], argv[2]);

        strcpy(msg.src, argv[1]);
        strcpy(msg.dst, argv[2]);

        msgsnd(msqid, &msg, sizeof(struct msgbuff), 0);

        printf("%d\n", __LINE__);
    }
    else
    {
        printf("2 args needed\n");
    }

    return 0;
}