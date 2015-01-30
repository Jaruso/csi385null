// Team null
// bootSector.h


struct BootSector
{
	int bytesPerSector;
	int sectorsPerCluster;
	int numberOfReservedSectors;
	int numberOfFats;
	int maximumNumberOfRootEntries;
	int totalSectorCount;
	int sectorsPerFat;
	int sectorsPerTrack;
	int numberOfHeads;
	int sectorCount;
	int bootSignature;
	int volumeId;

	char volumeLabel[12];
	char fileSystemType[9];
};
