/* Program to use system calls through the SYSCALL functionality */

# define _GNU_SOURCE

# include <stdio.h>
# include <fcntl.h> // open()
# include <errno.h> // program_invocation_short_name
# include <unistd.h> 
# include <stdlib.h> // EXIT_*
# include <sys/syscall.h> //syscall

/* library wrapper for SYS_open */
int sys_open(char *pathname, int flags)
{
        return syscall(SYS_open, pathname, flags);
}

/* library wrappr for SYS_read */
size_t sys_read(int fd, void* buf, size_t count)
{
	return syscall(SYS_read, fd, buf, count);
}

/*library wrapper for SYS_close */
int sys_close(int fd)
{
	return syscall(SYS_close, fd);
}

int main( int argc, char *argv[])
{
	if ( argc != 2 )
        {
                printf("usage %s </path/to/file>\n", program_invocation_short_name);
                return(EXIT_FAILURE);
        }

        char *pathname = argv[1];

	int flags = ( O_RDONLY );
	int fh;
	int nr_bytes;
	char content[10] = {};


	fh = sys_open(pathname, flags);
	if( fh > -1)
	{
		nr_bytes = read(fh, content, 9);
		content[nr_bytes] = '\0';
		if( nr_bytes > -1)
		{
			printf("Read %s \n", content);
	
		}
		else 
		{
			printf("Error reading file: %s \n", strerror(errno));
			return(EXIT_FAILURE);
		}
	}
	else 
	{
		printf("Error opening file: %s \n", strerror(errno));
		return(EXIT_FAILURE);
	}
	
	return(EXIT_SUCCESS);
}
