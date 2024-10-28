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
printf("Select Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");
}
void SelectFile(unsigned char a1, unsigned char a2)
{
unsigned char bb[100],aa[100];
int len,d,i;
bb[0] = a1;
bb[1] = a2;
d = WriteFile(h, bb , 2  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("SelectFile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");
}
void ReadRecord(unsigned char recordnumber, unsigned char mode,unsigned char recordlength, unsigned char *p)
{
unsigned char bb[100];
int d,len;
bb[0] = 0xa0;
bb[1] = 0xb2;
bb[2] = recordnumber;
bb[3] = mode;
bb[4] = recordlength;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, p , 100 , &len, 0);
}
main(int argc, char *argv[])
{
unsigned char aa[1000];
int i;
unsigned char ii;
SetCommPort(argv[1]);
ReadAtrBytes();
Select();
SelectFile(0x7F , 0x4d);
Select();
SelectFile(0x8F , 0x0c);
for ( ii = 2 ; ii <= 13 ; ii++)
{
ReadRecord(ii, 0x04 , 18 , aa);
if ( aa[22] == 0 && aa[23] == 0)
printf("Slot No %02d: <Empty>",ii-1);
else
{
printf("Slot No %02d: ",ii-1);
for ( i = 6 ; i <= 17 ; i++)
{
if ( aa[i] == 0xff)
break;
printf("%c",aa[i]);
}
}
printf("\n");
}
CloseHandle(h);
}
