#define _GNU_SOURCE

 

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

 

int main(int argc, char *argv[])
{
        int shm_option, shmid_flag = 0, ret_shmctl;
        long shmid;
        struct shmid_ds shm_buff;

        if((shm_option = getopt(argc, argv, ":hi:")) != -1)
        {
                switch(shm_option)
                {
                        case 'h':
                                printf("USAGE : %s < -i shmid >\n", program_invocation_short_name);
                                exit(EXIT_SUCCESS);
                        case 'i':
                                shmid = strtol(optarg, NULL, 0);
                                shmid_flag = 1;
                                break;
                        case ':':
                                printf("Missing shmid argument in -%c option\n", optopt);
                                exit(EXIT_FAILURE);
                        case '?':
                                printf("Option %c Not Supported\n", optopt);
                                exit(EXIT_FAILURE);
                        default:
                                printf("USAGE : %s < -i shmid >\n", program_invocation_short_name);
                                exit(EXIT_SUCCESS);

 

                }

 

        }
        if(!shmid_flag)
        {
                printf("USAGE : %s < -i shmid >\n", program_invocation_short_name);
                exit(EXIT_SUCCESS);
        }
        ret_shmctl = shmctl(shmid, IPC_STAT, &shm_buff);
        if (ret_shmctl == -1)
        {
                printf("Error in Shmctl %i:%m\n", errno);
                exit(EXIT_FAILURE);
        }
        printf("\n SHARED Memory Information with Shmid %ld \n", shmid);
        printf(" 1. Shared Memory Key %#x\n", shm_buff.shm_perm.__key);
        printf(" 2. Segment Size  %lu Bytes\n", shm_buff.shm_segsz);
        printf(" 3. PID of Creator %d\n", shm_buff.shm_cpid);
        printf(" 4. PID of last shmat(2)/shmdt(2) %d\n", shm_buff.shm_lpid);
        printf(" 5. No. of current attaches %lu\n", shm_buff.shm_nattch);
        printf(" 6. Effective UID of owner %ld\n", shm_buff.shm_perm.uid);
        printf(" 7. Effective GID of owner %ld\n", shm_buff.shm_perm.gid);
        printf(" 8. Effective UID of creator %ld\n", shm_buff.shm_perm.cuid);
        printf(" 9. Effective GID of creator %ld\n", shm_buff.shm_perm.cgid);
        printf(" 10. Permissions + SHM_DEST and SHM_LOCKED flags %u\n", shm_buff.shm_perm.mode);
        printf(" 11. Sequence number %u\n", shm_buff.shm_perm.__seq);
        printf(" 12. Last attach time %ld\n", shm_buff.shm_atime);
        printf(" 13. Last detach time %ld\n", shm_buff.shm_dtime);
        printf(" 14. Last change time %ld\n", shm_buff.shm_ctime);
        return 0;
}
