//Team null
//pfe.c

//PREPROCESSOR DIRECTIVES

//Include C-libraries first
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

//Include local .c/.h files next
#include "fatSupport.h"

/******************************************************************************
 * You must set these global variables:
 *    FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                      filesystem)
 *    BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 *
 * Define FLOPPY bestartIndex as desired:
 *    An appropriate file (for testing), or the real floppy drive
 *
 * You may use these support functions (defined in FatSupport.c)
 *    read_sector    write_sector    get_fat_entry    set_fat_entry
 *****************************************************************************/

//We will define these variables for use in here (pfe.c) for now...
//extern FILE *FILE_SYSTEM_ID;
//extern int  BYTES_PER_SECTOR;

int    BYTES_PER_SECTOR = 512; //512 bytes per FAT12 sector
FILE   *FILE_SYSTEM_ID; //File pointer, define which floppy image to use for file IO

int main(int argc, char** argv)
{
    int startIndex, finishIndex, i;      
    unsigned char *buffer;  
 
    FILE_SYSTEM_ID = fopen("floppy1", "r+"); //Hard-coded to flopp1 for now for testing... in r+ mode

    if (((argc == 3) || (argc == 4)) &&
        ((startIndex = atoi(argv[1])) <= (finishIndex = atoi(argv[2]))) &&
        (atoi(argv[2]) < BYTES_PER_SECTOR * 9 * 2 / 3)) {

        buffer = (char*) malloc(9 * BYTES_PER_SECTOR * sizeof(char));

        for (i = 1; i <= 9; i++) { //Readl all
            read_sector(i, buffer + BYTES_PER_SECTOR * (i - 1));
        }

        for(i = startIndex; i <= finishIndex; i++) { //Display all within specified range
            printf("Entry %d:\t%X\n", i, get_fat_entry(i, buffer));
        }
    }
    else { //Invalid arguments provided, print usage message
        printf("Usage: %s startIndex finishIndex\n", argv[0]);
    }

    exit(EXIT_SUCCESS);
}

