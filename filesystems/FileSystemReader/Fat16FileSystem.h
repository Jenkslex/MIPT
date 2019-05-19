#pragma once

#include <list>
#include <string>
#include <linux/types.h>
#include <cstring>

#include "FileSystem.h"

struct fatBootSector {
    __u8	ignored[3];     /* Boot strap short or near jump */
    __u8	systemId[8];    /* Name - can be used to special case
				               partition manager volumes */
    __u8	sectorSize[2];  /* bytes per logical sector */
    __u8	secPerClus;     /* sectors/cluster */
    __le16	reserved;       /* reserved sectors */
    __u8	fats;           /* number of FATs */
    __u8	dirEntries[2];  /* root directory entries */
    __u8	sectors[2];     /* number of sectors */
    __u8	media;          /* media code */
    __le16	fatLength;      /* sectors/FAT */
    __le16	secsTrack;      /* sectors per track */
    __le16	heads;          /* number of heads */
    __le32	hidden;         /* hidden sectors (unused) */
    __le32	totalSect;      /* number of sectors (if sectors == 0) */

    /*  Extended BPB Fields for FAT16 */
    __u8	driveNumber;    /* Physical drive number */
    __u8	state;          /* undocumented, but used
		                                for mount state. */
    __u8	signature;      /* extended boot signature */
    __u8	volOd[4];       /* volume ID */
    __u8	volLabel[11];   /* volume label */
    __u8	fsType[8];      /* file system type */
    /* other fields are not added here */

};

struct fatDirEntry {
    __u8	name[11];	    /* name and extension */
    __u8	attr;           /* attribute bits */
    __u8    lcase;          /* Case for base and extension */
    __u8	ctime_cs;       /* Creation time, centiseconds (0-199) */
    __le16	ctime;          /* Creation time */
    __le16	cdate;          /* Creation date */
    __le16	adate;          /* Last access date */
    __le16	starthi;        /* High 16 bits of cluster in FAT32 */
    __le16	time,date,start;/* time, date and first cluster */
    __le32	size;           /* file size (in bytes) */
};

class Fat16DirEntry {
public:
    explicit Fat16DirEntry(fatDirEntry& entry);
private:

    bool isDirectory;
    std::string Name;
    int Size;

    int StartCluster;

    std::vector<long> Sectors;

    friend class Fat16FileSystem;
    friend std::ostream& operator<<(std::ostream& os, const Fat16DirEntry& dirEntry);
};

class Fat16FileSystem : public FileSystem {
public:
    Fat16FileSystem() = default;

    virtual void openImage(std::string& fName);

    virtual void listDir();
    virtual void catFile(std::string& fName);
    virtual void printWorkingDirectory();

private:
    int ClusterSize;
    int SectorSize;
    int RootEntriesCapacity;
    int FatSize;
    int FatStart;
    int RootStart;
    int DataStart;
    int RootCluster;
    int FatEntrySize;

    std::vector<Fat16DirEntry> RootEntries;
    std::list<Fat16DirEntry> CurrPath;
};
