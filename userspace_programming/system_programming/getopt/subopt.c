/* Program to experiment subopt */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char  *argv[])
{
	char *volName = NULL, *mntPoint = NULL, *poolName = NULL;
	int opt;
	char *sopts, ret, *value;
	int ro = 0, rw = 0;
	enum {
		OPT_VOLNAME,
		OPT_MNTPNT, 
		OPT_POOLNAME,
		OPT_RO,
		OPT_RW
	};

	char *const token[] = {
		[OPT_VOLNAME]  = "volName",
		[OPT_MNTPNT]   = "mntPoint", 
		[OPT_POOLNAME] = "poolName",
		[OPT_RO]       = "readOnly",
		[OPT_RW]       = "readWrite",
		NULL
	};

	while((opt = getopt(argc, argv, "o:h")) != -1)
	{
		switch(opt)
		{
			case 'o':
				sopts = optarg;
				while((ret = getsubopt(&sopts, token, &value)) != -1)
				{
					switch(ret)
					{
						case OPT_VOLNAME:
							if(value == NULL)
							{
								printf("Missing value for volume name\n");
								exit(EXIT_FAILURE);
							}
							volName = value;
							break;

						case OPT_MNTPNT:
							if(value == NULL)
							{
								printf("Missing value for mount path\n");
								exit(EXIT_FAILURE);
							}
							mntPoint = value;
							break;

						case OPT_POOLNAME:
							if(value == NULL)
							{
								printf("Missing value for pool name\n");
								exit(EXIT_FAILURE);
							}
							poolName = value;
							break;

						case OPT_RO:
							ro = 1;
							break;

						case OPT_RW:
							rw = 1;
							break;	
						default:
							printf("Unrecognized option %d \n", ret);
							break;
					}
				}
				break;
			case '?':
				printf("Usage %s [ -o options ] \n", program_invocation_short_name);
				exit(EXIT_FAILURE);
			case 'h':
				printf("Help\n");
				exit(0);
		}
	}

			
	printf("poolName=%s mntPoint=%s volName=%s flags=%s,%s\n", poolName, mntPoint, volName, (ro)? "ro": "", (rw)? "rw" : "");

	return(0);

}
