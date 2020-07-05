/* Tool to change file owners */

#define _GNU_SOURCE

#include <stdio.h> //pritnf
#include <unistd.h> //chown, EXIT_*
#include <stdlib.h> //exit
#include <errno.h> //program_invocation_short_name

int main(int argc, char *argv[])
{
	
	if( argc != 4)
	{
		printf("Usage %s </path/to/file> <uid> <gid>. Pass -1 if uid or gid are not to be changed.\n", program_invocation_short_name);
		return(EXIT_FAILURE); 
	}

	const char *pathname;
	uid_t owner;
	gid_t group;
	int ret_value;
	char *uid_endp;	
	char *gid_endp;

	pathname = argv[1];

	owner = strtol(argv[2], &uid_endp, 10);
	group = strtol(argv[3], &gid_endp, 10); 

	ret_value = chown(pathname, owner, group);
	if(ret_value == -1 )
        {
                printf("Could not change owners for file %s. Error %s, %d", pathname, strerror(errno), errno);
                exit(EXIT_FAILURE);
        }

	return(EXIT_SUCCESS);
}
