#include<stdio.h>

short _mm_inI[8] __attribute__((aligned(16)));
short _mm_inQ[8] __attribute__((aligned(16)));

short _mm_outI[8] __attribute__((aligned(16)));
short _mm_outQ[8] __attribute__((aligned(16)));


extern short _mm_GHGS_Ibuf0[64];
extern short _mm_GHGS_Qbuf0[64];
extern short _mm_GHGS_Ibuf1[2048];
extern short _mm_GHGS_Qbuf1[2048];
extern short _mm_GHGS_Ibuf2[16];
extern short _mm_GHGS_Qbuf2[16];
extern short _mm_GHGS_Ibuf3[32];
extern short _mm_GHGS_Qbuf3[32];
extern short _mm_GHGS_Ibuf4[256];
extern short _mm_GHGS_Qbuf4[256];
extern short _mm_GHGS_Ibuf5[1024];
extern short _mm_GHGS_Qbuf5[1024];
extern short _mm_GHGS_Ibuf6[512];
extern short _mm_GHGS_Qbuf6[512];
extern short _mm_GHGS_Ibuf7[4096];
extern short _mm_GHGS_Qbuf7[4096];
extern short _mm_GHGS_Ibuf8[8192];
extern short _mm_GHGS_Qbuf8[8192];
extern short _mm_GHGS_Ibuf9[128];
extern short _mm_GHGS_Qbuf9[128];
extern short _mm_GHGS_Ibuf10[16384];
extern short _mm_GHGS_Qbuf10[16384];
extern "C" void GHGS( void *inI, void *inQ, void *outI, void *outQ );

int main()
{

	int i,j,a,s0,y,t,s1,lt,k;
	
	int lx,ly,ret;
	
	FILE *fpi = fopen("dat//GHGS.txt","wt");
	
	for( i=0;i<8;i++ )
	{
		_mm_inI[i]=0;
		_mm_inQ[i]=0;
	}
	
	_mm_inI[0]=1;
	
	GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
	fprintf(fpi,"%d %d\n",(int)_mm_outI[0],(int)_mm_outQ[0]);
	printf("%d %d\n",(int)_mm_GHGS_Ibuf1[0],(int)_mm_GHGS_Qbuf1[0]);
	_mm_inI[0]=0;
	for( i=1;i<8192;i++)
	{
		GHGS(_mm_inI,_mm_inQ,_mm_outI,_mm_outQ);
		fprintf(fpi,"%d %d\n",(int)_mm_outI[0],(int)_mm_outQ[0]);
	}
	fclose(fpi);
	
}
