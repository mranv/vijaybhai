#include <windows.h>
HANDLE h;
void SetCommPort(char *pp)
{
DCB dcb;
COMMTIMEOUTS CommTimeouts;
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
void ReadAtrBytes()
{
unsigned char aa[1000];
int d,i,len;
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
int d,i,len;
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
int d,i,len;
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
void WriteRecord(unsigned char recordnumber, unsigned char mode,unsigned char recordlength,char *p)
{
unsigned char bb[100];
char cc[18];
int i,d,len;
bb[0] = 0xa0;
bb[1] = 0xdc;
bb[2] = recordnumber;
bb[3] = mode;
bb[4] = recordlength;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
for ( i = 0 ; i <= 17 ; i++)
cc[i] = 0xff;
for ( i = 0 ; i < strlen(p) ; i++)
cc[i] = p[i];
cc[16] = strlen(p);
cc[17] = 01;
d = WriteFile(h, cc , 18  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
}
void WriteRecord1(unsigned char recordnumber, unsigned char mode,unsigned char recordlength,char *p)
{
unsigned char bb[100];
unsigned char cc[74];
int i,d,len;
bb[0] = 0xa0;
bb[1] = 0xdc;
bb[2] = recordnumber;
bb[3] = mode;
bb[4] = recordlength;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
cc[0] = 0xc9;
cc[1] = 0xfe;
cc[2] = 0x92;
cc[3] = 0x9e;
cc[4] = 0x7f;
cc[5] = 0x7b;
cc[6] = 0xcd;
cc[7] = 0xb0;
cc[8] = 0x1c;
cc[9] = 0xef;
cc[10] = 0xed;
cc[11] = 0xc8;
cc[12] = 0xac;
cc[13] = 0x12;
cc[14] = 0x29;
cc[15] = 0x99;
cc[16] = 0x3f;
cc[17] = 0x00;
cc[18] = 0x2f;
cc[19] = 0xe2;
cc[20] = 0x0a;
cc[21] = 0x98;
cc[22] = 0x19;
cc[23] = 0x12;
cc[24] = 0x20;
cc[25] = 0x90;
cc[26] = 0x00;
cc[27] = 0x20;
cc[28] = 0x24;
cc[29] = 0x94;
cc[30] = 0xf0;
cc[31] = 0x7f;
cc[32] = 0x20;
cc[33] = 0x6f;
cc[34] = 0x07;
cc[35] = 0x09;
cc[36] = 0x08;
cc[37] = 0x49;
cc[38] = 0x40;
cc[39] = 0x12;
cc[40] = 0x12;
cc[41] = 0x00;
cc[42] = 0x48;
cc[43] = 0x38;
cc[44] = 0x00;
cc[45] = 0x6f;
cc[46] = 0x30;
cc[47] = 0x18;
cc[48] = 0x04;
cc[49] = 0xf4;
cc[50] = 0x72;
cc[51] = 0x04;
cc[52] = 0xf4;
cc[53] = 0x72;
cc[54] = 0x04;
cc[55] = 0xf4;
cc[56] = 0x64;
cc[57] = 0x04;
cc[58] = 0xf4;
cc[59] = 0x34;
cc[60] = 0x04;
cc[61] = 0xf4;
cc[62] = 0x40;
cc[63] = 0x04;
cc[64] = 0xf4;
cc[65] = 0x14;
cc[66] = 0x04;
cc[67] = 0xf4;
cc[68] = 0x44;
cc[69] = 0x04;
cc[70] = 0xf4;
cc[71] = 0x41;
cc[72] = 0xff;
cc[73] = 0xff;
d = WriteFile(h, cc , recordlength  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord2 Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
}
void WriteRecord2(unsigned char recordnumber, unsigned char mode,unsigned char recordlength,char *p)
{
unsigned char bb[100];
unsigned char cc[50];
int i,d,len;
bb[0] = 0xa0;
bb[1] = 0xdc;
bb[2] = recordnumber;
bb[3] = mode;
bb[4] = recordlength;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
cc[0] = 0x3f;
cc[1] = 0x00;
cc[2] = 0x7f;
cc[3] = 0x10;
cc[4] = 0x6f;
cc[5] = 0x42;
cc[6] = 0x01;
cc[7] = 0x28;
cc[8] = 0x53;
cc[9] = 0x4d;
cc[10] = 0x53;
cc[11] = 0x20;
cc[12] = 0x43;
cc[13] = 0x65;
cc[14] = 0x6e;
cc[15] = 0x74;
cc[16] = 0x72;
cc[17] = 0x65;
cc[18] = 0xff;
cc[19] = 0xff;
cc[20] = 0xfd;
cc[21] = 0xff;
cc[22] = 0xff;
cc[23] = 0xff;
cc[24] = 0xff;
cc[25] = 0xff;
cc[26] = 0xff;
cc[27] = 0xff;
cc[28] = 0xff;
cc[29] = 0xff;
cc[30] = 0xff;
cc[31] = 0xff;
cc[32] = 0xff;
cc[33] = 0x07;
cc[34] = 0x91;
cc[35] = 0x19;
cc[36] = 0x89;
cc[37] = 0x12;
cc[38] = 0x00;
cc[39] = 0x00;
cc[40] = 0x50;
cc[41] = 0xff;
cc[42] = 0xff;
cc[43] = 0xff;
cc[44] = 0xff;
cc[45] = 0xff;
cc[46] = 0xff;
cc[47] = 0xff;
cc[48] = 0xff;
cc[49] = 0xff;
d = WriteFile(h, cc , recordlength  , &len, 0);
Sleep(1000);
d = ReadFile(h, bb , 100 , &len, 0);
printf("WriteRecord3 Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",bb[i]);
printf("\n");
}
void WriteTerminalProfile()
{
unsigned char bb[100],aa[100];
int d,i,len;
char cc[12];
bb[0] = 0xa0;
bb[1] = 0x10;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x04;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0x0f;
bb[1] = 0x03;
bb[2] = 0xff;
bb[3] = 0x77;
d = WriteFile(h, bb , 4  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0xa0;
bb[1] = 0x12;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x2c;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0xa0;
bb[1] = 0x14;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x0c;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

cc[0] = 0x81;
cc[1] = 0x03;
cc[2] = 0x01;
cc[3] = 0x25;
cc[4] = 0x00;
cc[5] = 0x82;
cc[6] = 0x02;
cc[7] = 0x82;
cc[8] = 0x81;
cc[9] = 0x83;
cc[10] = 0x01;
cc[11] = 0x00;
d = WriteFile(h, cc , 12  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0xa0;
bb[1] = 0xf2;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x0f;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Fetch Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0xa0;
bb[1] = 0xf2;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x0f;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Fetch Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");

bb[0] = 0xa0;
bb[1] = 0xf2;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = 0x0f;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("WriteTerminalProfile Fetch Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");


}
main(int argc, char *argv[])
{
unsigned char aa[1000];
unsigned char recordno;
SetCommPort(argv[1]);
ReadAtrBytes();
Select();
SelectFile(0x7f,0x4d);
Select();
SelectFile(0x8f,0x0c);
WriteTerminalProfile();
recordno = atoi(argv[2]);
recordno++;
WriteRecord(recordno,4,18,argv[3]);
Select();
SelectFile(0x8f,0x0d);
WriteRecord1(recordno,4,0x4a,argv[3]);
Select();
SelectFile(0x8f,0x0e);
WriteRecord2(recordno,4,0x32,argv[3]);
}
