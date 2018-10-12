#include"State.h"
#include<stdio.h>

unsigned short table[256];

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

	int i,j,a,s0,y,t,s1,lt,ly;
	for( i=0;i<256;i++ )
	{
		a=i;
		s0=0;
		y=0;
		t=0;
		ly=0;
		for( j=0;j<8;j++ )
		{
			y<<=1;
			a<<=1;
			if( (a&0x100)==0 )
			{
				y+=Trellis[s0].NextBranch0;
				s0=Trellis[s0].NextState0;
			}
			else
			{
				y+=Trellis[s0].NextBranch1;
				s0=Trellis[s0].NextState1;
			}
		}
		for( j=0;j<3;j++ )
		{
			t<<=1;
			ly<<=1;
			t+=Trellis[s0].Terminal;
			if( Trellis[s0].Terminal==0 )
			{
				ly+=Trellis[s0].NextBranch0;
				
				s0=Trellis[s0].NextState0;
			}
			else
			{
				ly+=Trellis[s0].NextBranch1;
				s0=Trellis[s0].NextState1;
			}
		}
		t<<=13;
		t|=y;
		t|=ly<<8;
		table[i]=t;

	
	}
	printf("unsigned short TurboEncodeTable[256]={\n");
	printf("\t");
	for( i=0;i<255;i++ )
	{
		printf("0x%04.4x, ",(int)table[i]);
		if( (i%16) == 15 )
			printf("\n\t");
	}
	printf("0x%04.4x \n};\n",(int)table[i]);
	
	printf("//");

/*
	
	for( i=0;i<8;i++ )
	{
		for( s0=0;s0<8;s0++ )
		{
			s1=s0;
			for( j=0;j<i;j++ )
			{
				s1=Trellis[s1].NextState0;
			}
			lt=0;
			for( j=0;j<3;j++ )
			{
				lt<<=1;
				lt+=Trellis[s1].Terminal;
				if( Trellis[s1].Terminal==0 )
				{
					s1=Trellis[s1].NextState0;
				}
				else
				{
					s1=Trellis[s1].NextState1;
				}
			}
			t=0;
			y=0;
			s1=s0;
			for( j=0;j<3;j++ )
			{
				t<<=1;
				y<<=1;
				t+=Trellis[s1].Terminal;
				if( Trellis[s1].Terminal==0 )
				{
					y+=Trellis[s1].NextBranch0;
					s1=Trellis[s1].NextState0;
				}
				else
				{
					y+=Trellis[s1].NextBranch1;
					s1=Trellis[s1].NextState1;
				}
			}
			table[i*8+lt]=y*16+t;
		}
	
	}
	printf("unsigned char TurboEncodeTerminalTable[64]={\n");
	printf("\t");
	for( i=0;i<63;i++ )
	{
		printf("0x%02.2x, ",(int)table[i]&0xff);
		if( (i%16) == 15 )
			printf("\n\t");
	}
	printf("0x%02.2x \n};\n// ",(int)table[i]&0xff);
*/	
	
}