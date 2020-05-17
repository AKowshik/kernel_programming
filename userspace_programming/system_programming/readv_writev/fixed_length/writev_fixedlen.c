/*Tool to demo iter based write using writev */

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

typedef struct emp_s
{
	char e_name[20];
	char e_des[20];
	int e_id;
} emp_s;

int main (int argc, char *argv[])
{
	const char *filename;
	int fh;
	size_t nrBytes;
	size_t sret;
	int flags;
	mode_t mode;

	emp_s e1 = {"emp1", "entrylevel", 1234};
	struct iovec iov[3];

	if( argc != 2 )
	{
		printf("usage: %s </path/to/file>\n", program_invocation_short_name);
		
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	
	/* open the file in write/append mode and create if needed */
	flags = ( O_RDWR | O_APPEND | O_CREAT | O_EXCL);
	mode = ( S_IRUSR );
	fh = open(filename, flags, mode);
	if( fh == -1 )
	{

		printf("Failed to open \"%s\" %m\n", filename);
		exit(EXIT_FAILURE);
	}
	
	/* write using iter syntax */
	iov[0].iov_base = e1.e_name;
	iov[0].iov_len = sizeof(e1.e_name);
	
	iov[1].iov_base = e1.e_des;
	iov[1].iov_len = sizeof(e1.e_des);

	iov[2].iov_base = &e1.e_id;
	iov[2].iov_len = sizeof(e1.e_id);

	sret = writev(fh, iov, sizeof(iov)/sizeof(iov[0]));
	if (sret == -1)
	{
		printf("Failed to write emp data using writev: %m\n");
		close(fh);
		exit(EXIT_FAILURE);
	}

	close(fh);	
	return 0;
}
