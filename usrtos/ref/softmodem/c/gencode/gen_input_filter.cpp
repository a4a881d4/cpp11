typedef float real32_T;
#include<stdio.h>
#include<math.h>
#include"inputfilter.h"
float BB[BL];
int main()
{
	int center=(BL-1)/2;
	int start=0;
	int pos=0;
	float pow=0.;
	int a;
	for( a=0;a<BL;a++ )
		pow+=fabs(B[a]);
    for( a=0;a<BL;a++ )
		BB[a]=B[a]/pow*1024.+0.5;
	printf("#include<xmmintrin.h>\n");
	printf("short coeff[] __attribute__((aligned(16)))= {\n");
	for( a=0;a<BL;a++ )
	{
		int ba=(int)(BB[a]);
		printf(" %d, %d, %d, %d, %d, %d, %d, %d ",ba,ba,ba,ba,ba,ba,ba,ba);
		if( a!=BL-1 )
			printf(",\n");
		else
			printf("};\n");
	}
	printf("__m128i *pmCoeff=(__m128i *)coeff;\n");
	printf("void interp( void *indata, void *outdata, int DataLength )\n"); 
	printf("{\n\tint i;\n\t__m128i *pdata =indata;\n\t__m128i *pfilterout =outdata;\n\t__m128i mA,mB,alu;\n");
	printf("\t__m128i *pfdata;\n\t__m128i *pfcoeff;\n");	
	printf("\tfor( i=0;i<DataLength;i+=8 )\n\t{\n");
	for( int i=0;i<13;i++ )
	{
		printf("//The %d coeff\n",i);
		printf("//pos = %d \n",pos);
		
		printf("\t\talu=_mm_xor_si128(alu,alu);\n");
		int ss=0;
		for(int j=start;j<BL;j+=13)
		{
			int ba=(int)BB[j];
			printf("//%d \n",ba);
			if( ba!=0 && ba!=1 && ba!=-1 )
			{
				printf("\t\tpfcoeff=pmCoeff+%d;\n",j);
				printf("\t\tpfdata=pdata+%d;\n",ss+pos);
				printf("\t\tmA=_mm_load_si128(pfcoeff);\n");
				printf("\t\tmB=_mm_load_si128(pfdata);\n");
				printf("\t\tmB=_mm_mullo_epi16(mA,mB);\n");
				printf("\t\talu=_mm_adds_epi16(alu,mB);\n");
			}
			else if( ba==1 )
			{
				printf("\t\tpfdata=pdata+%d;\n",ss+pos);
				printf("\t\tmB=_mm_load_si128(pfdata);\n");
				printf("\t\talu=_mm_adds_epi16(alu,mB);\n");
			}
			else if( ba==-1 )
			{
				printf("\t\tpfdata=pdata+%d;\n",ss+pos);
				printf("\t\tmB=_mm_load_si128(pfdata);\n");
				printf("\t\talu=_mm_subs_epi16(alu,mB);\n");
			}
			ss++;
		}

		printf("\t\t_mm_store_si128(pfilterout,alu);\n");
		printf("\t\tpfilterout++;\n");
		
		start+=8;
		if( start>13 )
		{
			start-=13;
			pos++;
		}
		printf("\n");

	}
	printf("\t\tpdata+=8;\n");
	printf("\t}\n");
	printf("}\n");
	
	return(0);
}
