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
      printf("Usage: \n      mycp <sourceFile> <destFile>\n");
      return EXIT_FAILURE;
   }

   int srcf = open(argv[1], O_RDONLY);
   if(srcf == -1){
      printf("Error opening file: %s", argv[1]);
      return EXIT_FAILURE;
   }
   int destf = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);      
   if(destf == -1){
      printf("Error opening file: %s", argv[2]);
      return EXIT_FAILURE;
   }
   char buf[1];
   while(eof(srcf) != true){
      read(srcf, buf, sizeof(char));
      write(destf, buf, sizeof(char));
   }

   close(srcf);
   close(destf);

   return EXIT_SUCCESS;
}
