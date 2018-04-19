#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

int processRunner();

int main(int argc, char **argv){

   if(argc > 1){
      printf("This command does not take arguments or options\n");
   }

   char *commandBuffer;
   size_t commandLength = 0;
   char **myArgv;
   char **redirects;
   int myArgc = 0;
   int piped = 0;
   char **pipedArgv;
   char **pipedRedirects;
   int pipedArgc = 0;

   printf("mysh$> ");
   while( getline(&commandBuffer, &commandLength, stdin) != -1) {
      myArgc = 0;
      pipedArgc = 0;
      if(commandLength == 8 && strncmp(commandBuffer, "exit", (size_t) 4) == 0){
         return EXIT_SUCCESS;
      }

      if (strncmp(commandBuffer, "myls", (size_t) 4) == 0) {
         if (processRunner() == -1) printf("Error");
      }

	// Remove new line
	if(commandBuffer[commandLength - 1] == '\n')
		commandBuffer[commandLength - 1] = '\0';

      myArgv = (char**)malloc(sizeof(char*) * (commandLength));
      redirects = (char**)malloc(sizeof(char*) * (commandLength));
      myArgv[0] = strtok(commandBuffer, " ");
      myArgc++;
      for(size_t i = 1; i < commandLength; i++){
         myArgv[i] = strtok(NULL, " ");
         if(myArgv[i] == NULL)
            break;
         myArgc++; 
      }

      int temp = 0;
      for(int i = 0; i < myArgc; i++){
         if(strlen(myArgv[i]) == 1 && strncmp(myArgv[i], "|", 1)){
            piped = 1;
            pipedArgv = (char**)malloc( sizeof(char*) * (myArgc - i - 1) );
            pipedRedirects = (char**)malloc( sizeof(char*) * (myArgc - i) );
            i++;
         }

         if(piped == 1){
            if(strchr(myArgv[i], '<') != NULL || strchr(myArgv[i], '>') != NULL){
               //TODO: Handle Redirects
               continue;
            }
            pipedArgv[i-myArgc] = (char*) calloc( strlen(myArgv[i]) + 1, sizeof(char) );
            pipedArgc++;
         } else {
            if(strchr(myArgv[i], '<') != NULL || strchr(myArgv[i], '>') != NULL){
               //TODO: Handle Redirects
               continue;
            }
            temp++;
         }
      }
      myArgc = temp;

      //TODO:Make a child process and run parsed command with redirections
/*
      free(commandBuffer);
      free(myArgv);
      if(pipedArgv != NULL)
         free(pipedArgv);
      if(redirects != NULL)
         free(redirects);
      if(pipedRedirects != NULL)
         free(pipedRedirects);
  */    
      printf("mysh$> ");
   }

   return EXIT_FAILURE;

}

int processRunner() {
   pid_t pid;
   int status;
   int ret;
   if ((pid=fork())==0) {
      //printf("command buffer: %s\n", commandBuffer);
      char command[500];
      //strcpy(command, commandBuffer);
      execl("/bin/bash", "bash", "-c", "./myls", NULL);
      ret = -1;
   } else if (pid>0) {
      wait(&status);
      ret = WEXITSTATUS(status);
   } else {
      ret = -1;
   }
}
