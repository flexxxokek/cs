#include <features.h>

#define _XOPEN_SOURCE 10000

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

void backUp(const char* src, const char* dst);