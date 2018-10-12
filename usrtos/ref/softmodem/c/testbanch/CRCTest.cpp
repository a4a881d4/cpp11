#include<stdio.h>
#include<sse2Map.h>



extern "C" void Encode( void *in, void *x, void *y1, void *y2, void *t1, void *t2, int Length );
extern unsigned short CRCTable[1360];

int main()
{

	char *input = new char[ CODELENGTH/8+2 ];
	char *interlinput = new char[ CODELENGTH/8+2 ];
	
	short *output = new short[ CODELENGTH ];
	short *y1= new short[ CODELENGTH ];
	short *y2= new short[ CODELENGTH ];
	short t1[3];
	short t2[3];
	
	int i,j,a,s0,y,t,s1,lt;
	
	int lx,ly;
	
	for( i=0;i<CODELENGTH/8;i++ )
		input[i]=(i^0xf0)&0xff;
	
	Encode( input, output, y1, y2, t1, t2, CODELENGTH-3 );
	
	unsigned short state=0;
	
	for( i=0;i<16;i++ )
		printf("%d ",output[i]);
	printf("\n");
	
	for( i=0;i<CODELENGTH-19;i++ )
	{
		if( output[i]!=0 )
			state^=CRCTable[i];
	}
	input[CODELENGTH/8]=state>>8;
	input[CODELENGTH/8+1]=state&0xff;
	
	for( ;i<CODELENGTH-3;i++ )
	{
		if( output[i]==0 )
			state^=CRCTable[i];
	}
	
	printf(" CRC = %x \n",(int)state);
	
}