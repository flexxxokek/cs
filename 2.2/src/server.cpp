
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <semaphore.h>

#include <netinet/in.h>

#include <linux/limits.h>
#include <sys/socket.h>

#include <poll.h>
#include <queue>


#define PORT 8777
#define B_SIZE 1024
#define MAX_NUM_DISH_TYPES 10

int N = 0;

int maxint(int a, int b)
{
    return a > b ? a : b;
}

void work(int clean_fd, int wash_fd)
{
    char buffer[B_SIZE];

    std::queue <int> table;
    int now = 0;

    struct pollfd pfds[2];

    pfds[0].fd = clean_fd;
    pfds[0].events = POLLIN;

    pfds[1].fd = wash_fd;
    pfds[1].events = POLLIN;

    int iscleaner_free = true;

    while(0)
    {
        int events = poll(pfds, 2, -1);

        if(events == 0)
        {
            continue;
        }

        if(pfds[0].revents & POLLIN)
        {
            read(clean_fd, buffer, B_SIZE - 1);

            sprintf(buffer, "%d\0", table.front());
            table.pop();

            send        
        }

        if(pfds[1].revents & POLLIN)
        {
            read(wash_fd, buffer, B_SIZE - 1);
        }
    }
}

int main(int argc, char* argv[], char* envp[])
{
    char* strn = getenv("TABLE_LIMIT");

    if(strn == NULL)
    {
        printf("environment variable TABLE_LIMIT is not initialized\n");

        exit(1);
    }

    N = atoi(strn);

    if(N <= 0)
    {
        printf("environment variable TABLE_LIMIT = %s initialized incorrectly\n", strn);

        exit(1);
    }

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    socklen_t alen = sizeof(addr);

    char buffer[B_SIZE] = {};

    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket fail");

        exit(1);
    }

    int opt = 1;

    if (setsockopt(sockfd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        perror("bind fail");
        exit(1);
    }

    printf("carefuly listening\n");

    if(listen(sockfd, 2) < 0)
    {
        perror("listen fail");
        exit(1);
    }

    printf("carefuly accepting\n");

    int msg1, msg2;
    if((msg1 = accept(sockfd, (struct sockaddr*)&addr, &alen)) < 0)
    {
        if((msg2 = accept(sockfd, (struct sockaddr*)&addr, &alen)) < 0)
        {
            perror("accept fail");
            exit(1);
        }
        perror("accept fail");
        exit(1);
    }


    read(msg1, buffer, B_SIZE - 1);
    
    if(!strcmp(msg1, "I AM CLEANER"))
    {
        
    }
    else
    {

    }

    printf("%s\n", buffer);

    sleep(5);

    send(msg, "frick", 6, 0);

    close(sockfd);
    close(msg);
}