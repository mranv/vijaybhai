#include <windows.h>
HANDLE h;
int i,d,len,size,norec;
DCB dcb;
COMMTIMEOUTS CommTimeouts;
unsigned char cc[1000],dd[100];
void SetCommPort(char *pp)
{
h=CreateFile(pp,GENERIC_READ | GENERIC_WRITE ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL ,0);
printf("CreateFile returns h=%x\n",h);
dcb.DCBlength=sizeof(dcb);
GetCommState(h, &dcb);
dcb.DCBlength = sizeof ( DCB ) ;
dcb.BaudRate = 9600;
dcb.ByteSize = (BYTE) 8;
dcb.StopBits = 2; 
dcb.Parity=EVENPARITY;
dcb.fParity= 1 ;
dcb.fDtrControl     = DTR_CONTROL_DISABLE;
dcb.fRtsControl     = RTS_CONTROL_DISABLE;
dcb.fBinary         = 1;
dcb.XonLim          = 2;
dcb.XoffLim         = 4;
dcb.XonChar         = 0x13;
dcb.XoffChar        = 0x19;
SetCommState(h, &dcb);
GetCommTimeouts(h, &CommTimeouts);
CommTimeouts.ReadIntervalTimeout = MAXDWORD;//100;
CommTimeouts.ReadTotalTimeoutConstant = 0;//100;
CommTimeouts.ReadTotalTimeoutMultiplier =0;// 50;
CommTimeouts.WriteTotalTimeoutConstant = 0;//100;
CommTimeouts.WriteTotalTimeoutMultiplier = 0;//50;
SetCommTimeouts(h, &CommTimeouts);
}
main(int argc, char *argv[])
{
unsigned char bb[100],aa[100];
int i,d,len;
SetCommPort(argv[1]);
strcpy(bb,"at+csq\n");
d = WriteFile(h, bb , strlen(bb)  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("Select Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%c",aa[i]);
printf("\n");
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("Select Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%c",aa[i]);
printf("\n");
CloseHandle(h);
}

