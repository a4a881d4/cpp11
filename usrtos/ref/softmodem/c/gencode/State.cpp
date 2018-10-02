// State.cpp: implementation of the CState class.
//
//////////////////////////////////////////////////////////////////////

#include "State.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CState *CState::Trellis; 
CState::CState()
{

}

CState::~CState()
{

}

void CState::genRSC(int iDenominator, int iNumerator )
{
	int ip = iDenominator;
	int i;
	for( i=32;i>0;i-- ) {
		if( (ip & 0x80000000) != 0 )
			break;
		ip<<=1;
	}
	int iStateDeep = i-1;
	int iStateNumber = 1<<iStateDeep;
	Trellis = new CState[iStateNumber];
	int s0,s1,t1,t2,g1,g2,gen1,gen2;
	gen1=iDenominator>>1;
	gen2=iNumerator>>1;
	for (s0=0;s0<iStateNumber;s0++)
	{
		g1=s0&gen1;
		t1=0;
		for (i=0;i<iStateDeep;i++)
			t1=t1^((g1>>i)&1);
		g2=s0&gen2;
		t2=t1;
		for (i=0;i<iStateDeep;i++)
			t2=t2^((g2>>i)&1);
		s1=(s0<<1)%iStateNumber+t1;
		Trellis[s0].NextState0=s1;
		Trellis[s0].NextState1=s1^0x1;
		Trellis[s0].NextBranch0=t2;
		Trellis[s0].NextBranch1=t2^0x1;
		Trellis[s1].LastState0=s0;
		Trellis[s1^0x1].LastState1=s0;
		Trellis[s1].LastBranch0=t2;
		Trellis[s1^0x1].LastBranch1=t2^0x1;
		Trellis[s0].Terminal=t1;
	}
}
