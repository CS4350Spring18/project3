#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_COMMAND_SIZE = 1024;

char* ReadCommand(int count, int* size)
{
	char* command = malloc(sizeof(char) * MAX_COMMAND_SIZE);

	// Prompt
	printf("$> ", count);

	// Read user command

	fgets(command, MAX_COMMAND_SIZE, stdin);

	// Remove new line
	int i = strlen(command) - 1;
	if(command[i] == '\n')
		command[i] = '\0';

	return command;
}

int main(int arc, char** argv)
{
  int count = 1;
  
  while (1) 
  {
	  int size=0;
	  char* command = ReadCommand(count++, &size);
	  free(command);
  }
	  return 0;
}
