#pragma once

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <string.h>

struct option longopts[] = {
   { 0, 0, 0, 0 },
};

bool eof(int fd);

void SearchDirectory(const char *name, const char *newName);
