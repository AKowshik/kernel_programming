/*Tool to get extended attributes*/

#define _GNU_SOURCE

#include <stdio.h> //printf
#include <sys/types.h>
#include <sys/xattr.h> //setxattr
#include <errno.h> //program_invocation_short_name
#include <stdlib.h> //EXIT_*
#include <unistd.h> //exit
#include <malloc.h> //malloc
#include <string.h>


ssize_t my_getxattr(const char *path, const char *name)
{

	char *value;
	ssize_t size;

	size = getxattr(path, name, NULL, 0);
	if( size == -1 )
	{
		printf("Unable to get extended attributes to file %s. Error: %s, %d\n", path, strerror(errno), errno);
                exit(EXIT_FAILURE);
	}
	else if ( size > 0 )
	{
		value = malloc( size + 1 );
		if( value == NULL )
		{
			printf("Failed to allocate memory for value. Error: %s, %d\n", strerror(errno), errno);
			exit(EXIT_FAILURE);
		}
		
		size = getxattr(path, name, value, size);
		if( size == -1 )
        	{
                	printf("Unable to get extended attributes to file %s. Error: %s, %d\n", path, strerror(errno), errno);
                	exit(EXIT_FAILURE);
       		 } 
		
		printf("%s: %s\n", name, value);
	}
	else 
	{
		printf("No value exists\n");
		exit(EXIT_SUCCESS);
	}

	return (size);
}

			
int main(int argc, char *argv[])
{
	
	if( argc != 3 )
        {
                printf("Usage: %s <path/to/file> <key> \n", program_invocation_short_name);
                exit(EXIT_FAILURE);
        }

	ssize_t ret_value;
	const char *path, *name;
	
	path = argv[1];
        name = argv[2];


	ret_value = my_getxattr(path, name);

	return(EXIT_SUCCESS);
}
