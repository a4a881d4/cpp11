#include"sse2Map.h"


extern unsigned short InterleaverTable[CODELENGTH-3];
extern unsigned short CRCTable[(CODELENGTH-3)*8];
extern unsigned short TurboEncodeTable[256];

#define CODEPAD 16

void Encode( void *in, void *out, int blknum )
{
	int i,j,k;
	int l8 = (blknum*(CODELENGTH-3)-16)/8;
	
	unsigned char *pIn = ( unsigned char *)in;
	char *pOut=( char *) out;
	unsigned short state=0;
	unsigned char c1,e1,d1,ly1,c2,e2,d2,ly2;
	unsigned short s1,s2;
	unsigned short *crc;
	short pos;
	
	crc=CRCTable+((CODELENGTH-3)*(8-blknum));
	
	for( i=0;i<l8;i++ )
	{
		c1=pIn[i];
		for( j=0;j<8;j++ )
		{
			if( (c1&0x80)!=0 )
				state^=CRCTable[i*8+j];
			c1<<=1;
		}
	}
	state^=0xffff;
	pIn[l8]=(state>>8)&0xff;
	pIn[l8+1]=state&0xff;
	for( k=0;k<blknum;k++ )
	{
		l8=(CODELENGTH-3)/8;
		d1=d2=0;
		ly1=ly2=0;
		for( i=0;i<l8;i++ )
		{
			c1=pIn[i];
			for( j=0;j<8;j++ )
			{
				c2<<=1;
				pos=InterleaverTable[i*8+j];
				e2=(pIn[pos/8]>>(7-pos%8))&1;
				pOut[pos*3]=e2;
				c2|=e2;
			}
			d1^=c1;
			s1=TurboEncodeTable[d1&0xff];
			d1=(s1>>8);
			e1=(s1&0xff)^ly1;
			ly1=(d1&0x7)<<5;
			d1&=0xe0;
			for( j=0;j<8;j++ )
			{
				pOut[(i*8+j)*3+1]=(e1>>(7-j))&1;
			}	
			
			d2^=c2;
			s2=TurboEncodeTable[d2&0xff];
			d2=(s2>>8);
			e2=(s2&0xff)^ly2;
			ly2=(d2&0x7)<<5;
			d2&=0xe0;
			
			for( j=0;j<8;j++ )
			{
				pOut[(i*8+j)*3+2]=(e2>>(7-j))&1;
			}	
		}
		for( j=0;j<3;j++ )
		{
			pOut[l8*8*3+j*2]=(d1>>(7-j))&1;
			pOut[l8*8*3+6+j*2]=(d2>>(7-j))&1;
			pOut[l8*8*3+j*2+1]=(ly1>>(7-j))&1;
			pOut[l8*8*3+6+j*2+1]=(ly2>>(7-j))&1;
		}
		pIn+=l8;
		for( j=0;j<CODEPAD;j++ )
		{
			pOut[CODELENGTH*3+12+j]=0;
		}
		pOut+=(CODELENGTH-3)*3+12+CODEPAD;
	}
	
			
		
	
}
