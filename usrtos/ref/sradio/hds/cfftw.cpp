#include<fftw3.h>
#include<cfftw.h>
#include<math.h>

extern "C" fftwf_plan fftwf_plan_dft_1d(int , fftwf_complex *,fftwf_complex *, int , unsigned int );
extern "C" void fftwf_execute(fftwf_plan);
extern "C" void fftwf_destroy_plan(fftwf_plan);

CFFTW::CFFTW( )
{
}

void CFFTW::CFFTW_Init( float *input, float *output, int dir, int N )
{
	printf( "CFFTW.0\t\t%d\t%d\n", dir, N );
	
	in = (fftwf_complex *)input;
	out = (fftwf_complex *)output;

	point = N;
	if( dir==1 )
	{
		plan = fftwf_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_PATIENT );
	}
	else
	{
		plan = fftwf_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_PATIENT );
	}
	direct=dir;
}


CFFTW::~CFFTW()
{
	fftwf_destroy_plan(plan); 
}

void CFFTW::doit()
{
	fftwf_execute(plan); 
}

void CFFTW::setf( int f )
{
	float p;
	int i;
	for( i=0;i<point;i++ )
	{
		p=2.*3.14159265*(float)i*(float)f/(float)point;
		if( direct==1 )
		{
			in[i][0]=cos(p);
			in[i][1]=sin(p);
		}
		else
		{
			in[i][0]=cos(-p);
			in[i][1]=sin(-p);
		}
		
	}	
}

int CFFTW::findf()
{
	float p,max;
	int i,maxi;
	maxi=0;
	max=out[0][0]*out[0][0]+out[0][1]*out[0][1];
	for( i=1;i<point;i++ )
	{
		p=out[i][0]*out[i][0]+out[i][1]*out[i][1];
		if( p>max )
		{
			max=p;
			maxi=i;	
		}
	}
	return maxi;
}
