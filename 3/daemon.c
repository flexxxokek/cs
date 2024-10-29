#include "home2.h"

#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <syslog.h>

#include <fcntl.h>

#include <linux/limits.h>

struct msgbuff
{
    char src [PATH_MAX];
    char dst [PATH_MAX];
};

static void daemonize()
{
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid_t pid = fork();
    
    if(pid < 0)
    {
        perror("error");
        exit(1);
    }
    else if(pid != 0)
    {
        exit(pid);
    }

    if(pid = setsid() < 0)
    {
        exit(pid);
    }

    pid = fork();

    if(pid < 0)
    {
        perror("error");
        exit(1);
    }
    else if(pid != 0)
    {
        exit(pid);
    }

    chdir("/");
    umask(0);

    for(int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    openlog(NULL, LOG_PID, LOG_DAEMON);
}

int main()
{
    umask(0);

    int fd = open("/tmp/new", O_CREAT | O_RDWR, 0666);
        
    if(fd < 0)
    {
        printf("file can not open\n");
    }

    daemonize();

    syslog(LOG_NOTICE, "DAEMON STARTED WITH PPID %u\n", getppid());

    sleep(1);

    char* buff = malloc(1 << 12);
    char* src = malloc(1 << 11);
    char* cpy = malloc(1 << 11);

    key_t key = ftok("/tmp/new", 0);

    if(key < 0)
    {
        syslog(LOG_NOTICE, "KEYTOK ERROR\n");

        exit(1);
    }
    
    struct msgbuff msg;
    
    int msqid = msgget(key, 0666 | IPC_CREAT);

    if(msqid < 0)
    {
        syslog(LOG_NOTICE, "MSGGET ERROR\n");

        exit(1);
    }

    while(1)
    {
        if(msgrcv(msqid, &msg, sizeof(struct msgbuff), 0, 0) < 0)
        {
            sleep(2);
        }
        else
        {
            syslog(LOG_INFO, "src: %s | dst: %s\n", msg.src, msg.dst);

            backUp(msg.src, msg.dst);
        }
    }
}