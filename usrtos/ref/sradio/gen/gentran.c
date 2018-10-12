#include<stdio.h>

#define outins( out, in0, in1, ins ) printf("\tt%2.2x = %s( t%2.2x, t%2.2x );\n",out,ins,in0,in1);
#define inMem( out, in0, in1, ins ) printf("\tt%2.2x = %s( inbuf[0x%3.3x], inbuf[0x%3.3x] );\n",out,ins,in0,in1);
#define outMem( out, in0, in1, ins ) printf("\toutbuf[0x%3.3x] = %s( t%2.2x, t%2.2x );\n",out,ins,in0,in1);

int flip(int in)
{
	int i;
	int ret=0;
	for( i=0;i<4;i++ )
	{
		ret<<=1;
		ret|=in&1;
		in>>=1;
	}
	return ret;
}
void build( int c, int r, int flag )
{
	int inIndex[16];
	int outIndex[16];
	char ins0[128];
	int i;
	for( i=0;i<16;i++ )
	{
		inIndex[i]=c*4*16+r+i*4;
		outIndex[i]=r*4*16+c+flip(i)*4;	
	}	
	if( flag==0 )
		sprintf(ins0,"_mm_unpacklo_epi8");
	else
		sprintf(ins0,"_mm_unpackhi_epi8");
	for( i=0;i<8;i++ )
		inMem( i, inIndex[2*i], inIndex[2*i+1], ins0 );
	sprintf(ins0,"_mm_unpacklo_epi16");
	for( i=0;i<4;i++ )
		outins( 2*i+8, 2*i, 2*i+1, ins0 );
	sprintf(ins0,"_mm_unpackhi_epi16");
	for( i=0;i<4;i++ )
		outins( 2*i+1+8, 2*i, 2*i+1, ins0 );
	sprintf(ins0,"_mm_unpacklo_epi32");
	outins( 0, 8+0, 8+2, ins0 );
	outins( 1, 8+1, 8+3, ins0 );
	outins( 4, 8+4, 8+6, ins0 );
	outins( 5, 8+5, 8+7, ins0 );
	
	sprintf(ins0,"_mm_unpackhi_epi32");
	outins( 2, 8+0, 8+2, ins0 );
	outins( 3, 8+1, 8+3, ins0 );
	outins( 6, 8+4, 8+6, ins0 );
	outins( 7, 8+5, 8+7, ins0 );
	
	sprintf(ins0,"_mm_unpacklo_epi64");
	for( i=0;i<4;i++ )
		outMem( outIndex[2*i+flag], i, i+4, ins0 );
	sprintf(ins0,"_mm_unpackhi_epi64");
	for( i=0;i<4;i++ )
		outMem( outIndex[2*i+8+flag], i, i+4, ins0 );
}

void buildonce( int c, int r )
{
	int inIndex[16];
	int outIndex[16];
	char ins0[128];
	int i,j;
	for( i=0;i<16;i++ )
	{
		inIndex[i]=c*4*16+r+i*4;
		outIndex[i]=r*4*16+c+flip(i)*4;	
	}	
	sprintf(ins0,"_mm_unpacklo_epi8");
	for( i=0;i<8;i++ )
		inMem( 2*i, inIndex[2*i], inIndex[2*i+1], ins0 );
	sprintf(ins0,"_mm_unpackhi_epi8");
	for( i=0;i<8;i++ )
		inMem( 2*i+1, inIndex[2*i], inIndex[2*i+1], ins0 );
	sprintf(ins0,"_mm_unpacklo_epi16");
	for( i=0;i<4;i++ )
		for( j=0;j<2;j++ )
			outins( 4*i+16+j, 4*i+j, 4*i+2+j, ins0 );
	sprintf(ins0,"_mm_unpackhi_epi16");
	for( i=0;i<4;i++ )
		for( j=0;j<2;j++ )
			outins( 4*i+16+j+2, 4*i+j, 4*i+2+j, ins0 );
	
	sprintf(ins0,"_mm_unpacklo_epi32");
	for( i=0;i<2;i++ )
		for( j=0;j<4;j++ )
			outins( 8*i+j, 8*i+j+16, 8*i+4+j+16, ins0 );
	sprintf(ins0,"_mm_unpackhi_epi32");
	for( i=0;i<2;i++ )
		for( j=0;j<4;j++ )
			outins( 8*i+j+4, 8*i+j+16, 8*i+4+j+16, ins0 );
	sprintf(ins0,"_mm_unpacklo_epi64");
	for( i=0;i<8;i++ )
		outMem( outIndex[i], i, i+8, ins0 );
	sprintf(ins0,"_mm_unpackhi_epi64");
	for( i=0;i<8;i++ )
		outMem( outIndex[i+8], i, i+8, ins0 );
}
int tran1()
{
	int i,j;

	printf("__m128i *inbuf=(__m128i *)ca_in;\n");
	printf("__m128i *outbuf=(__m128i *)ca_out;\n");
	
	printf("__m128i ");
	for( i=0;i<15;i++ ) printf("t%2.2x, ",i);
	printf("t%2.2x;\n",15);
	
	for( i=0;i<4;i++ ) {
		for( j=0;j<4;j++ ) {
			build(i,j,0);
			build(i,j,1);
		}
	}
	
	return 0;	
}

int tran2()
{
	int i,j;
	for( i=0;i<64;i++ )
		for( j=0;j<64;j++ )
			printf("ca_out[0x%3.3x]=ca_in[0x%3.3x];\n",i*64+j,i+j*64);
	return 0;	
}

int tran3()
{
	int i,j;

	printf("__m128i *inbuf=(__m128i *)ca_in;\n");
	printf("__m128i *outbuf=(__m128i *)ca_out;\n");
	
	printf("__m128i ");
	for( i=0;i<31;i++ ) printf("t%2.2x, ",i);
	printf("t%2.2x;\n",31);
	
	for( i=0;i<4;i++ ) {
		for( j=0;j<4;j++ ) {
			buildonce(i,j);
		}
	}
	
	return 0;	
}
int main( int argc, char *argv[])
{
	if( argc!=2 )
		return -1;
	if( *argv[1]=='1' )
		tran1();
	else if( *argv[1]=='2' )
		tran2();
	else
		tran3();
	return 0;
}
