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
	

	//初始化当前信道估计结果（频域）以及频域数据缓冲区
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
	//初始化当前信道估计结果（频域）以及频域数据缓冲区

	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		RecTBLengthChanEstFFT[ i ].CFFTW_Init( fa_RecTBLength_ChanEstFFTIn[ i ], fpa_CurChanEst_FD[ i ], 1, REC_TBLENGTH );
		//初始化信道估计fa_RecTBLength_ChanEstFFTIn，将其后半部分置零
		memset( fa_RecTBLength_ChanEstFFTIn[ i ], 0, ( REC_TBLENGTH ) * 2 * sizeof( float ) );
		//初始化信道估计fa_RecTBLength_ChanEstFFTIn，将其后半部分置零
	}
	
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		RecTBLengthFFT[ i ].CFFTW_Init( fa_RecTBLengthFFTIn[ i ], fpa_CurRecData_FD[ i ], 1, REC_TBLENGTH );
	}
	
	TranTBLengthIFFT.CFFTW_Init( fa_TranTBLengthIFFTIn, fa_TranTBLengthIFFTOut, 0, TRAN_TBLENGTH );
	
	i_RecPLength_Div2 = ( int )( REC_PILOTLENGTH / 2 );
	i_RecPCPLength_Div2 = ( int )( REC_PCPLENGTH / 2 );
	
	//初始化频域导频
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
	//在边缘处分别保留两个值，其他位置置为1
	//初始化频域导频

	//初始化从RecTBLength截取到TranTBLength时的截取位置，该位置的选取与频域导频的初始化有关
	i_RecTBLength_2_TranTBLength_TruncPos = ic_InsertPos * ( ( int )( REC_TBLENGTH / REC_PILOTLENGTH ) ) + ( ( int )( REC_TBLENGTH / REC_PILOTLENGTH / 2 ) );
	//注意：REC_TBLENGTH必须是REC_PILOTLENGTH的整倍数，即要求TRAN_PILOTLENGTH是TRAN_TBLENGTH的1/N（N为正整数），这一点标在了HDS_BS_Config.h中
	//初始化从RecTBLength截取到TranTBLength时的截取位置，该位置的选取与频域导频的初始化有关
	
	//频域均衡校正因子
	//注意：目前使用的FFT以及IFFT，其结果均未乘系数，即1/sqrt(N)
	//准确信道估计值，应该在目前结果基础上乘以sqrt( TRAN_PILOTLENGTH ) / sqrt( REC_PILOTLENGTH ) / sqrt( REC_PILOTLENGTH )
	//前两项是抵消FFT增益差异，后一项是抵消IFFT增益差异
	//计算当前信道估计值时，累加结果未除2，所以这里应该乘2，注意：此因子与信道估计算法有关
	//IFFT时按照TRAN_TBLENGTH点做，应该除以sqrt(TRAN_TBLENGTH)
	//理想情况下，最后均衡输出应该是模为1的数，为了适应译码器的需要，将数据实部以及虚部控制在13左右，所以要乘以18以调整幅度
	f_FDE_Revise_Factor = ( float( REC_PILOTLENGTH ) ) / sqrt( float( TRAN_PILOTLENGTH ) ) / sqrt( ( float )( TRAN_TBLENGTH ) ) * 2.0 / 4.0 * 18.0;//18.0;
	// printf( "%f\n", f_FDE_Revise_Factor );
	//频域均衡校正因子
	
	//初始化信道估计结果缓冲区
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
	//初始化信道估计结果缓冲区
			
	//初始化AFC中信道估计共扼复乘结果缓冲区
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
	//初始化AFC中信道估计共扼复乘结果缓冲区
	
	//初始化AFC中累加结果缓冲区
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
	//初始化AFC中累加结果缓冲区

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
	SCodeInit( ul_SCodeNum ); // 初始化扰码序列
}

void HDS_FDE::FDE_1Block( int i_BlockNum, bool * bp_ZeroFlag, float * * fp_RecData_1Block, float * fp_FDEResult_1Block, int * ip_AFC_ACCInt, float * fp_Angle_Radian ) // 注意：输入、输出均为__attribute__((aligned(16)))
{
	int i;
	
	//依次对各天线数据进行信道估计
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecPLengthFFTIn, ( *( fp_RecData_1Block + i ) ) + ( REC_TBLENGTH - REC_PILOTLENGTH ) * 2, REC_PILOTLENGTH * 2 * sizeof( float ) );
			//解调时一块内数据顺序：D+PCP+P; fp_RecPLengthFFTIn指向导频起始
			ChanEst( i );
			if ( ( ( *( ip_AFC_ACCInt + i ) ) > AFC_MAXCHANESTINT ) || ( ( *( ip_AFC_ACCInt + i ) ) < 0 ) )
			{
				printf( "Invalid AFC_ACCInt!\n" );
				exit( 1 );
			}
			if ( ( *( ip_AFC_ACCInt + i ) ) != 0 ) AFC( i, *( ip_AFC_ACCInt + i ), fp_Angle_Radian + i );
			// AFC_ACCInt是累加间隔选择。AFC_ACCInt = 0 不作AFC
		}
	}
	//依次对各天线数据进行信道估计
	
	if ( i_BlockNum != 0 )
	{
		//频域均衡
		//FDE( fp_RecData_1Block, bp_ZeroFlag );
		FDE_Weight( fp_RecData_1Block, bp_ZeroFlag );
		//频域均衡	
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
	
	// 信道估计
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
	// 信道估计
	
/*	Fp = fopen( "data/ChanEstResult.dat", "a" );
	for ( i = 0; i < REC_PILOTLENGTH; i++ )
	{
		fprintf( Fp, "%f\t\t%f\n", fa_RecPLengthIFFTOut[ i * 2 ], fa_RecPLengthIFFTOut[ i * 2 + 1 ] );
	}
	fclose( Fp );*/
	
	// 刷新信道估计缓冲区
	fp_ChanEstBuf_Temp = fpa_ChanEstBuf[ i_Ant_ID ][ CHANEST_BUFDEPTH - 1 ];
	for ( i = CHANEST_BUFDEPTH - 1; i > 0; i-- )
	{
		fpa_ChanEstBuf[ i_Ant_ID ][ i ] = fpa_ChanEstBuf[ i_Ant_ID ][ i - 1 ];
	}
	fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] = fp_ChanEstBuf_Temp;
	memcpy( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ], fa_RecPLengthIFFTOut, REC_PILOTLENGTH * 2 * sizeof( float ) );
	// 刷新信道估计缓冲区
	
	//CurChanEst( i_Ant_ID ); //计算当前信道估计值
	//CurChanEst_Win_RecPLength( i_Ant_ID ); //计算当前信道估计值
	CurChanEst_Win_RecPCPLength( i_Ant_ID );
}
		
void HDS_FDE::CurChanEst( int i_Ant_ID )
{
	int i;
	__m128 * m128p_Temp0 = NULL;
	__m128 * m128p_Temp1 = NULL;
	__m128 m128_Q;
	
	//计算当前信道估计的时域值，长度为REC_PILOTLENGTH，后面的点在构造函数中已经置零
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128_Q = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
		CO.m128_2_Float_Store( m128_Q, fa_RecTBLength_ChanEstFFTIn[ i_Ant_ID ] + i * 4 );
	}
	//计算当前信道估计的时域值
	//需要注意的是：这里没有对信道估计值进行修正，事实上，应该/2，到最后统一处理
	//若修改了信道估计的累加方法，则修正量也应该相应变化
	
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
	
	//计算当前信道估计的时域值，长度为REC_PILOTLENGTH，后面的点在构造函数中已经置零
	for ( i = 0; i < i_RecPLength_Div2; i++ )
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
	}//前后两次相加
	//计算当前信道估计的时域值
	//需要注意的是：这里没有对信道估计值进行修正，事实上，应该/2，到最后统一处理
	//若修改了信道估计的累加方法，则修正量也应该相应变化
	
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
	
	//计算当前信道估计的时域值，长度为REC_PCPLENGTH，后面的点在构造函数中已经置零
	for ( i = 0; i < i_RecPLength_Div2; i++ )		//这里实际上算i_RecPCPLength_Div2即可，但为了统计噪声功率，就把后面的也算了
	{
		m128p_Temp0 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + i * 4 );
		m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 1 ] + i * 4 );
		m128p_Temp2 = ( __m128 * )( fa_CurChanEst_Win + i * 4 );
		*m128p_Temp2 = CO.ComplexAdd_2( *m128p_Temp0, *m128p_Temp1 );
	}//前后两次相加
	//计算当前信道估计的时域值
	//需要注意的是：这里没有对信道估计值进行修正，事实上，应该/2，到最后统一处理
	//若修改了信道估计的累加方法，则修正量也应该相应变化
	
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
	fa_AveNPow[ i_Ant_ID ] = fa_Abs2Sum[ 0 ] + fa_Abs2Sum[ 2 ];	//不用算平均也行
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
	
	//依次计算各天线数据的频域值
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecTBLengthFFTIn[ i ], *( fp_RecData_1Block + i ), REC_TBLENGTH * 2 * sizeof( float ) );
			RecTBLengthFFT[ i ].doit( );
			//memcpy( fpa_CurRecData_FD[ i ], fa_RecTBLengthFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
		}
	}
	//依次计算各天线数据的频域值

	//计算频域均衡结果
	memset( fa_CurFDEResult_FD_RecTBLength, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//频域结果清零
	memset( fa_CurChanEst_FD_Abs2, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//信道估计模平方清零
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
	}//在各频点上，将各天线数据乘以信道估计共扼，累加；信道估计模平方，累加
	
	memmove( fa_CurFDEResult_FD_RecTBLength + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurFDEResult_FD_RecTBLength + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	memmove( fa_CurChanEst_FD_Abs2 + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurChanEst_FD_Abs2 + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	//去掉中间无用的部分，拼接出TRAN_TBLENGTH长的有用信号，将其放在缓冲区的前部

	m128_Q = CO.Float_2_m128_Load( fa_CurChanEst_FD_Abs2 );
	for ( i = 1; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		m128_Q = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}
	CO.m128_2_Float_Store( m128_Q, fa_Temp );
	f_NPow = ( ( *fa_Temp ) + ( *( fa_Temp + 2 ) ) ) / TRAN_TBLENGTH / FDE_SNR;
	if ( f_NPow <= 0.0 ) f_NPow = 1.0; //丢包后，可能会送进来全0的数据
	m128_Q = _mm_set_ps1( f_NPow ); //计算噪声功率

	for ( i = 0; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		*m128p_Temp1 = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}//将噪声功率加到信道估计模平方和中
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
	//计算频域均衡结果
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
	
	//依次计算各天线数据的频域值
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			memcpy( fa_RecTBLengthFFTIn[ i ], *( fp_RecData_1Block + i ), REC_TBLENGTH * 2 * sizeof( float ) );
			RecTBLengthFFT[ i ].doit( );
			//memcpy( fpa_CurRecData_FD[ i ], fa_RecTBLengthFFTOut, REC_TBLENGTH * 2 * sizeof( float ) );
		}
	}
	//依次计算各天线数据的频域值

	//计算频域均衡结果
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) ) f_AveNPow_Sum += fa_AveNPow[ i ]; //平均噪声功率和
	}
	
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) ) m128a_Weight[ i ] = _mm_set_ps1( fa_AveNPow[ i ] / f_AveNPow_Sum ); //权重
	}
	
	memset( fa_CurFDEResult_FD_RecTBLength, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//频域结果清零
	memset( fa_CurChanEst_FD_Abs2, 0, REC_TBLENGTH * 2 * sizeof( float ) );	//信道估计模平方清零
	for ( i = 0; i < FDE_ANTNUM; i++ )
	{
		if ( !( *( bp_ZeroFlag + i ) ) )
		{
			for ( j = 0; j < ( ( int )( REC_TBLENGTH / 2 ) ); j++ )
			{
				m128p_Temp0 = ( __m128 * )( fpa_CurRecData_FD[ i ] + j * 4 );
				m128p_Temp1 = ( __m128 * )( fpa_CurChanEst_FD[ i ] + j * 4 );
				
				*m128p_Temp0 = _mm_mul_ps( *m128p_Temp0, m128a_Weight[ i ] );//加权
				*m128p_Temp1 = _mm_mul_ps( *m128p_Temp1, m128a_Weight[ i ] );//加权
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp0, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurFDEResult_FD_RecTBLength + j * 4 );
				*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				
				m128_Q = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp1 );
				m128p_Temp0 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + j * 4 );
				*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );
				*m128p_Temp0 = _mm_shuffle_ps( *m128p_Temp0, *m128p_Temp0, _MM_SHUFFLE( 2, 2, 0, 0 ) ); 
			}
		}
	}//在各频点上，将各天线数据乘以信道估计共扼，累加；信道估计模平方，累加
	
	memmove( fa_CurFDEResult_FD_RecTBLength + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurFDEResult_FD_RecTBLength + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	memmove( fa_CurChanEst_FD_Abs2 + i_RecTBLength_2_TranTBLength_TruncPos * 2, fa_CurChanEst_FD_Abs2 + ( i_RecTBLength_2_TranTBLength_TruncPos + REC_TBLENGTH - TRAN_TBLENGTH ) * 2, ( TRAN_TBLENGTH - i_RecTBLength_2_TranTBLength_TruncPos ) * 2 * sizeof( float ) );
	//去掉中间无用的部分，拼接出TRAN_TBLENGTH长的有用信号，将其放在缓冲区的前部

	m128_Q = CO.Float_2_m128_Load( fa_CurChanEst_FD_Abs2 );
	for ( i = 1; i < ( ( int )( TRAN_TBLENGTH / 2 ) ); i++ )
	{
		m128p_Temp1 = ( __m128 * )( fa_CurChanEst_FD_Abs2 + i * 4 );
		m128_Q = CO.ComplexAdd_2( m128_Q, *m128p_Temp1 );
	}
	CO.m128_2_Float_Store( m128_Q, fa_Temp );
	f_NPow = ( ( *fa_Temp ) + ( *( fa_Temp + 2 ) ) ) / TRAN_TBLENGTH / FDE_SNR;
	if ( f_NPow <= 0.0 ) f_NPow = 1.0; //丢包后，可能会送进来全0的数据
	m128_Q = _mm_set_ps1( f_NPow ); //计算噪声功率
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
	//计算频域均衡结果
}

void HDS_FDE::AFC( int i_Ant_ID, int i_AFC_ACCInt, float * fp_Angle_Radian ) // i_AFC_ACCInt是累加间隔选择。i_AFC_ACCInt = 0 不作AFC，1 <= i_AFC_ACCInt <= AFC_MAXCHANESTINT
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
			*m128p_Temp0 = CO.ComplexSub_2( *m128p_Temp0, *m128p_Temp1 ); //从累加结果中减掉最早的一个
			m128p_Temp1 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ 0 ] + j * 4 );
			m128p_Temp2 = ( __m128 * )( fpa_ChanEstBuf[ i_Ant_ID ][ i + 1 ] + j * 4 );
			m128_Q = CO.ComplexMulConj_2( *m128p_Temp1, *m128p_Temp2 ); //本次信道估计结果乘以上一次的共扼
			*m128p_Temp0 = CO.ComplexAdd_2( *m128p_Temp0, m128_Q );//累加
			CO.m128_2_Float_Store( m128_Q, ( fpa_AFC_ChanEstConjMul_Buf[ i_Ant_ID ][ i ][ AFC_ACCTIMES - 1 ] + j * 4 ) );
			// 将复乘结果放入目前最早的缓冲区中
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
	
	//注意，解调时乘的是扰码的共扼×（1－j）
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
		switch ( ia_Descramble_SCode[ i_BaseAddr + i ] ) //注意，解调时乘的是扰码的共扼×（1－j），扰码为0、3时换，1、2不换
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
