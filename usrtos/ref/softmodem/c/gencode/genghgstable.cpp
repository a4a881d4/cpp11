#include<stdio.h>

short _mm_inI[8] __attribute__((aligned(16)));
short _mm_inQ[8] __attribute__((aligned(16)));

short _mm_outI[8] __attribute__((aligned(16)));
short _mm_outQ[8] __attribute__((aligned(16)));

extern "C" void GHGS( void *inI, void *inQ, void *outI, void *outQ );

int main()
{

	int i,j,a,s0,y,t,s1,lt,k;
	
	int lx,ly,ret;
	
	short *out = new short[4096];
	
	for( i=0;i<8;i++ )
	{
		_mm_inI[i]=0;
		_mm_inQ[i]=0;
	}
/*	
	for( i=0;i<4096;i++ )
	{
		GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
	}
*/	
		
	
	_mm_inI[0]=2048;
	
	GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
	
	out[0]=_mm_outI[0];
	out[1]=_mm_outQ[0];
	
	
	_mm_inI[0]=0;
	GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
	
	for( i=1;i<2048;i++)
	{
		GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
		out[i*2]=_mm_outI[0];
		out[i*2+1]=_mm_outQ[0];
		GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
	
	}
	
	for( i=0;i<4096;i++ )
	{
		if( out[i]>0 )
			out[i]=1;
		if( out[i]<0 )
			out[i]=-1;
	}

	
	printf("char GHGSTable[4096]={\n");
	printf("\t");
	for( i=2047;i>0;i-- )
	{
		printf(" %d, %d,",(int)out[i*2],0-(int)out[i*2+1]);
		if( (i%16) == 0 )
			printf("\n\t");
	}
	printf(" %d, %d \n};\n",(int)out[i*2],0-(int)out[i*2+1]);
	
	printf("//");

	
}
