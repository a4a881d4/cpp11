#ifndef _FDE_H
#define _FDE_H

#include <bs_config.h>
#include <cfftw.h>
#include <complexoperation.h>
#include <xmmintrin.h>

class HDS_FDE				
{
	
	public:
		HDS_FDE(  ); 
		~HDS_FDE( );
		void HDS_FDE_Init( unsigned long ul_SCodeNum ); 
		void FDE_1Block( int i_BlockNum, bool * bp_ZeroFlag, float * * fp_RecData_1Block, float * fp_FDEResult_1Block, int * ip_AFC_ACCInt, float * fp_Angle_Radian );
		ComplexOperation CO;
	
	private:
		
		int i_RecPLength_Div2; 
		int i_RecPCPLength_Div2; 
		int i_RecTBLength_2_TranTBLength_TruncPos;
		float f_FDE_Revise_Factor;
		
		float fa_Pilot_FD[ REC_PILOTLENGTH * 2 ] __attribute__((aligned(16)));
		
		float fa_RecPLengthFFTIn[ REC_PILOTLENGTH * 2 ] __attribute__((aligned(16)));
		float fa_RecPLengthFFTOut[ REC_PILOTLENGTH * 2 ] __attribute__((aligned(16)));
		CFFTW RecPLengthFFT;
		float fa_RecPLengthIFFTOut[ REC_PILOTLENGTH * 2 ] __attribute__((aligned(16)));
		CFFTW RecPLengthIFFT;
		
		float fa_RecTBLength_ChanEstFFTIn[ FDE_ANTNUM ][ REC_TBLENGTH * 2 ] __attribute__((aligned(16)));
		CFFTW RecTBLengthChanEstFFT[ FDE_ANTNUM ];
		
		float fa_RecTBLengthFFTIn[ FDE_ANTNUM ][ REC_TBLENGTH * 2 ] __attribute__((aligned(16)));
		CFFTW RecTBLengthFFT[ FDE_ANTNUM ];
		
		float fa_TranTBLengthIFFTIn[ TRAN_TBLENGTH * 2 ] __attribute__((aligned(16)));
		float fa_TranTBLengthIFFTOut[ TRAN_TBLENGTH * 2 ] __attribute__((aligned(16)));
		CFFTW TranTBLengthIFFT;
		
		float * fpa_ChanEstBuf[ FDE_ANTNUM ][ CHANEST_BUFDEPTH ];
		
		float * fpa_CurChanEst_FD[ FDE_ANTNUM ];
		float * fpa_CurRecData_FD[ FDE_ANTNUM ];
		float fa_CurFDEResult_FD_RecTBLength[ REC_TBLENGTH * 2 ] __attribute__((aligned(16)));
		float fa_CurChanEst_FD_Abs2[ REC_TBLENGTH * 2 ] __attribute__((aligned(16)));
		
		
		int ia_Descramble_SCode[ MAX_TRAN_DBNUM_1PSCBLOCK * ( ( int )( TRAN_DLENGTH / 2 ) ) ];
		__m128 m128a_Descramble_Factor[ 16 ]; 
		
		void ChanEst( int i_Ant_ID );

		void CurChanEst( int i_Ant_ID );
		void CurChanEst_Win_RecPLength( int i_Ant_ID );
		float fa_CurChanEst_Win[ REC_PILOTLENGTH * 2 ]; 
		float fa_CurChanEst_WinCoe[ REC_PILOTLENGTH * 2 ]; 
		void CurChanEst_Win_RecPCPLength( int i_Ant_ID );
		float fa_CurChanEst_WinCoe_PCPLength[ REC_PCPLENGTH * 2 ]; 

		int CurChanEst_SearchMaxAbs2( int i_Ant_ID );		
		float fa_AveNPow[ FDE_ANTNUM ];				
		void FDE( float * * fp_RecData_1Block, bool * bp_ZeroFlag );
		void FDE_Weight( float * * fp_RecData_1Block, bool * bp_ZeroFlag );
		
		float * fpa_AFC_ChanEstConjMul_Buf[ FDE_ANTNUM ][ AFC_MAXCHANESTINT ][ AFC_ACCTIMES ];
		float * fpa_AFC_ACCResult[ FDE_ANTNUM ][ AFC_MAXCHANESTINT ];
		
		void AFC( int i_Ant_ID, int i_AFC_ACCInt, float * fp_Angle_Radian ); 
		int AFC_SearchMax( int i_Ant_ID, int i_AFC_ACCInt );
		
		void SCodeInit( unsigned long ul_SCodeNum );
		unsigned int SCode_1Point( unsigned long * ulp_X, unsigned long * ulp_Y );
		unsigned int XorAllBits( unsigned long ul_A, int i_Width_Bit );
		
		void Descramble( int i_BlockNum, float * fp_FDEResult_1Block );
};

#endif
