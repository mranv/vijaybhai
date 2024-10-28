#include <windows.h>
main()
{
HANDLE hFile;
int ret,bytesread,cnt;
unsigned char diskbuffer[512];
//hFile = CreateFile("\\\\.\\PhysicalDrive0",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
hFile = CreateFile("\\\\.\\c:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);

if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,0,0,FILE_BEGIN);
ret = ReadFile(hFile,diskbuffer,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
for ( cnt = 0; cnt<= 511 ; cnt++)
printf("%x  ",diskbuffer[cnt]);
printf("\n");
}

