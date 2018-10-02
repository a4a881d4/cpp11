#include<xmmintrin.h>

#ifdef DEBUG
#include"mmdebug.h"
#endif



void _mm_cabs( void *inI, void *inQ, void *out, int Length )
{
	__m128i zero,one,two,iabs,qabs,id,qd,od,min,max,*ibuf,*qbuf,*obuf;
	
	int i,j;
	one=_mm_set_epi16(1,1,1,1,1,1,1,1);
	two=_mm_set_epi16(2,2,2,2,2,2,2,2);
	ibuf=(__m128i *)inI;
	qbuf=(__m128i *)inQ;
	obuf=(__m128i *)out;
	for( i=0;i<Length;i++ )
	{
		zero=_mm_xor_si128(zero,zero);
		id=_mm_load_si128(ibuf);
		zero=_mm_cmpgt_epi16(zero,id);
		zero=_mm_mullo_epi16(zero,two);
		zero=_mm_add_epi16(one,zero);
		iabs=_mm_mullo_epi16(zero,id);

		zero=_mm_xor_si128(zero,zero);

		qd=_mm_load_si128(qbuf);

		zero=_mm_cmpgt_epi16(zero,qd);

		zero=_mm_mullo_epi16(zero,two);

		zero=_mm_add_epi16(one,zero);

		qabs=_mm_mullo_epi16(zero,qd);

		od=_mm_max_epi16(iabs,qabs);
		min=_mm_min_epi16(iabs,qabs);

		od=_mm_add_epi16(od,od);
		od=_mm_add_epi16(od,min);
		_mm_store_si128(obuf,od);

		ibuf++;
		qbuf++;
		obuf++;
	}
		
		
}
