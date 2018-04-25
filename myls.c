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

   char *filename;
   struct stat fs;
   Node *headPtr = NULL;

   // Check if the path is a file
   if ((stat(tgt_path, &fs) == 0) && S_ISREG(fs.st_mode))
     insert(&headPtr, basename(tgt_path), sizeof(char) * strlen(tgt_path));
   else {
      // Read files from the path
      DIR *dir = opendir(tgt_path);
      struct dirent *dp;

      while ((dp=readdir(dir))) {
         filename = dp->d_name;

         // if viewMode set to limited we don't want to include
         // hidden files or ./..
         if (filename[0] == '.' && viewMode == 'l') continue;
         if (!strcmp(filename, ".") && viewMode == 'l') continue;
         if (!strcmp(filename, "..") && viewMode == 'l') continue;
         insert(&headPtr, filename, sizeof(char) * strlen(tgt_path));
      }
      closedir(dir);
   }

   // Print contents and release resources
   printStatList(headPtr, tgt_path, mode);
   freeNodes(headPtr);
}


void printStatList(const Node *node, char* tgt_path, char mode) {
   if (node == NULL) return;

   char *filepath,
        *filename;
   bool firstPrint = true;
   struct stat fs;

   while (node != NULL) {
      filename = node->data;
      // Create the full path to the file or directory
      if (0 > asprintf(&filepath, "%s/%s", tgt_path, filename)) {
         perror(filename);
         continue;
      }
      // Get stat information
      if (stat(filepath, &fs)) perror(filename);
      else {
         // If mode is read print only the filename
         if (mode == 'r') {
            if (firstPrint) {
               printf("%s", filename);
               firstPrint = false;
            } else printf("%10s", filename);
         // If mode is list print all the details
         } else listPrint(&fs, filename);
      }
      node = node->next;
   }
   if (mode == 'r') printf("\n");
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
