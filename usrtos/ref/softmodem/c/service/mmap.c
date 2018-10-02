#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/bpf.h>
#include <sys/mman.h>


#define DEBUG

int _mmap_fd;
void *_mmap_buf;

#define _mmap_tp_block_size 4096
#define _mmap_tp_frame_size 2048
#define _mmap_tp_block_nr 64
#define _mmap_tp_frame_nr 128
#define PROTOCOL_TYPE 0x0800

int _mmap_pos;

void _mmap_init ()
{

	struct tpacket_req req;

	struct bpf_insn insns[] = {
	        BPF_STMT(BPF_LD+BPF_H+BPF_ABS, 12),
	        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, PROTOCOL_TYPE, 0, 1),
	        BPF_STMT(BPF_RET+BPF_K, (u_int)-1),
	        BPF_STMT(BPF_RET+BPF_K, 0),
	};
	
	struct bpf_program filter ={
		4,
		insns
	};
    
	_mmap_fd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
	
	req.tp_block_size = _mmap_tp_block_size;
  	req.tp_frame_size = _mmap_tp_frame_size;
  	req.tp_block_nr = _mmap_tp_block_nr;
  	req.tp_frame_nr = _mmap_tp_frame_nr;

	setsockopt(_mmap_fd, SOL_SOCKET, SO_ATTACH_FILTER, &filter, sizeof(filter));
	setsockopt (_mmap_fd, SOL_PACKET, PACKET_RX_RING, (void *) &req, sizeof (req));

    	_mmap_buf = mmap (0, _mmap_tp_block_size * _mmap_tp_block_nr, PROT_READ | PROT_WRITE, MAP_SHARED,_mmap_fd, 0);
    	
    	_mmap_pos = 0;

}

int  _srv_recv_eth_packet( unsigned char *buf )
{
	
	char *head;
	struct tpacket_hdr *phd;

	int curPos = _mmap_pos;
	
	int k;
	do
	{
		head = (char *) _mmap_buf + _mmap_pos * _mmap_tp_frame_size;
		_mmap_pos++;
		if( _mmap_pos>=_mmap_tp_frame_nr )
			_mmap_pos-=_mmap_tp_frame_nr;
		phd = (struct tpacket_hdr *) head;
		if( phd->tp_status )
		{
#ifdef DEBUG
			printf("I transmit: \n");
	    		for (k = phd->tp_mac; k < phd->tp_mac + phd->tp_len; k++)
				printf ("%2.2x ", head[k] & 0xff);
	      		printf ("\n\n");
#endif
			*(short *)buf = phd->tp_len+6;
			memcpy(buf+2,head+phd->tp_mac,phd->tp_len);
			phd->tp_status = 0;
			return phd->tp_len+6;
		}
	} while( _mmap_pos!=curPos );
	return 0;
}

#ifdef DEBUG
void _srv_send_eth_packet( unsigned char *buf, int length )
{
	int k;
	printf(" I receive :\n");
	for( k=0;k<length;k++ )
		printf ("%2.2x ", buf[k] & 0xff);
	printf ("\n\n");
}
		
#endif	
 