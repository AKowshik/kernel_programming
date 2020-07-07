/* Tool to read the disk */

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>

//#ifndef BLKPBSZGET
//#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
//#endif



size_t get_block_size(int fd)
{
	int block_size;
	int ret_value;
	
	ret_value = ioctl(fd, BLKSSZGET, &block_size);
	if( ret_value == -1 )
	{
		printf("Unable to retrieve the block size. Error: %s, %d\n", strerror(errno), errno);
		exit(EXIT_FAILURE); 
	}

	printf("Block size is %d\n", block_size);
	return block_size;
}

int main(int argc, char *argv[])
{
	if( argc != 4)
	{
		printf("Usage: %s </path/to/disk> <bytes_to_read> <o_flag>\n", program_invocation_short_name);
		return(EXIT_FAILURE);
	
	}

	int fd, flags;
	const char *pathname;
	ssize_t bytes_read;
	size_t count;
	char *buffer;
//	char buffer[24576] __attribute__ ((__aligned__ (512)));
	off_t offset;
	int offset_flags;
	char direct[] = "direct";
	char *end_ptr;
	int block_size;
	
	pathname = argv[1];
	count = strtoul(argv[2], &end_ptr, 10);
	
	fd = open(pathname, flags);
	if( fd == -1 )
	{
		printf("Unable to open file %s. Error %s, %d\n", pathname, strerror(errno), errno);
		exit(EXIT_FAILURE);
	}
	
	if ( !strcmp(direct, argv[3]) )
        {
                flags = ( O_RDONLY | O_DIRECT);
                printf("Allocating aligned memory\n");
               	block_size = get_block_size(fd);
		buffer = aligned_alloc(block_size, count);

        }
        else
        {
                flags = ( O_RDONLY );
                printf("Allocating non aligned memory\n");
                buffer = malloc(count);
        }	

	offset_flags = ( SEEK_SET );
	offset = lseek(fd, 0, offset_flags);
 	if(offset == -1)
	{
		printf("Unable to set offset for file %s. Error %s, %d\n", pathname, strerror(errno), errno);
                exit(EXIT_FAILURE);
	}	

	bytes_read = read(fd, buffer, count);
	if( bytes_read  == -1 )
        {
                printf("Unable to read file %s. Error %s, %d\n", pathname, strerror(errno), errno);
                exit(EXIT_FAILURE);
        }
	
	printf("Read %zd bytes \n", bytes_read);
	free(buffer);
	close(fd);

	return(EXIT_SUCCESS);
}
