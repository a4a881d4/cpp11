#include<stdio.h>
#include<stdlib.h>

#include<channelEst.h>



void interp( void *indata, void *outdata, int DataLength );
/*
void GHGS( void *inI, void *inQ, void *outI, void *outQ );
void _mm_cabs( void *inI, void *inQ, void *out, int Length );
void _mm_findMax( void *inI, int Length, int *max, int *avg );
*/
void _mm_demodu( void *inI, void *inQ, void *chI, void *chQ, void *out, int coePos[],int coeNum, int Length );
/*
void _mm_sumRow( void *in, void * out, int Length );
*/

void _mm_Map( void *lX, void *tY, void *tX );

extern short coeff[];

int cpos[128];

int main()
{

	int i,j,max,avg;
	
	void *data8MI = malloc( INPUTLENGTH*16);
	void *data13MI = malloc( INPUTLENGTH*16 );
	void *data8MQ = malloc( INPUTLENGTH*16);
	void *data13MQ = malloc( INPUTLENGTH*16 );
	void *dataABS = malloc( INPUTLENGTH*16);
	void *dataCHI = malloc( INPUTLENGTH*16);
	void *dataCHQ = malloc( INPUTLENGTH*16);
	void *dataDemodu = malloc( INPUTLENGTH*16);
	void *dataSUMABS = malloc( INPUTLENGTH*16);
	char *d8i,*d13i,*d8q,*d13q,*dabs,*dchi,*dchq,*dout,*dsumabs;
	short *wabs;
	
	RET_CHANNEL_ESTIMATE_t channel;
	
	for( i=0;i<100000000;i++ )
	{
		d8i = (char *)(((int)data8MI+15)&0xfffffff0); 
		d13i =(char *)(((int)data13MI+15)&0xfffffff0);
		d8q = (char *)(((int)data8MQ+15)&0xfffffff0); 
		d13q =(char *)(((int)data13MQ+15)&0xfffffff0);
		
		dout =(char *)(((int)dataDemodu+15)&0xfffffff0);
		
		interp( d8i, d13i, INPUTLENGTH*8/13 );
		interp( d8q, d13q, INPUTLENGTH*8/13 );
	
		channelEst(d13i, d13q, &channel );
		
		_mm_demodu( d13i,d13q,(void *)channel.pITap,(void *)channel.pQTap, (void *)dout, channel.pos,channel.iTapNum, 2048 );
		
		for( j=0;j<12;j++ )
			_mm_Map( dout, d13i, d8i );
		if( (i%10000)==0 )
			printf("%d\n",i);
	}	
	
/*
	wabs =(short *)(((int)dataABS+15)&0xfffffff0);
	_mm_cabs( (void *)coeff,(void *)coeff, (void *)wabs,127 );
	_mm_sumRow( (void *)coeff, (void *)wabs,120 );

	_mm_findMax((void*)wabs,127,&max,&avg);	
	printf("%d is max %d is avg \n",max,avg);
	for( i=0;i<127/8;i++ )
	{
		for( j=0;j<8;j++ )
			printf("%d ",(int)(wabs[i*8+j]));
		printf("\n");
	}
	
	for( i=0;i<128;i++ )
		cpos[i]=i;
		
	for( i=0;i<100000000;i++ )
	{
		d8i = (char *)(((int)data8MI+15)&0xfffffff0); 
		d13i =(char *)(((int)data13MI+15)&0xfffffff0);
		d8q = (char *)(((int)data8MQ+15)&0xfffffff0); 
		d13q =(char *)(((int)data13MQ+15)&0xfffffff0);
		dabs =(char *)(((int)dataABS+15)&0xfffffff0);
		dchi =(char *)(((int)dataCHI+15)&0xfffffff0);
		dchq =(char *)(((int)dataCHQ+15)&0xfffffff0);
		interp( d8i, d13i, 2900 );
		interp( d8q, d13q, 2900 );
		
		for( j=0;j<4096;j++ )
		{
			
			GHGS((void *)d13i,(void *)d13q,(void *)dchi,(void *)dchq );
			d13i+=16;
			d13q+=16;
		}
		for( j=0;j<768;j++ )
		{
			
			GHGS((void *)d13i,(void *)d13q,(void *)dchq,(void *)dchi );
			d13i+=16;
			d13q+=16;
			dchi+=16;
			dchq+=16;
		}
		dout =(char *)(((int)dataDemodu+15)&0xfffffff0);
		d8i = (char *)(((int)data8MI+15)&0xfffffff0); 
		d13i =(char *)(((int)data13MI+15)&0xfffffff0);
		d8q = (char *)(((int)data8MQ+15)&0xfffffff0); 
		d13q =(char *)(((int)data13MQ+15)&0xfffffff0);
		dabs =(char *)(((int)dataABS+15)&0xfffffff0);
		dchi =(char *)(((int)dataCHI+15)&0xfffffff0);
		dchq =(char *)(((int)dataCHQ+15)&0xfffffff0);
		dsumabs =(char *)(((int)dataSUMABS+15)&0xfffffff0);
		
		_mm_cabs((void *)dchi,(void *)dchq,(void *)dabs,768);
		
		

		_mm_demodu( (void *)d13i,(void *)d13q,(void *)dchq,(void *)dchi, (void *)dout, cpos,32, 2048 );
		if( (i%10000)==0 )
			printf("%d\n",i);
	}

	 
*/


}
