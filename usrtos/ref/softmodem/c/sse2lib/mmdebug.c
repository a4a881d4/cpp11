#include<xmmintrin.h>
#include"mmdebug.h"
void _mm_printf(__m128i a,int flag)
{
//	char buf[16] __attribute__((aligned(16)));
	int *ip;
	short *sp;
	char *cp;
	int i;
//	a=_mm_set_epi16(1,1,1,1,1,1,1,1);	
//	_mm_store_si128(buf,a);
	switch( flag )
	{
		case EPI8:
			cp=(char *)&a;
			for( i=0;i<16;i++ )
				printf("%d ",(int)cp[i]);
			break;
		case EPI16:		
			sp=(short *)&a;
			for( i=0;i<8;i++ )
				printf("%d ",(int)sp[i]);
			break;
		case EPI32:		
			ip=(int *)&a;
			for( i=0;i<4;i++ )
				printf("%d ",(int)ip[i]);
			break;
		default:
			break;
	}
	printf("\n");
}

		
