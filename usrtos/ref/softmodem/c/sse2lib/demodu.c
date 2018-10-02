#include<xmmintrin.h>
#include"channelEst.h"

#ifdef DEBUG
#include"mmdebug.h"
#endif



void _mm_demodu( void *inI, void *inQ, void *chI, void *chQ, void *out, int coePos[],int coeNum, int Length )
{
	__m128i id,qd,ic,qc,oi,tempo,oq,*ibuf,*qbuf,*ich,*qch,*obuf,*ipos,*qpos;
	
	int i,j;
	ibuf=(__m128i *)inI;
	qbuf=(__m128i *)inQ;
	obuf=(__m128i *)out;
	
	for( i=0;i<Length;i++ )
	{
		
		ich=(__m128i *)chI;
		qch=(__m128i *)chQ;
		oi=_mm_xor_si128(oi,oi);
		oq=_mm_xor_si128(oq,oq);
		
		for( j=0;j<coeNum;j++ )
		{
			ipos=ibuf+coePos[j];
			qpos=qbuf+coePos[j];
			id=_mm_load_si128(ipos);
			qd=_mm_load_si128(qpos);
			ic=_mm_load_si128(ich);
			qc=_mm_load_si128(qch);
			tempo=_mm_mullo_epi16(id,ic);
			oi=_mm_adds_epi16(oi,tempo);
			tempo=_mm_mullo_epi16(qd,qc);
			oi=_mm_adds_epi16(oi,tempo);
			tempo=_mm_mullo_epi16(id,qc);
			oq=_mm_subs_epi16(oq,tempo);
			tempo=_mm_mullo_epi16(qd,ic);
			oq=_mm_adds_epi16(oq,tempo);
			ich++;
			qch++;
		}
		
		_mm_store_si128(obuf,oi);
		obuf++;
		_mm_store_si128(obuf,oq);
		obuf++;	
		ibuf+=2;
		qbuf+=2;
		
	}
		
}
