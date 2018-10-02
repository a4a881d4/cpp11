#include<xmmintrin.h>
#include"channelEst.h"


#ifdef DEBUG
#include<stdio.h>
#endif

short _mm_IChannelBuf[CHANNELLENGTH*8] __attribute__((aligned(16)));
short _mm_QChannelBuf[CHANNELLENGTH*8] __attribute__((aligned(16)));

short _mm_AbsChannelBuf[CHANNELLENGTH*8] __attribute__((aligned(16)));
short _mm_AbsSumBuf[CHANNELLENGTH] __attribute__((aligned(16)));

short _mm_IRakeTap[MAX_RAKE_TAP*8] __attribute__((aligned(16)));
short _mm_QRakeTap[MAX_RAKE_TAP*8] __attribute__((aligned(16)));

void channelEst(void *inI, void *inQ, RET_CHANNEL_ESTIMATE_t *res )
{
	
	int i,j,chip,counter;
	int max,avg;
	double th;
	short ith;
	char *pinI,*pinQ,*pchI,*pchQ;
	
	__m128i *_mm_pchI,*_mm_pchQ,*_mm_RakeI,*_mm_RakeQ,*_mm_pTemp;
	__m128i id;
	
	pinI = ( char * ) inI;
	pinQ = ( char * ) inQ;
	pchI = ( char * ) _mm_IChannelBuf;
	pchQ = ( char * ) _mm_QChannelBuf;
	 
	for( i=0;i<(PSCLENGTH-1)*2;i++ )
	{
		GHGS((void *)pinI,(void *)pinQ,(void *)pchI,(void *)pchQ );
		pinI+=16;
		pinQ+=16;
	}
	
	for( i=0;i<CHANNELLENGTH;i++ )
	{
		GHGS((void *)pinI,(void *)pinQ,(void *)pchI,(void *)pchQ );
		pinI+=16;
		pinQ+=16;
		pchI+=16;
		pchQ+=16;
	}
	
	_mm_cabs( (void *)_mm_IChannelBuf,(void *)_mm_QChannelBuf, (void *)_mm_AbsChannelBuf,CHANNELLENGTH );
	_mm_sumRow( (void *)_mm_AbsChannelBuf, (void *)_mm_AbsSumBuf,CHANNELLENGTH );
	_mm_findMax((void*)_mm_AbsSumBuf,CHANNELLENGTH/8,&max,&avg);	
	th=(double)max*PEAK_TH;
	if( th<(double)avg*NOISE_TH )
		th=(double)avg*NOISE_TH;
	ith=(short)(th+0.5);
	
	counter=0;
	chip=0;
	
	_mm_pchI = ( __m128i * ) _mm_IChannelBuf;
	_mm_pchQ = ( __m128i * ) _mm_QChannelBuf;
	_mm_RakeI = ( __m128i * ) _mm_IRakeTap;
	_mm_RakeQ = ( __m128i * ) _mm_QRakeTap;
#ifdef DEBUG
	for( chip=0;chip<CHANNELLENGTH;chip++ )
	{	
		printf(" %4.4d|%4.4d ",chip,(int)_mm_AbsSumBuf[chip]);
		if( (chip&0x7)==0x7 )
			printf("\n");
	}
	printf("Channel Estiminat th is %d max is %d agv is %d\n",(int)ith,(int)max,(int)avg);

#endif	
	for( chip=0;chip<CHANNELLENGTH;chip++ )
	{
		if( _mm_AbsSumBuf[chip]>ith )
		{
			res->pos[counter]=chip;
			_mm_pTemp = _mm_pchI+chip;
			id=_mm_load_si128(_mm_pTemp);
			id=_mm_srai_epi16(id,CHANNELSHIFT);
			_mm_store_si128(_mm_RakeI,id);
			_mm_RakeI++;
			_mm_pTemp = _mm_pchQ+chip;
			id=_mm_load_si128(_mm_pTemp);
			id=_mm_srai_epi16(id,CHANNELSHIFT);
			_mm_store_si128(_mm_RakeQ,id);
			_mm_RakeQ++;
			counter++;
			if( counter>=MAX_RAKE_TAP )
				break;
		}
	}
	res->iTapNum=counter;
	res->pITap=_mm_IRakeTap;
	res->pQTap=_mm_QRakeTap;
	
}
