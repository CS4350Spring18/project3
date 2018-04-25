//***********************************************/
// myls.h:
//   Print contents of a path. If no path is provided
//   use the current directory.
//***********************************************
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

/* myls:
 *    Take a pathname or a NULL value to print the contents
 *    for the pathname. Use the CWD if NULL is provided.
 *    mode - (l)ist or (r)egular
 *       mode controls whether we list file details or just
 *       the filenames in the directory.
 *    viewMode - (a)ll or (l)imited
 *       viewMode controls whether we include hidden files
 *       and the parent/child directory.
 */
void myls(const char *pathname, char mode, char viewMode);

/* printStatList:
 *    Starting from the provided node, traverse the linked list
 *    and print the data in each node. Use the mode to determine
 *    whether the we print the filename with or without detail.
 *    tgt_path - provided pathname or CWD
 *    mode - (l)ist or (r)egular
 *       mode controls whether we include details when printing
 *       the filename.
 */
void printStatList(const Node *node, char* tgt_path, char mode);

/* printLastMod:
 *    Print the time value with the correct format.
 *    format: "%b %d %H:%M" E.g. Apr 25 16:36
 */
void printLastMod(const time_t val);

/* listPrint:
 *    Function to print all the list details. Calls fns:
 *       - printPermission
 *       - printLastMod
 *    Manages spacing for each print ln
 */
void listPrint(const struct stat* fs, const char* filename);

/* printPermission:
 *    Print the permissions for the file.
 *    format: "<type><owner><group><user>" E.g. drwxrwxr-x
 *
 */
void printPermission(const struct stat* fs);

