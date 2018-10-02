
#include "Interleaver.h"
#include<stdio.h>
	int pattern_A[20]={19,9,14,4,0,2,5,7,12,18,10,8,13,17,3,1,16,6,15,11};
	int pattern_B[20]={19,9,14,4,0,2,5,7,12,18,16,13,17,15,3,1,6,11,8,10};
	int pattern_C[10]={9,8,7,6,5,4,3,2,1,0};
	int pattern_D[5]={4,3,2,1,0};
	int Prime_Number[80]=	{	7,	11,	13,	17,	19,	23,	29,	31,	37,	41,
								43,	47,	53,	59,	61,	67,	71,	73,	79,	83,	
								89,	97,	101,103,107,109,113,127,131,137,
								139,149,151,157,163,167,173,179,181,191,
								193,197,199,211,223,227,229,233,239,241,
								251,257,263,269,271,277,281,283,293,307,
								311,313,317,331,337,347,349,353,359,367,
								373,379,383,389,397,401,409};
	int Primitive_Root[80]=	{	3,	2,	2,	3,	2,	5,	2,	3,	2,	6,
								3,	5,	2,	2,	2,	2,	7,	5,	3,	2,
								3,	5,	2,	5,	2,	6,	3,	3,	2,	3,
								2,	2,	6,	5,	2,	5,	2,	2,	2,	19,
								5,	2,	3,	2,	3,	2,	6,	3,	7,	7,
								6,	3,	5,	2,	6,	5,	3,	3,	2,	5,
								17,	10,	2,	3,	10,	2,	2,	3,	7,	6,
								2,	2,	5,	2,	5,	3,	21};


Interleaver::Interleaver(int codelength)
{
	int I,J,idxi,idxo,Cji[20],prd[20];
	int i,j;
	code_length=codelength;
	Inter_Table=new int [code_length];
	Interleaver_Parameter(code_length);
	idxo=0;
	for (i=0;i<Interleaver_Row_Number;i++) 
		Cji[i]=1;
	for (i=0;i<Interleaver_Row_Number;i++) 
		prd[i]=0;
		
	for (I=0;I<Interleaver_Column_Number;I++)
	{
		for (J=0;J<Interleaver_Row_Number;J++)
		{
			if (Interleaver_Case==0)	j=pattern_A[J];
			else if (Interleaver_Case==1)	j=pattern_B[J];
			else if (Interleaver_Case==2)	j=pattern_C[J];
			else	j=pattern_D[J];				//	行间交织表，对不同的行号J变换成j
			if (Interleaver_Column_Number==Interleaver_Prime-1) 
				i=Cji[J]-1;
			else 
				i=Cji[J];
			if (I==Interleaver_Prime-1) 
				i=0;
			if (I==Interleaver_Prime) 
				i=Interleaver_Prime;
			if (j==Interleaver_Row_Number-1)
			{	
				if (code_length==Interleaver_Row_Number*(Interleaver_Prime+1))
				{	if (I==Interleaver_Prime) i=1;
					if (I==0) i=Interleaver_Prime;
				}
			}
			idxi=j*Interleaver_Column_Number+i;
			Cji[J]=(Cji[J]*Interleaver_Root[J])%Interleaver_Prime;
			if ((Cji[J]==2)&&(prd[J]==0)) prd[J]=I+1;
			if (idxi<code_length)
			{
				Inter_Table[idxo]=idxi;
				idxo++;
			}
		}
	}
}

Interleaver::~Interleaver()
{
	delete Inter_Table;
}


void Interleaver::Interleaver_Parameter(int Length)
{
	int i,j,pi,g0,tt,pf;
	float t;
	int qqq[20];

	if ((Length<40)||(Length>5120))
	{
		printf("Code length error.\n");
		return;
	}
	if ((Length>480)&&(Length<531))
	{
		Interleaver_Case=2;		//Case C
		Interleaver_Prime=53;
		Interleaver_Column_Number=53;
		Interleaver_Row_Number=10;
		pi=12;
	}
	else
	{
		Interleaver_Row_Number=20;
		if (Length<160)
		{
			Interleaver_Case=3;
			Interleaver_Row_Number=5;
		}
		if ((Length>159)&&(Length<201))
		{
			Interleaver_Case=2;
			Interleaver_Row_Number=10;
		}
		if ((Length<481)&&(Length>200)) Interleaver_Case=0;
		if ((Length>530)&&(Length<2281))	Interleaver_Case=0;
		if ((Length>2280)&&(Length<2481))	Interleaver_Case=1;
		if ((Length>2480)&&(Length<3161))	Interleaver_Case=0;
		if ((Length>3160)&&(Length<3211))	Interleaver_Case=1;
		if ((Length>3210)&&(Length<5120))	Interleaver_Case=0;

		t=(float)(Length/(float)Interleaver_Row_Number-1.-.0001);  // find prime
		for (i=0;i<76;i++)
		{
			pi=i;
			if (Prime_Number[i]>=t) i=80;
		}
		Interleaver_Prime=Prime_Number[pi];
		if (0.>((float)Interleaver_Prime-(float)Length/(float)Interleaver_Row_Number+0.001))
			Interleaver_Column_Number=Interleaver_Prime+1;
		else if (0.<((float)Interleaver_Prime-1.-(float)Length/(float)Interleaver_Row_Number+0.001))
			Interleaver_Column_Number=Interleaver_Prime-1;
		else
			Interleaver_Column_Number=Interleaver_Prime;
	}
	g0=Primitive_Root[pi];
	Interleaver_PRoot=g0;
	qqq[0]=1;
	i=1;
	for (j=0;j<76;j++)
	{
		if ((Interleaver_Prime-1)%Prime_Number[j])
		{
			qqq[i]=Prime_Number[j];
			i++;
			if (i==Interleaver_Row_Number) j=80;
		}
	}
	for (i=0;i<Interleaver_Row_Number;i++)
	{
		Interleaver_Root[i]=1;
		for (j=0;j<qqq[i];j++)
			Interleaver_Root[i]=(Interleaver_Root[i]*g0)%Interleaver_Prime;
	}
	return;
}

CRC::CRC(int codelength)
{

	code_length=codelength;
	CRC_Table=new int [code_length];
	int state=1;
	for( int i=0;i<codelength;i++ )
	{
		if( (state&0x10000)!=0 )
		{
			state^=CRC_POLYNOMIAL;
		}
		CRC_Table[codelength-1-i]=state;
		state<<=1;
	}
}
CRC::~CRC()
{
	delete CRC_Table;
}

