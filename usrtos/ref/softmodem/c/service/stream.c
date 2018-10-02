typedef struct {
	int state;
	int length;
	unsigned char buf[0x2048];
} STREAM_t;

STREAM_t _srv_TxBuf,_srv_RxBuf;

#define STREAM_IDLE 0
#define STREAM_BUSY 1

#define STREAM_IDLE_FLAG 0x7e
#define STREAM_BEGIN_FLAG 0x81

#define MAXMTU 1600

void _srv_send_eth_packet( unsigned char *buf, int length );
int  _srv_recv_eth_packet( unsigned char *buf );

void _srv_Stream_init()
{
	_srv_TxBuf.state=STREAM_IDLE;
	_srv_TxBuf.length = 0;
	
	_srv_RxBuf.state=STREAM_IDLE;
	_srv_RxBuf.length = 0;
	
}

void _srv_Stream_rx( unsigned char *block, int length )
{
	int i;
	int len;
	for( i=0;i<length;i++ )
	{
		if( _srv_RxBuf.state==STREAM_IDLE && block[i]==STREAM_IDLE_FLAG )
			continue;
		if( _srv_RxBuf.state==STREAM_IDLE && block[i]==STREAM_BEGIN_FLAG )
		{
			_srv_RxBuf.state=STREAM_BUSY;
			_srv_RxBuf.length=0;
			continue;
		}
		if( _srv_RxBuf.state==STREAM_BUSY )
		{
			if( _srv_RxBuf.length<2 )
			{
				_srv_RxBuf.buf[_srv_RxBuf.length]=block[i];
				_srv_RxBuf.length++;
				continue;
			}
			if( _srv_RxBuf.length>=2 )
			{
				len=*(short *)_srv_RxBuf.buf;
				len&=0xffff;
				if( len>MAXMTU )
					_srv_RxBuf.state=STREAM_IDLE;
				else
				{
					if( _srv_RxBuf.length<len )
					{
						_srv_RxBuf.buf[_srv_RxBuf.length]=block[i];
						_srv_RxBuf.length++;
						continue;
					}
					else
					{
						_srv_send_eth_packet( _srv_RxBuf.buf, _srv_RxBuf.length );
						_srv_RxBuf.state=STREAM_IDLE;
						_srv_RxBuf.length=0;
						i--;
						continue;
					}
				}
			}			
		}
	}	
}


void _srv_Stream_tx( unsigned char *block, int length )
{
	int i,len;
	int flag=0;
	for( i=0;i<length;i++ )
	{
		if( _srv_TxBuf.state==STREAM_IDLE && flag==0 )
		{
			len=_srv_recv_eth_packet( _srv_TxBuf.buf );
			if( len==0 )
			{
				flag=1;
				block[i]=STREAM_IDLE_FLAG;
				continue;
			}
			block[i]=STREAM_BEGIN_FLAG;
			_srv_TxBuf.state=STREAM_BUSY;
			_srv_TxBuf.length=0;
			continue;
		}
		if( _srv_TxBuf.state==STREAM_BUSY )
		{
			len=*(short *)_srv_RxBuf.buf;
			len&=0xffff;
			if( _srv_TxBuf.length<len )
			{
				block[i]=_srv_TxBuf.buf[_srv_TxBuf.length];
				_srv_TxBuf.length++;
				continue;
			}
			else
			{
				_srv_TxBuf.state=STREAM_IDLE;
				_srv_TxBuf.length=0;
				i--;
				continue;		
			}
		}
		block[i]=STREAM_IDLE_FLAG;
	}
}