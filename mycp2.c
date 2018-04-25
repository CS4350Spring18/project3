#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>

bool eof(int fd){

   off_t currentOff = lseek(fd, 0, SEEK_CUR);

   bool isEof = (currentOff == lseek(fd, 0, SEEK_END));

   lseek(fd, currentOff, SEEK_SET);

   return isEof;
}


int main(int argc, char** argv){

   if(argc != 3){
      printf("Usage: \n      mycp <sourceFile/Dir> <destFile/Dir>\n");
      return EXIT_FAILURE;
   }

   struct stat arg1;
   struct stat arg2;
   lstat(argv[1], &arg1);
   lstat(argv[2], &arg2);

   // 
   if(S_ISDIR(arg1.st_mode) && S_ISDIR(arg2.st_mode)) {
      printf("arg1 and arg2 are directories.\n");
   }

}
