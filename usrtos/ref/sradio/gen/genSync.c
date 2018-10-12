#include<stdio.h>
#include<math.h>

#define QPSKLEN 116688
double d_QPSK[QPSKLEN*2];
#define RING 5185536
char c_QPSK[RING];

int main()
{
	FILE *fp=fopen("RecQPSKSig.dat","rb");
	fread(d_QPSK,sizeof(double),QPSKLEN*2,fp);
	fclose(fp);
	
	double max=0.;
	double avg=0.;
	int i,j;
	double c;
	for( i=0;i<QPSKLEN;i++ )
	{
		c=d_QPSK[i]*d_QPSK[i]+d_QPSK[i+QPSKLEN]*d_QPSK[i+QPSKLEN];
		if( max<c )
			max=c;
		avg+=c;
	}
	avg/=QPSKLEN;
	max=sqrt(max);
	avg=sqrt(avg);
	
	printf("max=%lf avg=%lf\n",max,avg);
	double factor=255./avg;
	for( i=0;i<QPSKLEN*2;i++ )
	{
		c=d_QPSK[i]*factor;
		if( c>127. )
			c=127.;
		if( c<-127. )
			c=-127.;
		d_QPSK[i]=c;
	}
	for( i=0;i<RING/4;i++ )
	{
		c_QPSK[2*i]=rand()%128-64;
		c_QPSK[2*i+1]=rand()%128-64;
	}
	for( i=RING/4;i<QPSKLEN+RING/4;i++ )
	{
		j=i-RING/4;
		c_QPSK[2*i+1]=(char)d_QPSK[QPSKLEN-1-j];
		c_QPSK[2*i]=(char)d_QPSK[2*QPSKLEN-j-1];
		//printf("%d + j*%d\n",c_QPSK[2*i],c_QPSK[2*i+1]);
	}
	for( i=QPSKLEN+RING/4;i<RING/2;i++ )
	{
		c_QPSK[2*i]=rand()%128-64;
		c_QPSK[2*i+1]=rand()%128-64;
	}
	fp=fopen("SyncSig.dat","wb");
	fwrite(c_QPSK,sizeof(char),RING,fp);
	fclose(fp);
	
	return 0;	
}
