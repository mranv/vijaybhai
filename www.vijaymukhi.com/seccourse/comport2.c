#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <process.h>
//#include <math.h>
#include "stdlib.h"
#include "ctype.h"
#include "string.h"
COMSTAT comState;
HANDLE hArray[2];
DWORD dwError;
HANDLE hEndComPortEvent;
DCB dcb;
DWORD dwEvtMask;
HANDLE   hCom;
COMMTIMEOUTS CommTimeouts;
OVERLAPPED overlapped;
int ii,d,len,jj;
int iBytesReadThisTime;
BOOL bRead;
char bb[10000];
int main()
{
hCom = CreateFile("com2" ,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING, FILE_FLAG_OVERLAPPED,NULL);
hEndComPortEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
dcb.DCBlength=sizeof(dcb);
SetupComm(hCom, 128, 128);
GetCommState(hCom, &dcb);
dcb.DCBlength = sizeof ( DCB ) ;
dcb.BaudRate = 9600;
dcb.ByteSize = (BYTE) 8;
dcb.Parity=NOPARITY;
dcb.fParity= 0 ;
dcb.StopBits = ONESTOPBIT; 
dcb.fDtrControl     = DTR_CONTROL_DISABLE;
dcb.fRtsControl     = RTS_CONTROL_DISABLE;
dcb.fBinary         = 1;
dcb.XonLim          = 2;
dcb.XoffLim         = 4;
dcb.XonChar         = 0x13;
dcb.XoffChar        = 0x19;
SetCommState(hCom, &dcb);
GetCommTimeouts(hCom, &CommTimeouts);
CommTimeouts.ReadIntervalTimeout = MAXDWORD;//100;
CommTimeouts.ReadTotalTimeoutConstant = 0;//100;
CommTimeouts.ReadTotalTimeoutMultiplier =0;// 50;
CommTimeouts.WriteTotalTimeoutConstant = 0;//100;
CommTimeouts.WriteTotalTimeoutMultiplier = 0;//50;
SetCommTimeouts(hCom, &CommTimeouts);
overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
SetCommMask(hCom,EV_RXCHAR | EV_RXFLAG);
hArray[0]=overlapped.hEvent;
hArray[1]=hEndComPortEvent;
ClearCommError(hCom,&dwError,&comState); 
for(;;)
{
unsigned char szData[128];
jj++;
WaitCommEvent(hCom, &dwEvtMask,&overlapped);
ClearCommError(hCom,&dwError,&comState); 
Sleep(1000);
WaitForMultipleObjects(2,hArray,FALSE, INFINITE);
ResetEvent(overlapped.hEvent);
bRead= ReadFile(hCom, &szData, 100, &iBytesReadThisTime,&overlapped);
printf("ReadFIle returns %d Bytes read %d\n",bRead,iBytesReadThisTime);
for ( ii = 0; ii < iBytesReadThisTime ; ii++)
printf("%02x ", szData[ii]);
printf("\n");
if ( jj == 1)
{
Sleep(1000);
bb[0] = 0xA0;
bb[1] = 0xA4;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x02;
d = WriteFile(hCom, bb , 5 , &len, &overlapped);
printf("WriteFile returns d=%d len=%d jj=%d\n",d , len,jj);
}
if ( jj == 2)
{
Sleep(1000);
bb[0] = 0x3f;
bb[1] = 0x00;
d = WriteFile(hCom, bb , 2 , &len, &overlapped);
printf("WriteFile returns d=%d len=%d jj=%d\n",d , len,jj);
}


}
PurgeComm(hCom,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT);
CloseHandle(hCom);
CloseHandle(hEndComPortEvent);
}
