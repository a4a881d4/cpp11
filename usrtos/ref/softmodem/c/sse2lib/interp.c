#include<xmmintrin.h>
short coeff[] __attribute__((aligned(16)))= {
 2, 2, 2, 2, 2, 2, 2, 2 ,
 29, 29, 29, 29, 29, 29, 29, 29 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 1, 1, 1, 1, 1, 1, 1, 1 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 5, 5, 5, 5, 5, 5, 5, 5 ,
 5, 5, 5, 5, 5, 5, 5, 5 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -3, -3, -3, -3, -3, -3, -3, -3 ,
 -4, -4, -4, -4, -4, -4, -4, -4 ,
 -5, -5, -5, -5, -5, -5, -5, -5 ,
 -6, -6, -6, -6, -6, -6, -6, -6 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -6, -6, -6, -6, -6, -6, -6, -6 ,
 -5, -5, -5, -5, -5, -5, -5, -5 ,
 -3, -3, -3, -3, -3, -3, -3, -3 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 1, 1, 1, 1, 1, 1, 1, 1 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 7, 7, 7, 7, 7, 7, 7, 7 ,
 11, 11, 11, 11, 11, 11, 11, 11 ,
 14, 14, 14, 14, 14, 14, 14, 14 ,
 18, 18, 18, 18, 18, 18, 18, 18 ,
 22, 22, 22, 22, 22, 22, 22, 22 ,
 26, 26, 26, 26, 26, 26, 26, 26 ,
 29, 29, 29, 29, 29, 29, 29, 29 ,
 32, 32, 32, 32, 32, 32, 32, 32 ,
 34, 34, 34, 34, 34, 34, 34, 34 ,
 37, 37, 37, 37, 37, 37, 37, 37 ,
 38, 38, 38, 38, 38, 38, 38, 38 ,
 39, 39, 39, 39, 39, 39, 39, 39 ,
 39, 39, 39, 39, 39, 39, 39, 39 ,
 39, 39, 39, 39, 39, 39, 39, 39 ,
 38, 38, 38, 38, 38, 38, 38, 38 ,
 37, 37, 37, 37, 37, 37, 37, 37 ,
 34, 34, 34, 34, 34, 34, 34, 34 ,
 32, 32, 32, 32, 32, 32, 32, 32 ,
 29, 29, 29, 29, 29, 29, 29, 29 ,
 26, 26, 26, 26, 26, 26, 26, 26 ,
 22, 22, 22, 22, 22, 22, 22, 22 ,
 18, 18, 18, 18, 18, 18, 18, 18 ,
 14, 14, 14, 14, 14, 14, 14, 14 ,
 11, 11, 11, 11, 11, 11, 11, 11 ,
 7, 7, 7, 7, 7, 7, 7, 7 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 1, 1, 1, 1, 1, 1, 1, 1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -3, -3, -3, -3, -3, -3, -3, -3 ,
 -5, -5, -5, -5, -5, -5, -5, -5 ,
 -6, -6, -6, -6, -6, -6, -6, -6 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -7, -7, -7, -7, -7, -7, -7, -7 ,
 -6, -6, -6, -6, -6, -6, -6, -6 ,
 -5, -5, -5, -5, -5, -5, -5, -5 ,
 -4, -4, -4, -4, -4, -4, -4, -4 ,
 -3, -3, -3, -3, -3, -3, -3, -3 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 5, 5, 5, 5, 5, 5, 5, 5 ,
 5, 5, 5, 5, 5, 5, 5, 5 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 4, 4, 4, 4, 4, 4, 4, 4 ,
 3, 3, 3, 3, 3, 3, 3, 3 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 2, 2, 2, 2, 2, 2, 2, 2 ,
 1, 1, 1, 1, 1, 1, 1, 1 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -2, -2, -2, -2, -2, -2, -2, -2 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 0, 0, 0, 0, 0, 0, 0, 0 ,
 -1, -1, -1, -1, -1, -1, -1, -1 ,
 29, 29, 29, 29, 29, 29, 29, 29 ,
 2, 2, 2, 2, 2, 2, 2, 2 };
__m128i *pmCoeff=(__m128i *)coeff;
void interp( void *indata, void *outdata, int DataLength )
{
	int i;
	__m128i *pdata =indata;
	__m128i *pfilterout =outdata;
	__m128i mA,mB,alu;
	__m128i *pfdata;
	__m128i *pfcoeff;
	for( i=0;i<DataLength;i+=8 )
	{
//The 0 coeff
//pos = 0 
		alu=_mm_xor_si128(alu,alu);
//2 
		pfcoeff=pmCoeff+0;
		pfdata=pdata+0;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+13;
		pfdata=pdata+1;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+26;
		pfdata=pdata+2;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-5 
		pfcoeff=pmCoeff+39;
		pfdata=pdata+3;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//11 
		pfcoeff=pmCoeff+52;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//38 
		pfcoeff=pmCoeff+65;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+6;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//0 
//2 
		pfcoeff=pmCoeff+104;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+9;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 1 coeff
//pos = 0 
		alu=_mm_xor_si128(alu,alu);
//-1 
		pfdata=pdata+0;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//1 
		pfdata=pdata+1;
		mB=_mm_load_si128(pfdata);
		alu=_mm_adds_epi16(alu,mB);
//0 
//-3 
		pfcoeff=pmCoeff+47;
		pfdata=pdata+3;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//37 
		pfcoeff=pmCoeff+60;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//14 
		pfcoeff=pmCoeff+73;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-6 
		pfcoeff=pmCoeff+86;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//5 
		pfcoeff=pmCoeff+99;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+112;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//29 
		pfcoeff=pmCoeff+125;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 2 coeff
//pos = 1 
		alu=_mm_xor_si128(alu,alu);
//0 
//-1 
		pfdata=pdata+2;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+29;
		pfdata=pdata+3;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+42;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//22 
		pfcoeff=pmCoeff+55;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//32 
		pfcoeff=pmCoeff+68;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-6 
		pfcoeff=pmCoeff+81;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//3 
		pfcoeff=pmCoeff+94;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//0 
//0 
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 3 coeff
//pos = 1 
		alu=_mm_xor_si128(alu,alu);
//-2 
		pfcoeff=pmCoeff+11;
		pfdata=pdata+1;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//3 
		pfcoeff=pmCoeff+24;
		pfdata=pdata+2;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-3 
		pfcoeff=pmCoeff+37;
		pfdata=pdata+3;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+50;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//39 
		pfcoeff=pmCoeff+63;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+76;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-3 
		pfcoeff=pmCoeff+89;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//3 
		pfcoeff=pmCoeff+102;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+115;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 4 coeff
//pos = 2 
		alu=_mm_xor_si128(alu,alu);
//0 
//0 
//3 
		pfcoeff=pmCoeff+32;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-6 
		pfcoeff=pmCoeff+45;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//32 
		pfcoeff=pmCoeff+58;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//22 
		pfcoeff=pmCoeff+71;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+84;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+97;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+10;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//0 
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 5 coeff
//pos = 3 
		alu=_mm_xor_si128(alu,alu);
//29 
		pfcoeff=pmCoeff+1;
		pfdata=pdata+3;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+14;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//5 
		pfcoeff=pmCoeff+27;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-6 
		pfcoeff=pmCoeff+40;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//14 
		pfcoeff=pmCoeff+53;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//37 
		pfcoeff=pmCoeff+66;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-3 
		pfcoeff=pmCoeff+79;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//0 
//1 
		pfdata=pdata+11;
		mB=_mm_load_si128(pfdata);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+12;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 6 coeff
//pos = 3 
		alu=_mm_xor_si128(alu,alu);
//-1 
		pfdata=pdata+3;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//2 
		pfcoeff=pmCoeff+22;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//0 
//-1 
		pfdata=pdata+6;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//38 
		pfcoeff=pmCoeff+61;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//11 
		pfcoeff=pmCoeff+74;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-5 
		pfcoeff=pmCoeff+87;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+100;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+113;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//2 
		pfcoeff=pmCoeff+126;
		pfdata=pdata+12;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 7 coeff
//pos = 4 
		alu=_mm_xor_si128(alu,alu);
//0 
//-1 
		pfdata=pdata+5;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+30;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+43;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//26 
		pfcoeff=pmCoeff+56;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//29 
		pfcoeff=pmCoeff+69;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+82;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//3 
		pfcoeff=pmCoeff+95;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//0 
//0 
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 8 coeff
//pos = 4 
		alu=_mm_xor_si128(alu,alu);
//-2 
		pfcoeff=pmCoeff+12;
		pfdata=pdata+4;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+25;
		pfdata=pdata+5;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-4 
		pfcoeff=pmCoeff+38;
		pfdata=pdata+6;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//7 
		pfcoeff=pmCoeff+51;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//39 
		pfcoeff=pmCoeff+64;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//1 
		pfdata=pdata+9;
		mB=_mm_load_si128(pfdata);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+10;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//2 
		pfcoeff=pmCoeff+103;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+12;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 9 coeff
//pos = 5 
		alu=_mm_xor_si128(alu,alu);
//0 
//0 
//2 
		pfcoeff=pmCoeff+33;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-5 
		pfcoeff=pmCoeff+46;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//34 
		pfcoeff=pmCoeff+59;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//18 
		pfcoeff=pmCoeff+72;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+85;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//5 
		pfcoeff=pmCoeff+98;
		pfdata=pdata+12;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+111;
		pfdata=pdata+13;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+14;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 10 coeff
//pos = 6 
		alu=_mm_xor_si128(alu,alu);
//-1 
		pfdata=pdata+6;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+15;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//5 
		pfcoeff=pmCoeff+28;
		pfdata=pdata+8;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+41;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//18 
		pfcoeff=pmCoeff+54;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//34 
		pfcoeff=pmCoeff+67;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-5 
		pfcoeff=pmCoeff+80;
		pfdata=pdata+12;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//2 
		pfcoeff=pmCoeff+93;
		pfdata=pdata+13;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//0 
//0 
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 11 coeff
//pos = 6 
		alu=_mm_xor_si128(alu,alu);
//-1 
		pfdata=pdata+6;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//2 
		pfcoeff=pmCoeff+23;
		pfdata=pdata+7;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+8;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//1 
		pfdata=pdata+9;
		mB=_mm_load_si128(pfdata);
		alu=_mm_adds_epi16(alu,mB);
//39 
		pfcoeff=pmCoeff+62;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//7 
		pfcoeff=pmCoeff+75;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-4 
		pfcoeff=pmCoeff+88;
		pfdata=pdata+12;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+101;
		pfdata=pdata+13;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-2 
		pfcoeff=pmCoeff+114;
		pfdata=pdata+14;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

//The 12 coeff
//pos = 7 
		alu=_mm_xor_si128(alu,alu);
//0 
//0 
//3 
		pfcoeff=pmCoeff+31;
		pfdata=pdata+9;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+44;
		pfdata=pdata+10;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//29 
		pfcoeff=pmCoeff+57;
		pfdata=pdata+11;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//26 
		pfcoeff=pmCoeff+70;
		pfdata=pdata+12;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-7 
		pfcoeff=pmCoeff+83;
		pfdata=pdata+13;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//4 
		pfcoeff=pmCoeff+96;
		pfdata=pdata+14;
		mA=_mm_load_si128(pfcoeff);
		mB=_mm_load_si128(pfdata);
		mB=_mm_mullo_epi16(mA,mB);
		alu=_mm_adds_epi16(alu,mB);
//-1 
		pfdata=pdata+15;
		mB=_mm_load_si128(pfdata);
		alu=_mm_subs_epi16(alu,mB);
//0 
		_mm_store_si128(pfilterout,alu);
		pfilterout++;

		pdata+=8;
	}
}
