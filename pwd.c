#include "pwd.h"

void pwd(){
   char cwd[250];
   getcwd(cwd,sizeof(cwd));
   printf("%s\n", cwd);
}

// For testing independent of pwd
int main(int argc, char **argv)
// {
//    pwd();
// }