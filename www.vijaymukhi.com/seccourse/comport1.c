#include <windows.h>
HANDLE h;
int i,d,len,size,norec;
DCB dcb;
COMMTIMEOUTS CommTimeouts;
unsigned char cc[1000],dd[100];
int ii,times,j,ans,ans1;
char *ftype[] = {"RFU" , "MF" , "DF" ,  "" , "EF"};
char *efstruct[] = {"Transparent" , "Linear Fixed" , "" ,  "Cyclic" };
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
void ReadAtrBytes()
{
unsigned char aa[1000];
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
void SelectDF(char *p)
{
char zz[2];
if ( strcmp(p,"None") == 0)
return;
if ( strcmp(p,"GSM") == 0)
zz[1] = 0x20;
else
zz[1] = 0x10;
zz[0] = 0x7f;
SelectFile(zz[0],zz[1]);
}
void GetResponse(unsigned char a1, unsigned char ga[])
{
unsigned char bb[100];
bb[0] = 0xa0;
bb[1] = 0xc0;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = a1;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, ga , 100 , &len, 0);
printf("GetResponse Bytes are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",ga[i]);
printf("\n");
}
void SelectFileString(char *cc)
{
unsigned char bb[100],aa[100];
ii = 0;
times = (strlen(cc) / 3) + 1;
for ( j = 1 ; j <= times ; j++)
{
if ( cc[ii] >= 97 )
ans = (cc[ii] - 87) ;
else
ans = (cc[ii] - 48) ;
if ( cc[ii+1] >= 97 )
ans1 = (cc[ii+1] - 87);
else
ans1 = (cc[ii+1] - 48) ;
ii = ii + 3;
bb[j-1] = ans*16 + ans1;
}
d = WriteFile(h, bb , times , &len, 0);
Sleep(1000);
d = ReadFile(h, aa , 100 , &len, 0);
printf("SelectFileString Read are ");
for ( i = 0 ; i < len ; i++)
printf("%02x ",aa[i]);
printf("\n");
}
void DisplayEFHeader(unsigned char *p)
{
char s[100];
unsigned char d;
printf("FileID           :%02x%02x\n",p[5],p[6]);
printf("Type of file     :%s\n",ftype[p[7]]);
printf("Structure of file:%s\n",efstruct[p[14]]);
size = p[4] * 1 + p[3] * 256;
printf("File Size        :%04X\n",size);
if ( p[15] != 0)
{
norec = size / p[15];
printf("Number of a rec. :%02X\n",norec);
printf("Length of a rec. :%02X\n",p[15]);
}
d = p[9] >> 4;
if ( d == 0)
strcpy(s, "Always");
else
sprintf(s, "CHV (PIN) %d", d);
printf("Read Access      :%s\n",s);
d = p[9] & 0x0f;
if ( d == 0)
strcpy(s, "Always");
else
sprintf(s, "CHV (PIN) %d", d);
printf("Write Access     :%s\n",s);
d = p[10] >> 4 ;
if ( d == 0)
strcpy(s, "Always");
else
sprintf(s, "CHV (PIN) %d", d);
printf("Increase Access  :%s\n",s);
d = p[11] >> 4;
if ( d == 0)
strcpy(s, "Always");
else
sprintf(s, "CHV (PIN) %d", d);
printf("Invalidate       :%s\n",s);
d = p[11] & 0x0f;
if ( d == 0)
strcpy(s, "Always");
else
sprintf(s, "CHV (PIN) %d", d);
printf("Rehabilitate     :%s\n",s);
if ( p[12] & 0x01 == 1)
printf("File Status      :Not Invalidated\n");
if ( p[12] & 0x01 == 0)
printf("File Status      :Invalidated\n");
printf("Bytes at the end %x\n",p[13]);
if ( p[8] && 0x40 )
printf("Cyclic file Increase allowed\n");
if ( p[12] & 0x04 == 4)
printf("File Status Readable and Updateble when Invalidated\n");
if ( p[12] & 0x04 == 0)
printf("File Status Not Readable and Updateble when Invalidated\n");
}
void ReadBinaryRecord(unsigned char length, unsigned char *p)
{
unsigned char bb[100];
bb[0] = 0xa0;
bb[1] = 0xb0;
bb[2] = 0x00;
bb[3] = 0x00;
bb[4] = length;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, p , 100 , &len, 0);
}
void DisplayICCIdentification()
{
unsigned char aa[100];
unsigned char a,b;
ReadBinaryRecord(0x0a , aa);
printf("\n3f00:2fe2 ICC Identification Bytes\n");
for ( i = 6 ; i < 16 ; i++)
printf("%02x ",aa[i]);
printf("\n");
printf("Identification Number: ");
for ( i = 6 ; i < 16 ; i++)
{
a = aa[i] & 0x0f;
b = aa[i] >> 4;
printf("%X%X",a,b);
}
printf("\n");
}
void DisplayIMSI()
{
unsigned char aa[100];
unsigned char a,b;
ReadBinaryRecord(0x9, aa);
printf("\n3f00:2f20:6f07 IMSI\n");
for ( i = 6 ; i < 16 ; i++)
printf("%02x ",aa[i]);
printf("\n");
printf("Length of IMSI is %x\n",aa[6]);
printf(" MCC: %x%x%x (404 is India)\n",aa[7] >> 4 , aa[8] & 0x0f , aa[8] >> 4 );
printf(" MNC: %x%x \n",aa[9] & 0x0f , aa[9] >> 4);
printf("MSIN: ");
for ( i = 10 ; i < 15 ; i++)
{
a = aa[i] & 0x0f;
b = aa[i] >> 4;
printf("%X%X",a,b);
}
printf("\n");
}
void DisplaySmsStatus()
{
unsigned char aa[100];
ReadBinaryRecord(2, aa);
printf("\n3f00:7f10:6f43 SMS Status\n");
printf("Last used TP Message Reference parameter for last sms send %02x\n",aa[6]);
printf("Memory cap not execeeded flag  %02X %d\n" ,  aa[7] , aa[7] & 0x01);
printf("1 flag unset memory capacity not exceeded and is available\n");
}
void DisplayLanguagePreference(unsigned char *p)
{
unsigned char aa[100];
unsigned char size;
size = p[9] * 1 + p[8] * 256;
ReadBinaryRecord(size, aa);
printf("\n3f00:7f20:6f05 Language Preference\n");
for ( i = 6 ; i < 10 ; i++)
printf("%02X ",aa[i]);
printf("\n");
}
void DisplayCipheringKey()
{
unsigned char aa[100];
ReadBinaryRecord(9, aa);
printf("\n3f00:7f20:6f20 Ciphering Key Kc\n");
for ( i = 6 ; i < 15 ; i++)
printf("%02X ",aa[i]);
printf("\n");
printf("Kc :");
for ( i = 6 ; i < 14 ; i++)
printf("%02X",aa[i]);
printf("\n");
printf("Ciphering Key Sequence: %x\n",aa[14]);
}
void DisplayPLMNSelector(unsigned char *p)
{
unsigned char aa[100];
unsigned char size;
int entries;
unsigned char a,b;
size = p[9] * 1 + p[8] * 256;
ReadBinaryRecord(size, aa);
printf("\n3f00:7f20:6f30 PLMN Selector\n");
for ( i = 6 ; i < size ; i++)
printf("%02X ",aa[i]);
printf("\n");
entries = size / 3;
for ( i = 0 ; i < entries ; i++)
{
printf("MCC: ");
a = aa[6+i*3] & 0x0f;
b = aa[6+i*3] >> 4;
printf("%X%X",a,b);
printf("%X\n",aa[7 + i*3] & 0x0f);
printf("MNC: ");
a = aa[8+i*3] & 0x0f;
b = aa[8+i*3] >> 4;
printf("%X%X\n",a,b);
}
}
void DisplayHPLMNSeearchPeriod()
{
unsigned char aa[100];
ReadBinaryRecord(3, aa);
printf("\n3f00:7f20:6f31 HPLMN Search Period\n");
printf("%02X\n",aa[6]);
printf("Time Interval: ");
if ( aa[6] == 0xff)
printf("No HPLM search attempts\n");
else
printf("%0X\n", aa[6]);
}
void DisplayACMMAximumValue()
{
unsigned char aa[100];
ReadBinaryRecord(3, aa);
printf("\n3f00:7f20:6f37 ACM  Maximum Values\n");
printf("%02X %02X %02X\n", aa[6] , aa[7] , aa[8]);
printf("Maximum Value of Accumalated Call Meter: ");
if ( aa[6] == 0 && aa[7] == 0 && aa[8] == 0 )
printf("Not Valid\n");
else
printf("%x\n", aa[6] * 65536 + aa[7] * 256 + aa[8] * 1);
}
void DisplaySIMServiceTable()
{
}
void ReadRecord(unsigned char recordnumber, unsigned char mode,unsigned char recordlength, unsigned char *p)
{
unsigned char bb[100];
bb[0] = 0xa0;
bb[1] = 0xb2;
bb[2] = recordnumber;
bb[3] = mode;
bb[4] = recordlength;
d = WriteFile(h, bb , 5  , &len, 0);
Sleep(1000);
d = ReadFile(h, p , 100 , &len, 0);
}
void DisplayADN(unsigned char *p)
{
unsigned char count,recordlen,norec;
int size,j;
unsigned char aa[1000];
unsigned char a,b;
size = p[9] * 1 + p[8] * 256;
norec = size / p[20];
recordlen = p[20];
printf("Number of Records %d\n",norec);
for ( count = 1 ; count <= norec ; count++)
{
ReadRecord(count, 0x04 , recordlen , aa);
printf("%02d: ",count);
for ( i = 6 ; i < 18 ; i++)
if ( aa[i] >= ' ' && aa[i] <= 'z' )
printf("%c",aa[i]);
printf(":");
for ( j = 0 ; j < 12 ; j++)
{
if ( aa[20+j] != 0xff)
{
a = aa[20+j] & 0x0f;
b = aa[20+j] >> 4;
if ( b == 0x0f)
printf("%x",a);
else
printf("%x%x",a,b);
}
}
printf("\n");
}
}
void DisplaySMS(unsigned char *p)
{
unsigned char count,recordlen,norec;
int size,j;
unsigned char aa[1000];
unsigned char a,b;
size = p[9] * 1 + p[8] * 256;
norec = size / p[20];
recordlen = p[20];
printf("\n3f00:7f10:6f3c Short Messages\n");
printf("Number of Records %d\n",norec);
printf("recordlen=%x norec=%x\n",recordlen,norec);
//for ( count = 1 ; count <= norec ; count++)
{
ReadRecord(1, 0x04 , recordlen , aa);
printf("%x %x %x %x %x %x %x\n",aa[0],aa[1],aa[2],aa[3],aa[4],aa[5],aa[6]);
printf("SMS %02d: ",count);
for ( j = 0 ; j < 6 ; j++)
{
if ( aa[9+j] != 0xff)
{
a = aa[9+j] & 0x0f;
b = aa[9+j] >> 4;
if ( b == 0x0f)
printf("%x",a);
else
printf("%x%x",a,b);
}
}
printf("\n");
}
ReadRecord(2, 0x04 , recordlen , aa);
printf("%x %x %x %x %x %x %x\n",aa[0],aa[1],aa[2],aa[3],aa[4],aa[5],aa[6]);
printf("SMS %02d: ",count);
for ( j = 0 ; j < 6 ; j++)
{
if ( aa[9+j] != 0xff)
{
a = aa[9+j] & 0x0f;
b = aa[9+j] >> 4;
if ( b == 0x0f)
printf("%x",a);
else
printf("%x%x",a,b);
}
}
printf("\n");

ReadRecord(3, 0x04 , recordlen , aa);
printf("%x %x %x %x %x %x %x\n",aa[0],aa[1],aa[2],aa[3],aa[4],aa[5],aa[6]);
printf("SMS %02d: ",count);
for ( j = 0 ; j < 6 ; j++)
{
if ( aa[9+j] != 0xff)
{
a = aa[9+j] & 0x0f;
b = aa[9+j] >> 4;
if ( b == 0x0f)
printf("%x",a);
else
printf("%x%x",a,b);
}
}
printf("\n");


}
void DisplayFile(unsigned char *p)
{
if ( p[10]  == 0x2f && p[11] == 0xe2)
DisplayICCIdentification();
if ( p[10] == 0x6f && p[11] == 0x7)
DisplayIMSI();
if ( p[10] == 0x6f && p[11] == 0x43)
DisplaySmsStatus();
if ( p[10] == 0x6f && p[11] == 0x05)
DisplayLanguagePreference(p);
if ( p[10] == 0x6f && p[11] == 0x20)
DisplayCipheringKey();
if ( p[10] == 0x6f && p[11] == 0x30)
DisplayPLMNSelector(p);
if ( p[10] == 0x6f && p[11] == 0x31)
DisplayHPLMNSeearchPeriod();
if ( p[10] == 0x6f && p[11] == 0x37)
DisplayACMMAximumValue();
if ( p[10] == 0x6f && p[11] == 0x38)
DisplaySIMServiceTable();
if ( p[10] == 0x6f && p[11] == 0x3A)
DisplayADN(p);
if ( p[10] == 0x6f && p[11] == 0x3c)
DisplaySMS(p);
}
main(int argc, char *argv[])
{
unsigned char aa[1000];
SetCommPort(argv[1]);
ReadAtrBytes();
Select();
SelectFile(0x3f,0x00);
Select();
SelectDF(argv[2]);
GetResponse(0x1a , aa);
Select();
SelectFileString(argv[3]);
GetResponse(0x0f, aa );
DisplayEFHeader(&aa[5]);
DisplayFile(aa );
CloseHandle(h);
}

