/* Tool to create an anonymous file */
#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define HAVE_MEMFD_CREATE 1

int main(int argc, char *argv[])
{
	int mfd;
	unsigned int mfd_flags;
	const char *mfd_name, *mfd_data;
	ssize_t rec;

	if(argc != 2)
	{
		printf("Usage %s </path/to/file> /n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}
	
	mfd_flags = ( MFD_ALLOW_SEALING );
	mfd = memfd_create(argv[1], mfd_flags);

	if(mfd == -1)
	{
		int err = errno;
		printf("Could not create memfd file \"%s\". Error &d, Error Message: %s\n", err, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	mfd_data = "Hello World";
	rec = write(mfd, mfd_data, strlen(mfd_data));
	if(rec == -1)
	{
		int err = errno;
		printf("Could not write to the file. Error %d, Message: %s\n", err, strerror(err));
	}

	pause();
	return(EXIT_SUCCESS);
}
