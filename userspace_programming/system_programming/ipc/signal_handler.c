#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

union semun {
	int 				val;
	struct  semid_ds 	*buf;
	unsigned short 		*array;
	struct seminfo 		*__buf;
};

void my_signal_handler(int signal)
{
	setsid();
	printf("Got signal %d\n", signal);
}

int main(int argc, char *argv[])
{

	int opt;
	int signal_id, sem_id, ret;
	pid_t proc_pid, child_pid;
	union semun sem1;

	while((opt = getopt(argc, argv, ":s:i:")) != -1)
	{
		switch(opt)
		{
			case 's':
				signal_id = strtol(optarg, NULL, 0);
				break;
			case 'i': 
				sem_id = strtol(optarg, NULL, 0);
				break;
			case ':':
			case '?':
				printf("Usage: %s <signal_id>\n", program_invocation_short_name);
				exit(EXIT_FAILURE);
			default:
				printf("Usage: %s <signal_id>\n", program_invocation_short_name);
				exit(EXIT_FAILURE);
		}
	}

	proc_pid = getpid();
	printf("Process pid is %d\n", proc_pid);
	signal(signal_id, my_signal_handler);

	sem1.val = 0;
	ret = semctl(sem_id, 0, SETVAL, sem1.val);
	if(ret == -1)
	{
		printf("Failed to set semaphore value. Error %d, %m\n", errno);
		exit(EXIT_FAILURE);
	}

	child_pid = fork();
	if(child_pid == (pid_t)-1)
	{
		printf("Unable to fork process. Error %d, %m\n", errno);
		exit(EXIT_FAILURE);
	}
	if(child_pid > (pid_t)0)
	{
		printf("Child process id is %d\n", child_pid);
	}
	if(child_pid == (pid_t)0)
	{
		if(semctl(sem_id, 0, GETVAL, sem1) == 1)
		{
			printf("Exitting child\n");
			exit(EXIT_SUCCESS);
		}
	}
	printf("Before pause\n");
	sem1.val = 1;
	ret = semctl(sem_id, 0, SETVAL, sem1.val);
	if(ret == -1)
	{
		printf("Failed to set semaphore value. Error %d, %m\n", errno);
		exit(EXIT_FAILURE);
	}
	pause();

	return 0;

}
