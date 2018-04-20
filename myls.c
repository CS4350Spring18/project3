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


void myls(const char *pathname, char mode, char viewMode);

void printLastMod(const time_t val);

void listPrint(const struct stat* fs, const char* filename);

void printPermission(const struct stat* fs);

struct option longopts[] = {
   { 0, 0, 0, 0 },
};


int main(int argc, char* argv[]) {
   int c;
   int lopt=false,
       aopt=false;
   while ((c = getopt_long(argc, argv, ":al", longopts, NULL)) != -1) {
      switch (c) {
         case 'a':
            aopt = true;
            break;
         case 'l':
            lopt = true;
            break;
         case '?': default:
            break;
      }
   }

   // Determine the path
   const char* pathname = (optind == argc) ? NULL:argv[optind++];

   // Run myls with flags
   myls(
      pathname,            // path to print. defaults to '.'
      (lopt) ? 'l':'r',    // toggle list mode
      (aopt) ? 'a':'l'     // toggle all or limited viewMode
   );
   exit(EXIT_SUCCESS);
}


void myls(const char* pathname, char mode, char viewMode) {
   int buffSize = 1024;
   char tgt_dir[buffSize];
   // TODO: implement relative path handling

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
   bool firstPrint = true;
   struct dirent *dp;
   struct stat fs;

   // TODO: print the total number of files in dir
   while ((dp=readdir(dir))) {
      filename = dp->d_name;
      // Print ls data
      if (stat(filename, &fs)) perror(filename);
      else {
         // handle hidden files
         if (filename[0] == '.' && viewMode == 'l') continue;
         if (!strcmp(filename, ".") && viewMode == 'l') continue;
         if (!strcmp(filename, "..") && viewMode == 'l') continue;

         // handle directory pathing
         if (mode == 'r') {
            if (firstPrint) {
               printf("%s", filename); //filename
               firstPrint = false;
            } else printf("%10s", filename);
         } else listPrint(&fs, filename);
      }
   }
   if (mode == 'r') printf("\n");
   closedir(dir);
}

void printLastMod(const time_t val) {
   char lastMod[80];
   struct tm *info = localtime(&val); /* convert to struct tm */
   strftime(lastMod, 80, "%b %d %H:%M", info);
   printf(" %s", lastMod);
}

void listPrint(const struct stat* fs, const char* filename) {
   printPermission(fs); // Permissions
   printf(" %3d", fs->st_nlink); // # of hard links
   printf(" %s", getpwuid(fs->st_uid)->pw_name); // Owner name
   printf("  %s", getgrgid(fs->st_gid)->gr_name); // Owner group
   printf(" %8lld", fs->st_size); // file size
   printLastMod(fs->st_mtime); // last modified
   printf(" %s\n", filename); //filename
}

void printPermission(const struct stat* fs) {
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
