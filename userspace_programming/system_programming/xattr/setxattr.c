/*Tool to set extended attributes*/

#define _GNU_SOURCE

#include <stdio.h> //printf
#include <sys/types.h>
#include <sys/xattr.h> //setxattr
#include <errno.h> //program_invocation_short_name
#include <stdlib.h> //EXIT_*
#include <unistd.h> //exit
#include <string.h> //strlen

int main(int argc, char *argv[])
{

	if( argc != 4 ) 
	{
		printf("Usage: %s <path/to/file> <key> <value>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	int res;
	const char *path, *name, *value;
	size_t size;
	int flags;

	path = argv[1];
	name = argv[2];
	value = argv[3];
	size = strlen(value);
	flags = ( XATTR_CREATE );

	res = setxattr(path, name, value, size, flags);
	if( res == -1 )
	{
		printf("Unable to  sett extended attributes to file %s. Error: %s, %d\n", path, strerror(errno), errno);
		exit(EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}

			

