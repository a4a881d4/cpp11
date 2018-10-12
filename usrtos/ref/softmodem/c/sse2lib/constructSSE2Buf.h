#define MAXBLOCKNUM 32
typedef struct {
	int blkNum;
	int length[MAXBLOCKNUM];
	char *point[MAXBLOCKNUM];
	short *buf_I;
	short *buf_Q;
	int curBlk;
	int curPos;
	char *curBuf;
} RECEIVER_BUF_IND_t;




	