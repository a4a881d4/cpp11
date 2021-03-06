#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sr.h>
#include "testHelper.h"

static struct sradio_semaphore * sem=NULL;
static unsigned char *recvmmap = NULL;
#define NBLOCK 16
#define LOSTBIG 512*128
#define RING 5185536

int buf[128];

void recvhandler(int signo)
{
    static struct sradio_semaphore local = { 
    	.wpos = 0, 
    	.dropped = 0, 
    };
    int diff,i,pos;
    struct sradio_semaphore localcopy; 
    if (signo!=SIGIO)
    	return;
    if( sem!=NULL )
    {
    	memcpy(&localcopy,sem,sizeof(struct sradio_semaphore));
    	if( localcopy.dropped != local.dropped )
    	{
    		printf("dropped at offset= %d dropped=%d \n",localcopy.wpos, localcopy.dropped);		
    		local.dropped = localcopy.dropped;
    	}
    	int diff = (localcopy.wpos - local.wpos+RING)%RING;
    	if( diff>RING/2 )
    		diff-=RING;
    	if( diff>LOSTBIG )
    	{
    		printf("many package lost %d recv=%d local=%d\n",diff,localcopy.wpos,local.wpos);
    		local.wpos = (localcopy.wpos)%RING;
    		return;
    	}	
    	for( i=0;i<diff;i+=4 )
    	{
    		pos = local.wpos + i;
    		pos %= RING;
    		if( checkint( *(int *)(recvmmap+pos) )!=0 )
    		{
    			printf("error at %x data=%x rdata=%x\n",pos,*(int *)(recvmmap+pos),buf[i/4] );
#ifdef DRIVERLOG
    			printf("start=%x end=%x pn=%x addr=%x\n\n",
    				localcopy.start,localcopy.end,localcopy.pn,localcopy.addr);
#endif
    			
    		}
    	}
    	local.wpos+=diff;
    	local.wpos%=RING;
    }
    return;
}

int main(int argc, char **argv)
{
    int count;
    int i,j;
    struct sigaction action;
	int fd;
	fd = open("/dev/sr1",O_RDWR);
	if( fd<0 )
	{
		printf("open driver failure\n");
		exit(-1);
	}
	int blksize=4096;
	int blk_nr=1300;
	recvmmap = mmap(NULL, blksize*blk_nr , PROT_READ|PROT_WRITE, MAP_SHARED , fd , 0);
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

	int handle = open ("/dev/sr0", O_RDWR);

    while(1) {
        usleep(1); 
        for( j=0;j<NBLOCK;j++ )
        {
        	for( i=0;i<128;i++ )
        		buf[i]=buildint();
        	write (handle, (void *) buf, 512);
        }
        for( i=0;i<128;i++ )
        {
        //	if( checkint( buf[i] )!=0 )
        //		printf("t error\n");
        }
        
     }
}

