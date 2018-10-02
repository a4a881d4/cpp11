#include<sse2Map.h>
#include<channelEst.h>

#undef DEBUG

#ifdef DEBUG
#include<stdio.h>
#endif

short _mm_dout[4096*8] __attribute__((aligned(16)));

short _mm_X[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y1[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Y2[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_OUT[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_T2[3*8] __attribute__((aligned(16)));


RET_CHANNEL_ESTIMATE_t channel;

extern  void channelEst(void *inI, void *inQ, RET_CHANNEL_ESTIMATE_t *res );
extern  void _mm_demodu( void *inI, void *inQ, void *chI, void *chQ, void *out, int coePos[],int coeNum, int Length );
extern  void Descrambler( void *in, void *code, int Length );
extern  void PreDecode(void *demodu_data, void *x, void *y1, void *y2, void *t2);
extern  int Decode( void *x, void *t2, void *y1, void *y2, void *out );



int Receiver( void *inI, void *inQ, void *out, char alpha, char beta, unsigned char *code )
{
	int ret;
	int i,j,k;
	char *pOut;
	
	channelEst( inI, inQ, &channel );
	
#ifdef DEBUG	
	printf("I find %d path\n",channel.iTapNum);
	for(j=0;j<8;j++ )
	for( i=0;i<channel.iTapNum;i++ )
	{
		printf(" The %d blk %dth pos is %d i=%d q=%d\n",j,i,channel.pos[i],(int)channel.pITap[i*8+j],(int)channel.pQTap[i*8+j]);
	}
#endif
	
	_mm_demodu( inI, inQ,(void *)channel.pITap,(void *)channel.pQTap, (void *)_mm_dout, channel.pos,channel.iTapNum, 2048 );
	
	Descrambler( _mm_dout, code, 2048 );

#ifdef DEBUG	
	
	for( i=0;i<100;i+=3 )
		printf(" %d",(int)_mm_dout[i*8+1]);

#endif
		
	PreDecode( _mm_dout, _mm_X, _mm_Y1, _mm_Y2, _mm_T2 );
		
	ret=Decode( _mm_X,_mm_T2,_mm_Y1,_mm_Y2,_mm_OUT );
	
	pOut=(char *)out;
	
	for( i=0;i<8;i++ )
	{
		for( j=0;j<CODELENGTH/8;j++ )
		{
			for( k=0;k<8;k++ )
			{
				pOut[i*(CODELENGTH/8)+j]<<=1;
				pOut[i*(CODELENGTH/8)+j]|=(~(_mm_OUT[(j*8+k)*8+i]>>15))&1;
			}
		}
	}
	
	return ret;
	
}
