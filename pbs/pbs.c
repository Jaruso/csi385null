//Team null
//pbs.c

//PREPROCESSOR DIRECTIVES

//Include C-libraries first
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//Include local .c/.h files next
#include "bootSector.h"

/******************************************************************************
 * You must set these global variables:
 *    FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                      filesystem)
 *    BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 *
 * You may use these support functions (defined in FatSupport.c)
 *    read_sector
 *    write_sector
 *    get_fat_entry
 *    set_fat_entry
 *****************************************************************************/

//We will define these variables for use in here (pbs.c) for now...
//extern FILE *FILE_SYSTEM_ID;
//extern int  BYTES_PER_SECTOR;

//VARIABLES
int    BYTES_PER_SECTOR = 512; //512 bytes per FAT12 sector
FILE   *FILE_SYSTEM_ID; //File pointer, define which floppy image to use for file IO

BootSector bootSector;

//PROTOTYPES
extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);

extern int  get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

void readBootSector();
void printBootSector();

main()
{
    unsigned char* boot; //Buffer, basically

    FILE_SYSTEM_ID = fopen("floppy1", "r+"); //Hard-coded to flopp1 for now for testing... in r+ mode

    int mostSignificantBits;
    int leastSignificantBits;
    int bytesPerSector;

    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
         exit(1);
    }

    // Then reset it per the value in the boot sector
    boot = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

    if (read_sector(0, boot) == -1) {
         printf("Something has gone wrong -- could not read the boot sector\n");
    }

    // 12 (not 11) because little endian
    //int bytesPerSector -- [ - ]
    mostSignificantBits  = (((int) boot[12]) << 8) & 0x0000ff00;
    leastSignificantBits =   ((int) boot[11]) & 0x000000ff;
    bootSector.bytesPerSector = mostSignificantBits | leastSignificantBits;

    BYTES_PER_SECTOR = bootSector.bytesPerSector;

    readBootSector(boot);
    printBootSector();
}

void readBootSector()
{
    unsigned char* boot;

    int sectorNumber = 0, i = 0, bytesRead = 0, mostSignificantBit = 0, leastSignificantBit = 0;

    boot = (unsigned char*) malloc (BYTES_PER_SECTOR * sizeof(unsigned char));

    bytesRead = read_sector(sectorNumber, boot);

    //int sectorsPerCluster -- [ - ]
    bootSector.sectorsPerCluster = ((int) boot[13]);

    //int numberOfReservedSectors -- [14 - 15]
    mostSignificantBit  = (((int) boot[15]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[14]) & 0x000000ff;
    bootSector.numberOfReservedSectors = mostSignificantBit | leastSignificantBit;

    //int numberOfFats -- [ - ]
    bootSector.numberOfFats = ((int) boot[16]);

    //int maximumNumberOfRootEntries -- [17 - 18]
    mostSignificantBit  = (((int) boot[18]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[17]) & 0x000000ff;
    bootSector.maximumNumberOfRootEntries = mostSignificantBit | leastSignificantBit;

    //int totalSectorCount -- [19 - 20]
    mostSignificantBit  = (((int) boot[20]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[19]) & 0x000000ff;
    bootSector.totalSectorCount = mostSignificantBit | leastSignificantBit;

    //int sectorsPerFat -- [22 - 23]
    mostSignificantBit  = (((int) boot[23]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[22]) & 0x000000ff;
    bootSector.sectorsPerFat = mostSignificantBit | leastSignificantBit;

    //int sectorsPerTrack -- [24 - 25]
    mostSignificantBit  = (((int) boot[25]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[24]) & 0x000000ff;
    bootSector.sectorsPerTrack = mostSignificantBit | leastSignificantBit;

    //int numberOfHeads -- [26 - 27]
    mostSignificantBit  = (((int) boot[27]) << 8) & 0x0000ff00;
    leastSignificantBit = ((int) boot[26]) & 0x000000ff;
    bootSector.numberOfHeads = mostSignificantBit | leastSignificantBit;

    //int sectorCount -- [32 - 35]
    mostSignificantBit  = (((int) boot[35]) << 24) & 0xff000000;
    leastSignificantBit = (((int) boot[34]) << 16) & 0x00ff0000;
    mostSignificantBit = mostSignificantBit | leastSignificantBit;
    leastSignificantBit = (((int) boot[33]) <<  8) & 0x0000ff00;
    mostSignificantBit = mostSignificantBit | leastSignificantBit;
    leastSignificantBit = ((int) boot[32]) & 0x000000ff;
    bootSector.sectorCount = mostSignificantBit | leastSignificantBit;

    //int bootSignature -- [ - ] 
    bootSector.bootSignature = ((int) boot[38]);

    //int volumeId -- [39 - 42]
    mostSignificantBit  = (((int) boot[42]) << 24) & 0xff000000;
    leastSignificantBit = (((int) boot[41]) << 16) & 0x00ff0000;
    mostSignificantBit = mostSignificantBit | leastSignificantBit;
    leastSignificantBit = (((int) boot[40]) <<  8) & 0x0000ff00;
    mostSignificantBit = mostSignificantBit | leastSignificantBit;
    leastSignificantBit = ((int) boot[39]) & 0x000000ff;
    bootSector.volumeId = mostSignificantBit | leastSignificantBit;

    //char volumeLabel[12] -- [43 - 53]
    //use "i"
    for (i = 0; i < 11; i ++) { // 0 < 11 (10)
        bootSector.volumeLabel[i] = boot[43 + i];
    }
    bootSector.volumeLabel[11] = '\0'; //Don't forget the null terminator

    //char fileSystemType[9] -- [54 - 61]
    //use "i"
    for (i = 0; i < 8; i ++) { // 0 < 8 (7)
        bootSector.fileSystemType[i] = boot[54 + i];
    }
    bootSector.fileSystemType[8] = '\0'; //Don't forget the null terminator
}

void printBootSector() //pbs()
{ 
	printf("Bytes per sector:            %i\n", bootSector.bytesPerSector);
  	printf("Sectors per cluster:         %i\n", bootSector.sectorsPerCluster);
  	printf("Number of FATs:              %i\n", bootSector.numberOfFats);
  	printf("Number of reserved sectors:  %i\n", bootSector.numberOfReservedSectors);
  	printf("Number of root entries:      %i\n", bootSector.maximumNumberOfRootEntries);
  	printf("Total sector count:          %i\n", bootSector.totalSectorCount);
  	printf("Sectors per FAT:             %i\n", bootSector.sectorsPerFat);
  	printf("Sectors per track:           %i\n", bootSector.sectorsPerTrack);
  	printf("Number of heads:             %i\n", bootSector.numberOfHeads);
  	printf("Boot signature:              %p\n", bootSector.bootSignature); //h; use p
  	printf("Volume ID:                   %p\n", bootSector.volumeId); //h; use p
  	printf("Volume label:                %s\n", bootSector.volumeLabel); //char array
  	printf("File system type:            %s\n", bootSector.fileSystemType); //char array
}

