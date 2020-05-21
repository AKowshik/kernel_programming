/* A program to implement the tac command using lseek */

#define _GNU_SOURCE
#include <stdio.h> //printf()
#include <sys/stat.h> // SEEK_*
#include <fcntl.h> // open()
#include <unistd.h> // read(), lseek()
#include <stdlib.h> //  EXIT_*
#include <errno.h> // program_invocation_short_name


int main(int argc, char *argv[])
{
        int fd;
        int charCount;
	char *mystr;

        if( argc != 2)
        {
                printf("usage %s </path/to/file> <number> \n", program_invocation_short_name);
                exit( EXIT_FAILURE);
        }

        char *filename = argv[1];

        fd = open(filename, O_RDONLY);
        if( fd == -1)
        {
                return(EXIT_FAILURE);
        }

	/* Get the offset of the last character in the file using lseek as EOF might not be there */
        charCount = lseek(fd, 0, SEEK_END);

	/* Reset the offset back to the beginning of the file */ 
	lseek(fd, 0, SEEK_SET);

	mystr = (char*) malloc( charCount * sizeof(char));
	read(fd, mystr, charCount);
	write(1, mystr, charCount);
	free(mystr);
	close(fd);

        return(EXIT_SUCCESS);

}

