#ifndef _COMPLEXOPERATION_H
#define _COMPLEXOPERATION_H

#include <xmmintrin.h>

typedef __m128 _mm_real;
typedef struct {
  _mm_real re;
  _mm_real im;
} _mm_complex;

class ComplexOperation
{
public:
    ComplexOperation( );
    ~ComplexOperation( );

	
inline void CMUL(_mm_complex a0, _mm_real wre, _mm_real wim) 
{
	_mm_real t1,t2,t3,t4; 
	t1=_mm_mul_ps(a0.re,wre); 
	t2=_mm_mul_ps(a0.im,wim); 
	t3=_mm_mul_ps(a0.re,wim); 
	t4=_mm_mul_ps(a0.im,wre); 
	a0.re=_mm_add_ps(t1 , t2); 
	a0.im=_mm_sub_ps(t4 , t3); 
};

inline __m128 Float_2_m128_Load( float * a ) 
{
    __m128 m128_Q;
    m128_Q = _mm_load_ps( a );
    return m128_Q;
};

inline void m128_2_Float_Store( __m128 m128_A, float * a ) 
{
    _mm_store_ps( a, m128_A );
};

inline __m128 ComplexAdd_2( __m128 m128_A, __m128 m128_B )
{
    __m128 m128_Q;
    m128_Q = _mm_add_ps( m128_A , m128_B );
    return m128_Q;
};

inline __m128 ComplexSub_2( __m128 m128_A, __m128 m128_B )
{
    __m128 m128_Q;
    m128_Q = _mm_sub_ps( m128_A , m128_B );
    return m128_Q;
};

inline __m128 ComplexMul_2( __m128 m128_A, __m128 m128_B )
{
    __m128 m128_Q;
    __m128 m128_Temp0, m128_Temp1, m128_Temp2, m128_Temp3;
    m128_Temp0 = _mm_shuffle_ps( m128_A, m128_A, _MM_SHUFFLE( 2, 2, 0, 0 ) );
    m128_Temp1 = _mm_shuffle_ps( m128_A, m128_A, _MM_SHUFFLE( 3, 3, 1, 1 ) );
    m128_Temp2 = _mm_shuffle_ps( m128_B, m128_B, _MM_SHUFFLE( 2, 3, 0, 1 ) );

    m128_Temp0 = _mm_mul_ps( m128_Temp0, m128_B );
    m128_Temp1 = _mm_mul_ps( _mm_mul_ps( m128_Temp2, m128_MulSign ), m128_Temp1 );
    m128_Q = _mm_add_ps( m128_Temp0 , m128_Temp1 );
    return m128_Q;
};

inline __m128 ComplexMulConj_2( __m128 m128_A, __m128 m128_B )
{
    __m128 m128_Q;
    __m128 m128_ConjB;

    m128_ConjB = _mm_mul_ps( m128_B, m128_ConjSign );
    m128_Q = ComplexMul_2( m128_A, m128_ConjB );
    return m128_Q;
};


inline __m128 ComplexDiv_2( __m128 m128_A, __m128 m128_B )
{
    __m128 m128_Q;
    __m128 m128_ConjB, m128_A_Mul_ConjB, m128_B_Mul_ConjB, m128_Temp0;

    m128_ConjB = _mm_mul_ps( m128_B, m128_ConjSign );
    m128_A_Mul_ConjB = ComplexMul_2( m128_A, m128_ConjB );
    m128_B_Mul_ConjB = ComplexMul_2( m128_B, m128_ConjB );
    m128_Temp0 = _mm_shuffle_ps( m128_B_Mul_ConjB, m128_B_Mul_ConjB, _MM_SHUFFLE( 2, 2, 0, 0 ) );
    m128_Q = _mm_div_ps( m128_A_Mul_ConjB, m128_Temp0 );
    return m128_Q;
};

	void char2Float( unsigned char * ucp_in, float * fp_out, int len );
private:
    __m128 m128_MulSign;
    __m128 m128_ConjSign;

};

#endif
