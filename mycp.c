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

   //determines if -R or -r argument is provided (Rescursive)
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

   if(ropt){ //decides if how which argument to open
      stat(argv[2],&buf);
   }
   else{
      stat(argv[1],&buf);
   }

   if(S_ISDIR(buf.st_mode)){
      if(ropt){
         char *oldName;
         char newName[300];
         
         //copies arguments to be used as paths
         strcat(oldName, argv[2]);
         strcat(newName, argv[3]);

         //creates inital directory before function
         mkdir(argv[3], 0777);
         chdir(argv[3]);
         getcwd(newName, 299);
         chdir("..");//returns to parent
         SearchDirectory(oldName, newName);
      }
      else{
         mkdir(argv[2], 0777);
      }
   }
   else{
      if (ropt){ //opens file if argument provided
         srcf = open(argv[2], O_RDONLY); 
      }
      else{ //opens file if no argument provided
         srcf = open(argv[1], O_RDONLY);
      }  
      if(srcf == -1){ //errors if no parameters
         printf("Error opening file: %s \n", argv[1]);
         return EXIT_FAILURE;
      }

      //opens file to write to and errors if does not open
      int destf = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC); 
      if(destf == -1){
         printf("Error opening file: %s \n", argv[2]);
         return EXIT_FAILURE;
      }

      char buf1[1]; //reads and writes
      while(eof(srcf) != true){
         read(srcf, buf1, sizeof(char));
         write(destf, buf1, sizeof(char));
      }

      close(srcf);
      close(destf);
   }
   return EXIT_SUCCESS;
}

/* end of file                 */
/* reads until end of file    */
bool eof(int fd){

   off_t currentOff = lseek(fd, 0, SEEK_CUR);
   bool isEof = (currentOff == lseek(fd, 0, SEEK_END));
   lseek(fd, currentOff, SEEK_SET);

   return isEof;
}
/* Search The Directory   */
/* Takes a current path and a new path
 * newName - New path that files and directories will be created
 * name - Old path that files are copied from  */
void SearchDirectory(const char *name, const char *newName) {
    char tempName[PATH_MAX];

    realpath(name, tempName);
    DIR *dir = opendir(tempName);
    
    if(dir) {
        char Path[256], *EndPtr = Path; //pointer path
        struct dirent *iter;
        strcpy(tempName, "/");
        strcpy(Path, tempName);
        EndPtr += strlen(tempName);


        while((iter = readdir(dir)) != NULL) { //reads dir into iterator
            struct stat info;
            
            strcpy(EndPtr, iter->d_name);

            //does not path through . or .. dir
            if (!strcmp(EndPtr, ".")) continue;
            if (!strcmp(EndPtr, "..")) continue;

            //retrieved from a file relative to the dir w/ file descriptor instead of the current working dir
            if (fstatat(dirfd(dir), iter->d_name, &info, 0) < 0)
            {
                  perror(iter->d_name);
                  continue;
            }

            if (S_ISDIR(info.st_mode)){ //if directory
                  chdir (newName); //change to new directory
                  strcat(newName, Path); //copies the name of the new directory to the new path
                  mkdir(newName, 0777); //makes the directory
                  chdir (Path); //changes back to directory
                  SearchDirectory(Path, newName);   //iterates down dir
            } 
            else if(S_ISREG(info.st_mode)) { //if regular file
               strcat(newName, Path); //copies file name ontop of new path
               int srcf = open(Path, O_RDONLY); //opens file in current dir
               int destf = open(newName, O_WRONLY | O_CREAT | O_TRUNC); //writes in new dir

               char buf1[1];
               while(eof(srcf) != true){ //reads and writes
                  read(srcf, buf1, sizeof(char));
                  write(destf, buf1, sizeof(char));
               }

               close(srcf);
               close(destf);
            }
        }
    }
}
