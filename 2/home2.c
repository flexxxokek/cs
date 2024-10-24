#include <features.h>

#define _XOPEN_SOURCE 501

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>

#include <stddef.h>

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

int height = 1;

const char* srcpath = NULL;
const char* cpypath = NULL;

static void addToPath(char* buffer, const char* path, const char* file);

static const char* getFileName(const char* path);

static time_t getLastModTime(const char* path);

static void backUp_();

int cpyfile(const char* filepath, const char* cpypath);

void backUp(const char* ndirpath, const char* nbackUpPath)
{
    srcpath = ndirpath;

    cpypath = nbackUpPath;

    mkdir(cpypath, 0700);

    printf("STARTING BACKUP dirpath: %s | backupPath: %s\n\n\n", srcpath, cpypath);

    height = 1;

    backUp_(srcpath, cpypath);
}

static void backUp_(const char* src, const char* cpy)
{
    if(src == NULL || cpy == NULL)
    {
        return;
    }

    DIR* folder = opendir(src);

    if(folder == NULL)
    {
        return;
    }

    char* insrc = calloc(256 * (height + 1), sizeof(char));

    char* incpy = calloc(256 * (height + 1), sizeof(char));

    switch((insrc == NULL) << 1 | incpy == NULL)
    {
        case 0b11:
            return;

        case 0b10:
            free(incpy);
            return;

        case 0b01:
            free(insrc);
            return;

        default:
            break;
    }

    struct dirent* drt = readdir(folder);

    while(drt)
    {
        addToPath(insrc, src, drt->d_name);

        addToPath(incpy, cpy, drt->d_name);

        int isSysDir = !strncmp(drt->d_name, "..", 3) || !strncmp(drt->d_name, ".", 2);

        struct stat fst;

        if(stat(insrc, &fst) < 0)
        {
            printf("\nERROR LINE: %u\n", __LINE__);

            printf("%s | %s\n", insrc, incpy);

            height--;

            return;
        }

        if(S_ISDIR(fst.st_mode) && !isSysDir)
        {
            if(opendir(insrc) != NULL)
            {
                printf("making dir | %s | %s\n", insrc, incpy);
                mkdir(incpy, 0777);
            }
            
            height++;

            backUp_(insrc, incpy);
        }
        else if(!isSysDir)
        {
            cpyfile(insrc, incpy);
        }

        drt = readdir(folder);
    }

    free(insrc);
    free(incpy);

    closedir(folder);

    height--;
    return;
}

static void addToPath(char* buffer, const char* path, const char* file)
{
    if(buffer == NULL || path == NULL || file == NULL)
    {
        return;
    }

    while(*path != '\0')
    {
        *buffer = *path;

        buffer++;
        path++;
    }

    if(*buffer != '/')
    {
        *buffer = '/';
    }

    buffer++;

    while(*file != '\0')
    {
        *buffer = *file;

        buffer++;
        file++;
    }

    *buffer = '\0';
}

static void addStrings(char* buffer, const char* s1, const char* s2)
{
    if(buffer == NULL || s1 == NULL || s2 == NULL)
    {
        return;
    }

    while(*s1 != '\0')
    {
        *buffer = *s1;

        buffer++;
        s1++;
    }

    while(*s2 != '\0')
    {
        *buffer = *s2;

        buffer++;
        s2++;
    }

    *buffer = '\0';
}

static const char* getFileName(const char* path)
{
    if(path == NULL)
    {
        return NULL;
    }

    const char* ret = path;

    while(*path != '\0')
    {
        if(*path == '/')
        {
            ret = path + 1;
        }

        path++;
    }
    
    return ret;
}

static int fcmp(char* buff1, char* buff2)
{
    while(*buff1 != EOF || *buff2 != EOF)
    {
        if(*buff1 != *buff2)
        {
            return 0;
        }
    }

    return *buff1 == *buff2;
}

int cpyfile(const char* src, const char* cpy)
{
    struct stat fst, cst;

    cst.st_mtime = 0;

    if(stat(src, &fst) < 0)
    {
        return -1;
    }

    char* namebuff = malloc(256 * height + 1);

    if(namebuff == NULL)
    {
        return -2;
    }

    addStrings(namebuff, cpy, ".gz");

    if(stat(namebuff, &cst) < 0  || fst.st_mtime >= cst.st_mtime )   //backup file is not existing yet | file was modified, copying is neccesary
    {
        printf("\nin cpyfile filepath: %s mod time %lu | backupPath: %s mod time %lu\n", src, fst.st_mtime, namebuff, cst.st_mtime);

        int fd = open(src, O_RDONLY);
        int cfd = open(cpy, O_CREAT | O_RDWR, 0666);

        if(fd < 0)
        {
            printf("can not openfile: %s\n", src);

            return -1;
        }

        if(fd < 0)
        {
            printf("can not openfile: %s\n", cpy);

            return -1;
        }

        char* fbuffer = calloc(fst.st_size + 1, sizeof(char));

        read(fd, fbuffer, fst.st_size);

        close(fd);

        write(fd, fbuffer, fst.st_size);

        close(cfd);

        // char* namebuff = malloc(256 * height + 1);

        // if(namebuff == NULL)
        // {
        //     free(fbuffer);

        //     return 1;
        // }

        // addStrings(namebuff, cpy, ".gz");

        pid_t pid = fork();

        switch(pid)
        {
            case 0:
                remove(namebuff);

                execl("/bin/gzip", "/bin/gzip", cpy, NULL);
                break;
            
            default:
            {
                int err = 0;

                pid_t endid = 0;
                while(endid = wait(&err) == pid) ;

                break;
            }
        }

        free(fbuffer);
    }

    free(namebuff);

    return 1;
}

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        backUp(".", argv[1]);
    }
    else if(argc == 3)
    {
        backUp(argv[1], argv[2]);
    }

    exit(0);
}