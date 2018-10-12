extern char GHGSTable[4096];
extern void Scrambler( void *in, void *code, int Length );
extern void Encode( void *in, void *out, int blknum );

void Transmiter( void *in, void *out, int blknum, char alpha, char beta, unsigned char *code )
{
	char *pdata;
	int i,j;
	
	Encode( in, out, blknum );
	
	pdata=(char *)out;
	
	
	for( i=0;i<blknum;i++ )
	{
		Scrambler( pdata, code, 2048 );
		pdata+=2054*2;
	}
	
		
	
	pdata=(char *)out;
		
	for( i=0;i<blknum;i++ )
	{
		for( j=0;j<4096;j++ )
			pdata[j]=(char)((int)alpha*(int)pdata[j]+(int)beta*(int)GHGSTable[j]);
		pdata+=2054*2;
	}
	
	
}
