#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <linux/limits.h>
#include <string.h>

bool eof(int fd);

void SearchDirectory(const char *name, const char *newName);

struct option longopts[] = {
   { 0, 0, 0, 0 },
};

int main(int argc, char** argv){
   int c;
   int ropt=false;
   int srcf;

   while((c = getopt_long(argc, argv, ":Rr", longopts, NULL)) != -1){
      switch (c) {
         case 'R': case 'r':
            ropt = true;
            break;
         case '?': default:
            break;
      }
   }

   struct stat buf;

   if(ropt){
      stat(argv[2],&buf);
   }
   else{
      stat(argv[1],&buf);
   }

   if(S_ISDIR(buf.st_mode)){
      if(ropt){
         char *oldName;
         char newName[300];
         
         strcat(oldName, argv[2]);
         strcat(newName, argv[3]);

         mkdir(argv[3], 0777); 
         SearchDirectory(oldName, newName);
      }
      else{
         mkdir(argv[2], 0777);
      }
   }
   else{
      if (ropt){
         srcf = open(argv[2], O_RDONLY);
      }
      else{
         srcf = open(argv[1], O_RDONLY);
      }  
      if(srcf == -1){
         printf("Error opening file: %s \n", argv[1]);
         return EXIT_FAILURE;
      }

      int destf = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);      
      if(destf == -1){
         printf("Error opening file: %s \n", argv[2]);
         return EXIT_FAILURE;
      }

      char buf1[1];
      while(eof(srcf) != true){
         read(srcf, buf1, sizeof(char));
         write(destf, buf1, sizeof(char));
      }

      close(srcf);
      close(destf);
   }
   return EXIT_SUCCESS;
}

bool eof(int fd){

   off_t currentOff = lseek(fd, 0, SEEK_CUR);
   bool isEof = (currentOff == lseek(fd, 0, SEEK_END));
   lseek(fd, currentOff, SEEK_SET);

   return isEof;
}

void SearchDirectory(const char *name, const char *newName) {
    char tempName[PATH_MAX];

    realpath(name, tempName);
    DIR *dir = opendir(tempName);
    
    if(dir) {
        char Path[256], *EndPtr = Path;
        struct dirent *iter;
        strcpy(Path, tempName);
        EndPtr += strlen(tempName);

        while((iter = readdir(dir))) { //reads dir into iterator
            struct stat info;
            strcpy(EndPtr, iter->d_name);

            if(!stat(Path, &info)) {
               printf("ss");
                if(S_ISDIR(info.st_mode)) { //S_ISDIR checks if directory
                    printf("ranISDIR");
                    SearchDirectory(Path, newName);   //iterates down dir
                    //todo
                } 
                else if(S_ISREG(info.st_mode)) { 
                   //todo
                }
            }
        }
    }
}
