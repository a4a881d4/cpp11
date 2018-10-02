#ifndef __PN_H
#define __PN_H


#define GI 0xa3a1
#define GQ 0x9c79
#define IS95FRAMEORDER 15
#define IS95FRAMELEN (1<<IS95FRAMEORDER)

class CPN  
{
public:
	unsigned char * getCodeOff( int Off );
	virtual ~CPN();
	CPN();
	void genCode();
	void readCode( char *fn );

private:
	int pCheck( int a );
	unsigned char *code;
	
};

#endif 
