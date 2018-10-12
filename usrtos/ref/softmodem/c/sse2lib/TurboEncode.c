extern unsigned short TurboEncodeTable[256];

void TurboEncode( void *in, void *out, int Length )
{
	int i,j;
	int l8 = Length/8;
	int r8 = Length%8;
	
	unsigned char *pIn = ( unsigned char *)in;
	unsigned char *pOut = ( unsigned char * )out;
	
	unsigned char c = 0xff,d,e,ly;
	unsigned short s;

	
	d=0;
	ly=0;
	for( i=0;i<l8;i++ )
	{
		d^=pIn[i];
		s=TurboEncodeTable[d&0xff];
		d=(s>>8);
		pOut[i]=(s&0xff)^ly;
		ly=(d&0x7)<<5;
		d&=0xe0;
	}
	pIn[l8]=(TurboEncodeTable[d]&0xe0);
	pOut[l8]=(TurboEncodeTable[d]&0x7)<<5;
		
}
