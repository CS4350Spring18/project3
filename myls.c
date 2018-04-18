#include "myls.h"

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
   DIR *dir;
   struct dirent *dp;
   char * file_name;
   dir = opendir(tgt_dir);
   while ((dp=readdir(dir)) != NULL) {
      printf("debug: %s\n", dp->d_name);
      if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )    {
         // do nothing (straight logic)
      } else {
         file_name = dp->d_name; // use it
         printf("file_name: \"%s\"\n",file_name);
      }
   }
   closedir(dir);
}
