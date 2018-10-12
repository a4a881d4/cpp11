#include<xmmintrin.h>


#ifdef DEBUG
#include"mmdebug.h"
#endif



void _mm_sumRow( void *inI, void *out, int Length )
{
	__m128i m0,m1,m2,m3,m4,m5,m6,m7,one,*ibuf,*obuf;
	
	int i;
	ibuf=(__m128i *)inI;
	obuf=(__m128i *)out;
	one=_mm_set_epi16(1,1,1,1,1,1,1,1);
	for( i=0;i<Length;i+=8 )
	{
		m0=_mm_load_si128(ibuf);
		m0=_mm_madd_epi16(m0,one);

#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		
		ibuf++;
		m1=_mm_load_si128(ibuf);
		m1=_mm_madd_epi16(m1,one);
		m0=_mm_packs_epi32(m0,m1);
#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		m0=_mm_madd_epi16(m0,one);
#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		ibuf++;
		m2=_mm_load_si128(ibuf);
		m2=_mm_madd_epi16(m2,one);
		ibuf++;
		m3=_mm_load_si128(ibuf);
		m3=_mm_madd_epi16(m3,one);
		m2=_mm_packs_epi32(m2,m3);
		m2=_mm_madd_epi16(m2,one);
		m0=_mm_packs_epi32(m0,m2);
#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		m0=_mm_madd_epi16(m0,one);
#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		
		ibuf++;
		m4=_mm_load_si128(ibuf);
		m4=_mm_madd_epi16(m4,one);
		ibuf++;
		m5=_mm_load_si128(ibuf);
		m5=_mm_madd_epi16(m5,one);
		m4=_mm_packs_epi32(m4,m5);
		m4=_mm_madd_epi16(m4,one);
		ibuf++;
		m6=_mm_load_si128(ibuf);
		m6=_mm_madd_epi16(m6,one);
		ibuf++;
		m7=_mm_load_si128(ibuf);
		m7=_mm_madd_epi16(m7,one);
		m6=_mm_packs_epi32(m6,m7);
		m6=_mm_madd_epi16(m6,one);
		m4=_mm_packs_epi32(m4,m6);
		m4=_mm_madd_epi16(m4,one);
		m0=_mm_packs_epi32(m0,m4);
#ifdef DEBUG
		_mm_printf(m0,EPI16);
#endif		
		_mm_store_si128(obuf,m0);
		ibuf++;
		obuf++;
	}
		
}
