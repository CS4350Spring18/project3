#include "mycd.h"

// Returns true if changed directory successfully, false if it did not.
bool mycd(char *filename){
   char cwd[250];

   // if the file is already an absolute path
   if(filename[0] == '/') {
      if( chdir(filename) )
         return true;
      else
         return false;
   }
   else {
      getcwd(cwd,sizeof(cwd));
      strcat(cwd,"/");
      strcat(cwd,filename);
      if( chdir(cwd) )
         return true;
      else
         return false;
   }
}

// For testing independent of mysh
// int main(int argc, char **argv)
// {
//    char cwd[250];
//    getcwd(cwd,sizeof(cwd));
//    printf("cwd is %s\n", cwd);

//    mycd("..");

//    getcwd(cwd,sizeof(cwd));
//    printf("cwd is %s\n", cwd);
// }
