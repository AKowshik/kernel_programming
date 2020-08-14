/* Tool to attach shared memory */

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#define ARG_OPTSHMID 0x01
#define ARG_OPTSTR   0x02

int main(int argc, char *argv[])
{

	int opt, ret;
	char *sopts, *value;
	int shm_id = -1;
	char *shared_mem_string, *shared_mem_string2;
	int flags = (0);
	char *input_string;
	int args_passed = 0;	

	enum {
		OPT_SHM_EXEC,
		OPT_SHM_RDONLY, 
		OPT_SHM_REMAP,
		OPT_SHM_RND,
		OPT_CLEAR
	};

	char *const token[] = {
		[OPT_SHM_EXEC]	= "exec",
		[OPT_SHM_RDONLY]= "rdonly", 
		[OPT_SHM_REMAP]	= "remap",
		[OPT_SHM_RND]  	= "random",
		[OPT_CLEAR]	= "clear",
		NULL
	};
	
	while((opt = getopt(argc, argv, ":i:hs:f:")) != -1)
	{
		switch(opt)
		{
			case 'i':
				shm_id = strtol(optarg, NULL, 0);
				args_passed |= ARG_OPTSHMID;
				break;
			case 'h':
				printf("Usage: %s <shm_id>\n", program_invocation_short_name);
				exit(EXIT_SUCCESS);
			case 's':
				input_string = optarg;
				args_passed |= ARG_OPTSTR;
				break;				
			case 'f':
				sopts = optarg;
				while((ret = getsubopt(&sopts, token, &value)) != -1)
				{
					switch(ret)
					{
						case OPT_SHM_EXEC:
							flags |= SHM_EXEC;
							break;
						case OPT_SHM_RDONLY:
							flags |= SHM_RDONLY;
							break;
						case OPT_SHM_REMAP:
							flags |= SHM_REMAP;
							printf("Set flag remap\n");
							break;
						case OPT_SHM_RND:
							flags |= SHM_RND;
							printf("set flag randonm\n");
							break;
						case OPT_CLEAR:
							flags = 0;
							break;
						default:
							printf("Unrecgonized flag %d\n", ret);
							break;
					}
				}
				break;			
			case '?':
			case ':':
				printf("Usage: %s <shm_id> [flags]\n", program_invocation_short_name);
                                exit(EXIT_FAILURE);
			default:
				printf("Usage: %s <shm_id> [flags]\n", program_invocation_short_name);
                                exit(EXIT_FAILURE);
		}
	}

	
	if(args_passed & ARG_OPTSHMID)
	{
		shared_mem_string = shmat(shm_id, NULL,	flags);
		if( shared_mem_string == (char *)-1 )
		{
			printf("Failed to attach memory. Error: %d, %m \n", errno);
			goto exitProgram;
		}
		if(args_passed & ARG_OPTSTR)
		{	
			if(shared_mem_string)
			{
				strcpy(shared_mem_string, input_string);
				printf("String in shared memory segment is %s\n", shared_mem_string);
			}
		}
		printf("Attached memory address is %p\n", shared_mem_string);
	}

exitProgram:	
	printf("Exiting \n");
	
	return(0);
}
