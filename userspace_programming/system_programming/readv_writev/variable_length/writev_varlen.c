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
	int e_name_size;
	int e_des_size;
	int emp_s_size;
	char *e_name;
	char *e_des;
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

	struct iovec iov[6];

	if( argc != 5 )
	{
		printf("usage: %s </path/to/file> employee_name employee_designation employee_id \n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	
	emp_s e1 = { strlen(argv[2]) + 1, strlen(argv[3]) + 1, strlen(argv[2]) + strlen(argv[3]) + 4*sizeof(int) + 2 , argv[2], argv[3], atoi(argv[4]) };
 	
	/* open the file in write/append mode and create if needed */
	flags = ( O_RDWR | O_APPEND | O_CREAT | O_EXCL);
	mode = ( S_IRUSR );
	fh = open(filename, flags, mode);
	if( fh == -1 )
	{

		printf("Failed to open \"%s\" %m\n", filename);
		exit(EXIT_FAILURE);
	}
	
	printf("e_name_size is %d\n",e1.e_name_size);
	printf("size of e_name_size is %d\n",sizeof(e1.e_name_size));
	printf("e_des_size is %d\n",e1.e_des_size);
        printf("size of e_des_size is %d\n",sizeof(e1.e_des_size));
	printf("emp_s__size is %d\n",e1.emp_s_size);
        printf("size of epm_s_suze is %d\n",sizeof(e1.emp_s_size));
	printf("e_name is %s\n",e1.e_name);
	printf("e_des is %s\n",e1.e_des);
	printf("e_id is %d\n",e1.e_id);

	iov[0].iov_base = &e1.e_name_size;
	iov[0].iov_len = sizeof(e1.e_name_size);
	
	iov[1].iov_base = &e1.e_des_size;
	iov[1].iov_len = sizeof(e1.e_des_size);

	iov[2].iov_base = &e1.emp_s_size;
	iov[2].iov_len = sizeof(e1.emp_s_size);

	iov[3].iov_base = e1.e_name;
	iov[3].iov_len = e1.e_name_size;
	
	iov[4].iov_base = e1.e_des;
	iov[4].iov_len = e1.e_des_size;

	iov[5].iov_base = &e1.e_id;
	iov[5].iov_len = sizeof(e1.e_id);

	sret = writev(fh, iov, 6);
	if(sret == -1)
	{
		printf("Failed to write em data using writev: %m\n");
		close(fh);
		exit(EXIT_FAILURE);	
	}

	close(fh);	
	
	return 0;
}
