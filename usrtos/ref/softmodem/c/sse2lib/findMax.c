#include<xmmintrin.h>

#ifdef DEBUG
#include"mmdebug.h"
#endif



void _mm_findMax( void *inI, int Length, int *imax, int *avg )
{
	__m128i id,max,sum,*ibuf;
	__m128i ones;

	int i;
	short *sp;
	int *ip;
	ibuf=(__m128i *)inI;
	max=_mm_xor_si128(max,max);
	sum=_mm_xor_si128(sum,sum);
	ones=_mm_set_epi16(1,1,1,1,1,1,1,1);

	for( i=0;i<Length;i++ )
	{
		id=_mm_load_si128(ibuf);
		max=_mm_max_epi16(max,id);
		id=_mm_madd_epi16(id,ones);
		sum=_mm_add_epi32(sum,id);
		ibuf++;
	}
	sp=(short *)&max;
	*imax=0;
	*avg=0;
	for( i=0;i<8;i++ )
	{
		if( sp[i]>*imax )
			*imax=sp[i];
	}
	ip=(int *)&sum;
	for( i=0;i<4;i++ )
	{
		*avg+=(int)ip[i];
		
	}
	*avg/=8;
	*avg/=Length;
}
