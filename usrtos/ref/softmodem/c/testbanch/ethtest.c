int count=0;
void _srv_send_eth_packet( unsigned char *buf, int length )
{
	int i;
	printf(" I receive %d\n",length);
	for( i=0;i<length;i++ )
		printf(" %x",(int)buf[i]&0xff);
}
int  _srv_recv_eth_packet( unsigned char *buf )
{
	int i;
	if( (count%2)==1 )
		return 0;
	for( i=0;i<20;i++ )
	{
		buf[i]=(count+i)&0xff;
	}
	count++;
	return 20;
	
}