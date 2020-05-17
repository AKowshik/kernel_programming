/*Tool to demo iter based read using readv */

# define _GNU_SOURCE
# include <errno.h> //program_invication_short_name
# include <stdio.h> //printf()
# include <unistd.h> // readv(), writev()
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h> // open()
# include <string.h>
# include <stdlib.h> //exit()
# include <sys/uio.h> 
# include <sys/types.h>

typedef struct emp_s
{
	char e_name[20];
	char e_des[20];
	char e_id[4];
} emp_s;

int main (int argc, char *argv[])
{
	const char *filename;
	int fh;
	size_t nrBytes;
	size_t sret;
	int flags;
	mode_t mode;

	emp_s e1;

	struct iovec iov[3];
	
	if( argc != 2 )
	{
		printf("usage: %s </path/to/file>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	
	/* open the file in read mode */
	flags = ( O_RDONLY);
	mode = ( S_IRUSR );
	fh = open(filename, flags, mode);
	if( fh == -1 )
	{

		printf("Failed to open \"%s\" %m\n", filename);
		exit(EXIT_FAILURE);
	}


	iov[0].iov_base = e1.e_name;
        iov[0].iov_len = sizeof(e1.e_name);

        iov[1].iov_base = e1.e_des;
        iov[1].iov_len = sizeof(e1.e_des);

        iov[2].iov_base = e1.e_id;
        iov[2].iov_len = sizeof(e1.e_id);


	nrBytes = readv(fh, iov, 3);
	printf("Number of bytes read is %d\n", nrBytes);	

	printf("Employee name: %s\n", iov[0].iov_base);
	printf("Employee designation: %s\n", iov[1].iov_base);

	/* Cast the void* value in struct iovec to int* and store it */	
	int *id = iov[2].iov_base;
	printf("Employee ID: %d\n", *id);
		
	close(fh);
	return 0;
}
