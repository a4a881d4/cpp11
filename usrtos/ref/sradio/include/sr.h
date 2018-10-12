#ifndef _SR_H
#define _SR_H
#undef DRIVERLOG
struct sradio_semaphore {
	unsigned int wpos;
	unsigned int dropped;
#ifdef DRIVERLOG
	unsigned int addr;
    	unsigned int pn;
    	unsigned int start;
    	unsigned int end;
    	int diff;
    	int left;
    	
	unsigned char vd[1024];
#endif
};
#endif

