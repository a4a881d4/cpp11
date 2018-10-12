

class CState  
{
public:
	void genRSC( int iDenominator, int iNumerator );
	CState();
	virtual ~CState();
	int NextState0;
	int NextState1;
	int NextBranch0;
	int NextBranch1;
	int LastState0;
	int LastState1;
	int LastBranch0;
	int LastBranch1;
	int Terminal;
	static CState * Trellis;
	

};

