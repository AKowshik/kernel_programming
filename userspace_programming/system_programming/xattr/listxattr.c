/*Tool to list extended attributes*/

#define _GNU_SOURCE

#include <stdio.h> //printf
#include <sys/types.h>
#include <sys/xattr.h> //setxattr
#include <errno.h> //program_invocation_short_name
#include <stdlib.h> //EXIT_*
#include <unistd.h> //exit
#include <malloc.h> //malloc
#include "my_getxattr.h" //my_getxattr
#include <string.h>

int main(int argc, char *argv[])
{

	if( argc != 2 ) 
	{
		printf("Usage: %s <path/to/file>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	const char *path;
	char *list, *value, *key;
	ssize_t list_size, keylen;

	path = argv[1];
	
	list_size = listxattr(path, NULL, 0);
	if( list_size == -1 )
	{
		printf("Unable to get extended attributes to file %s. Error: %s, %d\n", path, strerror(errno), errno);
                exit(EXIT_FAILURE);
	}
	else if ( list_size > 0 )
	{
		list = malloc( list_size );
		if( list == NULL )
		{
			printf("Failed to allocate memory for value. Error: %s, %d\n", strerror(errno), errno);
			exit(EXIT_FAILURE);
		}
		
		list_size = listxattr(path, list, list_size);
		if( list_size == -1 )
        	{
                	printf("Unable to get extended attributes to file %s. Error: %s, %d\n", path, strerror(errno), errno);
                	exit(EXIT_FAILURE);
       		} 
		
		key = list;	
		while ( list_size > 0 )
		{
			
			ssize_t attr_size;
			attr_size = my_getxattr(path, key);
			keylen = strlen(key) + 1;
               		list_size -= keylen;
               		key += keylen;
		}
	}
	else 
	{
		printf("No extended attributes exist\n");
		exit(EXIT_SUCCESS);
	}

	return (EXIT_SUCCESS);
}

			

