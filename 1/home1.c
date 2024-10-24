#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

inline int isSpace(char c)
{
    return (c == '\n') || (c == ' ') || (c == '\t') || (c == '\0') || (c == EOF);
}

char* skipToNextArg(char* p)
{
    if(!p)
    {
        return NULL;
    }

    while(!isSpace(*p))
    {
        p++;
    }

    while(isSpace(*p))
    {
        p++;
    }

    return p;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("2 args needed\n");

        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);

    if(fd < 0)
    {
        printf("File is not found\n");

        exit(1);
    }

    u_int64_t fsize = lseek(fd, 0, SEEK_END) + 1;

    lseek(fd, 0, SEEK_SET);

    printf("file size: %lu\n", fsize);

    char* buff = calloc(fsize, sizeof(char));

    buff[fsize - 1] = '\n';

    if(read(fd, buff, fsize - 1) < 0)
    {
        perror("read");

        exit(1);
    }

    char* bip = buff;

    unsigned int secs;
    char name[16];

    while(bip < buff + fsize)
    {
        if(sscanf(bip, "%u %16s", &secs, name) != 2)
        {
            break;
        }

        printf("%u %s\n", secs, name);

        pid_t pid = fork();

        if(pid == 0)
        {
            sleep(secs);

            // printf("it is ancestor\n");

            execl(name, name, NULL);

            exit(1);
        }

        bip = skipToNextArg(bip);

        bip = skipToNextArg(bip);
    }
}