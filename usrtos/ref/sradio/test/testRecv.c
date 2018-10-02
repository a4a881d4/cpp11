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

void recvhandler(int signo)
{
    if (signo!=SIGIO)
    	return;
    if( sem!=NULL )
    {
    	printf("offset= %d dropped=%d \n",sem->wpos, sem->dropped);		
    }
    return;
}

int main(int argc, char **argv)
{
    int count;
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

    while(1) {
        sleep(86400); /* one day */
     }
}
