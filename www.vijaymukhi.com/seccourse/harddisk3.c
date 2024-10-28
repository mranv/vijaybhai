#pragma pack(1)
#include <windows.h>
main()
{
HANDLE hFile;
int ret,bytesread,cnt;
struct 
{
unsigned char a[487];
long freeclusters;
long nextavailablecluster;
char b[16];
}fsinfo;
hFile = CreateFile("\\\\.\\c:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,512,0,FILE_BEGIN);
ret = ReadFile(hFile,&fsinfo,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
printf("First Four Bytes of FSINFO structure %x %x %x %x\n",fsinfo.a[0],fsinfo.a[1],fsinfo.a[2],fsinfo.a[3]);
printf("IE4  Bytes of FSINFO structure %x %x %x %x\n",fsinfo.a[484],fsinfo.a[485],fsinfo.a[486],fsinfo.a[487]);
printf("Free clusters on disk %lu Total Space free %lu \n",fsinfo.freeclusters , fsinfo.freeclusters * 32 * 512);
printf("First free cluster %lu\n",fsinfo.nextavailablecluster);
}

