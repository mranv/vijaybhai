#pragma pack(1)
#include <windows.h>
unsigned char sector[1024];
HANDLE hFile;
int ret,bytesread,cnt,totalcnt;
long noofclusters;
char *GetFileNameFlags (unsigned char a)
{
char *aa = malloc(100);
if ( a == 0x0 )
strcpy(aa,"POSIX");
if ( a == 0x1 )
strcpy(aa,"WIN32 LFN");
if ( a == 0x2 )
strcpy(aa,"DOS");
if ( a == 0x3 )
strcpy(aa,"DOS & Windows same");
return aa;
}
char *GetFileAttribute(long a)
{
char *aa = malloc(100);
aa[0] = 0;
if ( (a&0x0001) == 0x0001)
strcat(aa,"READONLY ");
if ( (a&0x0002) == 0x0002)
strcat(aa,"HIDDEN ");
if ( (a&0x0004) == 0x0004)
strcat(aa,"SYSTEM ");
if ( (a&0x0010) == 0x0010)
strcat(aa,"DIRECTORY ");
if ( (a&0x0020) == 0x0020)
strcat(aa,"ARCHIVE ");
if ( (a&0x0040) == 0x0040)
strcat(aa,"DEVICE ");
if ( (a&0x0080) == 0x0080)
strcat(aa,"NORMAL ");
if ( (a&0x0100) == 0x0100)
strcat(aa,"TEMPORARY ");
if ( (a&0x0200) == 0x0200)
strcat(aa,"SPARSE ");
if ( (a&0x0400) == 0x0400)
strcat(aa,"PEPARSE ");
if ( (a&0x0800) == 0x0800)
strcat(aa,"COMPRESSED ");
if ( (a&0x1000) == 0x1000)
strcat(aa,"OFFLINE ");
if ( (a&0x2000) == 0x2000)
strcat(aa,"CONTENT NOT INDEXED ");
if ( (a&0x4000) == 0x4000)
strcat(aa,"ENCRYPTED ");
if ( (a&0x10000000) == 0x10000000)
strcat(aa,"DUPLICATE FILE NAME INDEX ");
if ( (a&0x20000000) == 0x20000000)
strcat(aa,"DUPLICATE VIEW NAME INDEX ");

return aa;
}
char *GetDateTimeAsString(SYSTEMTIME *s)
{
char *p = malloc(100);
sprintf(p,"%02d/%02d/%04d %02d:%02d",s->wMonth,s->wDay,s->wYear , s->wHour,s->wMinute);
return p;
}
struct mft
{
char magic[4];
short updateoffsetseq;
short sizeofupdateseq;
unsigned char logfileseqno[8];
short sequenceno;
short referencecount;
short offsetfirtsattribute;
short flags;
long filesz;
long sizeallocated;
long lowfilereference;
long highfilereference;
short nextattributeID;
char res[2];
long mftnumber;
char a[1024];
};
struct mft *pmft;
struct attribute
{
long attributecode;
long sizeofattribute;
unsigned char resident;
unsigned char namelength;
short namelengthoffset;
short flags;
short instance;
long lengthofattributedata;
short offsetofattributedata;
unsigned char residentflags;
unsigned char res;
};
struct attribute *pattribute;
struct standardattribute
{
FILETIME creation;
FILETIME accessattribute;
FILETIME mftchange;
FILETIME accessfile;
long attr;
long versionsallowed;
long versionnumber;
long classid;
long ownerid;
long securityid;
long quotal;
long quotah;
long usal;
long usnh;
};
struct standardattribute *sa;
SYSTEMTIME s,s1;
char timearry[255];
struct attrfilename
{
char mft[8];
FILETIME creation;
FILETIME accessattribute;
FILETIME mftchange;
FILETIME accessfile;
long allocatedfordatal;
long allocatedfordatah;
__int64 datasize;
long attr;
long reparsepoint;
char filenamelen;
char filenameflags;
char filename[1];
};
struct attrfilename *fa;
char *p;
void DisplayMFT(struct mft *pmft)
{
printf("Magic %c%c%c%c\n",pmft->magic[0],pmft->magic[1],pmft->magic[2],pmft->magic[3]);
printf("Update to Offset Sequence 0x%x %d\n",pmft->updateoffsetseq,pmft->updateoffsetseq);
printf("Size of Update Sequence %d\n",pmft->sizeofupdateseq);
printf("$LogFile Sequence Number %x %x %x %x %x %x %x %x\n",pmft->logfileseqno[0],pmft->logfileseqno[1],pmft->logfileseqno[2],pmft->logfileseqno[3],pmft->logfileseqno[4],pmft->logfileseqno[5],pmft->logfileseqno[6],pmft->logfileseqno[7]);
printf("Sequence Number %d\n",pmft->sequenceno);
printf("Reference Count %d\n",pmft->referencecount);
printf("Offset of the First Attribute 0x%x %d\n",pmft->offsetfirtsattribute,pmft->offsetfirtsattribute);
printf("Flags %x\n",pmft->flags);
printf("Size of FILE record 0x%x %d\n",pmft->filesz,pmft->filesz);
printf("Size allocated to FILE record 0x%x %d\n",pmft->sizeallocated,pmft->sizeallocated);
printf("File reference to base FILE record %x %x\n",pmft->lowfilereference,pmft->highfilereference);
printf("Next Attribute ID 0x%x %d\n",pmft->nextattributeID,pmft->nextattributeID);
printf("MFT Number %d\n",pmft->mftnumber);
}
void DisplayStandardInformationAttribute(struct standardattribute *sa)
{
printf("--Attribute Name Standard Information--\n");
FileTimeToSystemTime(&sa->creation,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Creation Time %s\n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&sa->accessattribute,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of Data Attribute %s\n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&sa->mftchange,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of MFT %s \n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&sa->accessfile,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of File %s \n",GetDateTimeAsString(&s1));
printf("File Attributes %x %s\n",sa->attr, GetFileAttribute(sa->attr));
printf("Versions Allowed %d\n",sa->versionsallowed);
printf("Version Number %d\n",sa->versionnumber);
printf("Class ID %d\n",sa->classid);
printf("Owner ID %d\n",sa->ownerid);
printf("Security ID %d\n",sa->securityid);
printf("Quotas %d %d\n",sa->quotal,sa->quotah);
printf("Update Sequence Number %d %d\n",sa->usal,sa->usnh);
}
void DisplayFileNameAttribute(struct attrfilename *fa)
{
int i,j;
printf("--Attribute Name File Name--\n");
FileTimeToSystemTime(&fa->creation,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Creation Time %s\n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&fa->accessattribute,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of Data Attribute %s\n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&fa->mftchange,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of MFT %s \n",GetDateTimeAsString(&s1));
FileTimeToSystemTime(&fa->accessfile,&s);
SystemTimeToTzSpecificLocalTime(0,&s,&s1),
printf("Last Change of File %s \n",GetDateTimeAsString(&s1));
printf("Allocated for data %d %d\n",fa->allocatedfordatal,fa->allocatedfordatah);
printf("Data Size %I64d \n",fa->datasize);
printf("File Attributes %x %s\n",fa->attr, GetFileAttribute(fa->attr));
printf("Reparse Point %x\n",fa->reparsepoint);
printf("Length of Filename %d\n",fa->filenamelen);
printf("File name flags %x %s\n",fa->filenameflags,GetFileNameFlags(fa->filenameflags));
for ( i = 0,j=0 ; i < fa->filenamelen ; i = i + 1, j = j +2 )
printf("%c",fa->filename[j]);
printf("\n");
}
void DisplayDataAttribute(char *data, long size)
{
printf("--Attribute Name Data--\n");
for ( cnt=0; cnt < size ; cnt++)
printf("%c",data[cnt]);
}
void DisplayVolumeattribute(char *data, long size)
{
int i,j;
printf("--Attribute Name Volume--\n");
for ( i = 0,j=0 ; i < size ; i = i + 2, j = j +2 )
printf("%c",data[j]);
printf("\n");
}
char *GetVolumeAttributeFlags(short p)
{
char *pc = (char *)malloc(100);
pc[0] = 0;
if ( (p&0x001) == 0x001)
strcat(pc,"VOLUME is Dirty");
if ( (p&0x002) == 0x002)
strcat(pc,"VOLUME Resize Log File");
if ( (p&0x004) == 0x004)
strcat(pc,"VOLUME Upgrade on Mount");
if ( (p&0x008) == 0x008)
strcat(pc,"VOLUME Mounted on NT4");
if ( (p&0x010) == 0x010)
strcat(pc,"VOLUME Delete USN Underway");
if ( (p&0x020) == 0x020)
strcat(pc,"VOLUME Repair Object ID");
return pc;
}
struct volumeattribute
{
__int64 reserved;
unsigned char majorver;
unsigned char minorver;
short flags;
};
void DisplayVolumeInformationAttribute(struct volumeattribute *pva)
{
printf("--Attribute Name Volume Informtion--\n");
printf("NTFS Volume version number %d.%d\n",pva->majorver,pva->minorver);
printf("Flags %04x %s\n",pva->flags , GetVolumeAttributeFlags(pva->flags));
}
char *GetCollationRules(long r)
{
char *p = (char *)malloc(100);
if ( r == 0)
strcpy(p,"Binary");
if ( r == 1)
strcpy(p,"File Name");
if ( r == 2)
strcpy(p,"Unicode");
if ( r == 16)
strcpy(p,"Long");
if ( r == 17)
strcpy(p,"SID");
if ( r == 18)
strcpy(p,"Decurity Hash");
if ( r == 19)
strcpy(p,"Unsigned Long");
return p;
}
struct indexheader
{
long indexoffset;
long indexlength;
long allocatedsize;
char flags;
char a[3];
};
struct indexrootattribute
{
long attributetype;
long collationrules;
long indexblocksize;
unsigned char clustersperindexblock;
char a[3];
struct indexheader ih;
};
void DisplayIndexRootAttribute(struct indexrootattribute *pira, long size)
{
printf("--Attribute Name Index Root--\n");
printf("Attribute Type %x\n",pira->attributetype);
printf("Collation Rules %s\n",GetCollationRules(pira->collationrules));
printf("Index Block Size %d\n",pira->indexblocksize);
printf("Clusters Per Index Block %d\n",pira->clustersperindexblock);
printf("Index offset %d\n",pira->ih.indexoffset);
printf("Index length %d\n",pira->ih.indexlength);
printf("Allocated Size %d\n",pira->ih.allocatedsize);
printf("Flags Bitmap %02x\n",pira->ih.flags);
}
void DisplayBitmapAttribute(unsigned long *data, int size)
{
int i;
printf("--Attribute Name Bitmap--\n");
for(i = 0 ; i < size/4 ; i++)
printf("%08x ",data[i]);
printf("\n");
}
void DisplayAttributeHeader(struct attribute *pattr)
{
printf("----------Attribute Start----------\n");
printf("Attribute Code 0x%x Size %ld\n",pattr->attributecode,pattr->sizeofattribute);
printf("Attribute is resident %x\n",pattr->resident);
printf("Length of Attribute Name %d\n",pattr->namelength);
printf("Offset of the name of the attribute %d\n",pattr->namelengthoffset);
printf("Flags 0x%x \n",pattr->flags);
printf("Instance %d\n",pattr->instance);
printf("Size of Attribute Data %d\n",pattr->lengthofattributedata);
printf("Offset of Attribute Data %d\n",pattr->offsetofattributedata);
printf("Resident Attribute Flags %x\n",pattr->residentflags);
if ( pattr->attributecode == 0x10)
{
struct standardattribute *psa;
psa = (struct standardattribute *)((char *)pattr + pattr->offsetofattributedata);
DisplayStandardInformationAttribute(psa);
}
if ( pattr->attributecode == 0x30)
{
struct attrfilename *pfa;
pfa = (struct attrfilename *)((char *)pattr + pattr->offsetofattributedata);
DisplayFileNameAttribute(pfa);
}
if ( pattr->attributecode == 0x60)
{
char *data;
data = (char *)pattr + pattr->offsetofattributedata;
DisplayVolumeattribute(data,pattr->lengthofattributedata);
}
if ( pattr->attributecode == 0x70)
{
struct volumeattribute *pva;
pva = (struct volumeattribute *)((char *)pattr + pattr->offsetofattributedata);
DisplayVolumeInformationAttribute(pva);
}
if ( pattr->attributecode == 0x80)
{
char *data;
data = (char *)pattr + pattr->offsetofattributedata;
DisplayDataAttribute(data,pattr->lengthofattributedata);
}
if ( pattr->attributecode == 0x90)
{
struct indexrootattribute *pira;
pira = (struct indexrootattribute *)((char *)pattr + pattr->offsetofattributedata);
DisplayIndexRootAttribute(pira,pattr->lengthofattributedata);
}
if ( pattr->attributecode == 0xb0)
{
char *data;
data = (char *)pattr + pattr->offsetofattributedata;
DisplayBitmapAttribute((unsigned long *)data,pattr->lengthofattributedata);
}
}
main()
{
hFile = CreateFile("\\\\.\\e:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,42*512,0,FILE_BEGIN);
ret = ReadFile(hFile,sector,1024,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
pmft = (struct mft *)sector;
DisplayMFT(pmft);
pattribute = (struct attribute *)(sector + pmft->offsetfirtsattribute);
while ( pattribute->attributecode != 0xffffffff )
{
DisplayAttributeHeader(pattribute);
pattribute = (struct attribute *)((char *)pattribute + pattribute->sizeofattribute) ;
}
}

