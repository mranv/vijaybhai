#pragma pack(1)
#include <windows.h>
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
struct
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
}mft;
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
main()
{
HANDLE hFile;
int ret,bytesread,cnt,totalcnt;
long noofclusters;
hFile = CreateFile("\\\\.\\e:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,90*512,0,FILE_BEGIN);
ret = ReadFile(hFile,&mft,1024,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
printf("Magic %c%c%c%c\n",mft.magic[0],mft.magic[1],mft.magic[2],mft.magic[3]);
printf("Update to Offset Sequence 0x%x %d\n",mft.updateoffsetseq,mft.updateoffsetseq);
printf("Size of Update Sequence %d\n",mft.sizeofupdateseq);
printf("$LogFile Sequence Number %x %x %x %x %x %x %x %x\n",mft.logfileseqno[0],mft.logfileseqno[1],mft.logfileseqno[2],mft.logfileseqno[3],mft.logfileseqno[4],mft.logfileseqno[5],mft.logfileseqno[6],mft.logfileseqno[7]);
printf("Sequence Number %d\n",mft.sequenceno);
printf("Reference Count %d\n",mft.referencecount);
printf("Offset of the First Attribute 0x%x %d\n",mft.offsetfirtsattribute,mft.offsetfirtsattribute);
printf("Flags %x\n",mft.flags);
printf("Size of FILE record 0x%x %d\n",mft.filesz,mft.filesz);
printf("Size allocated to FILE record 0x%x %d\n",mft.sizeallocated,mft.sizeallocated);
printf("File reference to base FILE record %x %x\n",mft.lowfilereference,mft.highfilereference);
printf("Next Attribute ID 0x%x %d\n",mft.nextattributeID,mft.nextattributeID);
printf("MFT Number %d\n",mft.mftnumber);
pattribute = &mft.a;
printf("Attribute Code 0x%x Size %ld\n",pattribute->attributecode,pattribute->sizeofattribute);
printf("Attribute is resident %x\n",pattribute->resident);
printf("Length of Attribute Name %d\n",pattribute->namelength);
printf("Offset of the name of the attribute %d\n",pattribute->namelengthoffset);
printf("Flags 0x%x \n",pattribute->flags);
printf("Instance %d\n",pattribute->instance);
printf("Size of Attribute Data %d\n",pattribute->lengthofattributedata);
printf("Offset of Attribute Data %d\n",pattribute->offsetofattributedata);
printf("Resident Attribute Flags %x\n",pattribute->residentflags);
pattribute++;
sa = (struct standardattribute *)pattribute;
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
sa++;
pattribute = (struct attrfilename *)sa;
printf("Attribute Code 0x%x Size %ld\n",pattribute->attributecode,pattribute->sizeofattribute);
printf("Attribute is resident %x\n",pattribute->resident);
printf("Length of Attribute Name %d\n",pattribute->namelength);
printf("Offset of the name of the attribute %d\n",pattribute->namelengthoffset);
printf("Flags 0x%x \n",pattribute->flags);
printf("Instance %d\n",pattribute->instance);
printf("Size of Attribute Data %d\n",pattribute->lengthofattributedata);
printf("Offset of Attribute Data %d\n",pattribute->offsetofattributedata);
printf("Resident Attribute Flags %x\n",pattribute->residentflags);
pattribute++;
fa = (struct standardattribute *)pattribute;
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
printf("File Name %S\n",fa->filename);
pattribute--;
p = pattribute;
p = p + pattribute->sizeofattribute;
pattribute = (struct attrfilename *)p;
printf("Attribute Code 0x%x Size %ld\n",pattribute->attributecode,pattribute->sizeofattribute);
printf("Attribute is resident %x\n",pattribute->resident);
printf("Length of Attribute Name %d\n",pattribute->namelength);
printf("Offset of the name of the attribute %d\n",pattribute->namelengthoffset);
printf("Flags 0x%x \n",pattribute->flags);
printf("Instance %d\n",pattribute->instance);
printf("Size of Attribute Data %d\n",pattribute->lengthofattributedata);
printf("Offset of Attribute Data %d\n",pattribute->offsetofattributedata);
printf("Resident Attribute Flags %x\n",pattribute->residentflags);
totalcnt = pattribute->lengthofattributedata ;
pattribute++;
p = pattribute;
for ( cnt=0; cnt < totalcnt ; cnt++)
printf("%c",p[cnt]);
}
