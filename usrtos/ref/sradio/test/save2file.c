#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sr.h>

static struct sradio_semaphore * sem=NULL;
static unsigned char *recvmmap = NULL;
static FILE *saveFile=NULL;

#define NBLOCK 16
#define RING 5185536
#define BLKSIZE (4096*3)
#define LOSTBIG (RING/4)
void doSave(int pos);
void recvhandler(int signo)
{
    static struct sradio_semaphore local; 
    struct sradio_semaphore localcopy; 
    int diff;
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
    	diff = (localcopy.wpos - local.wpos+RING)%RING;
    	if( diff>RING/2 )
    		diff-=RING;
    	if( diff>LOSTBIG )
    	{
    		printf("many package lost %d recv=%d local=%d\n",diff,localcopy.wpos,local.wpos);
    		local.wpos = localcopy.wpos;
		diff=0;
    	}	
    	for( ;diff >= BLKSIZE*2;diff-=BLKSIZE*2 )
	{
    		doSave(local.wpos);
    		local.wpos+=BLKSIZE*2;
    		local.wpos%=RING;
	}
    }
    return;
}

int main(int argc, char **argv)
{
    int count;
    struct sigaction action;
	int fd;
	fd = open("/dev/sr0",O_RDWR);
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
	saveFile = fopen(argv[1],"wb");	
    while(1) {
        sleep(1); /* one day */
	if( ftell(saveFile)>=(long)NBLOCK*RING )
	{
		fclose(saveFile);
		saveFile=NULL;	
		break;
	}
     }
}

void doSave(int pos)
{
	if( saveFile==NULL )
		return;
	if( (pos+BLKSIZE*2)<=RING )
		fwrite(recvmmap+pos,1,BLKSIZE*2,saveFile);
	else
	{
		int left = pos+BLKSIZE*2-RING;
		fwrite(recvmmap+pos,1,BLKSIZE*2-left,saveFile);
		fwrite(recvmmap,1,left,saveFile);
	}				
}
