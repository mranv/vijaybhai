#pragma pack(1)
#include <windows.h>
main()
{
HANDLE hFile;
int ret,bytesread,cnt;
struct
{
char buf[512];
}part;
struct partitiontable
{
unsigned char bootable;
unsigned char startinghead;
unsigned short startingcylindersector;
unsigned char ptype;
unsigned char endinghead;
unsigned short endingcylindersector;
unsigned long startingsector;
unsigned long total;
};
struct partitiontable *p;
int startsector,highbytestart,lowbytestart;
int endsector,highbyteend,lowbyteend;
hFile = CreateFile("\\\\.\\PhysicalDrive0",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,0,0,FILE_BEGIN);
ret = ReadFile(hFile,&part,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
p = &part.buf[0x1be];
for ( cnt = 0 ; cnt <= 3 ; cnt++)
{
if ( p->ptype == 0)
break;
startsector = p->startingcylindersector & 0x3F;
highbytestart = p->startingcylindersector >> 8;
lowbytestart = (p->startingcylindersector & 0xc0) << 2;

endsector = p->endingcylindersector & 0x3F;
highbyteend = p->endingcylindersector >> 8;
lowbyteend = (p->endingcylindersector & 0xc0) << 2;
printf("Bottable Partition %02x\n",p->bootable);
printf("Starting Head %u Starting Cylinder %u Starting sector %u\n",p->startinghead,highbytestart + lowbytestart , startsector);
printf("Ending Head %u Ending Cylinder %u Ending sector %u\n",p->endinghead,highbyteend + lowbyteend, endsector);
printf("total Number of sectors %lu Partition Type %X\n",p->total, p->ptype);
printf("\n");
p++;
}
}

