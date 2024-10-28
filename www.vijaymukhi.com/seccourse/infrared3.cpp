#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <af_irda.h>
#include <stdio.h>
#include <malloc.h>
// OBEX_CONTINUE 0x90 OBEX_SUCCESS 0xA0
WSADATA ws;
SOCKET ClientSock;
SOCKADDR_IRDA iraddr;
PDEVICELIST p;
char szServiceName[12],dataBuff[1030],recvBuff[256];
int nSize = 256,fileLen,count;
PIAS_QUERY pq;
FILE* fp;
int d;
void main()
{
WSAStartup(0x202,&ws);
pq = (PIAS_QUERY) malloc(1609);
ClientSock = socket(AF_IRDA, SOCK_STREAM, 0);
p = (PDEVICELIST)malloc(1000);
getsockopt(ClientSock, SOL_IRLMP, IRLMP_ENUMDEVICES, (char *)p, &nSize);
memcpy(&pq->irdaDeviceID, p->Device[0].irdaDeviceID, 4);
memcpy(&pq->irdaClassName, "OBEX", 5);
memcpy(&pq->irdaAttribName, "IrDA:TinyTP:LsapSel", 20);
nSize = 2000;
getsockopt(ClientSock, SOL_IRLMP, IRLMP_IAS_QUERY, (char *)pq, &nSize);
sprintf(szServiceName, "LSAP-SEL%d", pq->irdaAttribute.irdaAttribInt);
printf("Service Name is %s\n", szServiceName);
iraddr.irdaAddressFamily = AF_IRDA;
memcpy(iraddr.irdaDeviceID,p->Device[0].irdaDeviceID,4);
strcpy(iraddr.irdaServiceName, szServiceName);
d = connect(ClientSock, (struct sockaddr *)&iraddr, sizeof (iraddr));
printf("Connect returns %d\n",d);
Sleep(1000);        
dataBuff[0] =  0x80; //OBEX_CONNECT
*((unsigned short *)&dataBuff[1]) = htons((unsigned short)7);
dataBuff[3] = 0x10; //OBEX_VERSION
dataBuff[4] = 0 ; //OBEX_CONNECT_FLAGS
*((unsigned short *)&dataBuff[5]) = htons((unsigned short)0x800);
send(ClientSock, (const char*)dataBuff, 7, 0);
recv(ClientSock, (char *)recvBuff, 0x100, 0);
printf("First Byte recieved is %x\n",recvBuff[0]);
char *f = "hel1.txt";
fileLen = (strlen(f) + 1) * 2;
dataBuff[0] = 2 ;  //OBEX_PUT
*((unsigned short *)&dataBuff[1]) = htons((unsigned short)(fileLen + 6));
dataBuff[3] = 0x01 ; // OBEX_NAME;
*((unsigned short *)&dataBuff[4]) = htons((unsigned short)(fileLen + 3));
for (int i=0; i < (int)(fileLen/2); i++)
*((unsigned short *)&dataBuff[6 + 2*i]) = htons((unsigned short)f[i]);
send(ClientSock, (const char*)dataBuff, fileLen + 6, 0);
recv(ClientSock, (char *)recvBuff, 0x100, 0);
printf("First Byte recieved is %x\n",recvBuff[0]);
fp = fopen("C:\\vijay712.txt", "r");
dataBuff[0] = 2; //OBEX_PUT
dataBuff[3] = 0x48 ; // OBEX_BODY;
while ((count = fread(&dataBuff[6], 1, 0x400, fp)) != 0)
{
if (count < 0x400)
{
dataBuff[0] = 0x82 ;  //OBEX_PUT_FINAL
dataBuff[3] = 0x49 ; // OBEX_END_OF_BODY;
}
*((unsigned short *)&dataBuff[1]) = htons((unsigned short)(count + 6));
*((unsigned short *)&dataBuff[4]) = htons((unsigned short)(count + 3));
send(ClientSock, (const char*)dataBuff, count + 6, 0);
recv(ClientSock, (char *)recvBuff, 0x100, 0);
}
dataBuff[0] = 0x81; // OBEX_DISCONNECT;
*((unsigned short *)&dataBuff[1]) = htons((unsigned short)(3));
send(ClientSock, (const char*)dataBuff, 3, 0);
recv(ClientSock, (char *)recvBuff, 0x100, 0);
fclose(fp);
closesocket(ClientSock);
WSACleanup();
}
