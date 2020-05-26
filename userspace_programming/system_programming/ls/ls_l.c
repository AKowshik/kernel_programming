/* Personal implementation of ls -l command */

# define _GNU_SOURCE
# include <stdio.h>  //printf
# include <stdlib.h> // exit(), EXIT_*
# include <syscall.h> // SYS_*
# include <fcntl.h> // open()
# include <errno.h> // program_invocation_short_name
# include <inttypes.h> //int64_t
# include <string.h> //strcat(), strcpy()
# include <dirent.h> //DT_DIR
# include <pwd.h> // getcwd()
# include <grp.h>
# include <sys/stat.h>
# include <time.h> // localtime()
# include <unistd.h> // readlink()
# include <limits.h> //PATH_MAX

# define BLU   "\x1B[34m"
# define RESET "\x1B[0m"

# ifndef PAGE_SIZE
# define PAGE_SIZE 64
# endif

# ifndef _LINUX_DIRENT_H
# define _LINUX_DIRENT_H
# endif

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

char* get_link_file(char *file)
{
	char buf[1024];
	ssize_t len;

	if ((len = readlink(file, buf, sizeof(buf)-1)) != -1)
    		buf[len] = '\0';

	return buf;
}

char* file_permissions(char *file)
{
	struct stat st;
	char *modeval = malloc(sizeof(char) * 9 + 1);
	if(stat(file, &st) == 0)
	{
		mode_t perm = st.st_mode;
		modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
		modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
		modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
		modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
		modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
		modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
		modeval[6] = (perm & S_IROTH) ? 'r' : '-';
		modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
		modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
		modeval[9] = '\0';
		return modeval;     
	}
	else
	{
		printf("Checking file permissions\n");
		return strerror(errno);
	}   
}

char* format_time(struct timespec st_mtim)
{

	char *formatted_time;
	time_t rawtime= (time_t)st_mtim.tv_sec;
	struct tm *info;
   	formatted_time = malloc(80 * sizeof(char));

   	time( &rawtime );
	info = localtime( &rawtime );

	strftime(formatted_time,80,"%b %d %H:%M", info);
	return formatted_time;

}


int get_file_info(char *file)
{
	struct stat st;
	if( stat(file, &st) == 0 )
	{
		off_t size;
		struct passwd *pw;
		struct group *gr;
		char *owner;
		char *owner_group;
		int link_count;
		char *m_time;
		char *permissions;

		permissions = file_permissions(file);
		link_count = st.st_nlink;
		size = st.st_size;
		m_time = format_time(st.st_mtim);
		pw = getpwuid(st.st_uid);
        	gr = getgrgid(st.st_gid);
			
		owner = pw->pw_name;
		owner_group = gr->gr_name;
	
		printf("%s %d %s %s %d %s ", permissions, link_count, owner, owner_group, size, m_time);
	}
	else 
	{
		printf("Error occured: %s\n", strerror(errno));
		return errno;
	}
	
	return 0;

}

char file_type(struct linux_dirent64 *dirent)
{

	char ft;
	switch(dirent->d_type)
	{
		case DT_BLK:
			ft = 'b';
			break;
		case DT_CHR:
			ft = 'c';
			break;
		case DT_DIR:
			ft = 'd';
			break;
		case DT_FIFO:
			ft = 'f';
			break;
		case DT_LNK:
			ft = 'l';
			break;
		case DT_REG:
			ft = '-';
			break;
		case DT_SOCK:
			ft = 's';
			break;
		case DT_UNKNOWN:
			ft = 'u';
			break;
		default:
			printf("Cannot identify file type\n");
			return -1;
	}
	return(ft);

}

void list_files_in_directory(char *pathname)
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
		char ft;

                for( index = 0; index < ret; index += dirent->d_reclen)
                {
			char dirent_path[1024] = {};

			dirent = (struct linux_dirent64 *) (dirBuffer + index);
			if ( strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0 )
                       	{
                        	continue;
                        }

			ft = file_type(dirent);
			printf("%c", ft);
			
			get_file_info(dirent->d_name);

			if( ft == 'd')
			{
				printf( BLU "%s\n" RESET, dirent->d_name );
			}
			else if (ft == 'l')
			{
				char *link;
				link = get_link_file(dirent->d_name);
				printf( BLU "%s " RESET "-> %s\n" , dirent->d_name, link);

			}
			else 
			{
				printf("%s\n", dirent->d_name );
			}
		}
                dirent = (struct linux_dirent64 *)dirBuffer;
	}
        close(fh);
	return;

}

int main(int argc, char* argv[])
{
	
	char *pathname;
	if ( argc == 1 )
	{
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			pathname = cwd;
			strcat(pathname, "/");
		}
		else
		{
			printf("Error in getting path name\n");
			return(EXIT_FAILURE);
		}
	}
	else 
	{
		pathname = argv[1];
	}

	list_files_in_directory(pathname);

	return(EXIT_SUCCESS);
}
