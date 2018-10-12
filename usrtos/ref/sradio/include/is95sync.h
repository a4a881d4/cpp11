#ifndef __IS95_SYNC_H
#define __IS95_SYNC_H

#include<complexoperation.h>

typedef struct is95complex {
	float i;
	float q;
} is95c;

class CFFTW;
class CIS95Sync {
	public:
		CIS95Sync();
		virtual ~CIS95Sync();
		void setData( char * );
		int find( int from, int to, float& max );
		int sync( int freq, int& from, int to, is95c *chn );
#ifndef __IS95_SYNC_TEST		
	private:
#endif
		is95c *pn_fft;
		is95c *baseband_data;
		is95c *baseband_fft;
		is95c *rel_in;
		is95c *rel_out;
		is95c *relN2;
		int findMax( float& max );
		void shiftFreq( int shift );
		CFFTW *dataFFT;
		CFFTW *relIFFT;
		int findOnce( int freqShift, float& max );
		ComplexOperation aCop;
};		
		
#endif

