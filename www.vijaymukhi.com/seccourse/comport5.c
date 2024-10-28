#include <windows.h>
HANDLE h;
DCB dcb;
COMMTIMEOUTS CommTimeouts;
void SetCommPort(char *pp)
{
h=CreateFile(pp,GENERIC_READ | GENERIC_WRITE ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL ,0);
printf("CreateFile returns h=%x\n",h);
GetCommState(h, &dcb);
dcb.BaudRate = 9600;
dcb.ByteSize = (BYTE) 8;
dcb.Parity=EVENPARITY;
dcb.fRtsControl = RTS_CONTROL_DISABLE;
SetCommState(h, &dcb);
GetCommTimeouts(h, &CommTimeouts);
CommTimeouts.ReadIntervalTimeout = MAXDWORD;//100;
SetCommTimeouts(h, &CommTimeouts);
}
void ReadAtrBytes()
{
unsigned char aa[1000];
int i,len,d;
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("ATR Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");
}
void Select()
{
unsigned char bb[100],aa[100];
int len,d,i;
bb[0] = 0xa0;
bb[1] = 0xa4;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x02;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
}
void SelectMFFile(unsigned char a1, unsigned char a2)
{
unsigned char bb[100],aa[100];
int len,d,i;
bb[0] = a1;
bb[1] = a2;
d = WriteFile(h, bb , 2  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
}
void IsFilePresent(unsigned char a1, unsigned char a2)
{
unsigned char bb[100],aa[100];
int len,d,i;
bb[0] = 0xa0;
bb[1] = 0xa4;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x02;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(100);
d = ReadFile(h, aa , 100 , &len, 0);
bb[0] = a1;
bb[1] = a2;
d = WriteFile(h, bb , 2  , &len, 0);
Sleep(100);
d = ReadFile(h, aa , 100 , &len, 0);
if ( aa[0] == a1 && aa[1] == a2 && aa[2] == 0x9f)
printf("FileID %02x%02x exists\n",a1,a2);
else
printf("FileID %02x%02x does not exist\n",a1,a2);

}
main(int argc, char *argv[])
{
unsigned char aa[1000];
unsigned char ii;
SetCommPort(argv[1]);
ReadAtrBytes();
Select();
SelectMFFile(0x3f,0x00);
for ( ii = 0; ii < 255 ; ii++)
IsFilePresent(0x00 , ii );
CloseHandle(h);
}
