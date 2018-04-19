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

void myls(char *pathname);

void printLastMod(time_t val);

void printPermission(struct stat* fs);


int main(int argc, char* argv[]) {
   printf("hit myls %d %s\n", argc, argv[1]);
   myls(NULL);
   printf("%d ", argc);
   for (int i = 0; i < argc - 1; i++)
      printf("%s", argv[i]);
   exit(EXIT_SUCCESS);
}


void myls(char* pathname) {
   int buffSize = 1024;
   char tgt_dir[buffSize];

   // Check that the path is not a file

   // Get the path we will read
   if (pathname) strcpy(tgt_dir, pathname);
   else {
      // Get the cwd if no path is provided.
      if (getcwd(tgt_dir, buffSize) == NULL) {
         perror("getcwd() error");
         return;
      }
   }

   // Check read permission for the path


   // Read the path
   DIR * dir = opendir(tgt_dir);
   char * filename;
   struct dirent *dp;
   struct stat fs;

   // TODO: print the total number of files in dir
   while ((dp=readdir(dir))) {
      filename = dp->d_name;
      // Show/not show directory path
      if (!strcmp(filename, ".") || !strcmp(filename, "..")) continue;
      // Print ls data
      if (stat(filename, &fs)) perror(filename);
      else {
         // handle directory pathing
         if (false) printf(" %s", filename); //filename
         // handle hidden files
         else if (filename[0] == '.' && true) continue;
         else {
            printPermission(&fs); // Permissions
            printf(" %3d", fs.st_nlink); // # of hard links
            printf(" %s", getpwuid(fs.st_uid)->pw_name); // Owner name
            printf("  %s", getgrgid(fs.st_gid)->gr_name); // Owner group
            printf(" %8lld", fs.st_size); // file size
            printLastMod(fs.st_mtime); // last modified
            printf(" %s\n", filename); //filename
         }
      }
   }
   if (false) printf("\n");
   closedir(dir);
}

void printLastMod(time_t val) {
   char lastMod[80];
   struct tm *info = localtime(&val); /* convert to struct tm */
   strftime(lastMod, 80, "%b %d %H:%M", info);
   printf(" %s", lastMod);
}

void printPermission(struct stat* fs) {
   printf( (S_ISDIR(fs->st_mode)) ? "d" : "-");
   printf( (fs->st_mode & S_IRUSR) ? "r" : "-");
   printf( (fs->st_mode & S_IWUSR) ? "w" : "-");
   printf( (fs->st_mode & S_IXUSR) ? "x" : "-");
   printf( (fs->st_mode & S_IRGRP) ? "r" : "-");
   printf( (fs->st_mode & S_IWGRP) ? "w" : "-");
   printf( (fs->st_mode & S_IXGRP) ? "x" : "-");
   printf( (fs->st_mode & S_IROTH) ? "r" : "-");
   printf( (fs->st_mode & S_IWOTH) ? "w" : "-");
   printf( (fs->st_mode & S_IXOTH) ? "x" : "-");
}
