/*Tool to send messages via IPC*/

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>


int main(int argc, char *argv[])
{
	
	key_t key;
	int mid, msg_flags;
	size_t len;
	char *text = "Default", *msg;
	long mtype;
	
	if( argc < 2 || argc > 4 ) 
	{
		printf("Usage: %s <msgid> [mtype] [msg] \n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}
	
	mid = atoi(argv[1]);
	mtype = (argc >= 3) ? atoi(argv[2]) : 0L;
	
	if(argc >= 4 )
	{
		text = argv[3];
	}
	
	len = strlen(mtext) + sizeof(long);
	msg = malloc(len);
	
	sprintf(msg, "%ld%s", mtype, mtext);
	msg_flags = 0;
	
	if((msgsnd(mid, msg, len, msg_flags)) == -1) 
	{
		printf("msgsnd failed %m\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Type = %ld\nData = %s\n", msg->mtype, msg-mtext);
	
	exit(EXIT_SUCCESS);
}