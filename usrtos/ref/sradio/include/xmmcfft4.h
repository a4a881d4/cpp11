#ifndef _XMMCFFT4
#define _XMMCFFT4

#include<xmmintrin.h>
#include<math.h>

#define NEWOPT
#undef NEWPASS
#undef GENTRANS
#define PASSNUM 2
#define PASSTIME 8

typedef __m128 _mm_real;


typedef struct {
  _mm_real re;
  _mm_real im;
} _mm_complex;

#ifdef  __cplusplus
extern "C" {
#endif

void initd();
void _mm_c32(register _mm_complex *a);
void _mm_c64(register _mm_complex *a);
void _mm_c128(register _mm_complex *a);
void _mm_c256(register _mm_complex *a);
void _mm_c512(register _mm_complex *a);
void _mm_c1024(register _mm_complex *a);
void _mm_c2048(register _mm_complex *a);

static inline void _mm_u4(register _mm_complex *a);
static inline void _mm_u16(register _mm_complex *a);
void _mm_u32(register _mm_complex *a);
void _mm_u64(register _mm_complex *a);
void _mm_u128(register _mm_complex *a);
void _mm_u256(register _mm_complex *a);
void _mm_u512(register _mm_complex *a);
void _mm_u1024(register _mm_complex *a);
void _mm_u2048(register _mm_complex *a);

void _mm_fftfreq_ctable( int *table, int point );

#define CMUL(a0,wre,wim) { \
	register _mm_real t1,t2,t3,t4; \
	t1=_mm_mul_ps(a0.re,wre); \
	t2=_mm_mul_ps(a0.im,wim); \
	t3=_mm_mul_ps(a0.re,wim); \
	t4=_mm_mul_ps(a0.im,wre); \
	a0.re=_mm_sub_ps(t1 , t2); \
	a0.im=_mm_add_ps(t3 , t4); \
	}
	
#define CMULT(a0,wre,wim) { \
	register _mm_real t1,t2,t3,t4; \
	t1=_mm_mul_ps(a0.re,wre); \
	t2=_mm_mul_ps(a0.im,wim); \
	t3=_mm_mul_ps(a0.re,wim); \
	t4=_mm_mul_ps(a0.im,wre); \
	a0.re=_mm_add_ps(t1 , t2); \
	a0.im=_mm_sub_ps(t4 , t3); \
}




#define FFT(N,args) { \
  _mm_fft_##N args; \
  }
#define IFFT(N,args) { \
  _mm_ifft_##N args; \
}




#ifdef  __cplusplus
}
#endif


#endif
