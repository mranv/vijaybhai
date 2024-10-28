#define _WIN32_WINNT 0x0500
#include <WinSock2.h>
#include <AF_Irda.h>
#include <strsafe.h> 
#pragma comment ( lib, "WS2_32.Lib" )
// IrDA:IrCOMM", "IrLPT",  "IrNetv1", "IrLAN"  OBEX etc.
int i,d;
SOCKET s;
WSADATA ws;
SOCKADDR_IRDA A = {AF_IRDA, 0,0,0,0,"OBEX"};
int len=1000;
PDEVICELIST p;
char aa[1000];
int main()
{
p=(PDEVICELIST) malloc(sizeof(DEVICELIST)+(sizeof(IRDA_DEVICE_INFO) * 5));
d=WSAStartup(0x202,&ws);
printf("WSAStartup retuens %d\n",d);
s=socket(AF_IRDA,SOCK_STREAM,0);
printf("Socket returns %d\n",s);
getsockopt(s,SOL_IRLMP,IRLMP_ENUMDEVICES,(char *)p,&len);
printf("No of devices found %d\n",p->numDevice );
for ( i = 0; i < p->numDevice ; i++ )
{
printf("%s\n",p->Device[i].irdaDeviceName);
printf("addr: %02x-%02x-%02x-%02x\n",p->Device[i].irdaDeviceID[0],p->Device[i].irdaDeviceID[1],p->Device[i].irdaDeviceID[2],p->Device[i].irdaDeviceID[3]);
printf("Hint is %x %x\n",p->Device[i].irdaDeviceHints1,p->Device[i].irdaDeviceHints2);
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_PnP )
printf("Plug and Play\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_PDA_Palmtop )
printf("PalmTop\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_Computer )
printf("Computer\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_Printer )
printf("Printer\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_Modem )
printf("Modem\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_Fax )
printf("Fax\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB1_LANAccess )
printf("LanAcess\n");
if ( p->Device[i].irdaDeviceHints1 & LM_HB_Extension )
printf("Extension\n");
if ( p->Device[i].irdaDeviceHints2 & LM_HB2_Telephony )
printf("Telphony\n");
if ( p->Device[i].irdaDeviceHints2 & LM_HB2_FileServer )
printf("File Server\n");
if ( p->Device[i].irdaDeviceHints2 & 0x04 )
printf("IRComm\n");
if ( p->Device[i].irdaDeviceHints2 & 0x08 )
printf("Reserved\n");
if ( p->Device[i].irdaDeviceHints2 & 0x10 )
printf("Reserved\n");
if ( p->Device[i].irdaDeviceHints2 & 0x20 )
printf("Obex\n");
if ( p->Device[i].irdaDeviceHints2 & 0x40 )
printf("Reserved\n");
if ( p->Device[i].irdaDeviceHints2 & LM_HB_Extension )
printf("Extension\n");
}
memcpy (A.irdaDeviceID,p->Device[0].irdaDeviceID, 4 );
printf("Gonna connect to %s\n",A.irdaServiceName);
d = connect ( s, (const struct sockaddr *) &A,sizeof(SOCKADDR_IRDA));
printf("Connect returns %d sizeof struct %d\n",d,sizeof(SOCKADDR_IRDA));
Sleep (5000);
send (s,"Vijay Mukhi",10,0);
recv(s,aa, 100,0);
closesocket (s);
return 0;
}

