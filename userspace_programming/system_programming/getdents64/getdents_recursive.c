/* Tool to retieve all the files in a directory recursively using getdents */

# define _GNU_SOURCE
# include <stdio.h>  //printf
# include <stdlib.h> // exit(), EXIT_*
# include <syscall.h> // SYS_*
# include <fcntl.h> // open()
# include <errno.h> // program_invocation_short_name
# include <inttypes.h> //int64_t
# include <string.h> //strcat(), strcpy()
# include <dirent.h> //DT_DIR


# define PAGE_SIZE 64
# define _LINUX_DIRENT_H

typedef uint64_t u64;
typedef int64_t s64;

struct linux_dirent64
{
	u64 d_ino;
	s64 d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[0];
};

/* library wrapper for SYS_getdents64 */
long sys_getdents(unsigned int fd, struct linux_dirent64 *dirent, unsigned int count)
{
	return syscall(SYS_getdents64, fd, dirent, count);
}

void list_files_in_directory(char *pathname, int indent)
{
	
	char dirBuffer[PAGE_SIZE];
        struct linux_dirent64 *dirent;
        long int ret;

	int flags = ( O_RDONLY | O_DIRECTORY);
        int fh = open(pathname, flags);
        if(fh == -1)
        {
		printf("Failed to open \"%s\" %m\n", pathname);
                return;
        }

        dirent  = (struct linux_dirent64 *)dirBuffer;
        while((ret = sys_getdents(fh, dirent, sizeof(dirBuffer) )) > 0)
        {
                unsigned index;
                int ch;

                for( index = 0; index < ret; index += dirent->d_reclen)
                {
                        dirent = (struct linux_dirent64 *) (dirBuffer + index);

			/* Check if current entry is a directory */
			if (dirent->d_type == DT_DIR) {
            		
				/* Size of path set to 4096 as that is the max length of a pathname in linux*/
				char path[4096];
				strcpy(path, pathname);
				
				/* Ignore and continue if current entry is "." or ".." */
				if ( strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0 ) 
				{
                			continue;
				}
	
				/* Concatenate current path with the current directory */
            			strcat(path, "/");
				strcat(path, dirent->d_name);
				
				/* Print the directory name */
				printf("%*s[%s]\n", indent, "",dirent->d_name);
				list_files_in_directory(path, indent + 2);
        		} 
			else 
			{
				 printf("%*s- %s\n", indent, "", dirent->d_name);
			}

	
		}
                dirent = (struct linux_dirent64 *)dirBuffer;
	}
        close(fh);
	return;

}

int main(int argc, char* argv[])
{
	

	if ( argc != 2 )
	{
		printf("usage %s </path/to/dirfile>\n", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}
	
	char *pathname = argv[1];
	list_files_in_directory(pathname, 0);

	return(EXIT_SUCCESS);
}
