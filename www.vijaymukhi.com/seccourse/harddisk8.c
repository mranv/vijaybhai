#pragma pack(1)
#include <windows.h>
main()
{
HANDLE hFile;
int ret,bytesread,cnt;
struct
{
char jmp[3];
char oem[8];
short bytespersector;
char sectorspercluster;
short res1;
char mustbezero[5];
unsigned char mediades;
char mustbezero1[2];
char notused[8];
char mustbezero2[4];
char notused1[4];
long lowtotalsectors;
long hightotalsectors;
long lowcluster;
long highcluster;
long lowclustermirror;
long highclustermirror;
unsigned char sizesofmft;
char notused2[3];
unsigned char clusterperindexbuffer;
char notused3[3];
unsigned char vol[8];
long checksum;
char a[512];
char notused4[4];

}bpb;
long noofclusters;
hFile = CreateFile("\\\\.\\e:",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
if ( hFile == 0)
{
printf("Error opening Drive\n");
return -1;
}
SetFilePointer(hFile,0,0,FILE_BEGIN);
ret = ReadFile(hFile,&bpb,512,&bytesread,0);
if ( ret == 0)
{
printf("Error Reading Drive\n");
return -1;
}
printf("Oem string ");
for(cnt=0; cnt <= 7 ; cnt++)
printf("%c",bpb.oem[cnt]);
printf("\n");
printf("Bytes Per Sector %u\n",bpb.bytespersector);
printf("Sectors Per Cluster %u Actual Size of a cluster %d\n",bpb.sectorspercluster , bpb.sectorspercluster * 512);
printf("Must be zero %d%d%d%d%d %d%d %d%d%d%d\n",bpb.mustbezero[0],bpb.mustbezero[1],bpb.mustbezero[2],bpb.mustbezero[3],bpb.mustbezero[4] , bpb.mustbezero1[0],bpb.mustbezero1[1],bpb.mustbezero2[0],bpb.mustbezero2[1],bpb.mustbezero2[2],bpb.mustbezero2[3]);
printf("Media Descriptor Byte %x\n",bpb.mediades);
printf("Total Number of Sectors %ld %ld\n",bpb.lowtotalsectors, bpb.hightotalsectors);
printf("Starting cluster of the MFT %ld %ld\n",bpb.lowcluster,bpb.highcluster);
printf("Starting cluster of the MFT Mirror %ld %ld\n",bpb.lowclustermirror,bpb.highclustermirror);
printf("Size of MFT %d\n",bpb.sizesofmft);
printf("Clusters for the Index Buffer %d\n",bpb.clusterperindexbuffer);
printf("Volume Serial Number %x%x%x%x%X%X%x%X\n",bpb.vol[0],bpb.vol[1],bpb.vol[2],bpb.vol[3],bpb.vol[4],bpb.vol[5],bpb.vol[6],bpb.vol[7]);
printf("Checksum %ld\n",bpb.checksum);
}

