#include "mycp.h"

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
         char oldName[300];
         char newName[300];

         //copies arguments to be used as paths
         strcat(oldName, argv[2]);
         strcat(newName, argv[3]);

         //creates inital directory before function
         chdir(argv[2]);
         getcwd(oldName, 299);
         chdir("..");
         mkdir(argv[3], 0777);
         chdir(argv[3]);
         getcwd(newName, 299);
         chdir("..");//returns to parent
         SearchDirectory(oldName, newName);
      }
      else
         printf("cp: %s is a directory (not copied).\n", argv[1]);
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

      // give the permissions of the copy file to the destination file
      char cwd[250];
      getcwd(cwd,sizeof(cwd));
      // if the file is already an absolute path
      if( argv[2][0] != '/') {
         getcwd(cwd,sizeof(cwd));
         strcat(cwd,"/");
         strcat(cwd,argv[2]);
      }
      chmod(cwd, buf.st_mode);

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
    char newNameCopy[256] = "";
    strcat(newNameCopy, newName);

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
            strcpy(newName, newNameCopy);
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
                  strcat(name, Path);

                  mkdir(newName, 0777); //makes the directory
                  chdir (name); //changes back to directory

                  SearchDirectory(name, newName);   //iterates down dir
            } 
            else if(S_ISREG(info.st_mode)) { //if regular file
               char tempPath[256] = "";
               char origPath[256] = "";
               strcat(origPath, name);
               strcat(origPath, Path);
               strcat(tempPath, newName);
               strcat(tempPath, Path); //copies file name ontop of new path

               char buf1[256];
               int srcf = open(origPath, O_RDONLY); //opens file in current dir
               chdir(newName);
               int destf = open(tempPath, O_WRONLY | O_CREAT | O_TRUNC); //writes in new dir
               chmod(tempPath, info.st_mode);
               chdir(name);

               while(eof(srcf) != true){
                  read(srcf, buf1, sizeof(char));
                  write(destf, buf1, sizeof(char));
               }

               close(srcf);
               close(destf);
            }
        }
    }
    chdir("..");
}
