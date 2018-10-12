#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <memory.h>
#include <malloc.h>

#include <xmmintrin.h>
#include <sr.h>
#include <sync.h>
#include <complexoperation.h>


typedef struct { unsigned long t[2]; } timing;
#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
#define timing_diff(x,y) (((x)->t[0] - (double) (y)->t[0]) + 4294967296.0 * ((x)->t[1] - (double) (y)->t[1]))
timing before, after;
double clocktick;
    	
static struct sradio_semaphore * sem=NULL;
static unsigned char *recvmmap = NULL;
#define NBLOCK 16
#define RING 5185536
#define SYNCSIZE (4096)
#define LOSTBIG (RING/4)

float *pfbuf;
unsigned char *pcbuf;
class Sync aSync(SYNCSIZE,31);
class Sync *pSync;
void doSync(int);
int counterIR=0;
int counterDo=0;
int counterSync=0;
void recvhandler(int signo)
{
    static struct sradio_semaphore local; 
    struct sradio_semaphore localcopy; 
    int diff;
    if (signo!=SIGIO)
    	return;
    counterIR++;
    if( sem!=NULL )
    {
    	memcpy(&localcopy,sem,sizeof(struct sradio_semaphore));
    	if( localcopy.dropped != local.dropped )
    	{
    		printf("dropped at offset= %d dropped=%d \n",localcopy.wpos, localcopy.dropped);		
    			local.dropped = localcopy.dropped;
    	}
    	diff = (localcopy.wpos - local.wpos+RING)%RING;
    	if( diff>RING/2 )
    		diff-=RING;
    	if( diff>LOSTBIG )
    	{
    		printf("many package lost %d recv=%d local=%d\n",diff,localcopy.wpos,local.wpos);
    		local.wpos = localcopy.wpos;
		diff=0;
    	}	
    	for( ;diff >= SYNCSIZE*2;diff-=SYNCSIZE*2 )
	{
    		counterDo++;
    		doSync(local.wpos);
    		local.wpos+=SYNCSIZE*2;
    		local.wpos%=RING;
	}
    }
    return;
}

int main(int argc, char **argv)
{
    int count;
    int i,j;
    int pos;
    
    struct sigaction action;
	int fd;
	pSync = &aSync;
	pfbuf=( float * ) memalign( 16, SYNCSIZE * 2 * sizeof( float ) );
	timing_now(&before);
	//sleep(10);	
	timing_now(&after);
	clocktick= 2412.742e6;		
	printf("CPU freq is %lf\n",clocktick);
	if( argc==2 )
	{
		pcbuf = ( unsigned char * )memalign( 16, SYNCSIZE * 2 * sizeof( char ) );
		printf("3\n");
		FILE *fp = fopen(argv[1],"rb");
		if( fp==NULL )
		{
			printf("open file fail\n");
			exit(1);
		}
		timing_now(&before);
		for( int start=0;start<RING;start+=SYNCSIZE * 2 )
		{
			fread(pcbuf,1,SYNCSIZE * 2,fp);
			pSync->CO.char2Float(pcbuf,pfbuf,SYNCSIZE * 2);
			pSync->Synchronizer(pfbuf);
			if( pSync->b_SyncStatus )
				printf("Sync off=%d \n",pSync->i_RelativeOffset+start/2);
			else
				printf("No Sync max=%f avg=%f %f\n",pSync->f_MaxAbs2,pSync->f_AveAbs2,
				pSync->f_MaxAbs2/pSync->f_AveAbs2);
		}
		timing_now(&after);
		fclose(fp);
		printf("done in %7.2f secs\n",timing_diff(&after,&before)/clocktick);	
	}
	else
	{
		if( argc==3 )
			fd = open(argv[2],O_RDWR);
		else
			fd = open("/dev/sr1",O_RDWR);
		if( fd<0 )
		{
			printf("open driver failure\n");
			exit(-1);
		}
		int blksize=4096;
		int blk_nr=1300;
		recvmmap = (unsigned char *)mmap(NULL, blksize*blk_nr , PROT_READ|PROT_WRITE, MAP_SHARED , fd , 0);
		if (recvmmap == (void *)-1) {
        		fprintf(stderr,"mmap(): %s\n",strerror(errno));
        		exit(1);
    		}
		sem = (struct sradio_semaphore *)(recvmmap+blksize*(blk_nr-1));
	
    		memset(&action, 0, sizeof(action));
    		action.sa_handler = recvhandler;
    		action.sa_flags = 0;

    		sigaction(SIGIO, &action, NULL);

    		fcntl(fd, F_SETOWN, getpid());
    		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | FASYNC);
    		while(1) {
			
			sleep(1);
		}
	}
	pSync=NULL;
}


void doSync(int pos)
{
	if( pSync!=NULL )
	{
		if( (pos+SYNCSIZE*2)<=RING )
			pSync->CO.char2Float(recvmmap+pos,pfbuf,SYNCSIZE*2);
		else
		{
			int left = pos+SYNCSIZE*2-RING;
			pSync->CO.char2Float(recvmmap+pos,pfbuf,SYNCSIZE*2-left);
			pSync->CO.char2Float(recvmmap,pfbuf+(SYNCSIZE*2-left),left);
		}				
		pSync->Synchronizer(pfbuf);
		if( pSync->b_SyncStatus )
		{
			timing_now(&after);
			printf("Real:Sync off=%8d IR=%8d Do=%8d Sync=%8d time=%9.7lf \n",pSync->i_RelativeOffset+pos/2,
				counterIR,counterDo,counterSync++,timing_diff(&after,&before)/clocktick);
			timing_now(&before);
		}
		
	}
}
