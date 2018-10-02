#include<xmmintrin.h>
#include"sse2Map.h"
short _mm_Alpha[(CODELENGTH+4)*8*8] __attribute__((aligned(16)));
short _mm_Beta[(CODELENGTH+4)*8*8] __attribute__((aligned(16)));
void _mm_Map( void *lX, void *tY, void *tX )
{
	int i;
	__m128i *pAlpha=(__m128i *)_mm_Alpha;
	__m128i *pBeta=(__m128i *)_mm_Beta;
	__m128i *plX=(__m128i *)lX;
	__m128i *ptX=(__m128i *)tX;
	__m128i *ptY=(__m128i *)tY;

	__m128i zero,XplusY,XsubY,_mm_ls0,_mm_ls1,_mm_ls0_0,_mm_ls0_1,_mm_0,_mm_1;
// Init Alpha 
	zero=_mm_xor_si128(zero,zero);
	_mm_store_si128(pAlpha,zero);
	zero=_mm_set_epi16(-3200,-3200,-3200,-3200,-3200,-3200,-3200,-3200);
	_mm_store_si128(pAlpha+1,zero);
	_mm_store_si128(pAlpha+2,zero);
	_mm_store_si128(pAlpha+3,zero);
	_mm_store_si128(pAlpha+4,zero);
	_mm_store_si128(pAlpha+5,zero);
	_mm_store_si128(pAlpha+6,zero);
	_mm_store_si128(pAlpha+7,zero);
// Calculate Alpha 
	for( i=0;i<CODELENGTH;i++ )
	{
		XplusY=_mm_load_si128(plX);
		XsubY=_mm_subs_epi16(XplusY,*ptY);
		XplusY=_mm_adds_epi16(XplusY,*ptY);
//		Calculate State 0 4 to 0 1
//		State 0 Next0 is 0 Next1 is 1
//		State 0 Out0 is 0 Out1 is 1
//		State 4 Next0 is 1 Next1 is 0
//		State 4 Out0 is 0 Out1 is 1
//x=-1 y=-1
		_mm_ls0 = _mm_load_si128(pAlpha+0);
		_mm_ls1 = _mm_load_si128(pAlpha+4);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+8,_mm_ls0_0);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+9,_mm_ls0_0);
//		Calculate State 1 5 to 2 3
//		State 1 Next0 is 2 Next1 is 3
//		State 1 Out0 is 1 Out1 is 0
//		State 5 Next0 is 3 Next1 is 2
//		State 5 Out0 is 1 Out1 is 0
//x=-1 y=1
		_mm_ls0 = _mm_load_si128(pAlpha+1);
		_mm_ls1 = _mm_load_si128(pAlpha+5);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+10,_mm_ls0_0);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+11,_mm_ls0_0);
//		Calculate State 2 6 to 4 5
//		State 2 Next0 is 5 Next1 is 4
//		State 2 Out0 is 1 Out1 is 0
//		State 6 Next0 is 4 Next1 is 5
//		State 6 Out0 is 1 Out1 is 0
//x=1 y=-1
		_mm_ls0 = _mm_load_si128(pAlpha+2);
		_mm_ls1 = _mm_load_si128(pAlpha+6);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+12,_mm_ls0_0);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+13,_mm_ls0_0);
//		Calculate State 3 7 to 6 7
//		State 3 Next0 is 7 Next1 is 6
//		State 3 Out0 is 0 Out1 is 1
//		State 7 Next0 is 6 Next1 is 7
//		State 7 Out0 is 0 Out1 is 1
//x=1 y=1
		_mm_ls0 = _mm_load_si128(pAlpha+3);
		_mm_ls1 = _mm_load_si128(pAlpha+7);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+14,_mm_ls0_0);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pAlpha+15,_mm_ls0_0);
		pAlpha+=8;
		if((i&0xff)==0xff)
		{
			zero=_mm_load_si128(pAlpha);
			_mm_ls0 = _mm_load_si128(pAlpha+1);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+1,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+2);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+2,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+3);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+3,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+4);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+4,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+5);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+5,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+6);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+6,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pAlpha+7);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pAlpha+7,_mm_ls0);
			zero=_mm_xor_si128(zero,zero);
			_mm_store_si128(pAlpha,zero);
		}
		plX++;
		ptY++;
	}
// Init Beta 
	zero=_mm_xor_si128(zero,zero);
	_mm_store_si128(pBeta,zero);
	zero=_mm_set_epi16(-3200,-3200,-3200,-3200,-3200,-3200,-3200,-3200);
	_mm_store_si128(pBeta+1,zero);
	_mm_store_si128(pBeta+2,zero);
	_mm_store_si128(pBeta+3,zero);
	_mm_store_si128(pBeta+4,zero);
	_mm_store_si128(pBeta+5,zero);
	_mm_store_si128(pBeta+6,zero);
	_mm_store_si128(pBeta+7,zero);
// Calculate Beta 
	plX--;
	ptY--;
	for( i=0;i<CODELENGTH;i++ )
	{
		XplusY=_mm_load_si128(plX);
		XsubY=_mm_subs_epi16(XplusY,*ptY);
		XplusY=_mm_adds_epi16(XplusY,*ptY);
//		Calculate State 0 1 to 0 4
//		State 0 Last0 is 0 Last1 is 4
//		State 0 Out0 is 0 Out1 is 1
//		State 1 Last0 is 4 Last1 is 0
//		State 1 Out0 is 0 Out1 is 1
//x=-1 y=-1
		_mm_ls0 = _mm_load_si128(pBeta+0);
		_mm_ls1 = _mm_load_si128(pBeta+1);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+8,_mm_ls0_0);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+12,_mm_ls0_0);
//		Calculate State 2 3 to 1 5
//		State 2 Last0 is 1 Last1 is 5
//		State 2 Out0 is 1 Out1 is 0
//		State 3 Last0 is 5 Last1 is 1
//		State 3 Out0 is 1 Out1 is 0
//x=-1 y=1
		_mm_ls0 = _mm_load_si128(pBeta+2);
		_mm_ls1 = _mm_load_si128(pBeta+3);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+9,_mm_ls0_0);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+13,_mm_ls0_0);
//		Calculate State 4 5 to 2 6
//		State 4 Last0 is 6 Last1 is 2
//		State 4 Out0 is 1 Out1 is 0
//		State 5 Last0 is 2 Last1 is 6
//		State 5 Out0 is 1 Out1 is 0
//x=1 y=-1
		_mm_ls0 = _mm_load_si128(pBeta+4);
		_mm_ls1 = _mm_load_si128(pBeta+5);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+10,_mm_ls0_0);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+14,_mm_ls0_0);
//		Calculate State 6 7 to 3 7
//		State 6 Last0 is 7 Last1 is 3
//		State 6 Out0 is 0 Out1 is 1
//		State 7 Last0 is 3 Last1 is 7
//		State 7 Out0 is 0 Out1 is 1
//x=1 y=1
		_mm_ls0 = _mm_load_si128(pBeta+6);
		_mm_ls1 = _mm_load_si128(pBeta+7);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+11,_mm_ls0_0);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);
		_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);
		_mm_store_si128(pBeta+15,_mm_ls0_0);
		pBeta+=8;
		if((i&0xff)==0xff)
		{
			zero=_mm_load_si128(pBeta);
			_mm_ls0 = _mm_load_si128(pBeta+1);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+1,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+2);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+2,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+3);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+3,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+4);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+4,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+5);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+5,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+6);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+6,_mm_ls0);
			_mm_ls0 = _mm_load_si128(pBeta+7);
			_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);
			_mm_store_si128(pBeta+7,_mm_ls0);
			zero=_mm_xor_si128(zero,zero);
			_mm_store_si128(pBeta,zero);
		}
		plX--;
		ptY--;
	}
	pBeta-=8;
	pAlpha=(__m128i *)_mm_Alpha;
		plX++;
		ptY++;
// Calculate Output 
	for( i=0;i<CODELENGTH-3;i++ )
	{
//		Calculate State 0 
//		State 0 Next0 is 0 Next1 is 1
//		State 0 Out0 is 0 Out1 is 1
		_mm_ls0 = _mm_load_si128(pAlpha+0);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);
		_mm_0 = _mm_add_epi16(_mm_ls0_0,pBeta[0]);
		_mm_ls1 = _mm_load_si128(pAlpha+0);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);
		_mm_1 = _mm_add_epi16(_mm_ls0_1,pBeta[1]);
//		Calculate State 1 
//		State 1 Next0 is 2 Next1 is 3
//		State 1 Out0 is 1 Out1 is 0
		_mm_ls0 = _mm_load_si128(pAlpha+1);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[2]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+1);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[3]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 2 
//		State 2 Next0 is 5 Next1 is 4
//		State 2 Out0 is 1 Out1 is 0
		_mm_ls0 = _mm_load_si128(pAlpha+2);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[5]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+2);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[4]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 3 
//		State 3 Next0 is 7 Next1 is 6
//		State 3 Out0 is 0 Out1 is 1
		_mm_ls0 = _mm_load_si128(pAlpha+3);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[7]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+3);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[6]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 4 
//		State 4 Next0 is 1 Next1 is 0
//		State 4 Out0 is 0 Out1 is 1
		_mm_ls0 = _mm_load_si128(pAlpha+4);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[1]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+4);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[0]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 5 
//		State 5 Next0 is 3 Next1 is 2
//		State 5 Out0 is 1 Out1 is 0
		_mm_ls0 = _mm_load_si128(pAlpha+5);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[3]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+5);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[2]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 6 
//		State 6 Next0 is 4 Next1 is 5
//		State 6 Out0 is 1 Out1 is 0
		_mm_ls0 = _mm_load_si128(pAlpha+6);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[4]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+6);
		_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[5]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
//		Calculate State 7 
//		State 7 Next0 is 6 Next1 is 7
//		State 7 Out0 is 0 Out1 is 1
		_mm_ls0 = _mm_load_si128(pAlpha+7);
		_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);
		_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[6]);
		_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);
		_mm_ls1 = _mm_load_si128(pAlpha+7);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);
		_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[7]);
		_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);
		_mm_1 = _mm_sub_epi16(_mm_1,_mm_0);
		_mm_store_si128(ptX,_mm_1);
		pBeta-=8;
		pAlpha+=8;
		ptX++;
		ptY++;
	}
}
