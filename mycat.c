#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void DisplayStream(int fd)
{
    char buffer[BUFFER_SIZE];

    int bytesRead;
	do
	{
		bytesRead = read(fd, buffer, BUFFER_SIZE);
		for(int i = 0; i < bytesRead; ++i)
		{
			printf("%c", buffer[i]);
		}
	}
	while (bytesRead != 0);
}

int main(int argc, char** argv)
{
	// Open the specified file and display in stdout if provided by the args
	// Otherwise read the stdin for input.
	if (argc <= 1)
	{
		// read stdin, but do not close
		DisplayStream(0);
		return 0;
	}

	int fd = open(argv[1], O_RDONLY);
	DisplayStream(fd);
	close(fd);

	return 0;
}
