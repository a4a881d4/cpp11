#include<xmmintrin.h>

int pCheck(int);
int pCheck(int a)
{
	int b=0;
	int i;
	for(i=0;i<18;i++)
		b^=(a>>i)&1;
	return b;
}

void genCode(void *codebuf, int uPNnumber)
{

	int x,y;
	int i,q;
	int c,d;
	unsigned char *code = ( unsigned char *)codebuf;
	x=1;
	y=0x3ffff;
	d=0;
	for( c=0;c<2048+(int)uPNnumber;c++ )
	{
		q=pCheck((x&0x8050)^(y&0xff60));
		i=(x&1)^(y&1);
		x|=pCheck(x&0x81)<<18;
		y|=pCheck(y&0x4a1)<<18;
		x>>=1;
		y>>=1;
	
		if( c<(int)uPNnumber )
		{
			d=0;
			y=0x3ffff;
		}
		else
		{
			if( (d&3) == 0 )
				code[d/4]=0;
			
			{
				code[d/4]|=i<<((d&3)*2);
				code[d/4]|=q<<((d&3)*2+1);
			}
			d++;
		}
	}
}

void Descrambler( void *in, void *code, int Length )
{
	
	int i,j;
	
	unsigned char *pc = (unsigned char *)code;
	unsigned char c;
	__m128i *pbuf,_m_i,_m_q,_m_oi,_m_oq;
	__m128i nones;
	nones=_mm_set_epi16(-1,-1,-1,-1,-1,-1,-1,-1);
	pbuf=(__m128i *)in;
	for( i=0;i<Length;i++ )
	{
		c=pc[i/4]>>((i&3)*2);
		c&=3;
		_m_i=_mm_load_si128(pbuf);
		_m_q=_mm_load_si128(pbuf+1);
		switch( c )
		{
			case 0://code=1+j I=I+Q Q=Q-I
				_m_oi=_mm_adds_epi16(_m_i,_m_q);
				_m_oq=_mm_subs_epi16(_m_q,_m_i);
				break;
			case 1://code=-1+j I=Q-I Q=-I-Q
				_m_oi=_mm_subs_epi16(_m_q,_m_i);
				_m_oq=_mm_adds_epi16(_m_q,_m_i);
				_m_oq=_mm_mullo_epi16(_m_oq,nones);
				break;
			case 2://code=1-j I=I-Q Q=I+Q
				_m_oi=_mm_subs_epi16(_m_i,_m_q);
				_m_oq=_mm_adds_epi16(_m_q,_m_i);
				break;
			case 3://code=-1-j I=-Q-I Q=I-Q
				_m_oi=_mm_adds_epi16(_m_i,_m_q);
				_m_oi=_mm_mullo_epi16(_m_oi,nones);
				_m_oq=_mm_subs_epi16(_m_i,_m_q);
				break;
			default:
				break;
		}
		_mm_store_si128(pbuf,_m_oi);
		_mm_store_si128(pbuf+1,_m_oq);
		pbuf+=2;
	}
}
				
void Scrambler( void *in, void *code, int Length )
{
	
	int i,j;
	
	unsigned char *pc = (unsigned char *)code;
	unsigned char c;
	char *pbuf=(char *)in;
	char c_i,c_q;
	for( i=0;i<Length;i++ )
	{
		c=pc[i/4]>>((i&3)*2);
		c&=3;
		c_i=1-2*pbuf[0];
		c_q=1-2*pbuf[1];
		
		switch( c )
		{
			case 0://code=1+j I=I-Q Q=Q+I
				pbuf[0]=c_i-c_q;
				pbuf[1]=c_i+c_q;
				break;
			case 1://code=-1+j I=-Q-I Q=I-Q
				pbuf[0]=0-c_i-c_q;
				pbuf[1]=c_i-c_q;
				break;
			case 2://code=1-j I=I+Q Q=Q-I
				pbuf[0]=c_i+c_q;
				pbuf[1]=c_q-c_i;
				break;
			case 3://code=-1-j I=Q-I Q=-I-Q
				pbuf[0]=c_q-c_i;
				pbuf[1]=0-c_i-c_q;
				break;
			default:
				break;
		}
		pbuf+=2;
	}
}
				
				
		
	
	
	


