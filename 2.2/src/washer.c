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
#include <arpa/inet.h>

#define PORT 8777
#define B_SIZE 1024
#define MAX_NUM_DISH_TYPES 10

int dtime[MAX_NUM_DISH_TYPES];

void getDishTimes(const char* fname)
{
    FILE* file = fopen(fname, "r");

    if(file == NULL)
    {
        perror("invalid file");

        return;
    }

    int dish, time;

    fscanf(file, "%d %d", &dish, &time);
    dtime[dish] = time;
}

void work(const char* fname)
{
    FILE* file = fopen(fname, "r");
    
    if(file == NULL)
    {
        perror("invalid file");

        return;
    }

    int dish;

    fscanf(file, "%d", &dish);

    sleep(dish * dtime[dish]);
}

int main()
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if(
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) < 0)
    {
        perror("invalid address");

        exit(1);
    }

    socklen_t alen = sizeof(addr);

    char buffer[B_SIZE] = {};

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(sockfd, &addr, alen) < 0)
    {
        perror("connect fail");

        exit(1);
    }

    send(sockfd, "I AM WASHER", sizeof("I AM WASHER"), 0);

    read(sockfd, buffer, B_SIZE - 1);

    printf("%s\n", buffer);

    close(sockfd);
}