#include"State.h"
#include<stdio.h>


int main()
{

	int iDenominator = 0xd;
	int iNumerator = 0xb;
	int iStateDeep = 3;
	int iStateNumber = 8;
	int short_min=-3200;
	CState *Trellis;
	int i,j;
	Trellis->genRSC(iDenominator,iNumerator);
	Trellis=Trellis->Trellis;


	printf("#include<xmmintrin.h>\n");
	printf("#include\"sse2Map.h\"\n");
	printf("short _mm_Alpha[(CODELENGTH+%d)*8*8] __attribute__((aligned(16)));\n",iStateDeep+1);
	printf("short _mm_Beta[(CODELENGTH+%d)*8*8] __attribute__((aligned(16)));\n",iStateDeep+1);
	
	printf("void _mm_Map( void *lX, void *tY, void *tX )\n");
	
	printf("{\n\tint i;\n");

	printf("\t__m128i *pAlpha=(__m128i *)_mm_Alpha;\n\t__m128i *pBeta=(__m128i *)_mm_Beta;\n");
	printf("\t__m128i *plX=(__m128i *)lX;\n\t__m128i *ptX=(__m128i *)tX;\n\t__m128i *ptY=(__m128i *)tY;\n\n");

	
	printf("\t__m128i zero,XplusY,XsubY,_mm_ls0,_mm_ls1,_mm_ls0_0,_mm_ls0_1,_mm_0,_mm_1;\n");

	printf("// Init Alpha \n");

	printf("\tzero=_mm_xor_si128(zero,zero);\n");
	
	printf("\t_mm_store_si128(pAlpha,zero);\n");


	printf("\tzero=_mm_set_epi16(%d,%d,%d,%d,%d,%d,%d,%d);\n",
		short_min,short_min,short_min,short_min,
		short_min,short_min,short_min,short_min
		);

	for( i=1;i<iStateNumber;i++ )
		printf("\t_mm_store_si128(pAlpha+%d,zero);\n",i);

	
	printf("// Calculate Alpha \n");

	printf("\tfor( i=0;i<CODELENGTH;i++ )\n\t{\n");

	printf("\t\tXplusY=_mm_load_si128(plX);\n");

	printf("\t\tXsubY=_mm_subs_epi16(XplusY,*ptY);\n");

	printf("\t\tXplusY=_mm_adds_epi16(XplusY,*ptY);\n");

	int s0,s1,t1,t2,ttxy;
	
	for ( s0=0;s0<iStateNumber;s0+=2)
	{
		

		printf("//\t\tCalculate State %d %d to %d %d\n",(s0>>1),(s0+iStateNumber)>>1,s0,s0+1);
		
		printf("//\t\tState %d Next0 is %d Next1 is %d\n",(s0>>1),
			Trellis[(s0>>1)].NextState0,
			Trellis[(s0>>1)].NextState1);
		
		printf("//\t\tState %d Out0 is %d Out1 is %d\n",(s0)>>1,
			Trellis[(s0)>>1].NextBranch0,
			Trellis[(s0)>>1].NextBranch1);

		printf("//\t\tState %d Next0 is %d Next1 is %d\n",(s0+iStateNumber)>>1,
			Trellis[(s0+iStateNumber)>>1].NextState0,
			Trellis[(s0+iStateNumber)>>1].NextState1);
		
		printf("//\t\tState %d Out0 is %d Out1 is %d\n",(s0+iStateNumber)>>1,
			Trellis[(s0+iStateNumber)>>1].NextBranch0,
			Trellis[(s0+iStateNumber)>>1].NextBranch1);
		
		
		if( Trellis[(s0>>1)].NextState0 == s0 )
			t1=0;
		else
			t1=1;
		if( Trellis[(s0>>1)].NextState0 == s0 )
			t2=Trellis[(s0>>1)].NextBranch0;
		else
			t2=Trellis[(s0>>1)].NextBranch1;

		
		ttxy=t2*2+t1;

		switch( ttxy ) {
			case 0://x=-1,y=-1
				printf("//x=-1 y=-1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",(s0>>1));
				printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",(s0+iStateNumber)>>1);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+1+iStateNumber);
				break;
			
			case 1://x=1,y=-1
				printf("//x=1 y=-1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",(s0>>1));
				printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",(s0+iStateNumber)>>1);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+1+iStateNumber);
				break;
				
			case 2://x=-1,y=1
				printf("//x=-1 y=1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",(s0>>1));
				printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",(s0+iStateNumber)>>1);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+1+iStateNumber);
				break;
			
			case 3://x=1,y=1
				printf("//x=1 y=1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",(s0>>1));
				printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",(s0+iStateNumber)>>1);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pAlpha+%d,_mm_ls0_0);\n",s0+1+iStateNumber);
				break;

			default:
				break;
		}
	}
	
	printf("\t\tpAlpha+=%d;\n",iStateNumber);
	
	printf("\t\tif((i&0xff)==0xff)\n\t\t{\n");
	printf("\t\t\tzero=_mm_load_si128(pAlpha);\n");
	for(j=1;j<8;j++)
	{
		printf("\t\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",j);
		printf("\t\t\t_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);\n");
		printf("\t\t\t_mm_store_si128(pAlpha+%d,_mm_ls0);\n",j);
	}
	printf("\t\t\tzero=_mm_xor_si128(zero,zero);\n");
	printf("\t\t\t_mm_store_si128(pAlpha,zero);\n");
	printf("\t\t}\n");


	
	printf("\t\tplX++;\n");
	printf("\t\tptY++;\n");

	
	printf("\t}\n");

	printf("// Init Beta \n");
	printf("\tzero=_mm_xor_si128(zero,zero);\n");
	
	printf("\t_mm_store_si128(pBeta,zero);\n");


	printf("\tzero=_mm_set_epi16(%d,%d,%d,%d,%d,%d,%d,%d);\n",
		short_min,short_min,short_min,short_min,
		short_min,short_min,short_min,short_min
		);

	for( i=1;i<iStateNumber;i++ )
		printf("\t_mm_store_si128(pBeta+%d,zero);\n",i);

	printf("// Calculate Beta \n");
	
	printf("\tplX--;\n");
	printf("\tptY--;\n");

	printf("\tfor( i=0;i<CODELENGTH;i++ )\n\t{\n");

	printf("\t\tXplusY=_mm_load_si128(plX);\n");

	printf("\t\tXsubY=_mm_subs_epi16(XplusY,*ptY);\n");

	printf("\t\tXplusY=_mm_adds_epi16(XplusY,*ptY);\n");

	for ( s0=0;s0<iStateNumber;s0+=2)
	{
		

		printf("//\t\tCalculate State %d %d to %d %d\n",s0,s0+1,(s0>>1),(s0+iStateNumber)>>1);
		
		printf("//\t\tState %d Last0 is %d Last1 is %d\n",(s0),
			Trellis[(s0)].LastState0,
			Trellis[(s0)].LastState1);
		
		printf("//\t\tState %d Out0 is %d Out1 is %d\n",(s0),
			Trellis[(s0)].LastBranch0,
			Trellis[(s0)].LastBranch1);

		printf("//\t\tState %d Last0 is %d Last1 is %d\n",s0+1,
			Trellis[s0+1].LastState0,
			Trellis[s0+1].LastState1);
		
		printf("//\t\tState %d Out0 is %d Out1 is %d\n",(s0+1),
			Trellis[s0+1].LastBranch0,
			Trellis[s0+1].LastBranch1);
		
		
		if( Trellis[(s0>>1)].NextState0 == s0 )
			t1=0;
		else
			t1=1;
		if( Trellis[(s0>>1)].NextState0 == s0 )
			t2=Trellis[(s0>>1)].NextBranch0;
		else
			t2=Trellis[(s0>>1)].NextBranch1;

		
		int ttxy=t2*2+t1;

		switch( ttxy ) {
			case 0://x=-1,y=-1
				printf("//x=-1 y=-1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pBeta+%d);\n",s0);
				printf("\t\t_mm_ls1 = _mm_load_si128(pBeta+%d);\n",s0+1);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",(s0>>1)+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",((s0+iStateNumber)>>1)+iStateNumber);
				break;
			
			case 1://x=1,y=-1
				printf("//x=1 y=-1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pBeta+%d);\n",s0);
				printf("\t\t_mm_ls1 = _mm_load_si128(pBeta+%d);\n",s0+1);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",(s0>>1)+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",((s0+iStateNumber)>>1)+iStateNumber);
				break;
				
			case 2://x=-1,y=1
				printf("//x=-1 y=1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pBeta+%d);\n",s0);
				printf("\t\t_mm_ls1 = _mm_load_si128(pBeta+%d);\n",s0+1);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",(s0>>1)+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XsubY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XsubY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",((s0+iStateNumber)>>1)+iStateNumber);
				break;
			
			case 3://x=1,y=1
				printf("//x=1 y=1\n");
				printf("\t\t_mm_ls0 = _mm_load_si128(pBeta+%d);\n",s0);
				printf("\t\t_mm_ls1 = _mm_load_si128(pBeta+%d);\n",s0+1);
				printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",(s0>>1)+iStateNumber);
				printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,XplusY);\n");
				printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,XplusY);\n");
				printf("\t\t_mm_ls0_0 = _mm_max_epi16(_mm_ls0_0,_mm_ls0_1);\n");
				printf("\t\t_mm_store_si128(pBeta+%d,_mm_ls0_0);\n",((s0+iStateNumber)>>1)+iStateNumber);
				break;

			default:
				break;
		}
	}
	
	printf("\t\tpBeta+=%d;\n",iStateNumber);
	
	printf("\t\tif((i&0xff)==0xff)\n\t\t{\n");
	printf("\t\t\tzero=_mm_load_si128(pBeta);\n");
	for(j=1;j<8;j++)
	{
		printf("\t\t\t_mm_ls0 = _mm_load_si128(pBeta+%d);\n",j);
		printf("\t\t\t_mm_ls0 = _mm_subs_epi16(_mm_ls0,zero);\n");
		printf("\t\t\t_mm_store_si128(pBeta+%d,_mm_ls0);\n",j);
	}
	printf("\t\t\tzero=_mm_xor_si128(zero,zero);\n");
	printf("\t\t\t_mm_store_si128(pBeta,zero);\n");
	printf("\t\t}\n");



	printf("\t\tplX--;\n");
	printf("\t\tptY--;\n");

	
	printf("\t}\n");

	printf("\tpBeta-=%d;\n",iStateNumber);

	printf("\tpAlpha=(__m128i *)_mm_Alpha;\n");	
	
	printf("\t\tplX++;\n");
	printf("\t\tptY++;\n");

	printf("// Calculate Output \n");

	printf("\tfor( i=0;i<CODELENGTH-%d;i++ )\n\t{\n",iStateDeep);

	s0=0;

	printf("//\t\tCalculate State %d \n",s0);
		
	printf("//\t\tState %d Next0 is %d Next1 is %d\n",s0,
			Trellis[s0].NextState0,
			Trellis[s0].NextState1);
	printf("//\t\tState %d Out0 is %d Out1 is %d\n",s0,
			Trellis[s0].NextBranch0,
			Trellis[s0].NextBranch1);

	s1=Trellis[s0].NextState0;
	printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",s0);
	if( Trellis[s0].NextBranch0==0 )
		printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);\n");
	else
		printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);\n");
	printf("\t\t_mm_0 = _mm_add_epi16(_mm_ls0_0,pBeta[%d]);\n",s1);

	s1=Trellis[s0].NextState1;
	printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",s0);
	if( Trellis[s0].NextBranch1==0 )
		printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);\n");
	else
		printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);\n");
	printf("\t\t_mm_1 = _mm_add_epi16(_mm_ls0_1,pBeta[%d]);\n",s1);
	

	for ( s0=1;s0<iStateNumber;s0++)
	{
		

		printf("//\t\tCalculate State %d \n",s0);
		
		printf("//\t\tState %d Next0 is %d Next1 is %d\n",s0,
				Trellis[s0].NextState0,
				Trellis[s0].NextState1);
		printf("//\t\tState %d Out0 is %d Out1 is %d\n",s0,
				Trellis[s0].NextBranch0,
				Trellis[s0].NextBranch1);

		s1=Trellis[s0].NextState0;
		printf("\t\t_mm_ls0 = _mm_load_si128(pAlpha+%d);\n",s0);
		if( Trellis[s0].NextBranch0==0 )
			printf("\t\t_mm_ls0_0 = _mm_subs_epi16(_mm_ls0,*ptY);\n");
		else
			printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0,*ptY);\n");
		printf("\t\t_mm_ls0_0 = _mm_adds_epi16(_mm_ls0_0,pBeta[%d]);\n",s1);
		printf("\t\t_mm_0 = _mm_max_epi16(_mm_0,_mm_ls0_0);\n");
		
		s1=Trellis[s0].NextState1;
		printf("\t\t_mm_ls1 = _mm_load_si128(pAlpha+%d);\n",s0);
		if( Trellis[s0].NextBranch1==0 )
			printf("\t\t_mm_ls0_1 = _mm_subs_epi16(_mm_ls1,*ptY);\n");
		else
			printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls1,*ptY);\n");
		printf("\t\t_mm_ls0_1 = _mm_adds_epi16(_mm_ls0_1,pBeta[%d]);\n",s1);
		printf("\t\t_mm_1 = _mm_max_epi16(_mm_1,_mm_ls0_1);\n");
		
	}
	printf("\t\t_mm_1 = _mm_sub_epi16(_mm_1,_mm_0);\n");
	printf("\t\t_mm_store_si128(ptX,_mm_1);\n");
	printf("\t\tpBeta-=%d;\n",iStateNumber);
	printf("\t\tpAlpha+=%d;\n",iStateNumber);
	printf("\t\tptX++;\n");
	printf("\t\tptY++;\n");

	
	printf("\t}\n");
	
	printf("}\n");
}
