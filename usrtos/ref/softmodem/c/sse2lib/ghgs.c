#include<xmmintrin.h>
short _mm_GHGS_Ibuf0[64] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf0[64] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf1[2048] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf1[2048] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf2[16] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf2[16] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf3[32] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf3[32] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf4[256] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf4[256] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf5[1024] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf5[1024] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf6[512] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf6[512] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf7[4096] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf7[4096] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf8[8192] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf8[8192] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf9[128] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf9[128] __attribute__((aligned(16)));
short _mm_GHGS_Ibuf10[16384] __attribute__((aligned(16)));
short _mm_GHGS_Qbuf10[16384] __attribute__((aligned(16)));
__m128i * _mm_GHGS_pmIbuf0=(__m128i *)_mm_GHGS_Ibuf0;
__m128i * _mm_GHGS_pmQbuf0=(__m128i *)_mm_GHGS_Qbuf0;
__m128i * _mm_GHGS_pmIbuf1=(__m128i *)_mm_GHGS_Ibuf1;
__m128i * _mm_GHGS_pmQbuf1=(__m128i *)_mm_GHGS_Qbuf1;
__m128i * _mm_GHGS_pmIbuf2=(__m128i *)_mm_GHGS_Ibuf2;
__m128i * _mm_GHGS_pmQbuf2=(__m128i *)_mm_GHGS_Qbuf2;
__m128i * _mm_GHGS_pmIbuf3=(__m128i *)_mm_GHGS_Ibuf3;
__m128i * _mm_GHGS_pmQbuf3=(__m128i *)_mm_GHGS_Qbuf3;
__m128i * _mm_GHGS_pmIbuf4=(__m128i *)_mm_GHGS_Ibuf4;
__m128i * _mm_GHGS_pmQbuf4=(__m128i *)_mm_GHGS_Qbuf4;
__m128i * _mm_GHGS_pmIbuf5=(__m128i *)_mm_GHGS_Ibuf5;
__m128i * _mm_GHGS_pmQbuf5=(__m128i *)_mm_GHGS_Qbuf5;
__m128i * _mm_GHGS_pmIbuf6=(__m128i *)_mm_GHGS_Ibuf6;
__m128i * _mm_GHGS_pmQbuf6=(__m128i *)_mm_GHGS_Qbuf6;
__m128i * _mm_GHGS_pmIbuf7=(__m128i *)_mm_GHGS_Ibuf7;
__m128i * _mm_GHGS_pmQbuf7=(__m128i *)_mm_GHGS_Qbuf7;
__m128i * _mm_GHGS_pmIbuf8=(__m128i *)_mm_GHGS_Ibuf8;
__m128i * _mm_GHGS_pmQbuf8=(__m128i *)_mm_GHGS_Qbuf8;
__m128i * _mm_GHGS_pmIbuf9=(__m128i *)_mm_GHGS_Ibuf9;
__m128i * _mm_GHGS_pmQbuf9=(__m128i *)_mm_GHGS_Qbuf9;
__m128i * _mm_GHGS_pmIbuf10=(__m128i *)_mm_GHGS_Ibuf10;
__m128i * _mm_GHGS_pmQbuf10=(__m128i *)_mm_GHGS_Qbuf10;
int _GHGS_pos0;
int _GHGS_pos1;
int _GHGS_pos2;
int _GHGS_pos3;
int _GHGS_pos4;
int _GHGS_pos5;
int _GHGS_pos6;
int _GHGS_pos7;
int _GHGS_pos8;
int _GHGS_pos9;
int _GHGS_pos10;
void GHGS( void *inI, void *inQ, void *outI, void *outQ )
{
	__m128i Ibi,Qbi,Ibo,Qbo,Imi,Qmi,Imo,Qmo,*pbuf;
	Ibi=_mm_load_si128(inI);
	Qbi=_mm_load_si128(inQ);
	Imi=_mm_load_si128(inI);
	Qmi=_mm_load_si128(inQ);
// The 0th Delay = 4
	pbuf=_mm_GHGS_pmIbuf0+_GHGS_pos0;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf0+_GHGS_pos0;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos0++;
	_GHGS_pos0&=7;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -1 
	Ibi=_mm_subs_epi16(Ibo,Imo);
	Qbi=_mm_subs_epi16(Qbo,Qmo);
	Imi=_mm_adds_epi16(Ibo,Imo);
	Qmi=_mm_adds_epi16(Qbo,Qmo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
// The 1th Delay = 128
	pbuf=_mm_GHGS_pmIbuf1+_GHGS_pos1;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf1+_GHGS_pos1;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos1++;
	_GHGS_pos1&=255;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -1 
	Ibi=_mm_subs_epi16(Ibo,Imo);
	Qbi=_mm_subs_epi16(Qbo,Qmo);
	Imi=_mm_adds_epi16(Ibo,Imo);
	Qmi=_mm_adds_epi16(Qbo,Qmo);
// The 2th Delay = 1
	pbuf=_mm_GHGS_pmIbuf2+_GHGS_pos2;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf2+_GHGS_pos2;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos2++;
	_GHGS_pos2&=1;
	Imo=Imi;
	Qmo=Qmi;
 // weight = 1 
	Ibi=_mm_adds_epi16(Ibo,Imo);
	Qbi=_mm_adds_epi16(Qbo,Qmo);
	Imi=_mm_subs_epi16(Ibo,Imo);
	Qmi=_mm_subs_epi16(Qbo,Qmo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
// The 3th Delay = 2
	pbuf=_mm_GHGS_pmIbuf3+_GHGS_pos3;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf3+_GHGS_pos3;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos3++;
	_GHGS_pos3&=3;
	Imo=Imi;
	Qmo=Qmi;
 // weight = j 
	Ibi=_mm_adds_epi16(Ibo,Qmo);
	Qbi=_mm_subs_epi16(Qbo,Imo);
	Imi=_mm_subs_epi16(Ibo,Qmo);
	Qmi=_mm_adds_epi16(Qbo,Imo);
// The 4th Delay = 16
	pbuf=_mm_GHGS_pmIbuf4+_GHGS_pos4;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf4+_GHGS_pos4;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos4++;
	_GHGS_pos4&=31;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -j 
	Ibi=_mm_subs_epi16(Ibo,Qmo);
	Qbi=_mm_adds_epi16(Qbo,Imo);
	Imi=_mm_adds_epi16(Ibo,Qmo);
	Qmi=_mm_subs_epi16(Qbo,Imo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
// The 5th Delay = 64
	pbuf=_mm_GHGS_pmIbuf5+_GHGS_pos5;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf5+_GHGS_pos5;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos5++;
	_GHGS_pos5&=127;
	Imo=Imi;
	Qmo=Qmi;
 // weight = 1 
	Ibi=_mm_adds_epi16(Ibo,Imo);
	Qbi=_mm_adds_epi16(Qbo,Qmo);
	Imi=_mm_subs_epi16(Ibo,Imo);
	Qmi=_mm_subs_epi16(Qbo,Qmo);
// The 6th Delay = 32
	pbuf=_mm_GHGS_pmIbuf6+_GHGS_pos6;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf6+_GHGS_pos6;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos6++;
	_GHGS_pos6&=63;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -j 
	Ibi=_mm_subs_epi16(Ibo,Qmo);
	Qbi=_mm_adds_epi16(Qbo,Imo);
	Imi=_mm_adds_epi16(Ibo,Qmo);
	Qmi=_mm_subs_epi16(Qbo,Imo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
// The 7th Delay = 256
	pbuf=_mm_GHGS_pmIbuf7+_GHGS_pos7;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf7+_GHGS_pos7;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos7++;
	_GHGS_pos7&=511;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -j 
	Ibi=_mm_subs_epi16(Ibo,Qmo);
	Qbi=_mm_adds_epi16(Qbo,Imo);
	Imi=_mm_adds_epi16(Ibo,Qmo);
	Qmi=_mm_subs_epi16(Qbo,Imo);
// The 8th Delay = 512
	pbuf=_mm_GHGS_pmIbuf8+_GHGS_pos8;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf8+_GHGS_pos8;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos8++;
	_GHGS_pos8&=1023;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -j 
	Ibi=_mm_subs_epi16(Ibo,Qmo);
	Qbi=_mm_adds_epi16(Qbo,Imo);
	Imi=_mm_adds_epi16(Ibo,Qmo);
	Qmi=_mm_subs_epi16(Qbo,Imo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
// The 9th Delay = 8
	pbuf=_mm_GHGS_pmIbuf9+_GHGS_pos9;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf9+_GHGS_pos9;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos9++;
	_GHGS_pos9&=15;
	Imo=Imi;
	Qmo=Qmi;
 // weight = -1 
	Ibi=_mm_subs_epi16(Ibo,Imo);
	Qbi=_mm_subs_epi16(Qbo,Qmo);
	Imi=_mm_adds_epi16(Ibo,Imo);
	Qmi=_mm_adds_epi16(Qbo,Qmo);
// The 10th Delay = 1024
	pbuf=_mm_GHGS_pmIbuf10+_GHGS_pos10;
	Ibo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Ibi);
	pbuf=_mm_GHGS_pmQbuf10+_GHGS_pos10;
	Qbo=_mm_load_si128(pbuf);
	_mm_store_si128(pbuf,Qbi);
	_GHGS_pos10++;
	_GHGS_pos10&=2047;
	Imo=Imi;
	Qmo=Qmi;
 // weight = j 
	Ibi=_mm_adds_epi16(Ibo,Qmo);
	Qbi=_mm_subs_epi16(Qbo,Imo);
	Imi=_mm_subs_epi16(Ibo,Qmo);
	Qmi=_mm_adds_epi16(Qbo,Imo);
	Ibi=_mm_srai_epi16(Ibi,1);
	Qbi=_mm_srai_epi16(Qbi,1);
	Imi=_mm_srai_epi16(Imi,1);
	Qmi=_mm_srai_epi16(Qmi,1);
	_mm_store_si128(outI,Ibi);
	_mm_store_si128(outQ,Qbi);
}
//