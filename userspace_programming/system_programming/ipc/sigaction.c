#define _GNU_SOURCE
 
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
 
/*
struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/
 
void my_sighandler(int signal, siginfo_t *siginfo, void *unused)
{
    printf("Got signal: %d\n", signal);
    printf("signal number %d\n", siginfo->si_signo);
    printf("Error number %d\n", siginfo->si_errno);
    printf("Signal code: %d\n", siginfo->si_code);
    printf("Sending proc id %d\n", siginfo->si_pid);
    printf("User ID of sending process %d\n", siginfo->si_uid);
    printf("Exit value of signal %d\n", siginfo->si_status);
    printf("User time consumed %d\n", siginfo->si_utime);
    printf("System time consumed %d\n", siginfo->si_stime);
    printf("Siignal value %d\n", siginfo->si_value);
    printf("POSIX Signal %d\n", siginfo->si_int);
    printf("Timer overrun count %d\n", siginfo->si_overrun);
    printf("Timer ID, POSIX Timers %d\n", siginfo->si_timerid);
    printf("Memory location which caused fault %p\n", siginfo->si_addr);
    printf("Band event %ld\n", siginfo->si_band);
    printf("File descriptor %d\n", siginfo->si_fd);
    printf("Least significant bit of address %d\n", siginfo->si_addr_lsb);
}
 
int main(int argc, char *argv[])
{
    int status, ret;
    struct sigaction act;
    struct sigaction oldact;
	time_t endTime, startTime;
 
    memset(&act, '\0', sizeof(act));
    memset(&oldact, '\0', sizeof(oldact));
    act.sa_sigaction = &my_sighandler;
    act.sa_flags = SA_SIGINFO;
    
    status = sigaction(SIGCHLD, &act, &oldact);
    if(status == -1)
    {
        printf("Error performing sigaction. Error: %d, %m\n", errno);
        return(EXIT_FAILURE);
    }
    
    pid_t childPid = fork();
    if(childPid == 0)
    {
        printf("Inside child now");
        sleep(2);
        exit(EXIT_SUCCESS);
    }
 
    startTime = time(NULL);
    ret = sleep(5);
    endTime = time(NULL);
    printf("Return value of sleep: %d\n", ret);
    printf("Time lapsed: %u\n", (endTime - startTime));
 
    return 0;
}