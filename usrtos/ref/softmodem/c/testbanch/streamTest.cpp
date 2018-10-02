#include<stdio.h>


extern "C" void _srv_Stream_init();
extern "C" void _srv_Stream_rx( unsigned char *block, int length );
extern "C" void _srv_Stream_tx( unsigned char *block, int length );

main()
{
	
	int i,j;
	
	_srv_Stream_init();
	unsigned char *testdata = new unsigned char[1400];
	for( i=0;i<2;i++ )
	{
		_srv_Stream_tx( testdata, 1358 );
		for( j=0;j<1358;j++ )
		{	
			printf(" %x",(int)testdata[i]&0xff);
			if( (j&0x1f) == 0x1f )
				printf("\n");
		}
		printf("\n\n");
		
		_srv_Stream_rx( testdata,1358 );
	}	
}
