#include<xmmintrin.h>
#include"sse2Map.h"


#ifdef DEBUG
#include<stdio.h>
#endif

extern unsigned short sse2CRCTable[];
extern unsigned short InterleaverTable[1360];
extern short _mm_Alpha[];
extern short _mm_Beta[];

void CRCadd( short *x, short *tx, short *tu, short *ret, int Length)
{
	int i;
	__m128i *px,*ptx,*ptu,*pcrctable;
	px=(__m128i *)x;
	ptx=(__m128i *)tx;
	ptu=(__m128i *)tu;
	pcrctable=(__m128i *)sse2CRCTable;
	__m128i zero,d,s;
	zero=_mm_set_epi16(0,0,0,0,0,0,0,0);
	s=_mm_set_epi16(0,0,0,0,0,0,0,0);
	for( i=0;i<Length;i++ )
	{
		d=_mm_load_si128(px+i);
		d=_mm_adds_epi16(d,*(ptx+i));
		_mm_store_si128(ptu+i,d);
		d=_mm_cmpgt_epi16(d,zero);
		d=_mm_and_si128(d,*(pcrctable+i));
		s=_mm_xor_si128(s,d);
		
	}	
/*	
	for( ;i<Length;i++ )
	{
		d=_mm_load_si128(px+i);
		d=_mm_adds_epi16(d,*(ptx+i));
		d=_mm_cmpgt_epi16(zero,d);
		d=_mm_and_si128(d,*(pcrctable+i));
		s=_mm_xor_si128(s,d);
	}	
*/	
	_mm_store_si128((__m128i *)ret,s);

}

short turbo_decode_weight[] __attribute__((aligned(16)))={ 
	12,12,12,12,12,12,12,12, 
	9,9,9,9,    9,9,9,9,     
	9,9,9,9,    9,9,9,9,     
	9,9,9,9,    9,9,9,9,     
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10,10,10,10,10, 
	10,10,10,10, 10,10,10,10  
	};		

void addlimit_Interl(void *a, void *b, void * c, int count, int Length)
{
	__m128i pack_high,pack_high_me,*pa,*pb,*pc,d,e,weight;
	
	int i;
	pack_high=_mm_set_epi16(0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff);
	pack_high_me=_mm_set_epi16(LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT);
	
	
	__m128i *w=(__m128i *)turbo_decode_weight;
	
	w+=count;
	
	weight=_mm_load_si128(w);
	
	pack_high_me=_mm_subs_epi16(pack_high,pack_high_me);
	
	pa=(__m128i *)a;
	pb=(__m128i *)b;
	pc=(__m128i *)c;
	
	for( i=0;i<Length;i++ )
	{
		d=_mm_load_si128(pb+InterleaverTable[i]);
		d=_mm_mullo_epi16(d,weight);
		d=_mm_srai_epi16(d,5);
		d=_mm_adds_epi16(d,pc[InterleaverTable[i]]);
		d=_mm_adds_epi16(d,pack_high_me);
		d=_mm_subs_epi16(d,pack_high_me);
		d=_mm_subs_epi16(d,pack_high_me);
		d=_mm_adds_epi16(d,pack_high_me);
		_mm_store_si128(pa+i,d);
	}	
}

void addlimit_DeInterl(void *a, void *b, void * c, int count, int Length)
{
	__m128i pack_high,pack_high_me,*pa,*pb,*pc,d,e,weight;
	
	int i;
	pack_high=_mm_set_epi16(0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,0x7fff);
	pack_high_me=_mm_set_epi16(LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT,LIMIT);
	
	
	__m128i *w=(__m128i *)turbo_decode_weight;
	
	w+=count;
	
	weight=_mm_load_si128(w);
	
	pack_high_me=_mm_subs_epi16(pack_high,pack_high_me);
	
	pa=(__m128i *)a;
	pb=(__m128i *)b;
	pc=(__m128i *)c;
	
	for( i=0;i<Length;i++ )
	{
		d=_mm_load_si128(pb+i);
		d=_mm_mullo_epi16(d,weight);
		d=_mm_srai_epi16(d,5);
		d=_mm_adds_epi16(d,pc[InterleaverTable[i]]);
		d=_mm_adds_epi16(d,pack_high_me);
		d=_mm_subs_epi16(d,pack_high_me);
		d=_mm_subs_epi16(d,pack_high_me);
		d=_mm_adds_epi16(d,pack_high_me);
		_mm_store_si128(pa+InterleaverTable[i],d);
	}	
}


short _mm_Turbo_tX[CODELENGTH*8] __attribute__((aligned(16)));
short _mm_Turbo_nX[CODELENGTH*8] __attribute__((aligned(16)));
int Decode( void *x, void *t2, void *y1, void *y2, void *out )
{
	__m128i zeros,d;
	__m128i *px,*pnx,*ptx,*pt2;
	unsigned short ret[8] __attribute__((aligned(16)));
	unsigned short crc=0;
	int i,j;
	int count;
	
#ifdef DEBUG
	FILE *fp;
#endif

#ifdef DEBUG
	fp=fopen("record.txt","wt");
#endif
		
	px=(__m128i *)x;
	pnx=(__m128i *)_mm_Turbo_nX;
	ptx=(__m128i *)_mm_Turbo_tX;
	zeros=_mm_set_epi16(0,0,0,0,0,0,0,0);
	
	for( i=0;i<CODELENGTH-3;i++ )
	{
		
		d=_mm_load_si128(px+InterleaverTable[i]);
		_mm_store_si128(pnx+i,d);
	}
	 
	for( count=0;count<MAXINTERLEAVE;count++ )
	{ 
		pt2=(__m128i *)t2;
		
		for( i=CODELENGTH-3;i<CODELENGTH;i++ )
		{
			d=_mm_load_si128(pt2);
			_mm_store_si128(pnx+i,d);
			pt2++;
		}
		
		_mm_Map( pnx,y2,ptx );
#ifdef DEBUG
		fprintf(fp,"%d result\n",count*2);
		for( i=0;i<CODELENGTH;i++ )
		{
			fprintf(fp,"%d %d \n",(int)_mm_Turbo_nX[i*8],(int)_mm_Turbo_tX[i*8]);
/*			for( j=0;j<8;j++ )
				fprintf(fp,"%d ",(int)_mm_Alpha[i*64+j*8]);
			fprintf(fp," | ");
			for( j=0;j<8;j++ )
				fprintf(fp,"%d ",(int)_mm_Beta[i*64+j*8]);
			fprintf(fp,"\n");
*/
		}
		
#endif		
		addlimit_DeInterl(pnx,ptx,px,count*2,CODELENGTH-3);
		
		pt2=(__m128i *)x;
		pt2+=(CODELENGTH-3);
		
		for( i=CODELENGTH-3;i<CODELENGTH;i++ )
		{
			d=_mm_load_si128(pt2);
			_mm_store_si128(pnx+i,d);
			pt2++;
		}
		
		_mm_Map( pnx,y1,ptx );

#ifdef DEBUG
		fprintf(fp,"%d result\n",count*2+1);
		for( i=0;i<CODELENGTH;i++ )
			fprintf(fp,"%d %d\n",(int)_mm_Turbo_nX[i*8],(int)_mm_Turbo_tX[i*8]);
		
#endif		
		
		CRCadd( _mm_Turbo_tX, _mm_Turbo_nX, ( short * )out, ret, CODELENGTH-3 );
		crc=0;
		for( j=0;j<8;j++ )
			crc^=ret[j];
		if(  crc== 0xffff ) 
		{
#ifdef DEBUG
			fclose(fp);
#endif
			return 0;
		}
		
		addlimit_Interl(pnx,ptx,px,count*2+1,CODELENGTH-3);
		
	}
#ifdef DEBUG
	fclose(fp);
#endif
	
	return -1;
}
	
	
