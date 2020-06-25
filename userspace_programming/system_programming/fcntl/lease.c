/*Tool to set leases on a file */

# define GNU_SOURCE

# include<stdio.h>
# include<unistd.h>
# include<fcntl.h>
# include<errno.h>
# include<signal.h>
# include<sys/stat.h>
# include<sys/types.h>
# include<stdlib.h>

void sigHandler(int sig)
{
	printf("In  %s. Caught signal %d\n", __func__, sig);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int fd, ret;
	const char *pathname;
	
	if(argc !=2)
	{
		printf("Usage: %s </path/to/file>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}
	
	pathname = argv[1];
	
	fd = open(pathname, O_WRONLY);
	if(fd == -1)
	{
		printf("Error %d(%s) opening \'%s\' \n", errno, strerror(errno), pathname);
		exit(EXIT_FAILURE);
	}
 
	signal(SIGIO, sigHandler);
	ret = fcntl(fd, F_SETLEASE, F_WRLCK);
	if(ret == -1)
	{
		printf("Error %d(%s) in fcntl \n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	getchar();
	close(fd);
	
	exit(EXIT_SUCCESS);
}


	