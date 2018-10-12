#include<stdio.h>
#include<sse2Map.h>
#include<channelEst.h>
#include<constructSSE2Buf.h>



unsigned char pnCode[512];

extern "C" void Transmiter( void *in, void *out, int blknum, char alpha, char beta, unsigned char *code );
extern "C" int Receiver( void *inI, void *inQ, void *out, char alpha, char beta, unsigned char *code );
extern "C" void constructSSE2Buf( RECEIVER_BUF_IND_t *buf );
extern "C" void genCode(void *codebuf, int uPNnumber);
extern "C" void _srv_Stream_init();
extern "C" void _srv_Stream_rx( unsigned char *block, int length );
extern "C" void _srv_Stream_tx( unsigned char *block, int length );
extern "C" void _mmap_init();


RECEIVER_BUF_IND_t aBuf;

int MSEQ[13];
int MCheck( int a )
{
	int b;
	int i;
	b=a;
	b^=MSEQ[0]^MSEQ[1]^MSEQ[3]^MSEQ[4];
	for( i=0;i<12;i++ )
		MSEQ[i]=MSEQ[i+1];
	MSEQ[12]=a;
	return b;
}

int main()
{

	int i,ret;
	int j;
	genCode(pnCode, 0);
	
	printf("Gen PN code OK \n");
	
//	_mmap_init();
//	_srv_Stream_init();
	
	char *testdata=new char[1363];
	char *recout=new char[1363];
	
	for( i=0;i<1358;i++ )
		testdata[i]=i&0xff;
	
	char *moduout=new char[2054*2*8];
	
	char *recBuf=new char[2100*2*8*2];
	for( j=0;j<10000000;j++ )
	{
		//_srv_Stream_tx((unsigned char*)testdata,1358);
		
		Transmiter(testdata,moduout,8,10,5,pnCode);
		
			
		
		for( i=0;i<2054*2*8;i+=2 )
		{
			recBuf[2*i]=moduout[i];
			recBuf[2*i+1]=moduout[i+1];
			recBuf[2*i+2]=0;
			recBuf[2*i+3]=0;
		}
		aBuf.blkNum=1;
		aBuf.length[0]=2100*2*8*2;
		aBuf.point[0]=recBuf;
		constructSSE2Buf(&aBuf);
		
		
		ret=Receiver(aBuf.buf_I,aBuf.buf_Q,recout,10,5,pnCode);
		
		if( ret==0 )
			printf("decode ok\n");
		else
			printf("decode error\n");
			
		if( (j%1000)==0 )
			printf("%d\n",j);
		
	}
	
}
