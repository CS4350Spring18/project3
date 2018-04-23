#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "mycd.h"
#include "mypwd.h"

int parseAndExec();

int parseToken();

int processRunner();

int main(int argc, char **argv){

   if(argc > 1){
      printf("This command does not take arguments or options\n");
   }
   
   char *commandBuffer;
   size_t commandLength = 0;   

   printf("mysh$> ");

   while( getline(&commandBuffer, &commandLength, stdin) != -1) {
	// Remove new line
      int k = 0;
      while(commandBuffer[k] != '\n')
         k++;

      commandBuffer[k] = '\0';

      if(k >= 4 && strncmp(commandBuffer, "exit", (size_t) 4) == 0){
         return EXIT_SUCCESS;
      }
      
      parseAndExec(commandBuffer, k); 

      //free(commandBuffer);
    
      printf("mysh$> ");
   }

   return EXIT_FAILURE;

}


int parseAndExec(char* commandBuffer, int commandLength){

   char **myArgv;
   char **redirects;
   int redCount = 0;
   int myArgc = 0;

   int piped = 0;
   char **pipedArgv;
   char **pipedRedirects;
   int pipedRedCount = 0;
   int pipedArgc = 0;

   myArgc = 0;
   pipedArgc = 0;

   myArgv = (char**)malloc(sizeof(char*) * (commandLength));
   redirects = (char**)malloc(sizeof(char*) * (commandLength));
   myArgv[0] = strtok(commandBuffer, " ");
   myArgc++;
   for(int i = 1; i < commandLength; i++){
      myArgv[i] = strtok(NULL, " ");
      if(myArgv[i] == NULL)
         break;
      myArgc++; 
   }

   int tempArgc = 0;
   char** tempArgv = (char**)malloc(sizeof(char*) * (myArgc + 1));
   for(int i = 0; i < myArgc; i++){
      if(strlen(myArgv[i]) == 1 && strncmp(myArgv[i], "|", 1)){
         piped = 1;
         pipedArgv = (char**)malloc( sizeof(char*) * (myArgc - i - 1) );
         pipedRedirects = (char**)malloc( sizeof(char*) * (myArgc - i) );
         pipedRedirects[0] = "|";
         pipedRedCount++;
         redirects[redCount] = "|";
         redCount++;
         i++;
      }

      //Handle token as part of the piped command
      if(piped == 1){
         parseToken(myArgv[i], myArgc, &(pipedArgv[pipedArgc]), &pipedArgc); 
      } else { //Handle token as part of simple command 
         parseToken(myArgv[i], myArgc, &(tempArgv[tempArgc]), &tempArgc);
      }
      
      //free(myArgv[i]);
   }
   myArgc = tempArgc;
   //free(myArgv);
   myArgv = tempArgv;
   myArgv[myArgc] = '\0';

   processRunner(myArgv[0], myArgv);   
   

   return EXIT_SUCCESS;
}

int parseToken(char* srcToken, int srcCount, char** destToken, int* destCount){

   pid_t pid;

   if(strchr(srcToken, '<') != NULL || strchr(srcToken, '>') != NULL){
      //TODO: Handle Redirects
            
      
   } 
   else if(srcToken[0] == '$'){
      //Not implementing environment variales

      //Retrieve expression result
      int argLength = strlen(srcToken);
      if(srcToken[1] == '(' && srcToken[argLength - 1] == ')'){
         int pp[2];
         if(pipe(pp) < 0) return EXIT_FAILURE;
         if((pid=fork()) == 0){
            dup2(pp[1], 1);
            close(pp[0]);
            close(pp[1]);
            parseAndExec(srcToken, argLength);
            exit(EXIT_SUCCESS);   
         } else{
            int stdin_copy = dup(0);
            dup2(pp[0], 0);
            close(pp[0]);
            close(pp[1]);
            wait(NULL);
            *destToken = (char*) calloc( 10001, sizeof(char) );
            read(0, *destToken, 10000);
            *destCount = *destCount + 1;
            dup2(stdin_copy, 0);
            close(stdin_copy);
         }
      }
   } else {
      *destToken = (char*) calloc( strlen(srcToken) + 1, sizeof(char) );
      strncpy(*destToken, srcToken, sizeof(srcToken));
      *destCount = *destCount + 1;
   }

   return EXIT_SUCCESS;

}

int processRunner(const char* command, char** arguments){

   if(command == NULL || command[0] == '\0') return -1;

   pid_t pid;
   int status;
   int ret;
   if ((pid=fork())==0) {
      execvp(command, arguments);
      ret = -1;
   } else if (pid>0) {
      wait(&status);
      ret = WEXITSTATUS(status);
   } else {
      ret = -1;
   }
   return ret;

}


