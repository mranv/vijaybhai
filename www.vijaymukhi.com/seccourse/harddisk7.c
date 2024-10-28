#pragma pack(1)
#include <windows.h>
HANDLE hFile;
int ret,bytesread,cnt,i;
unsigned int fat[30000];
int fileclusters[1000];
struct rootdirectory
{
unsigned char name[8];
char ext[3];
unsigned char attribute;
char res;
char millisecondcreation;
short creationtime;
short creationdate;
short accessdate;
short highwordstartingcluster;
short writetime;
short writedate;
short lowwordstartingcluster;
long filesize;
};
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
char a[512*32];
}bpb;
struct longfilename
{
unsigned char sequenceno;
char name[10];
char attribute;
char type;
char checksum;
char name1[12];
short res;
char name2[4];
};
void decodetime(unsigned short time)
{
int hours, mins, seconds;
char *p = "p";
seconds = time & 0x001F;
mins = time >> 5;
mins = mins & 0x003F;
hours = time >> 11;
if ( hours < 12 )
p = "a";
if ( hours >= 13 )
hours = hours - 12;
printf("%02d:%02d%s ", hours , mins , p);
}
void decodedate(short date)
{
int day,month,year;
day = date & 0x001F;
month = date >> 5;
month = month & 0x000F;
year = date >> 9;
year = year + 1980;
printf("%02d/%02d/%04d ", month , day , year);
}
struct rootdirectory *r;
void printlfnentry()
{
struct longfilename *l;
struct rootdirectory *r1;
int seqno,i;
l = r;
seqno = l->sequenceno - 0x40;
//printf("Sequence Number %x %x\n",l->sequenceno , seqno);
r1 = r;
l = l + (seqno-1);
r = r + seqno;
printnormalentry(1);
for( i = 1 ; i <= seqno ; i++)
{ 
printf("%c%c%c%c%c",l->name[0],l->name[2],l->name[4],l->name[6],l->name[8]);
printf("%c%c%c%c%C%C",l->name1[0],l->name1[2],l->name1[4],l->name1[6],l->name1[8],l->name1[10]);
printf("%c%c",l->name2[0],l->name2[2]);
l--;
}
printf("\n");
}
int printnormalentry(int lfn)
{
int cnt;
decodedate(r->writedate);
decodetime(r->writetime);
if ( (r->attribute & 0x10) == 0x10)
printf("<DIR> ");
printf(" ");
if (! ((r->attribute & 0x10) == 0x10))
printf(" %lu ",r->filesize);
if ( !lfn ) 
{
for ( cnt = 0 ; cnt <= 7 ; cnt++)
if ( r->name[cnt] != 32) 
printf("%c",r->name[cnt]);
if ( !((r->attribute & 0x10) == 0x10))
{
if ( !(r->ext[0] == 32 && r->ext[1] == 32 && r->ext[2] == 32 ) ) 
printf(".");
printf("%c%c%c",r->ext[0],r->ext[1],r->ext[2]);
}
printf(" ");
}
printf("High %d Low %d ",r->highwordstartingcluster,r->lowwordstartingcluster);
dispalyclusters( (r->highwordstartingcluster << 16) + r->lowwordstartingcluster );
if ( !lfn ) 
printf("\n");
}
int dispalyclusters(int start)
{
int i = 0;
int j;
int oldstart = start;
int startsector;
unsigned char f[512];
printf("\nClusters %d ",start);
fileclusters[i] = start;
i++;
while ( fat[start] != 0xfffffff )
{
printf("%d ",fat[start]);
fileclusters[i] = fat[start];
i++;
start++;
}
printf("Last cluster is %d\n",start);
for ( j = 0  ; j < i ; j++)
printf("%d " , fileclusters[j]);
printf("\n");
startsector = bpb.sectorsbeforefat+ 2*bpb.sectorsperfat32 ;
printf("\nRoot Directory Start %d ",startsector);
startsector = startsector + bpb.sectorspercluster;
printf("Root Directory End %d ",startsector-1);
startsector = startsector + (oldstart-3)*bpb.sectorspercluster;
printf("File Starts %d \n",startsector);
SetFilePointer(hFile,startsector*512,0,FILE_BEGIN);
ret = ReadFile(hFile,f,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading from File\n");
return -1;
}
printf("%c%c%c%c\n",f[0],f[1],f[2],f[3]);
}
main()
{
long noofclusters;
char rd[512];
hFile = CreateFile("\\\\.\\d:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
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
SetFilePointer(hFile,bpb.sectorsbeforefat*512 ,0,FILE_BEGIN);
ret = ReadFile(hFile,fat,512*100,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Fat table\n");
return -1;
}
SetFilePointer(hFile,(bpb.sectorsbeforefat+ 2*bpb.sectorsperfat32)*512 ,0,FILE_BEGIN);
ret = ReadFile(hFile,rd,512*32,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
r = rd;
for ( i = 1 ; i <= 250 ; i++)
{
//printf("Start First Byte %x\n",r->name[0]);
if ( r->attribute == 0x8)
{
r++;
continue;
}
if ( r->name[0] == 0x00)
break;
if ( (r->name[0] == 0xE5 || r->name[0] == 0x5) )
{
r++;
continue;
}
if ( (r->attribute & 0xf) == 0xf)
{
printlfnentry();
}
else
{
printnormalentry(0);
}
r++;
}
}

