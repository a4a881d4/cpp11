#include <complexoperation.h>
#include <stdio.h>

ComplexOperation::ComplexOperation( )
{
    m128_MulSign = _mm_set_ps( 1.0, -1.0, 1.0, -1.0 );
    m128_ConjSign = _mm_set_ps( -1.0, 1.0, -1.0, 1.0 );
}

ComplexOperation::~ComplexOperation( )
{
}



void ComplexOperation::char2Float( unsigned char * ucp_in, float * fp_out, int len )
{
#if 0
    int i;
    char * cp_Temp0;
    __m64 m64_Temp0;
    __m128 * m128p_Temp0;
    __m128 m128_Q;
    m128_Q = _mm_set_ps( 256., 256., 256., 256. );
    for ( i = 0; i <len/4; i++ )
    {
        cp_Temp0 = ( char * )( ucp_in + i * 4 );
        m64_Temp0 = _mm_setr_pi8( 0, *( cp_Temp0 ), 0, *( cp_Temp0 + 1 ), 0, *( cp_Temp0 + 2 ), 0, *( cp_Temp0 + 3 ) );
        m128p_Temp0 = ( __m128 * )( fp_out + i * 4 );
        *m128p_Temp0 = _mm_cvtpi16_ps( m64_Temp0 );
        *m128p_Temp0 = _mm_div_ps( *m128p_Temp0, m128_Q );
    }
    _mm_empty( );
	for( ;len>0;len-=4,ucp_in+=4,fp_out+=4 )
	{
		*(fp_out+0)=(float)*(ucp_in+0);
		*(fp_out+1)=(float)*(ucp_in+1);
		*(fp_out+2)=(float)*(ucp_in+2);
		*(fp_out+3)=(float)*(ucp_in+3);
	}
#else
	__m128i t1,t2,t3,t4,t5,t6,t7,*p;
	__m128 *out;
	int i;
	p=(__m128i*)ucp_in;
	out = (__m128 *)fp_out;
	len/=16;
	while(len) {
		t1=p[0];
		t2=_mm_unpacklo_epi8(t1,t1);
		t3=_mm_unpackhi_epi8(t1,t1);
		t4=_mm_unpacklo_epi16(t2,t2);
		t5=_mm_unpackhi_epi16(t2,t2);
		t6=_mm_unpacklo_epi16(t3,t3);
		t7=_mm_unpackhi_epi16(t3,t3);
		t4=_mm_srai_epi32(t4,24);
		t5=_mm_srai_epi32(t5,24);
		t6=_mm_srai_epi32(t6,24);
		t7=_mm_srai_epi32(t7,24);
		out[0]=_mm_cvtepi32_ps(t4);
		out[1]=_mm_cvtepi32_ps(t5);
		out[2]=_mm_cvtepi32_ps(t6);
		out[3]=_mm_cvtepi32_ps(t7);
		out+=4;
		len--;
		p++;
	}
#endif
}
