/* Tool to understand signal masking */

#define _GNU_SOURCE

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void my_signal_handler(int signal_no)
{
	printf("Got signal %d\n", signal_no);
}

int main( int argc __attribute__((unused)), char *argv[] __attribute__((unused)) )
{

	sigset_t set, oldset;
	int signals_blocked __attribute__((unused));
	int how;

	printf( "Uid is %u\n", getuid() );
	printf( "Pid is %u\n", getpid() );

	sigemptyset(&set);
	sigaddset( &set, SIGINT );
	sigaddset( &set, SIGTERM );
	
	signal( SIGINT, my_signal_handler );
	signal( SIGTERM, my_signal_handler );

	how = ( SIG_BLOCK );
	if(sigprocmask(how, &set, &oldset) == -1)
	{
		printf("Could not set mask. Error %d, %m\n", errno);
		exit(EXIT_FAILURE);
	}

	getchar();

	how = ( SIG_UNBLOCK );
	if( sigprocmask(how, &set, &oldset) == -1 )
	{
		printf("Could not unblock mask. Error %d, %m\n", errno);
		exit(EXIT_FAILURE);
	}

	getchar();
	printf("Exting application\n");

	return 0;

}
