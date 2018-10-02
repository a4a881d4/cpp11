#include<sse2Map.h>
#include<stdio.h>

extern "C" void Encode( void *in, void *out, int blknum );

short _mm_dout[4096*8] __attribute__((aligned(16)));
short _mm_X[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y1[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y2[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_OUT[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_T2[3*8] __attribute__((aligned(16)));

extern "C" void PreDecode(void *demodu_data, void *x, void *y1, void *y2, void *t2);
extern "C" int Decode( void *x, void *t2, void *y1, void *y2, void *out );

int main()
{

	FILE *fp=fopen("dat\\modudata.txt","rt");
	unsigned char *pIn=new unsigned char [1400];
	int a;
	int i,j,l;
	int ret;
	for( i=0;i<1360/8;i++ )
	{
		for( l=0;l<8;l++ )
		{
			for( j=0;j<8;j++ )
			{
				pIn[i+j*(1360/8)]<<=1;
				fscanf(fp,"%d",&a);
				pIn[i+j*(1360/8)]|=(a&1);
			}
		}
	}
	printf("read file OK\n");
	/*
	for( i=0;i<100;i++ )
	{
		for( j=0;j<8;j++ )
			printf(" %d",(int)(pIn[i]>>(7-j))&1);
		printf("\n");
	}
	*/
	short *pOut=new short[2054*8*2];
	
	Encode(pIn,pOut,8);
	fclose(fp);
	
	FILE *fpo=fopen("dat\\encodeout.txt","wt");
	
	for( i=0;i<4092;i++ )
	{
		fprintf(fpo," %d",(int)pOut[i]);
		if( (i%32)==31 )
			fprintf(fpo,"\n");
		for( j=0;j<8;j++ )
		{
			_mm_dout[i*8+j]=256-pOut[i+j*(2054*2)]*512;
			
		}
	}
	
	PreDecode(_mm_dout, _mm_X, _mm_Y1, _mm_Y2, _mm_T2);
		
	ret=Decode(_mm_X,_mm_T2,_mm_Y1,_mm_Y2,_mm_OUT );
	
	printf("Decode result = %d\n",ret);
	
	fprintf(fpo,"\n");
	
	for( i=0;i<1360;i++ )
	{
		for( j=0;j<8;j++ )
		{
			if(_mm_OUT[i*8+j]>0 )		
				fprintf(fpo," %d",1);
			else
				fprintf(fpo," %d",0);
			
		}
	
		fprintf(fpo,"\n");
		
	}			
	
	
}