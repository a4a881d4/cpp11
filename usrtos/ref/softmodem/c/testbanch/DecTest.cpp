#include<stdio.h>
#include<sse2Map.h>



extern "C" void Encode( void *in, void *x, void *y1, void *y2, void *t1, void *t2, int Length );
extern "C" int Decode( void *x, void *t2, void *y1, void *y2, void *out );

short X[CODELENGTH*8] __attribute__((aligned(16)));
short Y1[CODELENGTH*8] __attribute__((aligned(16)));
short Y2[CODELENGTH*8] __attribute__((aligned(16)));
short OUT[CODELENGTH*8] __attribute__((aligned(16)));
short T2[3*8] __attribute__((aligned(16)));

int main()
{

	char *input = new char[ CODELENGTH/8+2 ];
	
	short *output = new short[ CODELENGTH ];
	
	short *y1= new short[ CODELENGTH ];
	short *y2= new short[ CODELENGTH ];
	short t1[3];
	short t2[3];
	
	int i,j,a,s0,y,t,s1,lt,k;
	
	int lx,ly,ret;
	
	for( i=0;i<CODELENGTH/8;i++ )
		input[i]=(i^0xf0)&0xff;
	
	Encode( input, output, y1, y2, t1, t2, CODELENGTH-3 );
	
	for( i=0;i<CODELENGTH-3;i++ )
	{
		for( j=0;j<8;j++ )
		{
			X[i*8+j]=output[i]*2-1;
			Y1[i*8+j]=y1[i]*2-1;
			Y2[i*8+j]=y2[i]*2-1;
		}
	}
	for(i=0;i<3;i++ )
	{
		k=CODELENGTH+i-3;
		for( j=0;j<8;j++ )
		{
			
			X[k*8+j]=t1[i]*2-1;
			T2[i*8+j]=t2[i]*2-1;
			
			Y1[k*8+j]=y1[k]*2-1;
			Y2[k*8+j]=y2[k]*2-1;
		}
	}
	
	ret=Decode(X,T2,Y1,Y2,OUT );
	
}