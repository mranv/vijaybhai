#pragma pack(1)
#include <windows.h>
main()
{
HANDLE hFile;
int ret,bytesread,cnt;
struct
{
char jmp[3];
char oem[8];
short bytespersector;
char sectorspercluster;
short sectorsbeforefat;
byte numberoffats;
short rootentries;
short smallsectors;
byte mediadesc;
short sectorsperfat;
short sectorspertrack;
short noofheads;
long sectorsbeforebootrecord;
long totalnumberofsectors;
long sectorsperfat32;
short flags;
char lowversion;
char highversion;
long rootdirectorycluster;
short fsinfosector;
short backupbootsector;
char reserved[12];
unsigned char physcialdrive;
char reserved1;
char extendedbootsignature;
long volumeserialnumber;
char volumelabel[11];
char systemid[11];
char a[512];
}bpb;
long noofclusters;
hFile = CreateFile("\\\\.\\c:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,0,0,FILE_BEGIN);
ret = ReadFile(hFile,&bpb,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
printf("Oem string ");
for(cnt=0; cnt <= 7 ; cnt++)
printf("%c",bpb.oem[cnt]);
printf("\n");
printf("Bytes Per Sector %u\n",bpb.bytespersector);
printf("Sectors Per Cluster %u Actual Size of a cluster %d\n",bpb.sectorspercluster , bpb.sectorspercluster * 512);
printf("Empty Sectors before FAT %u\n",bpb.sectorsbeforefat);
printf("Copies of the FAT %d\n",bpb.numberoffats);
printf("No of files in root directory ( Must be 0 ) %d\n",bpb.rootentries);
printf("Small Sectors ( Must be 0 ) %d\n",bpb.smallsectors);
printf("Media Descriptor byte ( F8 for hard disk ) %X\n",bpb.mediadesc);
printf("Sectors taken up by the FAT ( Must be 0 ) %d\n",bpb.sectorsperfat);
printf("Sectors contained in a track %d\n",bpb.sectorspertrack);
printf("Number Of heads %d\n",bpb.noofheads);
printf("Sectors before the boot record starts %d\n",bpb.sectorsbeforebootrecord);
printf("Total Number of Sectors on hard disk %lu\n",bpb.totalnumberofsectors);
printf("Sectors occupied by FAT32 %lu\n",bpb.sectorsperfat32);
printf("Extended flags %04x bits 0-3 %x bit 7 %d \n",bpb.flags,bpb.flags&0x0f,bpb.flags&0x80);
printf("Version Number of FAT %d.%d\n",bpb.highversion,bpb.lowversion);
printf("Root Diectory starts at cluster %lu\n",bpb.rootdirectorycluster);
printf("Sector where FSINFO structure begins %d\n",bpb.fsinfosector);
printf("Sector where Backup boot record is stored %d\n",bpb.backupbootsector);
printf("Physical Drive Number %x\n",bpb.physcialdrive);
printf("Extended boot Signature %x\n",bpb.extendedbootsignature);
printf("Volume Serial Number %x\n",bpb.volumeserialnumber);
printf("Volume Label ");
for(cnt=0; cnt <= 10 ; cnt++)
printf("%c",bpb.volumelabel[cnt]);
printf("\n");
printf("System ID ");
for(cnt=0; cnt <= 7 ; cnt++)
printf("%c",bpb.systemid[cnt]);
printf("\n");
printf("First copy of FAT starts at %d\n",bpb.sectorsbeforefat);
printf("Second  copy of FAT starts at %d\n",bpb.sectorsbeforefat+bpb.sectorsperfat32);
printf("Root Directory starts at %d\n",bpb.sectorsbeforefat+ 2*bpb.sectorsperfat32);
noofclusters = bpb.totalnumberofsectors / bpb.sectorspercluster;
printf("Number of Clusters %lu\n",noofclusters);
}

