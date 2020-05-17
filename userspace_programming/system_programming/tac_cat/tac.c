/* A program to implement the tac command using lseek */

#define _GNU_SOURCE
#include <stdio.h> //printf()
#include <sys/stat.h> // SEEK_*
#include <fcntl.h> // open()
#include <unistd.h> // read(), lseek()
#include <stdlib.h> // exit(), EXIT_*
#include <errno.h> // program_invocation_short_name
#include <string.h> //strcat


int main(int argc, char *argv[])
{
        int fd;
        int count = 0 ;
        int i;
        int position;
        char ch;

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

        position = lseek(fd, 0, SEEK_END);
        while(position > -1)
        {

            /* Read the previous character by moving the offset back 2 positions and reading the next character */
            position = lseek(fd, -2, SEEK_CUR);
            read(fd, &ch, 1 );

            /* If \n or the beginning of the file is reached, print the stored characters.
            * Else store each character */
            if( ch == '\n' || position == 0)
            {
                char *mystr;
			
                /*Increase count to include current character*/
                count++; 	
		
                if(position == 0)
                {
                    lseek(fd, -1, SEEK_CUR);
			     	count++; 
				}
					
				mystr = (char*) malloc( count * sizeof(char));
				read(fd, mystr, count);
				write(1, mystr, count);
		
         		/* Return the offset back to start of the line*/
                lseek(fd, -count , SEEK_CUR);
                count = 0;
                free(mystr);
            }
			 
			else
            {
				count++;
			}
		}
			
	close(fd);
	
    return(EXIT_SUCCESS);
	
}

