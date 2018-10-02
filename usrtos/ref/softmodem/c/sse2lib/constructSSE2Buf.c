#include"constructSSE2Buf.h"
#include"channelEst.h"

short _mm_inI[INPUTLENGTH*8] __attribute__((aligned(16)));
short _mm_inQ[INPUTLENGTH*8] __attribute__((aligned(16)));



void constructSSE2Buf( RECEIVER_BUF_IND_t *buf )
{
	
	int i,l;
	
	buf->curBlk=0;
	buf->curPos=0;
	buf->curBuf=buf->point[0];
	
	for( i=0;i<INPUTLENGTH;i++ )
	{
		_mm_inI[i*8]=(short)buf->curBuf[buf->curPos];
		_mm_inQ[i*8]=(short)buf->curBuf[buf->curPos+1];
		buf->curPos+=2;
		if( buf->curPos>=buf->length[buf->curBlk] )
		{
			buf->curPos=0;
			buf->curBlk++;
			buf->curBuf=buf->point[buf->curBlk];
		}
		
	}
	for( l=1;l<8;l++ )
	{
		for( i=CHANNELLENGTH;i<INPUTLENGTH;i++ )
		{
			_mm_inI[i*8+l]=(short)buf->curBuf[buf->curPos];
			_mm_inQ[i*8+l]=(short)buf->curBuf[buf->curPos+1];
			buf->curPos+=2;
			if( buf->curPos>=buf->length[buf->curBlk] )
			{
				buf->curPos=0;
				buf->curBlk++;
				buf->curBuf=buf->point[buf->curBlk];
			}
		}
	}
	for( l=1;l<8;l++ )
	{
		for( i=0;i<CHANNELLENGTH;i++ )
		{
			_mm_inI[i*8+l]=_mm_inI[(i+SLOTLENGTH)*8+l-1];
			
			_mm_inQ[i*8+l]=_mm_inQ[(i+SLOTLENGTH)*8+l-1];
			
			
		}
	}
	buf->buf_I=_mm_inI;
	buf->buf_Q=_mm_inQ;
}