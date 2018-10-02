void GHGS( void *inI, void *inQ, void *outI, void *outQ );
void _mm_cabs( void *inI, void *inQ, void *out, int Length );
void _mm_findMax( void *inI, int Length, int *max, int *avg );
void _mm_sumRow( void *in, void * out, int Length );

#define SLOTLENGTH (2054*2)
#define CHANNELLENGTH (160*2)
#define INPUTLENGTH (SLOTLENGTH+CHANNELLENGTH)
#define MAX_RAKE_TAP 32

#define PSCLENGTH (2048)


typedef struct {
	int iTapNum;
	int pos[MAX_RAKE_TAP];
	short *pITap;
	short *pQTap;
} RET_CHANNEL_ESTIMATE_t;
	

#define NOISE_TH 2.
#define PEAK_TH 0.5
#define CHANNELSHIFT 4
#define DEMODUSHIFT 7
#define DEMODUOFFSET 0x40


