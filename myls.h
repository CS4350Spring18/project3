#pragma once
#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <getopt.h>
#include <limits.h>
#include <libgen.h>

#include "mylsll.h"

struct option longopts[] = {
   { 0, 0, 0, 0 },
};

void myls(const char *pathname, char mode, char viewMode);

void printStatList(Node *node, char* tgt_path, char mode);

void printLastMod(const time_t val);

void listPrint(const struct stat* fs, const char* filename);

void printPermission(const struct stat* fs);

