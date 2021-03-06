#ifndef BS_CONFIG_H
#define BS_CONFIG_H
#if 1

enum {
	FDE_ANTNUM=4,	
	TRAN_TBLENGTH=3584,			
	TRAN_PILOTLENGTH=224,				
	TRAN_PCPLENGTH=56,				
	TRAN_PSCLENGTH=896,
	TRAN_CONTPSCNUM=3,
	CHANEST_BUFDEPTH=6,	
	MAX_TRAN_DBNUM_1PSCBLOCK=700,
	FDE_SNR=8,
};

enum {
	AFC_ACCTIMES=4,
};			

enum {
	MODE_REC_SENSITIVITY_TEST=0,
	MODE_CHANNEL_BW_2M=1,
	MODE_CHANNEL_BW_2_67M=2,
	MODE_CHANNEL_BW_1_2M=3,
};

enum {
	MODE_REC_SENSITIVITY_TEST_TRAN_DBNUM_1PSCBLOCK=24,
	MODE_CHANNEL_BW_2M_TRAN_DBNUM_1PSCBLOCK=504,
	MODE_CHANNEL_BW_2_67M_TRAN_DBNUM_1PSCBLOCK=632,
	MODE_CHANNEL_BW_1_2M_TRAN_DBNUM_1PSCBLOCK=312,
};

#define	TRAN_DLENGTH											( TRAN_TBLENGTH - TRAN_PILOTLENGTH - TRAN_PCPLENGTH ) 
#define REC_SAMPLERATE										( 8.0 / 7.0 )
#define REC_TBLENGTH											( ( int )( TRAN_TBLENGTH * REC_SAMPLERATE ) )		 
#define REC_PILOTLENGTH										( ( int )( TRAN_PILOTLENGTH * REC_SAMPLERATE ) ) 
#define REC_PCPLENGTH											( ( int )( TRAN_PCPLENGTH * REC_SAMPLERATE ) )	 
#define REC_PSCLENGTH											( ( int )( TRAN_PSCLENGTH * REC_SAMPLERATE ) )

#define AFC_MAXCHANESTINT									( CHANEST_BUFDEPTH - 1 )

#else

#define DIRECT_ETHREC




#define HALF_INTBLOCK_2_DECODER


#define COD_CRC_ORDER											16
#define COD_CRC_POLY											69665	
#define COD_CB_INSERT0ROWNUM							5	  
#define COD_CB_INFONUM										( ( 57 - COD_CB_INSERT0ROWNUM ) * 57 - COD_CRC_ORDER )
#define COD_CBROWNUM											( 64 - COD_CB_INSERT0ROWNUM )

#define INTBLOCK_TBNUM										8
#define INTBLOCK_CBNUM										( 7 * 2 )
#define INT_COLNUM												( COD_CBROWNUM * INTBLOCK_CBNUM )
#define INT_COL_WIDTH											10
#define INT_PRIMEFACTOR										65	
#define INT_ROWOFFSET											1

#define TPCDECODER_NUM										1

#define TPCDECODER_1CB_BYTE								368
#define TPCDECODER_1PAC_BYTE							370
#define TPCDECODER_BUF_TSPAC_BYTE					188
#define TPCDECODER_BUF_1UDPPAC_TSPACNUM		7
#define TPCDECODER_BUF_UDPPACNUM					512
#define TPCDECODER_PAC_DESADDR_MAX				0x200000
#define TPCDECODER_PAC_DESADDR_MAXDIFF		200
#define TPCDECODER_SELFDEC_INITVALUE			4000000

#define TS_SYNC_CNT_THRES									1000	
#define TS_ASYNC_CNT_THRES								100		


#define ETH_PACKET_SAMPLE_NUM							256		


#define PAC_DESADDR_MAX										0x10000
#define PAC_DESADDR_MAXDIFF								100				

#define PACNUMCOMP_THRES									100
#define SELFDEC_INITVALUE									40000000
#define ASYNC_CNT_THRES										10
#define AFC_POLICY_ADJUST_THRES						8			

#define SCODE_INIT												0x00001		

#define DIGITAL_IF_FREQ_MHZ								4.0
#define DIGITAL_IF_FS280									0x2405
#define DFE_AD_SAMPLE_RATE_MHZ						25.0
#define DFE_DDC_NCOFREQ_WIDTH							26

#endif

#endif
