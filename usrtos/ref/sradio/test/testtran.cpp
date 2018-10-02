#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <xmmintrin.h>
#define M 10000
int K1=0;
int K2=4096;

typedef struct { unsigned long t[2]; } timing;
#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
#define timing_diff(x,y) (((x)->t[0] - (double) (y)->t[0]) + 4294967296.0 * ((x)->t[1] - (double) (y)->t[1]))
timing before, after;
double clocktick;
    	
void tran1( char *ca_in, char *ca_out )
{
#include"transse.c"	
}
void tran2( char *ca_in, char *ca_out )
{
#include"tranmove.c"	
}
void tran3( char *ca_in, char *ca_out )
{
	int i,j;
	for( i=0;i<64;i++ )
		for( j=0;j<64;j++ )
			ca_out[i*64+j]=ca_in[i+j*64];
}

void tran4( char *ca_in, char *ca_out )
{
#include"transseonce.c"	
}

char  in[4096]  __attribute__((aligned(16)));
char out1[4096]  __attribute__((aligned(16)));
char out2[4096]  __attribute__((aligned(16)));
char out3[4096]  __attribute__((aligned(16)));
char out4[4096]  __attribute__((aligned(16)));

void diff( char *ca0, char *ca1 )
{
	int i;
	for( i=K1;i<(K1+K2)%4096;i++ )
		if( ca0[i]!=ca1[i] )
			printf("%d %d %d %d\n",i>>6,i&63,(int)ca0[i]&0xff,(int)ca1[i]&0xff );
}
int main(int argc, char **argv)
{
    int count;
    clocktick= 2412.742e6;		
	while(1)
    {
		for( count=0;count<4096;count++ )
			in[count]=rand()&0xff;
    	timing_now(&before);
		for( count=0;count<M;count++ )
			tran3(in,out1);
		timing_now(&after);
		printf("for tran need %lf %le\n",
			timing_diff(&after,&before)/M,
			timing_diff(&after,&before)/clocktick/M);
		
		timing_now(&before);
		for( count=0;count<M;count++ )
			tran1(in,out2);
		timing_now(&after);
		printf("sse tran need %lf %le\n",
			timing_diff(&after,&before)/M,
			timing_diff(&after,&before)/clocktick/M);
		diff( out1, out2 );
		
		timing_now(&before);
		for( count=0;count<M;count++ )
			tran2(in,out3);
		timing_now(&after);
		printf("move tran need %lf %le\n",
			timing_diff(&after,&before)/M,
			timing_diff(&after,&before)/clocktick/M);
		diff( out1, out3 );
		
		
		timing_now(&before);
		for( count=0;count<M;count++ )
			tran4(in,out4);
		timing_now(&after);
		printf("sse 32 reg tran need %lf %le\n",
			timing_diff(&after,&before)/M,
			timing_diff(&after,&before)/clocktick/M);
		diff( out1, out4 );
		K1+=K2;
		K1%=4096;
	}
}


