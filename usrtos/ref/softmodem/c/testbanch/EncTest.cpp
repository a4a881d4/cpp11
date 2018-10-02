#include<State.h>
#include<stdio.h>
#include<sse2Map.h>



unsigned short table[256];
extern "C" void TurboEncode( void *in, void *out, int Length );

int main()
{

	int iDenominator = 0xd;
	int iNumerator = 0xb;
	int iStateDeep = 3;
	int iStateNumber = 8;
	int short_min=-32000;
	CState *Trellis;
	Trellis->genRSC(iDenominator,iNumerator);
	Trellis=Trellis->Trellis;
	
	char *input = new char[ CODELENGTH/8+2 ];
	char *output = new char[ CODELENGTH/8+2 ];
	
	int i,j,a,s0,y,t,s1,lt;
	
	int lx,ly;
	
	int dgen=iDenominator>>1;
	int ngen=iNumerator>>1;
	
	for( i=0;i<CODELENGTH/8+2;i++ )
		input[i]=(i^0xf0)&0xff;
	
	TurboEncode( input, output, CODELENGTH-3 );
	
	lx=0;
	ly=0;
	s0=0;
	
	int t1;
	
	s1=0;
	for( i=0;i<32;i++ )
	{
		lx=input[i/8];
		lx>>=(7-i%8);
		lx&=1;
		printf("X= %d ",(int)lx);
		
		if( lx==0 )
		{
			printf("Y= %d ",Trellis[s1].NextBranch0);
			s1=Trellis[s1].NextState0;
		}
		else
		{
			printf("Y= %d ",Trellis[s1].NextBranch1);
			s1=Trellis[s1].NextState1;
		}
		
//		X[i*8]=2*lx-1;
		
		t1=s0&dgen;
		for( j=0;j<3;j++ )
			lx^=(t1>>j)&0x1;
		ly=lx;
		t1=s0&ngen;
		for( j=0;j<3;j++ )
			ly^=(t1>>j)&0x1;
		printf("sY=%d ",ly);
		s0>>=1;
		s0|=lx<<2;
			
		
		ly=output[i/8];
		ly>>=(7-i%8);
		ly&=1;
		printf("tableY= %d\n",(int)ly);
//		Y[i*8]=2*ly-1;
		
	}
	
	
}