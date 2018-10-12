int pCheck(int a)
{
	int b=0;
	int i;
	for(i=0;i<18;i++)
		b^=(a>>i)&1;
	return b;
}


int buildmseq()
{
	static int x=1;
	int ret=pCheck(x&0x81);
	x|=ret<<18;
	x>>=1;
	return ret;	
}
int buildint()
{
	int i;
	int ret=0;
	for( i=0;i<32;i++ )
	{
		ret|=buildmseq()<<(31-i);
	}	
	return ret;
}
int checkmseq(int c)
{
	static int x=0;
	c&=1;
	int ret=c^pCheck(x&0x81);
	x|=c<<18;
	x>>=1;
	return ret&1;	
}
int checkint( int c )
{
	int i;
	int ret=0;
	for( i=0;i<32;i++ )
	{
		ret|=checkmseq(c>>(31-i))&1;
		ret<<=1;
	}	
	return ret;

}

int incint()
{
	static int i=0;
		
	return i++;
}
		
