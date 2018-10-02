#include <fde.h>
#include <math.h>
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hds_fde_chanest_wincoe.c"

HDS_FDE::HDS_FDE(  ): CO( )
{
	int i;
	int j;
	int k;
	const float pi = 3.14159265359;
	const int ic_InsertPos = ( int )( TRAN_PILOTLENGTH / 2 + 1 );
	const float f_Pilot_FD_Amp = sqrt( float( TRAN_PILOTLENGTH ) );
	FILE * Fp;
	
	float fa_Pilot_TD[ TRAN_PILOTLENGTH * 2 ] __attribute__((aligned(16))); 
	float fa_Pilot_FD_Temp[ TRAN_PILOTLENGTH * 2 ] __attribute__((aligned(16)));
	CFFTW PilotFFT_Temp;
	PilotFFT_Temp.CFFTW_Init( fa_Pilot_TD, fa_Pilot_FD_Temp, 1, TRAN_PILOTLENGTH );
	
	RecPLengthFFT.CFFTW_Init( fa_RecPLengthFFTIn, fa_RecPLengthFFTOut, 1, REC_PILOTLENGTH );
	
	RecPLengthIFFT.CFFTW_Init( fa_RecPLengthFFTOut, fa_RecPLengthIFFTOut, 0, REC_PILOTLENGTH );
	

	//��ʼ����ǰ�ŵ����ƽ����Ƶ���Լ�Ƶ�����ݻ�����
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		fpa_CurChanEst_FD[ i ] = NULL;
		fpa_CurChanEst_FD[ i ] = ( float * ) memalign( 16, REC_TBLENGTH * 2 * sizeof( float ) );
		if ( fpa_CurChanEst_FD[ i ] == NULL )
		{
			printf( "Can't allocate memory for CurChanEst_FD, terminating.\n" );
			exit( 1 );
		}
		fpa_CurRecData_FD[ i ] = NULL;
		fpa_CurRecData_FD[ i ] = ( float * ) memalign( 16, REC_TBLENGTH * 2 * sizeof( float ) );
		if ( fpa_CurRecData_FD[ i ] == NULL )
		{
			printf( "Can't allocate memory for CurRecData_FD, terminating.\n" );
			exit( 1 );
		}
	}
	//��ʼ����ǰ�ŵ����ƽ����Ƶ���Լ�Ƶ�����ݻ�����

	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		RecTBLengthChanEstFFT[ i ].CFFTW_Init( fa_RecTBLength_ChanEstFFTIn[ i ], fpa_CurChanEst_FD[ i ], 1, REC_TBLENGTH );
		//��ʼ���ŵ�����fa_RecTBLength_ChanEstFFTIn�������벿������
		memset( fa_RecTBLength_ChanEstFFTIn[ i ], 0, ( REC_TBLENGTH ) * 2 * sizeof( float ) );
		//��ʼ���ŵ�����fa_RecTBLength_ChanEstFFTIn�������벿������
	}
	
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		RecTBLengthFFT[ i ].CFFTW_Init( fa_RecTBLengthFFTIn[ i ], fpa_CurRecData_FD[ i ], 1, REC_TBLENGTH );
	}
	
	TranTBLengthIFFT.CFFTW_Init( fa_TranTBLengthIFFTIn, fa_TranTBLengthIFFTOut, 0, TRAN_TBLENGTH );
	
	i_RecPLength_Div2 = ( int )( REC_PILOTLENGTH / 2 );
	i_RecPCPLength_Div2 = ( int )( REC_PCPLENGTH / 2 );
	
	//��ʼ��Ƶ��Ƶ
	for ( i = 0; i < TRAN_PILOTLENGTH; i++ )
	{
		fa_Pilot_TD[ i * 2 ] = cos( pi * i * i / TRAN_PILOTLENGTH );
		fa_Pilot_TD[ i * 2 + 1 ] = sin( pi * i * i / TRAN_PILOTLENGTH );
	}
	PilotFFT_Temp.doit( );
	
	memcpy( fa_Pilot_FD, fa_Pilot_FD_Temp, ic_InsertPos * 2 * sizeof( float ) );
	memcpy( fa_Pilot_FD + ( REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos ) * 2, fa_Pilot_FD_Temp + ic_InsertPos * 2, ( TRAN_PILOTLENGTH - ic_InsertPos ) * 2 * sizeof( float ) );
	for ( i = ic_InsertPos + 1; i < REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos - 1; i++ )
	{
		*( fa_Pilot_FD + i * 2 ) = f_Pilot_FD_Amp;
		*( fa_Pilot_FD + i * 2 + 1 ) = 0.0;
	}
	*( fa_Pilot_FD + ic_InsertPos * 2 ) = *( fa_Pilot_FD + ( REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos ) * 2 );
	*( fa_Pilot_FD + ic_InsertPos * 2 + 1 ) = *( fa_Pilot_FD + ( REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos ) * 2 + 1 );
	*( fa_Pilot_FD + ( REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos - 1 ) * 2 ) = *( fa_Pilot_FD + ( ic_InsertPos - 1 ) * 2 );
	*( fa_Pilot_FD + ( REC_PILOTLENGTH - TRAN_PILOTLENGTH + ic_InsertPos - 1 ) * 2 + 1 ) = *( fa_Pilot_FD + ( ic_InsertPos - 1 ) * 2 + 1 );
	//�ڱ�Ե���ֱ�������ֵ������λ����Ϊ1
	//��ʼ��Ƶ��Ƶ

	//��ʼ����RecTBLength��ȡ��TranTBLengthʱ�Ľ�ȡλ�ã���λ�õ�ѡȡ��Ƶ��Ƶ�ĳ�ʼ���й�
	i_RecTBLength_2_TranTBLength_TruncPos = ic_InsertPos * ( ( int )( REC_TBLENGTH / REC_PILOTLENGTH ) ) + ( ( int )( REC_TBLENGTH / REC_PILOTLENGTH / 2 ) );
	//ע�⣺REC_TBLENGTH������REC_PILOTLENGTH������������Ҫ��TRAN_PILOTLENGTH��TRAN_TBLENGTH��1/N��NΪ������������һ�������HDS_BS_Config.h��
	//��ʼ����RecTBLength��ȡ��TranTBLengthʱ�Ľ�ȡλ�ã���λ�õ�ѡȡ��Ƶ��Ƶ�ĳ�ʼ���й�
	
	//Ƶ�����У������
	//ע�⣺Ŀǰʹ�õ�FFT�Լ�IFFT��������δ��ϵ������1/sqrt(N)
	//׼ȷ�ŵ�����ֵ��Ӧ����Ŀǰ��������ϳ���sqrt( TRAN_PILOTLENGTH ) / sqrt( REC_PILOTLENGTH ) / sqrt( REC_PILOTLENGTH )
	//ǰ�����ǵ���FFT������죬��һ���ǵ���IFFT�������
	//���㵱ǰ�ŵ�����ֵʱ���ۼӽ��δ��2����������Ӧ�ó�2��ע�⣺���������ŵ������㷨�й�
	//IFFTʱ����TRAN_TBLENGTH������Ӧ�ó���sqrt(TRAN_TBLENGTH)
	//��������£����������Ӧ����ģΪ1������Ϊ����Ӧ����������Ҫ��������ʵ���Լ��鲿������13���ң�����Ҫ����18�Ե�������
	f_FDE_Revise_Factor = ( float( REC_PILOTLENGTH ) ) / sqrt( float( TRAN_PILOTLENGTH ) ) / sqrt( ( float )( TRAN_TBLENGTH ) ) * 2.0 / 4.0 * 18.0;//18.0;
	// printf( "%f\n", f_FDE_Revise_Factor );
	//Ƶ�����У������
	
	//��ʼ���ŵ����ƽ��������
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		for ( j = 0; j < CHANEST_BUFDEPTH; j++ )
		{
			fpa_ChanEstBuf[ i ][ j ] = NULL;
			fpa_ChanEstBuf[ i ][ j ] = ( float * ) memalign( 16, REC_PILOTLENGTH * 2 * sizeof( float ) );
			if ( ( fpa_ChanEstBuf[ i ][ j ] ) == NULL )
			{
				printf( "Can't allocate memory for ChanEstBuf, terminating.\n" );
				exit( 1 );
			}
			else
			{
				memset( fpa_ChanEstBuf[ i ][ j ], 0, REC_PILOTLENGTH * 2 * sizeof( float ) );
			}
		}
	}
	//��ʼ���ŵ����ƽ��������
			
	//��ʼ��AFC���ŵ����ƹ��󸴳˽��������
	for ( k = 0; k < FDE_ANTNUM; k++ )
	{
		for ( i = 0; i < AFC_MAXCHANESTINT; i++ )
		{
			for ( j = 0; j < AFC_ACCTIMES; j++ )
			{
				fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ] = NULL;
				fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ] = ( float * ) memalign( 16, REC_PCPLENGTH * 2 * sizeof( float ) );
				if ( ( fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ] ) == NULL )
				{
					printf( "Can't allocate memory for AFC_ChanEstConjMul_Buf, terminating.\n" );
					exit( 1 );
				}
				else
				{
					memset( fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ], 0, REC_PCPLENGTH * 2 * sizeof( float ) );
				}
			}
		}
	}
	//��ʼ��AFC���ŵ����ƹ��󸴳˽��������
	
	//��ʼ��AFC���ۼӽ��������
	for ( k = 0; k < FDE_ANTNUM; k++ )
	{
		for ( i = 0; i < AFC_MAXCHANESTINT; i++ )
		{
			fpa_AFC_ACCResult[ k ][ i ] = NULL;
			fpa_AFC_ACCResult[ k ][ i ] = ( float * ) memalign( 16, REC_PCPLENGTH * 2 * sizeof( float ) );
			if ( ( fpa_AFC_ACCResult[ k ][ i ] ) == NULL )
			{
				printf( "Can't allocate memory for AFC_ACCResult, terminating.\n" );
				exit( 1 );
			}
			else
			{
				memset( fpa_AFC_ACCResult[ k ][ i ], 0, REC_PCPLENGTH * 2 * sizeof( float ) );
			}
		}
	}
	//��ʼ��AFC���ۼӽ��������

	//i_Debug_Cnt = 0;
	
}

HDS_FDE::~HDS_FDE( )
{
	int i, j, k;
	
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		for ( j = 0; j < CHANEST_BUFDEPTH; j++ )
		{
			if ( fpa_ChanEstBuf[ i ][ j ] ) free( fpa_ChanEstBuf[ i ][ j ] );
		}
		if ( fpa_CurChanEst_FD[ i ] ) free( fpa_CurChanEst_FD[ i ] );
		if ( fpa_CurRecData_FD[ i ] ) free( fpa_CurRecData_FD[ i ] );
	}
	
	for ( k = 0; k < FDE_ANTNUM; k++ )
	{
		for ( i = 0; i < AFC_MAXCHANESTINT; i++ )
		{
			for ( j = 0; j < AFC_ACCTIMES; j++ )
			{
				if ( fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ] ) free( fpa_AFC_ChanEstConjMul_Buf[ k ][ i ][ j ] );
			}
			if ( fpa_AFC_ACCResult[ k ][ i ] ) free( fpa_AFC_ACCResult[ k ][ i ] );
		}
	}
	
}

void HDS_FDE::HDS_FDE_Init( unsigned long ul_SCodeNum )
{
	SCodeInit( ul_SCodeNum ); // ��ʼ����������
}

void HDS_FDE::FDE_1Block( int i_BlockNum, bool * bp_ZeroFlag, float * * fp_RecData_1Block, float * fp_FDEResult_1Block, int * ip_AFC_ACCInt, float * fp_Angle_Radian ) // ע�⣺���롢�����Ϊ__attribute__((aligned(16)))
{
	int i;
	
	//���ζԸ��������ݽ����ŵ�����
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecPLengthFFTIn, ( *( fp_RecData_1Block + i ) ) + ( REC_TBLENGTH - REC_PILOTLENGTH ) * 2, REC_PILOTLENGTH * 2 * sizeof( float ) );
			//���ʱһ��������˳��D+PCP+P; fp_RecPLengthFFTInָ��Ƶ��ʼ
			ChanEst( i );
			if ( ( ( *( ip_AFC_ACCInt + i ) ) > AFC_MAXCHANESTINT ) || ( ( *( ip_AFC_ACCInt + i ) ) < 0 ) )
			{
				printf( "Invalid AFC_ACCInt!\n" );
				exit( 1 );
			}
			if ( ( *( ip_AFC_ACCInt + i ) ) != 0 ) AFC( i, *( ip_AFC_ACCInt + i ), fp_Angle_Radian + i );
			// AFC_ACCInt���ۼӼ��ѡ��AFC_ACCInt = 0 ����AFC
		}
	}
	//���ζԸ��������ݽ����ŵ�����
	
	if ( i_BlockNum != 0 )
	{
		//Ƶ�����
		//FDE( fp_RecData_1Block, bp_ZeroFlag );
		FDE_Weight( fp_RecData_1Block, bp_ZeroFlag );
		//Ƶ�����	
		Descramble( i_BlockNum, fp_FDEResult_1Block ); // 
	}
}

void HDS_FDE::ChanEst( int i_Ant_ID )
{
	int i;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	float * fp_ChanEstBuf_Temp = NULL;
	FILE * Fp;
	
/*	Fp = fopen( "data/ChanEst_In_TD.dat", "a" );
	for ( i = 0; i < REC_PILOTLENGTH; i++ )
	{
		fprintf( Fp, "%f\t\t%f\n", fa_RecPLengthFFTIn[ i * 2 ], fa_RecPLengthFFTIn[ i * 2 + 1 ] );
	}
	fclose( Fp );*/
	
	// �ŵ�����
	RecPLengthFFT.doit( );
/*	Fp = fopen( "data/ChanEst_In_FD.dat", "a" );
	for ( i = 0; i < REC_PILOTLENGTH; i++ )
	{
		fprintf( Fp, "%f\t\t%f\n", fa_RecPLengthFFTOut[ i * 2 ], fa_RecPLengthFFTOut[ i * 2 + 1 ] );
	}
	fclose( Fp );*/
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp0 = ( __m128 * )( fa_RecPLengthFFTOut + i * 4 );
		m128p_Temp1 = ( __m128 * )( fa_Pilot_FD + i * 4 );
		*m128p_Temp0 = CO.ComplexDiv_2( *m128p_Temp0, *m128p_Temp1 );
	}
	RecPLengthIFFT.doit( );
	// �ŵ�����
	
/*	Fp = fopen( "data/ChanEstResult.dat", "a" );
	for ( i = 0; i < REC_PILOTLENGTH; i++ )
	{
		fprintf( Fp, "%f\t\t%f\n", fa_RecPLengthIFFTOut[ i * 2 ], fa_RecPLengthIFFTOut[ i * 2 + 1 ] );
	}
	fclose( Fp );*/
	
	// ˢ���ŵ����ƻ�����
	fp_ChanEstBuf_Temp = fpa_ChanEstBuf[ i_Ant_ID ][ CHANEST_BUFDEPTH - 1 ];
	for ( i = CHANEST_BUFDEPTH - 1; i > 0; i-- )
	{
		fpa_ChanEstBuf[ i_Ant_ID ][ i ] = fpa_ChanEstBuf[ i_Ant_ID ][ i - 1 ];
	}
	fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] = fp_ChanEstBuf_Temp;
	memcpy( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ], fa_RecPLengthIFFTOut, REC_PILOTLENGTH * 2 * sizeof( float ) );
	// ˢ���ŵ����ƻ�����
	
	//CurChanEst( i_Ant_ID ); //���㵱ǰ�ŵ�����ֵ
	//CurChanEst_Win_RecPLength( i_Ant_ID ); //���㵱ǰ�ŵ�����ֵ
	CurChanEst_Win_RecPCPLength( i_Ant_ID );
}
		
void HDS_FDE::CurChanEst( int i_Ant_ID )
{
	int i;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 m128_Q;
	
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ������ΪREC_PILOTLENGTH������ĵ��ڹ��캯�����Ѿ�����
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128_Q = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
		CO.m128_2_Float_Store( m128_Q, fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ] + i * 4 );
	}
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ
	//��Ҫע����ǣ�����û�ж��ŵ�����ֵ������������ʵ�ϣ�Ӧ��/2�������ͳһ����
	//���޸����ŵ����Ƶ��ۼӷ�������������ҲӦ����Ӧ�仯
	
	RecTBLengthChanEstFFT[ i_Ant_ID ].doit( );
//	memcpy( fpa_CurChanEst_FD[ i_Ant_ID ], fa_RecTBLength_ChanEstFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
}

void HDS_FDE::CurChanEst_Win_RecPLength( int i_Ant_ID )
{
	int i;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 * m128p_Temp2 = NULL;
	int i_MaxAbs2_Pos;
	__m128 m128_Q;
	
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ������ΪREC_PILOTLENGTH������ĵ��ڹ��캯�����Ѿ�����
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
	}//ǰ���������
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ
	//��Ҫע����ǣ�����û�ж��ŵ�����ֵ������������ʵ�ϣ�Ӧ��/2�������ͳһ����
	//���޸����ŵ����Ƶ��ۼӷ�������������ҲӦ����Ӧ�仯
	
	i_MaxAbs2_Pos = CurChanEst_SearchMaxAbs2( i_Ant_ID ); // 0<=i_MaxAbs2_Pos<=REC_PCPLENGTH
	memcpy( fa_CurChanEst_WinCoe, fa_ChanEstWinCoe + ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) * 2, 
					( REC_PILOTLENGTH - ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) ) * 2 * sizeof( float ) );
	memcpy( fa_CurChanEst_WinCoe + ( REC_PILOTLENGTH - ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) ) * 2, 
					fa_ChanEstWinCoe, ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) * 2 * sizeof( float ) );		
	
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_WinCoe + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = _mm_mul_ps( *m128p_Temp1, *m128p_Temp2 );
	}
	
	memset( fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ], 0, REC_PILOTLENGTH * 2 * sizeof( float ) );
	memset( fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ] + ( REC_TBLENGTH - REC_PILOTLENGTH ) * 2, 0, REC_PILOTLENGTH * 2 * sizeof( float ) );
	memcpy( fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ], fa_CurChanEst_Win, 
				( REC_PILOTLENGTH - ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) ) * 2 * sizeof( float ) );
	memcpy( fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ] + ( REC_TBLENGTH - ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) ) * 2, 
					fa_CurChanEst_Win + ( REC_PILOTLENGTH - ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) ) * 2, 
					( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) * 2 * sizeof( float ) );
	
	RecTBLengthChanEstFFT[ i_Ant_ID ].doit( );
	//memcpy( fpa_CurChanEst_FD[ i_Ant_ID ], fa_RecTBLength_ChanEstFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
}

void HDS_FDE::CurChanEst_Win_RecPCPLength( int i_Ant_ID )
{
	int i;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 * m128p_Temp2 = NULL;
	int i_MaxAbs2_Pos;
	
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ������ΪREC_PCPLENGTH������ĵ��ڹ��캯�����Ѿ�����
	for ( i = 0; i < i_RecPLength_Div2; i++ )		//����ʵ������i_RecPCPLength_Div2���ɣ���Ϊ��ͳ���������ʣ��ͰѺ����Ҳ����
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
	}//ǰ���������
	//���㵱ǰ�ŵ����Ƶ�ʱ��ֵ
	//��Ҫע����ǣ�����û�ж��ŵ�����ֵ������������ʵ�ϣ�Ӧ��/2�������ͳһ����
	//���޸����ŵ����Ƶ��ۼӷ�������������ҲӦ����Ӧ�仯
	
//	i_Debug_Cnt++;
/*	if ( i_Debug_Cnt == 5000 ) 
	{
		fp_Debug = fopen( "/mnt/ddriver/zhaoyf/HDS/Debug_ChanEst_0.dat", "a" );
		for ( i = 0; i < REC_PCPLENGTH; i++ ) fprintf( fp_Debug, "%f\t%f\n", *( fa_CurChanEst_Win + i * 2 ), *( fa_CurChanEst_Win + i * 2 + 1 ) );
		fclose( fp_Debug );
	}*/
	
	i_MaxAbs2_Pos = CurChanEst_SearchMaxAbs2( i_Ant_ID ); // 0<=i_MaxAbs2_Pos<=REC_PCPLENGTH<=1/2*REC_PILOTLENGTH
/*	if ( i_Debug_Cnt == 5000 ) 
	{
		fp_Debug = fopen( "/mnt/ddriver/zhaoyf/HDS/Debug_MaxPos.dat", "a" );
		fprintf( fp_Debug, "%d\t%f\n", i_MaxAbs2_Pos, fa_AveNPow[ i_Ant_ID ] );
		fclose( fp_Debug );
	}*/
	
	memcpy( fa_CurChanEst_WinCoe_PCPLength,	fa_ChanEstWinCoe + ( i_RecPLength_Div2 - 1 - i_MaxAbs2_Pos ) * 2, 
					REC_PCPLENGTH * 2 * sizeof( float ) );		
/*	if ( i_Debug_Cnt == 5000 ) 
	{
		fp_Debug = fopen( "/mnt/ddriver/zhaoyf/HDS/Debug_ChanEst_WinCoe.dat", "a" );
		for ( i = 0; i < REC_PCPLENGTH; i++ ) fprintf( fp_Debug, "%f\t%f\n", *( fa_CurChanEst_WinCoe_PCPLength + i * 2 ), *( fa_CurChanEst_WinCoe_PCPLength + i * 2 + 1 ) );
		fclose( fp_Debug );
	}*/
	
	for ( i = 0; i < i_RecPCPLength_Div2; i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_WinCoe_PCPLength + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = _mm_mul_ps( *m128p_Temp1, *m128p_Temp2 );
	}
	
	memcpy( fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ], fa_CurChanEst_Win, REC_PCPLENGTH * 2 * sizeof( float ) );
/*	if ( i_Debug_Cnt == 5000 ) 
	{
		fp_Debug = fopen( "/mnt/ddriver/zhaoyf/HDS/Debug_ChanEst_FFTIn.dat", "a" );
		for ( i = 0; i < REC_TBLENGTH; i++ ) fprintf( fp_Debug, "%f\t%f\n", *( fa_RecTBLength_ChanEstFFTIn + i * 2 ), *( fa_RecTBLength_ChanEstFFTIn + i * 2 + 1 ) );
		fclose( fp_Debug );
	}*/
	
	RecTBLengthChanEstFFT[ i_Ant_ID ].doit( );
//	memcpy( fpa_CurChanEst_FD[ i_Ant_ID ], fa_RecTBLength_ChanEstFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
/*	if ( i_Debug_Cnt == 5000 ) 
	{
		fp_Debug = fopen( "/mnt/ddriver/zhaoyf/HDS/Debug_ChanEst_1.dat", "a" );
		for ( i = 0; i < REC_TBLENGTH; i++ ) fprintf( fp_Debug, "%f\t%f\n", *( fpa_CurChanEst_FD[ i_Ant_ID ] + i * 2 ), *( fpa_CurChanEst_FD[ i_Ant_ID ] + i * 2 + 1 ) );
		fclose( fp_Debug );
	}
	if ( i_Debug_Cnt == 5000 ) 
	{
		i_Debug_Cnt = 0;
	}*/
	
}

int HDS_FDE::CurChanEst_SearchMaxAbs2( int i_Ant_ID )
{
	int i;
	int i_MaxPos[ 2 ] = { 0, 0 };
	float fa_MaxAbs2[ 4 ] __attribute__((aligned(16)));
	float fa_Abs2[ 4 ] __attribute__((aligned(16)));
	float fa_ComResult[ 4 ] __attribute__((aligned(16)));
	__m128 * m128p_MaxAbs2;
	__m128 * m128p_Temp1;
	__m128 * m128p_Abs2;
	
	float fa_Abs2Sum[ 4 ] __attribute__((aligned(16)));
	__m128 * m128p_Abs2Sum;
	
	m128p_MaxAbs2 = ( __m128 * )( fa_MaxAbs2 );
	*m128p_MaxAbs2 = _mm_setzero_ps( );
	m128p_Abs2 = ( __m128 * )( fa_Abs2 );
	for ( i = 0; i < i_RecPCPLength_Div2; i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Abs2 = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
		*( ( __m128 * )( fa_ComResult ) ) = _mm_cmplt_ps( *m128p_MaxAbs2, *m128p_Abs2 );
		if ( fa_ComResult[ 0 ] != 0 )
		{
			i_MaxPos[ 0 ] = i * 2;
			fa_MaxAbs2[ 0 ] = fa_Abs2[ 0 ];
		}
		if ( fa_ComResult[ 2 ] != 0 )
		{
			i_MaxPos[ 1 ] = i * 2 + 1;
			fa_MaxAbs2[ 2 ] = fa_Abs2[ 2 ];
		}
	}

	m128p_Abs2Sum = ( __m128 * )( fa_Abs2Sum );
	*m128p_Abs2Sum = _mm_setzero_ps( );
	for ( i = i_RecPCPLength_Div2; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Abs2 = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
		*m128p_Abs2Sum = _mm_add_ps( *m128p_Abs2Sum, *m128p_Abs2 );
	}
	fa_AveNPow[ i_Ant_ID ] = fa_Abs2Sum[ 0 ] + fa_Abs2Sum[ 2 ];	//������ƽ��Ҳ��
	if ( fa_AveNPow[ i_Ant_ID ] <= 0.0 ) fa_AveNPow[ i_Ant_ID ] = 0;
	else fa_AveNPow[ i_Ant_ID ] = 1.0 / fa_AveNPow[ i_Ant_ID ];

	if ( fa_MaxAbs2[ 0 ] > fa_MaxAbs2[ 2 ] ) 
		return i_MaxPos[ 0 ];
	else
		return i_MaxPos[ 1 ];
}
 
void HDS_FDE::FDE( float * * fp_RecData_1Block, bool * bp_ZeroFlag )
{
	int i, j;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 * m128p_Temp2 = NULL;
	__m128 m128_Q;
	float fa_Temp[ 4 ] __attribute__((aligned(16)));
	float f_NPow;
	
	//���μ�����������ݵ�Ƶ��ֵ
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecTBLengthFFTIn[ i ], *( fp_RecData_1Block + i ), REC_TBLENGTH * 2 * sizeof( float ) );
			RecTBLengthFFT[ i ].doit( );
			//memcpy( fpa_CurRecData_FD[ i ], fa_RecTBLengthFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
		}
	}
	//���μ�����������ݵ�Ƶ��ֵ

	//����Ƶ�������
	memset( fa_CurFDEResult_FD_RecTBLength, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//Ƶ��������
	memset( fa_CurChanEst_FD_Abs2, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//�ŵ�����ģƽ������
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			for ( j = 0; j < ( ( int )( REC_TBLENGTH / 2 ) ); j++ )
			{
				m128p_Temp0 = ( __m128 * )( fpa_CurRecData_FD[ i ] + j * 4 );
				m128p_Temp1 = ( __m128 * )( fpa_CurChanEst_FD[ i ] + j * 4 );
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp0, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurFDEResult_FD_RecTBLength + j * 4 );
				m128_Q = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				CO.m128_2_Float_Store( m128_Q, fa_CurFDEResult_FD_RecTBLength + j * 4 );
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + j * 4 );
				m128_Q = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				m128_Q = _mm_shuffle_ps( m128_Q, m128_Q, _MM_SHUFFLE( 2, 2, 0, 0 ) ); 
				CO.m128_2_Float_Store( m128_Q, fa_CurChanEst_FD_Abs2 + j * 4 );
			}
		}
	}//�ڸ�Ƶ���ϣ������������ݳ����ŵ����ƹ����ۼӣ��ŵ�����ģƽ�����ۼ�
	
	memmove( fa_CurFDEResult_FD_RecTBLength + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurFDEResult_FD_RecTBLength + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	memmove( fa_CurChanEst_FD_Abs2 + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurChanEst_FD_Abs2 + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	//ȥ���м����õĲ��֣�ƴ�ӳ�TRAN_TBLENGTH���������źţ�������ڻ�������ǰ��

	m128_Q = CO.Float_2_m128_Load( fa_CurChanEst_FD_Abs2 );
	for ( i = 1; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		m128_Q = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}
	CO.m128_2_Float_Store( m128_Q, fa_Temp );
	f_NPow = ( ( *fa_Temp ) + ( *( fa_Temp + 2 ) ) ) / TRAN_TBLENGTH / FDE_SNR;
	if ( f_NPow <= 0.0 ) f_NPow = 1.0; //�����󣬿��ܻ��ͽ���ȫ0������
	m128_Q = _mm_set_ps1( f_NPow ); //������������

	for ( i = 0; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		*m128p_Temp1 = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}//���������ʼӵ��ŵ�����ģƽ������
	m128_Q = _mm_set_ps1( f_FDE_Revise_Factor );
	for ( i = 0; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp0 = ( __m128 * )( fa_CurFDEResult_FD_RecTBLength + i * 4 );
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_TranTBLengthIFFTIn + i * 4 );
		*m128p_Temp2 = _mm_div_ps( *m128p_Temp0, *m128p_Temp1 );
		*m128p_Temp2 = _mm_mul_ps( *m128p_Temp2, m128_Q );
	}

	TranTBLengthIFFT.doit( );
	//����Ƶ�������
}
    
void HDS_FDE::FDE_Weight( float * * fp_RecData_1Block, bool * bp_ZeroFlag )
{
	int i, j;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 * m128p_Temp2 = NULL;
	__m128 m128_Q;
	float fa_Temp[ 4 ] __attribute__((aligned(16)));
	float f_NPow;
	float f_AveNPow_Sum = 0.0;
	__m128 m128a_Weight[ FDE_ANTNUM ];
	
	//���μ�����������ݵ�Ƶ��ֵ
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecTBLengthFFTIn[ i ], *( fp_RecData_1Block + i ), REC_TBLENGTH * 2 * sizeof( float ) );
			RecTBLengthFFT[ i ].doit( );
			//memcpy( fpa_CurRecData_FD[ i ], fa_RecTBLengthFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
		}
	}
	//���μ�����������ݵ�Ƶ��ֵ

	//����Ƶ�������
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) ) f_AveNPow_Sum += fa_AveNPow[ i ]; //ƽ���������ʺ�
	}
	
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) ) m128a_Weight[ i ] = _mm_set_ps1( fa_AveNPow[ i ] / f_AveNPow_Sum ); //Ȩ��
	}
	
	memset( fa_CurFDEResult_FD_RecTBLength, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//Ƶ��������
	memset( fa_CurChanEst_FD_Abs2, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//�ŵ�����ģƽ������
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			for ( j = 0; j < ( ( int )( REC_TBLENGTH / 2 ) ); j++ )
			{
				m128p_Temp0 = ( __m128 * )( fpa_CurRecData_FD[ i ] + j * 4 );
				m128p_Temp1 = ( __m128 * )( fpa_CurChanEst_FD[ i ] + j * 4 );
				
				*m128p_Temp0 = _mm_mul_ps( *m128p_Temp0, m128a_Weight[ i ] );//��Ȩ
				*m128p_Temp1 = _mm_mul_ps( *m128p_Temp1, m128a_Weight[ i ] );//��Ȩ
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp0, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurFDEResult_FD_RecTBLength + j * 4 );
				*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + j * 4 );
				*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				*m128p_Temp0 = _mm_shuffle_ps( *m128p_Temp0, *m128p_Temp0, _MM_SHUFFLE( 2, 2, 0, 0 ) ); 
			}
		}
	}//�ڸ�Ƶ���ϣ������������ݳ����ŵ����ƹ����ۼӣ��ŵ�����ģƽ�����ۼ�
	
	memmove( fa_CurFDEResult_FD_RecTBLength + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurFDEResult_FD_RecTBLength + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	memmove( fa_CurChanEst_FD_Abs2 + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurChanEst_FD_Abs2 + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	//ȥ���м����õĲ��֣�ƴ�ӳ�TRAN_TBLENGTH���������źţ�������ڻ�������ǰ��

	m128_Q = CO.Float_2_m128_Load( fa_CurChanEst_FD_Abs2 );
	for ( i = 1; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		m128_Q = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}
	CO.m128_2_Float_Store( m128_Q, fa_Temp );
	f_NPow = ( ( *fa_Temp ) + ( *( fa_Temp + 2 ) ) ) / TRAN_TBLENGTH / FDE_SNR;
	if ( f_NPow <= 0.0 ) f_NPow = 1.0; //�����󣬿��ܻ��ͽ���ȫ0������
	m128_Q = _mm_set_ps1( f_NPow ); //������������
  __m128 m128_fix = _mm_set_ps1( f_FDE_Revise_Factor );
	__m128 t0,t1;
  
	m128p_Temp0 = ( __m128 * )( fa_CurFDEResult_FD_RecTBLength);
	m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2);
	m128p_Temp2 = ( __m128 * )( fa_TranTBLengthIFFTIn);
	i=TRAN_TBLENGTH / 2;
	for ( ; i ; i--, m128p_Temp0++,m128p_Temp1++,m128p_Temp2++ )
	{
		
		t0 = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
		t1 = _mm_div_ps( *m128p_Temp0, t0 );
		t1 = _mm_mul_ps( t1, m128_fix );
		*m128p_Temp2=t1;
	}

	TranTBLengthIFFT.doit( );
	//����Ƶ�������
}

void HDS_FDE::AFC( int i_Ant_ID, int i_AFC_ACCInt, float * fp_Angle_Radian ) // i_AFC_ACCInt���ۼӼ��ѡ��i_AFC_ACCInt = 0 ����AFC��1 <= i_AFC_ACCInt <= AFC_MAXCHANESTINT
{
	int i;
	int j;
	__m128 * m128p_Temp0;
	__m128 * m128p_Temp1;
	__m128 * m128p_Temp2;
	__m128 m128_Q;
	int i_MaxPos;
	float f_Real;
	float f_Imag;
	const float pi = 3.14159265359;
	float * fp_Temp;
	
	for ( i = 0; i < AFC_MAXCHANESTINT; i++ )
	{
		for ( j = 0; j < ( ( int )( REC_PCPLENGTH / 2 ) ); j++ )
		{
			m128p_Temp0 = ( __m128 * )( fpa_AFC_ACCResult[ i_Ant_ID ][ i ] + j * 4 );
			m128p_Temp1 = ( __m128 * )( fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ AFC_ACCTIMES - 1 ] + j * 4 );
			*m128p_Temp0 = CO.ComplexSub_2( *m128p_Temp0, *m128p_Temp1 ); //���ۼӽ���м��������һ��
			m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + j * 4 );
			m128p_Temp2 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ i + 1 ] + j * 4 );
			m128_Q = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp2 ); //�����ŵ����ƽ��������һ�εĹ���
			*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );//�ۼ�
			CO.m128_2_Float_Store( m128_Q, ( fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ AFC_ACCTIMES - 1 ] + j * 4 ) );
			// �����˽������Ŀǰ����Ļ�������
		}
		fp_Temp = fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ AFC_ACCTIMES - 1 ];
		for ( j = AFC_ACCTIMES - 1; j > 0; j-- )
		{
			fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ j ] = fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ j - 1 ];
		}
		fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ 0 ] = fp_Temp;
	}
	
	i_MaxPos = AFC_SearchMax( i_Ant_ID, i_AFC_ACCInt );
	f_Real = *( fpa_AFC_ACCResult[ i_Ant_ID ][ i_AFC_ACCInt - 1 ] + i_MaxPos * 2 );
	f_Imag = *( fpa_AFC_ACCResult[ i_Ant_ID ][ i_AFC_ACCInt - 1 ] + i_MaxPos * 2 + 1 );
	if ( f_Real == 0 )
	{
		if ( f_Imag > 0 )
			*fp_Angle_Radian = pi / 2;
		else if ( f_Imag < 0 )
			*fp_Angle_Radian = ( -1.0 ) * pi / 2;
		else
			*fp_Angle_Radian = 0.0;	
	}
	else if ( ( f_Real > 0 ) && ( f_Imag >= 0 ) )
		*fp_Angle_Radian = atan( f_Imag / f_Real );
	else if ( ( f_Real < 0 ) && ( f_Imag >= 0 ) )
		*fp_Angle_Radian = atan( f_Imag / f_Real ) + pi;
	else if ( ( f_Real > 0 ) && ( f_Imag < 0 ) ) 
		*fp_Angle_Radian = atan( f_Imag / f_Real );
	else
		*fp_Angle_Radian = atan( f_Imag / f_Real ) - pi;
	
	//printf( "%d\t%d\t%f\t%f\t%f\n", i_Ant_ID, i_MaxPos, f_Real, f_Imag, *fp_Angle_Radian );				
}

int HDS_FDE::AFC_SearchMax( int i_Ant_ID, int i_AFC_ACCInt ) // 1 <= i_AFC_ACCInt <= AFC_MAXCHANESTINT
{
	int i;
	int i_MaxPos[ 2 ] = { 0, 0 };
	float fa_MaxAbs2[ 4 ] __attribute__((aligned(16)));
	float fa_Abs2[ 4 ] __attribute__((aligned(16)));
	float fa_ComResult[ 4 ] __attribute__((aligned(16)));
	__m128 * m128p_MaxAbs2;
	__m128 * m128p_Temp1;
	__m128 * m128p_Abs2;
	
	m128p_MaxAbs2 = ( __m128 * )( fa_MaxAbs2 );
	*m128p_MaxAbs2 = _mm_setzero_ps( );
	m128p_Abs2 = ( __m128 * )( fa_Abs2 );
	for ( i = 0; i < ( ( int )( REC_PCPLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fpa_AFC_ACCResult[ i_Ant_ID ][ i_AFC_ACCInt - 1 ] + i * 4 );
		*m128p_Abs2 = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
		*( ( __m128 * )( fa_ComResult ) ) = _mm_cmplt_ps( *m128p_MaxAbs2, *m128p_Abs2 );
		if ( fa_ComResult[ 0 ] != 0 )
		{
			i_MaxPos[ 0 ] = i * 2;
			fa_MaxAbs2[ 0 ] = fa_Abs2[ 0 ];
		}
		if ( fa_ComResult[ 2 ] != 0 )
		{
			i_MaxPos[ 1 ] = i * 2 + 1;
			fa_MaxAbs2[ 2 ] = fa_Abs2[ 2 ];
		}
	}
	if ( fa_MaxAbs2[ 0 ] > fa_MaxAbs2[ 2 ] ) 
		return i_MaxPos[ 0 ];
	else
		return i_MaxPos[ 1 ];
		
}

void HDS_FDE::SCodeInit( unsigned long ul_SCodeNum )
{
	unsigned long ul_X = ( ul_SCodeNum ) & 0x3FFFF;
	unsigned long ul_Y = 0x3FFFF;	
	unsigned int ui_SCode;
	int i;
//	float f_Temp0, f_Temp1, f_Temp2, f_Temp3;
	
//	for ( i = 0; i < ( TRAN_DBNUM_1PSCBLOCK * ( ( int )( TRAN_DLENGTH / 2 ) ) ); i++ )
	for ( i = 0; i < ( MAX_TRAN_DBNUM_1PSCBLOCK * ( ( int )( TRAN_DLENGTH / 2 ) ) ); i++ )
	{
		ui_SCode = 0;
		ui_SCode = SCode_1Point( &ul_X, &ul_Y );
		ia_Descramble_SCode[ i ] = ( SCode_1Point( &ul_X, &ul_Y ) << 2 ) + ui_SCode;
	}
	
	//ע�⣬���ʱ�˵�������Ĺ������1��j��
	m128a_Descramble_Factor[ 0 ] = _mm_set_ps( -1.0, 1.0, -1.0, 1.0 );
	m128a_Descramble_Factor[ 1 ] = _mm_set_ps( -1.0, 1.0, 1.0, 1.0 );
	m128a_Descramble_Factor[ 2 ] = _mm_set_ps( -1.0, 1.0, -1.0, -1.0 );
	m128a_Descramble_Factor[ 3 ] = _mm_set_ps( -1.0, 1.0, 1.0, -1.0 );
	
	m128a_Descramble_Factor[ 4 ] = _mm_set_ps( 1.0, 1.0, -1.0, 1.0 ); 
	m128a_Descramble_Factor[ 5 ] = _mm_set_ps( 1.0, 1.0, 1.0, 1.0 );  
	m128a_Descramble_Factor[ 6 ] = _mm_set_ps( 1.0, 1.0, -1.0, -1.0 );
	m128a_Descramble_Factor[ 7 ] = _mm_set_ps( 1.0, 1.0, 1.0, -1.0 ); 
	
	m128a_Descramble_Factor[ 8 ]  = _mm_set_ps( -1.0, -1.0, -1.0, 1.0 ); 
	m128a_Descramble_Factor[ 9 ]  = _mm_set_ps( -1.0, -1.0, 1.0, 1.0 );  
	m128a_Descramble_Factor[ 10 ] = _mm_set_ps( -1.0, -1.0, -1.0, -1.0 );
	m128a_Descramble_Factor[ 11 ] = _mm_set_ps( -1.0, -1.0, 1.0, -1.0 ); 
	
	m128a_Descramble_Factor[ 12 ] = _mm_set_ps( 1.0, -1.0, -1.0, 1.0 ); 
	m128a_Descramble_Factor[ 13 ] = _mm_set_ps( 1.0, -1.0, 1.0, 1.0 );  
	m128a_Descramble_Factor[ 14 ] = _mm_set_ps( 1.0, -1.0, -1.0, -1.0 );
	m128a_Descramble_Factor[ 15 ] = _mm_set_ps( 1.0, -1.0, 1.0, -1.0 ); 
	
}                                                   
                                                    
unsigned int HDS_FDE::SCode_1Point( unsigned long * ulp_X, unsigned long * ulp_Y )
{
	unsigned int i_ResultI, i_ResultQ;
	
	i_ResultI = ( unsigned int )( ( ( *ulp_X ) & 1 ) ^ ( ( *ulp_Y ) & 1 ) );
	i_ResultQ = XorAllBits( ( ( *ulp_X ) & 0x8050 ), 18 ) ^ XorAllBits( ( ( *ulp_Y ) & 0xFF60 ), 18 );
	*ulp_X = ( ( *ulp_X ) >> 1 ) | ( XorAllBits( ( ( *ulp_X ) & 0x81 ), 18 ) << 17 );
	*ulp_Y = ( ( *ulp_Y ) >> 1 ) | ( XorAllBits( ( ( *ulp_Y ) & 0x4A1 ), 18 ) << 17 );
	return ( ( i_ResultI << 1 ) | i_ResultQ );
}

unsigned int HDS_FDE::XorAllBits( unsigned long ul_A, int i_Width_Bit )
{
	unsigned long ul_Temp = 0;
	int i;
	
	for ( i = 0; i < i_Width_Bit; i++ )
	{
		ul_Temp ^= ( ( ul_A >> i ) & 1 );
	}
	
	return ( ( unsigned int )( ul_Temp ) );
}

void HDS_FDE::Descramble( int i_BlockNum, float * fp_FDEResult_1Block )
{
	int i;
	int i_BaseAddr = ( i_BlockNum - 1 ) * ( ( int )( TRAN_DLENGTH / 2 ) );
	__m128 * m128p_Temp0=( __m128 * )( fa_TranTBLengthIFFTOut);
	__m128 * m128p_Temp1=( __m128 * )( fp_FDEResult_1Block);
	 
	for (i=0;i<( TRAN_DLENGTH / 2 ); i++, m128p_Temp0++, m128p_Temp1++ )
	{
		__m128 t0=*m128p_Temp0;
		switch ( ia_Descramble_SCode[ i_BaseAddr + i ] ) //ע�⣬���ʱ�˵�������Ĺ������1��j��������Ϊ0��3ʱ����1��2����
		{
			case 0:		t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 0, 1 ) ); break;
			case 1: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 1, 0 ) ); break;
			case 2: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 1, 0 ) ); break;
			case 3: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 0, 1 ) ); break;
				                                                                                                
			case 4: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 0, 1 ) ); break;
			case 5: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 1, 0 ) ); break;
			case 6: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 1, 0 ) ); break;
			case 7: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 0, 1 ) ); break;
			                                                                                                 
			case 8: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 0, 1 ) ); break;
			case 9: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 1, 0 ) ); break;
			case 10: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 1, 0 ) ); break;
			case 11: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 3, 2, 0, 1 ) ); break;
			                                                                                                  
			case 12: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 0, 1 ) ); break;
			case 13: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 1, 0 ) ); break;
			case 14: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 1, 0 ) ); break;
			case 15: 	t0 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 2, 3, 0, 1 ) ); break;
			default:  break;
		}
		*m128p_Temp1 = _mm_mul_ps( t0, m128a_Descramble_Factor[ ia_Descramble_SCode[ i_BaseAddr + i ] ] );
	}
}
