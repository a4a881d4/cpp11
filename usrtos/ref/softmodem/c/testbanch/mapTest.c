#include<stdio.h>
#include<stdlib.h>

#include<sse2Map.h>

void TurboEncode( void *in, void *out, int Length );

void _mm_Map( void *lX, void *tY, void *tX );

short X[CODELENGTH*8] __attribute__((aligned(16)));
short Y[CODELENGTH*8] __attribute__((aligned(16)));
short tX[CODELENGTH*8] __attribute__((aligned(16)));

int main()
{

	int i,j,max,avg;
	
	char *input = malloc( CODELENGTH/8+2 );
	char *output = malloc( CODELENGTH/8+2 );
	
	short lx,ly;
	
	for( i=0;i<CODELENGTH/8+2;i++ )
		input[i]=i&0xff;
	
	TurboEncode( input, output, CODELENGTH-3 );
	lx=0;
	ly=0;
	for( i=0;i<CODELENGTH;i++ )
	{
		lx=input[i/8];
		lx>>=(7-i%8);
		lx&=1;
		X[i*8]=2*lx-1;
		ly=output[i/8];
		ly>>=(7-i%8);
		ly&=1;
		Y[i*8]=2*ly-1;
		
	}
	
	_mm_Map( X,Y,tX );
	
	for( i=0;i<CODELENGTH;i++ )
	{
		printf(" %d %d \n",X[i*8],tX[i*8]);	
	}
	

}
