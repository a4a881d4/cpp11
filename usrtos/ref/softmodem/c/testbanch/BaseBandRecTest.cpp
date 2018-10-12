#include<stdio.h>
#include<sse2Map.h>
#include<channelEst.h>



short _mm_inI[INPUTLENGTH*8] __attribute__((aligned(16)));
short _mm_inQ[INPUTLENGTH*8] __attribute__((aligned(16)));
short _mm_dout[4096*8] __attribute__((aligned(16)));

short _mm_X[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y1[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y2[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_OUT[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_T2[3*8] __attribute__((aligned(16)));

unsigned char pnCode[512];

extern "C" void channelEst(void *inI, void *inQ, RET_CHANNEL_ESTIMATE_t *res );
extern "C" void _mm_demodu( void *inI, void *inQ, void *chI, void *chQ, void *out, int coePos[],int coeNum, int Length );
extern "C" void Descrambler( void *in, void *code, int Length );
extern "C" void genCode(void *codebuf, int uPNnumber);
extern "C" void PreDecode(void *demodu_data, void *x, void *y1, void *y2, void *t2);
extern "C" int Decode( void *x, void *t2, void *y1, void *y2, void *out );


RET_CHANNEL_ESTIMATE_t channel;



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

	int i,j,l,a,s0,y,t,s1,lt,k;
	
	int lx,ly,ret;
	
	FILE *fpi = fopen("dat//TxDataI.txt","rt");
	FILE *fpq = fopen("dat//TxDataQ.txt","rt");
	
	for( i=0;i<13800;i++ )
	{
		fscanf(fpi,"%x",&a);
		fscanf(fpq,"%x",&a);
		
	}
	
	for( i=0;i<INPUTLENGTH;i++ )
	{
		for( j=0;j<4;j++ )
			fscanf(fpi,"%x",&a);
		if(a>128)
			a-=256;
		_mm_inI[i*8]=(short)(a+4)/8;
		for( j=0;j<4;j++ )
			fscanf(fpq,"%x",&a);
		if(a>128)
			a-=256;
		_mm_inQ[i*8]=(short)(a+4)/8;
		
		if( (i&0xff)==0xff )
			printf("%d \n",a);
		
	}
	for( l=1;l<8;l++ )
	{
		for( i=CHANNELLENGTH;i<INPUTLENGTH;i++ )
		{
			for( j=0;j<4;j++ )
				fscanf(fpi,"%x",&a);
			if(a>128)
				a-=256;
			_mm_inI[i*8+l]=(short)(a+4)/8;
			for( j=0;j<4;j++ )
				fscanf(fpq,"%x",&a);
			if(a>128)
				a-=256;
			_mm_inQ[i*8+l]=(short)(a+4)/8;
			
			if( (i&0xff)==0xff )
				printf("%d \n",a);
			
		}
	}
	for( l=1;l<8;l++ )
	{
		for( i=0;i<CHANNELLENGTH;i++ )
		{
			_mm_inI[i*8+l]=_mm_inI[(i+SLOTLENGTH)*8+l-1];
			
			_mm_inQ[i*8+l]=_mm_inQ[(i+SLOTLENGTH)*8+l-1];
			
			
		}
	}
	
	
	printf("Read file OK\n");
	fclose(fpi);
	fclose(fpq);
	
	genCode(pnCode, 0);
	
	printf("Gen PN code OK \n");

	
	printf("I find %d path\n",channel.iTapNum);
	
	for( i=0;i<channel.iTapNum;i++ )
	{
		printf(" The %dth pos is %d i=%d q=%d\n",i,channel.pos[i],(int)channel.pITap[i*8],(int)channel.pQTap[i*8]);
	}
	
	for( i=0;i<10000000;i++ )
	{
		channelEst(_mm_inI, _mm_inQ, &channel );
		
		_mm_demodu( _mm_inI,_mm_inQ,(void *)channel.pITap,(void *)channel.pQTap, (void *)_mm_dout, channel.pos,channel.iTapNum, 2048 );
		
		Descrambler( _mm_dout, pnCode, 2048 );
		
		PreDecode(_mm_dout, _mm_X, _mm_Y1, _mm_Y2, _mm_T2);
		
		ret=Decode(_mm_X,_mm_T2,_mm_Y1,_mm_Y2,_mm_OUT );
		
		if( (i%100000)==0 )
			printf("%d \n",i);
			
		
	}
	
	printf("ret=%d \n",ret);

	for( i=0;i<10;i++ )
	{
		printf("%d ",(int)_mm_X[i*8]);
	}

	printf("\n");

	for( i=0;i<13;i++ )
		MSEQ[i]=0;
	MSEQ[0]=0;	

	for( j=0;j<8;j++ )
	{
		for( i=0;i<1360;i+=1 )
		{
			if(_mm_OUT[i*8+j]>0 )		
				printf("%d",MCheck(1));
			else
				printf("%d",MCheck(0));
			if( ((i)&0x3f)==0x3f )
				printf("\n");
		}
	
		printf("\n");
		
	}		
}
