#include"Interleaver.h"
#include<stdio.h>
#include<sse2Map.h>


main()
{

	int i,j;
	Interleaver aIntl(CODELENGTH-3);
	CRC aCRC((CODELENGTH-3)*8);
	printf("unsigned short InterleaverTable[%d]={\n",CODELENGTH-3);
	printf("\t");
	for( i=0;i<CODELENGTH-3-1;i++ )
	{
		printf("0x%04.4x, ",(int)aIntl.Inter_Table[i]);
		if( (i%16) == 15 )
			printf("\n\t");
	}
	printf("0x%04.4x \n};\n",(int)aIntl.Inter_Table[i]);
	
	printf("unsigned short CRCTable[%d]={\n",(CODELENGTH-3)*8);
	printf("\t");
	for( i=0;i<(CODELENGTH-3)*8-1;i++ )
	{
		printf("0x%04.4x, ",(int)aCRC.CRC_Table[i]);
		if( (i%16) == 15 )
			printf("\n\t");
	}
	printf("0x%04.4x \n};\n",(int)aCRC.CRC_Table[i]);
	
	printf("unsigned short sse2CRCTable[] __attribute__((aligned(16)))= {\n");
	
	for( i=0;i<CODELENGTH-3-1;i++ )
	{
		for( j=0;j<8;j++ )
			printf("0x%04.4x, ",(int)aCRC.CRC_Table[i+j*(CODELENGTH-3)]);
		printf("\n\t");
	}
	for( j=0;j<7;j++ )
			printf("0x%04.4x, ",(int)aCRC.CRC_Table[i+j*(CODELENGTH-3)]);
	printf("0x%04.4x \n};\n",(int)aCRC.CRC_Table[i+j*(CODELENGTH-3)]);

}