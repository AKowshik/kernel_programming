/*Tool to change file permissions */

#define _GNU_SOURCE

#include <stdio.h> //printf
#include <sys/stat.h> //chmod
#include <errno.h> // program_invocation_short_name
#include <unistd.h> //EXIT_*
#include <stdlib.h> //exit()


int main(int argc, char *argv[])
{

	if( argc != 3 )
	{
		printf("Usage: %s </path/to/filename> <permissions>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	const char *pathname;
	mode_t mode;
	int ret_value;
	char *endptr;

	pathname = argv[1];
	mode = strtol(argv[2], &endptr, 8);

	ret_value = chmod(pathname, mode);
	
	if(ret_value == -1 )
	{
		printf("Could not change file permissions for file %s. Error %s, %d", pathname, strerror(errno), errno);
		exit(EXIT_FAILURE);
	}
	
	return(EXIT_SUCCESS);
}
