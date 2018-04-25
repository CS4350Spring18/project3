#include "mycd.h"

void mycd(char *filename){
   char cwd[250];
   struct stat info;
   lstat(filename, &info);

   // check to see if it's a directory
   if(S_ISDIR(info.st_mode)) {
      // if insufficient permissions
      if( access(filename, X_OK) ) {
         printf("mycd: %s: Permission denied.\n", filename);
         return;
      }
   }
   // not a directory
   else {
      if(S_ISREG(info.st_mode))
         printf("mycd: %s: Not a directory\n", filename);
      else
         printf("mycd: %s: No such file or directory\n", filename);
      return;
   }

   // if the file is already an absolute path
   if(filename[0] == '/') {
      chdir(filename);
   }
   else {
      getcwd(cwd,sizeof(cwd));
      strcat(cwd,"/");
      strcat(cwd,filename);
      chdir(cwd);
   }
}

//For testing independent of mysh
int main(int argc, char **argv)
// {
//    char cwd[250];
//    getcwd(cwd,sizeof(cwd));
//    printf("cwd is %s\n", cwd);

//    // mycd("..");
//    mycd(argv[1]);

//    getcwd(cwd,sizeof(cwd));
//    printf("cwd is %s\n", cwd);
// }