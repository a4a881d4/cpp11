#include<xmmintrin.h>
#include"sse2Map.h"
#include"channelEst.h"


void PreDecode(void *demodu_data, void *x, void *y1, void *y2, void *t2)
{
	__m128i *data, *px, *py1, *py2, *pt2;
	__m128i a,nones,offset;
	
	int i;
	
	nones=_mm_set_epi16(-1,-1,-1,-1,-1,-1,-1,-1);
	
	offset = _mm_set_epi16(DEMODUOFFSET,DEMODUOFFSET,DEMODUOFFSET,DEMODUOFFSET,
			DEMODUOFFSET,DEMODUOFFSET,DEMODUOFFSET,DEMODUOFFSET);	

	data=(__m128i *)demodu_data;
	px=(__m128i *)x;
	py1=(__m128i *)y1;
	py2=(__m128i *)y2;
	pt2=(__m128i *)t2;
	
	for( i=0;i<CODELENGTH-3;i++ )
	{
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(px,a);
		px++;
		data++;
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(py1,a);
		py1++;
		data++;
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(py2,a);
		py2++;
		data++;
	}
	
	for( i=0;i<3;i++ )
	{
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(px,a);
		px++;
		data++;
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(py1,a);
		py1++;
		data++;
	}
	for( i=0;i<3;i++ )
	{
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(pt2,a);
		pt2++;
		data++;
		a=_mm_load_si128(data);
		a=_mm_adds_epi16(a,offset);
		a=_mm_srai_epi16(a,DEMODUSHIFT);
		a=_mm_mullo_epi16(a,nones);
		_mm_store_si128(py2,a);
		py2++;
		data++;
	}
}
