#ifndef _CFFTW_H
#define _CFFTW_H

#include<fftw3.h>
class CFFTW {
private:
	fftwf_complex *in;
	fftwf_complex *out;
	fftwf_plan plan;
	int point;
	int direct;
public:
	
	CFFTW( );
	~CFFTW();
	void CFFTW_Init( float *input, float *output, int dir, int N );
	void doit();
	void setf( int f );
	int findf();
	
};

#endif
