#define _WIN32_WINNT 0x0500
#include <WinSock2.h>
#include <AF_Irda.h>
#include <strsafe.h> 
#pragma comment ( lib, "WS2_32.Lib" )
WSADATA ws;
SOCKET s;
int xOn,size,d;
PDEVICELIST p;
int main()
{
SOCKADDR_IRDA A = { AF_IRDA, 0, 0, 0, 0, "IrLPT" };
WSAStartup(0x202,&ws);
s=socket(AF_IRDA,SOCK_STREAM,0);
xOn= 1;
setsockopt(s,SOL_IRLMP,IRLMP_IRLPT_MODE,(char *) &xOn,sizeof xOn);
p = malloc(1000);
d = 1000;
getsockopt(s,SOL_IRLMP,IRLMP_ENUMDEVICES,(char *)p,&d);
printf("No of devices found %d\n",p->numDevice );
printf("%s\n",p->Device[0].irdaDeviceName);
printf("addr: %02x-%02x-%02x-%02x\n",p->Device[0].irdaDeviceID[0],p->Device[0].irdaDeviceID[1],p->Device[0].irdaDeviceID[2],p->Device[0].irdaDeviceID[3]);
printf("Hint is %x %x\n",p->Device[0].irdaDeviceHints1,p->Device[0].irdaDeviceHints2);
if ( p->Device[0].irdaDeviceHints1 & LM_HB1_Printer )
printf("Printer\n");
memcpy (A.irdaDeviceID,p->Device[0].irdaDeviceID, 4 );
connect(s,(const struct sockaddr *) &A, sizeof(SOCKADDR_IRDA));
d = 4;
getsockopt (s,SOL_IRLMP,IRLMP_SEND_PDU_LEN,(char *) &size,&d);
printf("Maximum send size is: %d\n", size );
Sleep ( 5000 );
send(s, "Vijay Mukhi\nSonal", 18 , 0 );
closesocket ( s );
WSACleanup ();
}
