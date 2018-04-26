#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
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

      if(k >= 4 && strncmp(commandBuffer, "exit", 4) == 0){
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
   int myArgc = 0;

   int piped = 0;
   char **pipedArgv;
   int pipedArgc = 0;

   int pp[2];
   int outf = STDOUT_FILENO;
   int inf = STDIN_FILENO;
   int poutf = STDOUT_FILENO;
   int pinf = STDIN_FILENO;

   myArgc = 0;
   pipedArgc = 0;

   myArgv = (char**)malloc(sizeof(char*) * (commandLength));
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
      if(strlen(myArgv[i]) == 1 && strncmp(myArgv[i], "|", 1) == 0){
         piped = 1;
         pipedArgv = (char**)malloc( sizeof(char*) * (myArgc - i - 1) );
         
         if(pipe(pp) < 0) {printf("Could not open pipe.\n"); return EXIT_FAILURE;}
         pinf = pp[0];
         outf = pp[1];

         continue;
      }

      if(myArgv[i][0] == '<' || myArgv[i][0] ==  '>'){
         if(myArgv[i][0] == '<'){
            int temp;
            if((temp = open(myArgv[i]+1, O_RDONLY)))
               inf = temp;
         }
         if(myArgv[i][0] == '>'){
            int temp;
            if((temp = open(myArgv[i]+1, O_WRONLY|O_TRUNC|O_CREAT, 0666)))
               outf = temp;
         }
   
         continue;
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

   if(strncmp(myArgv[0], "mycd", 4) == 0){
      mycd(myArgv[1]);
      return EXIT_SUCCESS;
   }
   if(strncmp(myArgv[0], "mypwd", 5) == 0){
      pwd();
      return EXIT_SUCCESS;
   }
   if(strncmp(myArgv[0], "myls", 4) == 0) {
      char arg[6] = "./myls";
      strncpy(myArgv[0], arg, 6);
   }

   int redirects[2];
   redirects[0] = inf;
   redirects[1] = outf;
   int ret = processRunner(myArgv[0], myArgv, redirects);
   if(ret != 0)
      printf("Error: Return status == %d\n", ret);   
   if(piped == 1){
      redirects[0] = pinf;
      redirects[1] = poutf;
      ret = processRunner(pipedArgv[0], pipedArgv, redirects);
      if(ret != 0)
         printf("Error: Return status == %d\n", ret);   
   }

   return EXIT_SUCCESS;
}

int parseToken(char* srcToken, int srcCount, char** destToken, int* destCount){

   pid_t pid;
 
   if(srcToken[0] == '$'){
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
            char* temp = (char*) calloc(argLength - 3, sizeof(char));
            strncat(temp, srcToken+2, argLength - 3);
            parseAndExec(temp, argLength-3);
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
      strncpy(*destToken, srcToken, strlen(srcToken));
      *destCount = *destCount + 1;
   }

   return EXIT_SUCCESS;

}

int processRunner(const char* command, char** arguments, int* redirects){

   if(command == NULL || command[0] == '\0') return -1;

   pid_t pid;
   int status;
   int ret;

   if ((pid=fork())==0) {
      dup2(redirects[0], 0);
      dup2(redirects[1], 1);
//      close(redirects[0]);
//      close(redirects[1]);
      ret = execvp(command, arguments);
      if(ret != 0 )
         printf("Could not run command.\n");
      exit(ret);
   } else if (pid>0) {
      wait(&status);
      ret = WEXITSTATUS(status);
   } else {
      ret = -1;
   }
   return ret;

}


