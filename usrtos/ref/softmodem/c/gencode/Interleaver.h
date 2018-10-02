
#define ROOT_TABLE
#define PNUMBER	17
#define INTER	0
#define DEINT	1
#define INT_FF	1
#define CRC_POLYNOMIAL		0x11021

class Interleaver  
{
public:
	Interleaver(int codelength);
	virtual ~Interleaver();
	int *Inter_Table;
	
private:
	int code_length;		
	void Interleaver_Parameter(int Length);
	int Interleaver_Row_Number,Interleaver_Column_Number,Interleaver_Prime;
	int Interleaver_Root[20];
	int Interleaver_PRoot;
	int Interleaver_Case;
};

class CRC  
{
public:
	CRC(int codelength);
	virtual ~CRC();
	int *CRC_Table;
	
private:
	int code_length;		
};
