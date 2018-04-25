#include "myls.h"


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
   char tgt_path[PATH_MAX];

   // Get the path we will read
   if (pathname) realpath(pathname, tgt_path);
   else {
      // Get the cwd if no path is provided.
      if (getcwd(tgt_path, PATH_MAX) == NULL) {
         perror("getcwd() error");
         return;
      }
   }

   // Check that the path exists
   if (!access(tgt_path, F_OK) == 0) {
      fprintf(stderr, "%s is not a valid pathname.\n", tgt_path);
      exit(EXIT_FAILURE);
   }

   // Check read permission for the path
   if (!access(tgt_path, R_OK) == 0) {
      fprintf(stderr, "%s is not readable (access denied)\n", tgt_path);
      exit(EXIT_FAILURE);
   }

   // Read the path
   DIR * dir = opendir(tgt_path);
   char * filename;
   bool firstPrint = true;
   struct dirent *dp;
   struct stat fs;
   Node *headPtr = NULL;

   // Check if the path is a file
   if ((stat(tgt_path, &fs) == 0) && S_ISREG(fs.st_mode)) {
      //listPrint(&fs, tgt_path);
     insert(&headPtr, fs->d_name, sizeof(char) * strlen(fs->d_name)) 
   } else {
      // TODO: print the total number of system blocks
      // printf("total \n");
      while ((dp=readdir(dir))) {
         filename = dp->d_name;
         char* filepath;
         if (0 > asprintf(&filepath, "%s/%s", tgt_path, filename)) perror(filename);
         insert(&headPtr, filepath, sizeof(char) * strlen(filepath));
         // Print ls data
         if (stat(filepath, &fs)) perror(filename);
         else {
            // handle hidden files
            if (filename[0] == '.' && viewMode == 'l') continue;
            if (!strcmp(filename, ".") && viewMode == 'l') continue;
            if (!strcmp(filename, "..") && viewMode == 'l') continue;

            // handle print mode
            if (mode == 'r') {
               if (firstPrint) {
                  printf("%s", filename);
                  firstPrint = false;
               } else printf("%10s", filename);
            } else listPrint(&fs, filename);
         }
         free(filepath);
      }
      if (mode == 'r') printf("\n");
      printf("**PRINTING NODES**");
      printList(headPtr, printLine);
   }
   closedir(dir);
}


void printLastMod(const time_t val) {
   char lastMod[80];
   struct tm *info = localtime(&val);  // convert to struct tm
   strftime(lastMod, 80, "%b %d %H:%M", info);
   printf(" %s", lastMod);
}


void listPrint(const struct stat* fs, const char* filename) {
   printPermission(fs);                            // Permissions
   printf(" %3d", fs->st_nlink);                   // # of hard links
   printf(" %s", getpwuid(fs->st_uid)->pw_name);   // Owner name
   printf("  %s", getgrgid(fs->st_gid)->gr_name);  // Owner group
   printf(" %8lld", fs->st_size);                  // file size
   printLastMod(fs->st_mtime);                     // last modified
   printf(" %s\n", filename);                      // filename
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
