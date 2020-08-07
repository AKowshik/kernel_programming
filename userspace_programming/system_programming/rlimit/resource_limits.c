#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void display_limits(int resource)
{
	struct rlimit current_limit;
	if(getrlimit(resource, &current_limit) == 0)
	{
		printf("Current limits are: \nSoft Limit: %ld\nHard Limit: %ld\n",current_limit.rlim_cur, current_limit.rlim_max);
	}
	else 
	{
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
}

int set_limits(int resource, unsigned long soft_limit, unsigned long hard_limit)
{

	struct rlimit new_limit;
	new_limit.rlim_cur = soft_limit;
	new_limit.rlim_max = hard_limit;
	
	if(setrlimit(resource, &new_limit) == -1)
	{
		fprintf(stderr, "Error: %s\n", strerror(errno));
	}
	else
        	fprintf(stderr, "%s\n", strerror(errno));
	
	display_limits(resource);

	return 0;

}

unsigned long get_hard_limit(int resource)
{

	struct rlimit limit;
	unsigned long hard_limit;
	if(getrlimit(resource, &limit) == 0)
	{
		hard_limit = limit.rlim_max;
		return hard_limit;
	}	
	return 0;
}

int open_fds(unsigned int fd_num)
{
	int i = 0;
	while(i < fd_num)
	{
		int fd = open("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        	if (fd < 0)
        	{
        		return -1;
        	}
      		i++;
  	}
	return 0;
}

int main()
{
	int resource;
	printf("Choose resource on which operation are to be performed \n");
	printf("1.RLIMIT_AS\n2.RLIMIT_CORE\n3.RLIMIT_CPU\n4.RLIMIT_DATA\n5.RLIMIT_FSIZE\n6.RLIMIT_LOCKS\n");
	printf("7.RLIMIT_MEMLOCK\n8.RLIMIT_MSGQUEUE\n9.RLIMIT_NICE\n10.RLIMIT_NOFILE\n11.RLIMIT_NPROC\n12.RLIMIT_RSS\n");
	printf("13.RLIMIT_RTPRIO\n14.RLIMIT_RTTIME\n15.RLIMIT_SIGPENDING\n16.RLIMIT_STACK\n");
	scanf("%d", &resource);
	switch (resource)
	{
		case 1:
			resource = RLIMIT_AS;
			break;
		case 2:
			resource = RLIMIT_CORE;
			break;
		case 3:
			resource = RLIMIT_CPU;
			break;
		case 4:
			resource = RLIMIT_DATA;
			break;
		case 5:
			resource = RLIMIT_FSIZE;
			break;
		case 6:
			resource = RLIMIT_LOCKS;
			break;
		case 7:
			resource = RLIMIT_MEMLOCK;
			break;
		case 8:
			resource = RLIMIT_MSGQUEUE;
			break;
		case 9:
			resource = RLIMIT_NICE;
			break;
		case 10:
			resource = RLIMIT_NOFILE;
			break;
		case 11:
			resource = RLIMIT_NPROC;
			break;
		case 12:
			resource = RLIMIT_RSS;
			break;
		case 13:
			resource = RLIMIT_RTPRIO;
			break;
		case 14:
			resource = RLIMIT_RTTIME;
			break;
		case 15:
			resource = RLIMIT_SIGPENDING;
			break;
		case 16:
			resource = RLIMIT_STACK;
			break;
		default:
			printf("Invalid Input!");
	}

        display_limits(resource);
	char choice;
	unsigned long new_soft_limit, new_hard_limit;
	printf("Change limits?(y/n)\n");
	scanf(" %c", &choice);
	if(choice == 'y')
	{
		printf("Enter new soft limit\n");
		scanf("%lu", &new_soft_limit);
		printf("Enter new hard limit\n");
		scanf("%lu", &new_hard_limit);
		if(new_hard_limit > get_hard_limit(resource))
		{
			printf("Hard limit cannot be greater than exisitng hard limit. Using current limit\n");
			new_hard_limit = get_hard_limit(resource);
		}
		set_limits(resource, new_soft_limit, new_hard_limit);
	}
	else {
		return 0;
	}
	int *ptr;
	ptr = (int*)malloc(6000 * sizeof(int));
	printf("Sleeping for 60 secs\n");
	sleep(60);
	return 0;
}

