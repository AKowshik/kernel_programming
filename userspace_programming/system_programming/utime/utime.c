/*Tool to write to a file without changing its access and modify time */

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <utime.h>
#include <string.h>

int main(int argc, char *argv[])
{

	if( argc != 3)
	{	
		printf("Usage %s </path/to/file> <text>\n", program_invocation_short_name);
		return(EXIT_FAILURE);
	}
	
	const char *pathname;
	const char *text;
	struct stat st;
	time_t mod_time;
	time_t acc_time;
	int fd;
	int open_flags;
	size_t size;
	ssize_t bytes_written;
	struct utimbuf file_time;
	int ret_value;
	const struct utimbuf *times;

	pathname = argv[1];
	text = argv[2];

	open_flags = ( O_RDWR | O_APPEND );
        fd = open(pathname, open_flags);
        if( fd == -1)
        {
                printf("Unable to open file %s. Error %s, %d\n", pathname, strerror(errno), errno);
                return(EXIT_FAILURE);
        }


	if( stat(pathname, &st) == 0 )
	{
		mod_time = st.st_mtim.tv_sec;
		acc_time = st.st_atim.tv_sec;

		file_time.actime = acc_time;
		file_time.modtime = mod_time;

		printf("acc time %ld, acc time %ld\n", file_time.actime, file_time.modtime); 

	}
	else 
	{
		printf("Could not get time from file\n");
		return(EXIT_FAILURE);
	}	

	bytes_written = write(fd, text, strlen(text) + 1);
	if(bytes_written == -1 )
	{
		printf("Unable to write to file %s. Error %s, %d\n", pathname, strerror(errno), errno);
        return(EXIT_FAILURE);
	}
	
	times = &file_time;
	ret_value = utime(pathname, times);
	if( ret_value == -1 )
	{
		printf("Unable to set time to file %s. Error %s, %d\n", pathname, strerror(errno), errno);
        return(EXIT_FAILURE);
	}
	
	close(fd);

	return(EXIT_SUCCESS);
}
