#define _GNU_SOURCE

#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{

	int sem_id, ret;
	int val = 0;
	
	if(argc !=2 )
	{
		printf("Usage: %s <sem_id>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}
	
	sem_id = atoi(argv[1]);
	
	ret = semctl(sem_id, 0, SETVAL, &val);
	if(ret == -1)
	{
		printf("Error on systemctl %d(%s)\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	return 0;

}