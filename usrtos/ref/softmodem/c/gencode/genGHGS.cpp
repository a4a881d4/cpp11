#include<stdio.h>

#define LENGTH 11
int D[]={4,128,1,2,16,64,32,256,512,8,1024};
int W[]={-1,-1,1,2,-2,1,-2,-2,-2,-1,2};


int main()
{
	int i;
	printf("#include<xmmintrin.h>\n");
	for( i=0;i<LENGTH;i++ )
	{
		printf("short _mm_GHGS_Ibuf%d[%d] __attribute__((aligned(16)));\n",i,D[i]*2*8);
		printf("short _mm_GHGS_Qbuf%d[%d] __attribute__((aligned(16)));\n",i,D[i]*2*8);
	}
	for( i=0;i<LENGTH;i++ )
	{
		printf("__m128i * _mm_GHGS_pmIbuf%d=(__m128i *)_mm_GHGS_Ibuf%d;\n",i,i);
		printf("__m128i * _mm_GHGS_pmQbuf%d=(__m128i *)_mm_GHGS_Qbuf%d;\n",i,i);
	}
	for( i=0;i<LENGTH;i++ )
	{
		printf("int _GHGS_pos%d;\n",i);
	}
	printf("void GHGS( void *inI, void *inQ, void *outI, void *outQ )\n");
	printf("{\n\t__m128i Ibi,Qbi,Ibo,Qbo,Imi,Qmi,Imo,Qmo,*pbuf;\n");
	printf("\tIbi=_mm_load_si128(inI);\n");
	printf("\tQbi=_mm_load_si128(inQ);\n");
	printf("\tImi=_mm_load_si128(inI);\n");
	printf("\tQmi=_mm_load_si128(inQ);\n");
	
	for( i=0;i<LENGTH;i++ )
	{
		printf("// The %dth Delay = %d\n",i,D[i]);
		printf("\tpbuf=_mm_GHGS_pmIbuf%d+_GHGS_pos%d;\n",i,i);
		printf("\tIbo=_mm_load_si128(pbuf);\n");
		printf("\t_mm_store_si128(pbuf,Ibi);\n");
		printf("\tpbuf=_mm_GHGS_pmQbuf%d+_GHGS_pos%d;\n",i,i);
		printf("\tQbo=_mm_load_si128(pbuf);\n");
		printf("\t_mm_store_si128(pbuf,Qbi);\n");
		printf("\t_GHGS_pos%d++;\n",i);
		printf("\t_GHGS_pos%d&=%d;\n",i,D[i]*2-1);
		printf("\tImo=Imi;\n");
		printf("\tQmo=Qmi;\n");
		switch( W[i] ) {
			case -1:
				printf(" // weight = -1 \n");
				printf("\tIbi=_mm_subs_epi16(Ibo,Imo);\n");
				printf("\tQbi=_mm_subs_epi16(Qbo,Qmo);\n");
				printf("\tImi=_mm_adds_epi16(Ibo,Imo);\n");
				printf("\tQmi=_mm_adds_epi16(Qbo,Qmo);\n");
				break;
			case 1:
				printf(" // weight = 1 \n");
				printf("\tIbi=_mm_adds_epi16(Ibo,Imo);\n");
				printf("\tQbi=_mm_adds_epi16(Qbo,Qmo);\n");
				printf("\tImi=_mm_subs_epi16(Ibo,Imo);\n");
				printf("\tQmi=_mm_subs_epi16(Qbo,Qmo);\n");
				break;
			case 2:
				printf(" // weight = j \n");
				printf("\tIbi=_mm_adds_epi16(Ibo,Qmo);\n");
				printf("\tQbi=_mm_subs_epi16(Qbo,Imo);\n");
				printf("\tImi=_mm_subs_epi16(Ibo,Qmo);\n");
				printf("\tQmi=_mm_adds_epi16(Qbo,Imo);\n");
				break;
			case -2:
				printf(" // weight = -j \n");
				printf("\tIbi=_mm_subs_epi16(Ibo,Qmo);\n");
				printf("\tQbi=_mm_adds_epi16(Qbo,Imo);\n");
				printf("\tImi=_mm_adds_epi16(Ibo,Qmo);\n");
				printf("\tQmi=_mm_subs_epi16(Qbo,Imo);\n");
				break;
			default:
				break;
		}
		if( (i%2)==0 )
		{
			
			printf("\tIbi=_mm_srai_epi16(Ibi,1);\n");
			printf("\tQbi=_mm_srai_epi16(Qbi,1);\n");
			printf("\tImi=_mm_srai_epi16(Imi,1);\n");
			printf("\tQmi=_mm_srai_epi16(Qmi,1);\n");
			
		}
	}
	
	printf("\t_mm_store_si128(outI,Ibi);\n");
	printf("\t_mm_store_si128(outQ,Qbi);\n");
	printf("}\n//");
}

		


	


