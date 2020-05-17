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

/*Making an assumption that the user knows the definition of the structure used to write into the file */
typedef struct emp_s
{
	char e_name_size[4];
	char e_des_size[4];
	char emp_s_size[4];
	char *e_name;
	char *e_des;
	char e_id[4];
} emp_s;

int main (int argc, char *argv[])
{
	const char *filename;
	int fh;
	size_t sret;
	int flags;
	mode_t mode;
	int iovcnt;
	emp_s e1,e2;

	/*struct to store the length of the strings written to file*/
	struct iovec iov_sizes[3];
	
	/*struct to store the complete data written to file*/
	struct iovec iov[6];	

	if( argc != 2 )
	{
		printf("usage: %s </path/to/file> \n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	

	/* open the file in read only mode*/
	flags = ( O_RDONLY);
	mode = ( S_IRUSR );
	fh = open(filename, flags, mode);
	if( fh == -1 )
	{

		printf("Failed to open \"%s\" %m\n", filename);
		exit(EXIT_FAILURE);
	}

	iov_sizes[0].iov_base = e1.e_name_size;
        iov_sizes[0].iov_len = sizeof(e1.e_name_size);

        iov_sizes[1].iov_base = e1.e_des_size;;
        iov_sizes[1].iov_len = sizeof(e1.e_des_size);

        iov_sizes[2].iov_base = e1.emp_s_size;;
        iov_sizes[2].iov_len = sizeof(e1.emp_s_size);
		
	iovcnt = sizeof(iov_sizes) / sizeof(struct iovec);
	sret = readv(fh, iov_sizes, iovcnt);
	
	printf("Number of bytes read is %d\n", sret);
	if(sret < 0)
	{
		printf("Erroe while reading! %s\n", strerror(errno));
		return( EXIT_FAILURE);
	}
	
	/*Cast the void* value of stuct iovec to int* and store it*/
	int *name_size = iov_sizes[0].iov_base;
	int *desig_size = iov_sizes[1].iov_base;
	int *emp_s_size = iov_sizes[2].iov_base;	

	/* Allocate memory equal to the length of the string written to file*/
	e1.e_name = (char*) malloc(*name_size * sizeof(char));
	e1.e_des = (char*) malloc(*desig_size * sizeof(char));
        
	printf("Employee name size: %d\n",*name_size);
        printf("Employee designation size: %d\n",*desig_size);
        printf("Struct size: %d\n",*emp_s_size);

	/*Bring offset back to the start of file */
	lseek(fh, 0, SEEK_SET);	

	iov[0].iov_base = e1.e_name_size;
        iov[0].iov_len = sizeof(e1.e_name_size);

        iov[1].iov_base = e1.e_des_size;
        iov[1].iov_len = sizeof(e1.e_des_size);

        iov[2].iov_base = e1.emp_s_size;
        iov[2].iov_len = sizeof(e1.emp_s_size);

	iov[3].iov_base = e1.e_name;
	iov[3].iov_len = *name_size;

	iov[4].iov_base = e1.e_des;
	iov[4].iov_len = *desig_size;
	
	iov[5].iov_base = e1.e_id;
	iov[5].iov_len = sizeof(e1.e_id);

	iovcnt = sizeof(iov) / sizeof(struct iovec);
        sret = readv(fh, iov, iovcnt);

	if(sret < 0)
        {
                printf("Error while reading! %s\n", strerror(errno));
		return(EXIT_FAILURE);	 
        }

	int *emp_id = iov[5].iov_base;
	
	printf("Employee name is %s\n", iov[3].iov_base);
	printf("Employee designation is %s\n", iov[4].iov_base);
	printf("Employee id is %d\n", *emp_id);
	
	free(e1.e_name);
	free(e1.e_des);
	close(fh);	

	return (EXIT_SUCCESS);
}
