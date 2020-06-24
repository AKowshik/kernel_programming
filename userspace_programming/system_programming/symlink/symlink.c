/* Tool to create symlinks on a file */

# define _GNU_SOURCE 
# include <unistd.h> // symlink
# include <stdio.h> // printf
# include <errno.h> // program_invocation_short_name
# include <stdlib.h> //exit

int main(int argc, char *argv[])
{
	if( argc != 3 )
        {
                printf("usage: %s </path/to/target> <path/to/linkpath>\n", program_invocation_short_name);
                exit(EXIT_FAILURE);
        }

	char *target;
	char *linkpath;
	int ret_val;

	target = argv[1];
	linkpath = argv[2];

	
	ret_val = symlink(target, linkpath);

	if( ret_val == -1) 
	{
		printf("Error creating symlink. Error %s \n", strerror(errno));
		return(EXIT_FAILURE);
	}

	return(EXIT_SUCCESS);
}


