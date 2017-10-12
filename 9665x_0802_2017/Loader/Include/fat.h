/**
    File system library

    This file is the library of file system

    @file       fat.h
    @ingroup    mISYSFile
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _FAT_H
#define _FAT_H

#include "constant.h"
#include "StorageDef.h"

/**
    @addtogroup mISYSFile
*/
//@{

#define FAT_READ_TOTAL_FILE_LENGTH          0
#define PARTITION_AREA_SECTOR               0
/* the offset of Master Boot Record and Partition Table */
#define MASTER_BOOT_RECORD_OFFSET           0
#define PARTITION_TABLE0_OFFSET             446
#define PARTITION_TABLE1_OFFSET             462
#define PARTITION_TABLE2_OFFSET             478
#define PARTITION_TABLE3_OFFSET             494
#define PARTITION_AREA_SIGNATURE_OFFSET     510

/* FAT constants */
#define FAT_CLUSTER_NOT_USED                0x000
#define FAT_CLUSTER_ALLOCATED               0x002
#define FAT_CLUSTER_RESERVED                0xFFFFFFF6
#define FAT_CLUSTER_DEFECTIVE               0xFFFFFFF7
#define FAT_CLUSTER_FINAL                   0xFFFFFFF8

/* FAT Type definition */
#define FAT_TYPE_FAT12                      0
#define FAT_TYPE_FAT16                      1
#define FAT_TYPE_FAT32                      2
#define FAT_TYPE_NOT_SUPPORT                3

/* Directory entry Attribute*/
#define ATTRIB_READ_ONLY                    0x01
#define ATTRIB_HIDDEN                       0x02
#define ATTRIB_SYSTEM                       0x04
#define ATTRIB_VOLUME_ID                    0x08
#define ATTRIB_DIRECTORY                    0x10
#define ATTRIB_ARCHIVE                      0x20
#define ATTRIB_LONG_NAME                    0x0F

typedef int (*READ_SECTOR)(UINT32 StartSector, UINT32 SectorCount, UINT8 *Buf);
typedef void (*TOGGLE_LED)(UINT8 WithCount);

/* partition table structure */
typedef struct
{
    UINT BootIndicator:8;
    UINT StartingHead:8;
    UINT StartingSector:6;
    UINT StartingCylinder:10;

    UINT SystemID:8;
    UINT EndingHead:8;
    UINT EndingSector:6;
    UINT EndingCylinder:10;

    UINT32 RelativeSector;
    UINT32 TotalSector;
}PartitionTableStruct;


/* Extended FDC Description structure */
typedef struct
{
    //UINT8 JumpCommand[3];
    //UINT8 SystemIdentifier[8];
    UINT16 SectorSize;
    UINT8 SectorPerCluster;
    UINT16 ReservedSectorCount;
    UINT8 FATNumber;
    UINT16 RootDirEntryNumber;
    //UINT16 TotalSector;
    //UINT8 MediumIdentifier;
    UINT16 SectorPerFAT;
    UINT16 SectorPerTrack;
    //UINT16 SlideNumber;
    UINT32 HiddenSectorNumber;
    //UINT32 TotalSector2;
    //UINT8 PhyDiskNumber;
    //UINT8 Reserved0;
    //UINT8 ExtendSignature;
    //UINT32 VolumeID;
    //UINT8 VolumeLable[11];
    UINT8 FileSystemType[8];
    //UINT8 Reserved1[448];
    //UINT16 SignatureWord;
    UINT32 FATSz32;
    UINT32 FAT32RootClus;
}ExtendFDCDescriptorStruct;


/* Directory Entry structure */
typedef struct
{
    UINT8 Name[8];
    UINT8 Extension[3];
    UINT8 Attrib;
    //#NT#2006/07/12#Shirley Cheng -begin
    //Fix FAT32 bug.
    UINT8 Reserved[8];
    UINT16 StartClusterH;
    //#NT#2006/07/12#Shirley Cheng -end
    UINT16 Time;
    UINT16 Date;
    UINT16 StartCluster;
    UINT32 FileLength;
}DirEntryStruct;


typedef struct
{
    //FileSysStruct *FileSys;

    //UINT32 Drive;
    //UINT8 FileName[MAX_DIRECTORY_PATH_SIZE];
    //UINT32 IsTTY;
    UINT32 StartCluster;
    UINT32 Attrib;
    UINT32 FileTotalSize;
    UINT32 CurrCluster;
    //UINT8 *ClusterBuf;
    //UINT32 ClusterBufSize;
    //UINT32 ClusterBufOffset;
    //UINT32 Pos;
    //UINT32 Dirty;
}FileHandleStruct;

typedef struct
{
    /* MBR */
    PartitionTableStruct PartitionTable;

    /* PBR */
    ExtendFDCDescriptorStruct FDCDesc;

    /* FAT table */
    UINT8 *FATTable;

    UINT32 MaxClusterNumber;

    /* for Partition Boot Sector */
    UINT32 PBSSector;

    /* for FAT table start Secotr */
    UINT32 FATSector;

    /* for FAT Type */
    UINT32 FATType;

    /* for ClusterSize */
    UINT32 ClusterSize;

    /* for first Data Cluster */
    UINT32 FirstClusterSector;

    /* for root directory */
    UINT32 RootDirCluster;
    UINT32 RootDirSector;
    UINT32 RootDirSectorCount;




    /* pointers */
    ////void *CardPtr;
    ////INIT_CARD InitCard;
    READ_SECTOR ReadSector;
    ////WRITE_SECTOR WriteSector;
    ////ERASE_SECTOR EraseSector;
    ////BOOL init;
}FileSysStruct;


//extern FileSysStruct FileSys;

extern void     fat_regfilesystem(READ_SECTOR pReadSectorPtr);

extern BOOL     fat_initFAT(UINT32 uiBuf, UINT32 uiSize);
extern void     fat_closeFAT(void);
extern BOOL     fat_open_rootfile(UINT8 *pfilename);
extern UINT32   fat_read_rootfile(UINT8 *pbuf, UINT32 uiRdLen);
extern void     fat_close_rootfile(void);
extern void     fat_regToggleLED(TOGGLE_LED pToggleLED);
extern void     fat_installAccessCB(STRG_ACCESS_CB accessCB);

//@}

#endif

