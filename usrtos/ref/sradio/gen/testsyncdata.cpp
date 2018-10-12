#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xmmintrin.h>
#include <sync.h>
#include <complexoperation.h>
#define QPSKLEN 116688
#define OVERHEAD 0
float fbuf[(QPSKLEN+OVERHEAD)*2];
double dbuf[116688*2];
unsigned char cbuf[8192];
class Sync aSync(QPSKLEN+OVERHEAD,31);

int main(int argc, char **argv)
{
    int count;
    int i,j;
	FILE *fp=fopen("RecQPSKSig.dat","rb");
	fread(dbuf,sizeof(double),116688*2,fp);
	fclose(fp);	
	i=0;
	//for( i=0;i<116688-4096;i+=4096 )
	{
		for( j=0;j<QPSKLEN;j++ )
		{
			{
				fbuf[2*j+1]=(float)dbuf[i+QPSKLEN-j];
				fbuf[2*j]=(float)dbuf[i+QPSKLEN-j+116688];
			}
			if( j==1000 )
				printf("%f + j* %f\n",fbuf[2001],fbuf[2000]);
		}
		aSync.Synchronizer(fbuf);
		if( aSync.b_SyncStatus )
			printf("%2d Sync off=%d \n",i/4096,aSync.i_RelativeOffset);
		printf("%2d No Sync max=%f avg=%f %f\n",i/4096,aSync.f_MaxAbs2,aSync.f_AveAbs2,
			aSync.f_MaxAbs2/aSync.f_AveAbs2);
	}
	
}
