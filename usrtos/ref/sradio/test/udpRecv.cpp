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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <sr.h>


typedef struct { unsigned long t[2]; } timing;
#define timing_now(x) asm volatile(".byte 15;.byte 49" : "=a"((x)->t[0]),"=d"((x)->t[1]))
#define timing_diff(x,y) (((x)->t[0] - (double) (y)->t[0]) + 4294967296.0 * ((x)->t[1] - (double) (y)->t[1]))
timing before, after;
double clocktick;
    	
static struct sradio_semaphore * sem=NULL;
static unsigned char *recvmmap = NULL;
#define NBLOCK 16
#define RING 5185536
#define SYNCSIZE (1024)
#define LOSTBIG (RING/4)

void doSend(int);
int counterIR=0;
int counterDo=0;

int saveFp;
struct sockaddr_in saddr;

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
    		doSend(local.wpos);
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
    
    if ( (saveFp = socket(PF_INET, SOCK_DGRAM, 0) ) == -1)
        fprintf(stderr,"cannot make socket for udp!\n");
   
//    bzero( (void *)&saddr, sizeof(struct sockaddr_in) );
    saddr.sin_addr.s_addr = inet_addr(argv[2]);	//dest ip address
    saddr.sin_port = htons(10000);	//dest port

    saddr.sin_family = AF_INET;

    struct sigaction action;
int fd;
	fd = open(argv[1],O_RDWR);
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


void doSend(int pos)
{
	if( (pos+SYNCSIZE*2)<=RING )
		sendto(saveFp, recvmmap+pos, SYNCSIZE*2, 0, (sockaddr*)&saddr, sizeof(saddr));
	else
	{
		int left = pos+SYNCSIZE*2-RING;
		sendto(saveFp, recvmmap+pos, SYNCSIZE*2-left, 0, (sockaddr*)&saddr, sizeof(saddr));
		sendto(saveFp, recvmmap, left, 0, (sockaddr*)&saddr, sizeof(saddr));
	}				
}
