#include <sync.h>
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159265359

Sync::Sync( int i_DInLength, int i_Ahead ): CO( )
{
    i_DLength = i_DInLength;
    i_DLength_Div2 = i_DLength / 2;
    i_Ahead_Pos = i_Ahead;

    ia_DelayVec[ 0 ] = 8;
    ia_DelayVec[ 1 ] = 512;
    ia_DelayVec[ 2 ] = 4;
    ia_DelayVec[ 3 ] = 32;
    ia_DelayVec[ 4 ] = 2;
    ia_DelayVec[ 5 ] = 64;
    ia_DelayVec[ 6 ] = 16;
    ia_DelayVec[ 7 ] = 128;
    ia_DelayVec[ 8 ] = 256;
    ia_DelayVec[ 9 ] = 1;

    //j,-1,j,-j,1,j,-j,j
    //1->0; j->1; -1->2; -j->3
    ia_WeightVec[ 0 ] = 2; 		ia_WeightVec[ 1 ] = 3;
    ia_WeightVec[ 2 ] = 1; 		ia_WeightVec[ 3 ] = 0;
    ia_WeightVec[ 4 ] = 1; 		ia_WeightVec[ 5 ] = 2;
    ia_WeightVec[ 6 ] = 2; 		ia_WeightVec[ 7 ] = 0;
    ia_WeightVec[ 8 ] = 1; 		ia_WeightVec[ 9 ] = 2;

    fp_Sum = NULL;
    fp_Sum = ( float * ) memalign( 16, i_DLength * 2 * sizeof( float ) );
    if ( fp_Sum == NULL )
    {
        printf( "Can't allocate memory for SyncSumBuf, terminating.\n" );
        exit( 1 );
    }
    fp_Sub = NULL;
    fp_Sub = ( float * ) memalign( 16, i_DLength * 2 * sizeof( float ) );
    if ( fp_Sub == NULL )
    {
        printf( "Can't allocate memory for SyncSubBuf, terminating.\n" );
        exit( 1 );
    }
    fp_Abs2 = NULL;
    fp_Abs2 = ( float * ) memalign( 16, i_DLength * 2 * sizeof( float ) );
    if ( fp_Abs2 == NULL )
    {
        printf( "Can't allocate memory for SyncAbs2Buf, terminating.\n" );
        exit( 1 );
    }

    b_SyncStatus = false;
    i_RelativeOffset = 0;
    b_NextBlockFlag = true;

    b_Angle_Radian_Valid = false;
}

Sync::~Sync( )
{
    if ( fp_Sum ) free( fp_Sum );
    if ( fp_Sub ) free( fp_Sub );
    if ( fp_Abs2 ) free( fp_Abs2 );
}

void Sync::MatchFilter_PSC1024( float * fp_DIn )
{
    int i, j;
    __m128 m128_Weight;
    __m128 m128_SubMulWeight;
    __m128 * m128p_Sum;
    __m128 * m128p_Sub;

    memset( fp_Sum, 0, ia_DelayVec[ 0 ] * 2 * sizeof( float ) );
    memcpy( fp_Sum + ia_DelayVec[ 0 ] * 2, fp_DIn, ( i_DLength - ia_DelayVec[ 0 ] ) * 2 * sizeof( float ) );
    memcpy( fp_Sub, fp_DIn, i_DLength * 2 * sizeof( float ) );

    for ( i = 0; i < 10; i++ )
    {
        switch( ia_WeightVec[ i ] )
        {
        case 0:
            m128_Weight = _mm_set_ps1( 1.0 );	break;
        case 1:
            m128_Weight = _mm_setr_ps( 1.0, -1.0, 1.0, -1.0 );	break;
        case 2:
            m128_Weight = _mm_set_ps1( -1.0 );	break;
        default:
            m128_Weight = _mm_setr_ps( -1.0, 1.0, -1.0, 1.0 );
        }

        for ( j = 0; j < i_DLength_Div2; j++ )
        {
            m128p_Sum = ( __m128 * )( fp_Sum + j * 4 );
            m128p_Sub = ( __m128 * )( fp_Sub + j * 4 );
            m128_SubMulWeight = _mm_mul_ps( *m128p_Sub, m128_Weight );
            if ( ia_WeightVec[ i ]&1 )
                m128_SubMulWeight = _mm_shuffle_ps(	m128_SubMulWeight, m128_SubMulWeight, _MM_SHUFFLE( 2, 3, 0, 1 ) );
            *m128p_Sub = _mm_sub_ps( *m128p_Sum , m128_SubMulWeight );
            *m128p_Sum = _mm_add_ps( *m128p_Sum , m128_SubMulWeight );
        }
        if ( i < 9 )
        {
            memmove( fp_Sum + ia_DelayVec[ i + 1 ] * 2, fp_Sum, ( i_DLength - ia_DelayVec[ i + 1 ] ) * 2 * sizeof( float ) );
            memset( fp_Sum, 0, ia_DelayVec[ i + 1 ] * 2 * sizeof( float ) );
        }
    }
}

void Sync::Synchronizer( float * fp_DIn )
{
    int i_BasePos;
    int i, i_Pos;

    b_SyncStatus = false;
    i_RelativeOffset = 0;
    b_NextBlockFlag = true;

    MatchFilter_PSC1024( fp_DIn );
    SearchMax( );

    if ( ( f_MaxAbs2 / THRESHOLD ) > f_AveAbs2 ) //判断最大值是否大于门限
    {
        i_BasePos = i_MaxAbs2_Pos % 1024;
        uc_ActivePosNum = 0;
        i_Pos = i_BasePos;
        while ( i_Pos < i_DLength ) //找出所有满足条件的位置
        {
            if ( ( *( fp_Abs2 + i_Pos * 2 ) ) > ( f_AveAbs2 * THRESHOLD ) )
            {
                ia_ActivePos[ uc_ActivePosNum ] = i_Pos;
                uc_ActivePosNum++;
            }
            i_Pos += 1024;
        }
/*	for( i=0;i<i_DLength;i++ )
	{
		if( fp_Abs2[ i*2 ]>( f_AveAbs2 * THRESHOLD ) )
			printf(" %10d is %8f\n",i,
				fp_Abs2[ i*2 ]);
	}
        printf("find %d\n",uc_ActivePosNum);
	for( i=0;i<uc_ActivePosNum;i++ )
		printf(" %2d pos is %10d %8f\n",i,ia_ActivePos[ i ],
			fp_Abs2[ia_ActivePos[ i ]*2]);
*/
	if ( uc_ActivePosNum == 1 )  // 只相关到一个有效位置
        {
//			b_SyncStatus = false;
//			b_NextBlockFlag = true;
            if ( ia_ActivePos[ 0 ] >= ( i_DLength - 1024 ) ) //位于尾部，有可能是第一个峰
            {
                i_RelativeOffset = ( -1 ) * ( i_DLength - ( ia_ActivePos[ 0 ] - 1023 ) + i_Ahead_Pos );
            }
            else
            {
//				i_RelativeOffset = 0;
            }
            return;
        }
        else
        {
            for ( i = 1; i < ( ( int )( uc_ActivePosNum ) ); i++ ) // 判断找到的位置是否连续
            {
                if ( ( ia_ActivePos[ i ] - ia_ActivePos[ i - 1 ] ) != 1024 )
                {
//					b_SyncStatus = false;
//					i_RelativeOffset = 0;
//					b_NextBlockFlag = true;
                    return;
                }
            }
            if ( uc_ActivePosNum == 3 ) //找到全部3个峰
            {
                b_SyncStatus = true;
                i_RelativeOffset = ( ia_ActivePos[ 0 ] - 1023 ) - i_Ahead_Pos;
//				b_NextBlockFlag = true;
                AFC( );
                return;
            }
            else
            {
//				b_SyncStatus = false;
                if ( ia_ActivePos[ 0 ] < 2047 ) // 第一个峰前面可能还有峰，可能由于截的位置引起上一次和这一次都未找到
                {
                    b_NextBlockFlag = false;
                    i_RelativeOffset = ( -1 ) * ( ( 3 - uc_ActivePosNum ) * 1024 + i_Ahead_Pos );
                    return;
                }
                else if ( ia_ActivePos[ uc_ActivePosNum - 1 ] >= ( i_DLength - 1024 ) )
                {
//					b_NextBlockFlag = true;
                    i_RelativeOffset = ( -1 ) * ( i_DLength - ( ia_ActivePos[ 0 ] - 1023 ) + i_Ahead_Pos );
                    return;
                }
                else
                {
//					b_NextBlockFlag = true;
//					i_RelativeOffset = 0;
                    return;
                }
            }
        }

    }
    else //最大值不大于门限
    {
//		b_NextBlockFlag = true;
//		b_SyncStatus = false;
        i_RelativeOffset = 0;
    }
}

void Sync::SearchMax( )
{
    float fa_MaxAbs2[ 4 ] __attribute__((aligned(16))) = { 0.0, 0.0, 0.0, 0.0 };
    int ia_MaxAbs2_Pos[ 2 ] = { 0, 0 };
    __m128 * m128p_MaxAbs2;
    __m128 * m128p_Sum;
    __m128 * m128p_Abs2;
    __m128 m128_Abs2Acc;
    __m128 m128_Temp0, m128_Temp1;
    float * fp_Temp1, * fp_Abs2Acc;
    int i;

    m128p_MaxAbs2 = ( __m128 * )( fa_MaxAbs2 );
    fp_Temp1 = ( float * )( &m128_Temp1 );
    m128_Abs2Acc = _mm_setzero_ps( );

    for ( i = 0; i < i_DLength_Div2; i++ )
    {
        m128p_Sum = ( __m128 * )( fp_Sum + i * 4 );
        m128p_Abs2 = ( __m128 * )( fp_Abs2 + i * 4 );
        m128_Temp0 = _mm_mul_ps( *m128p_Sum, *m128p_Sum );
        m128_Temp1 = _mm_shuffle_ps( m128_Temp0, m128_Temp0, _MM_SHUFFLE( 2, 3, 0, 1 ) );
        *m128p_Abs2 = _mm_add_ps( m128_Temp0, m128_Temp1 );
        m128_Temp1 = _mm_cmplt_ps( *m128p_MaxAbs2, *m128p_Abs2 );
        if ( ( *( fp_Temp1 ) ) != 0 )
        {
            fa_MaxAbs2[ 0 ] = *( fp_Abs2 + i * 4 );
            ia_MaxAbs2_Pos[ 0 ] = i * 2;
        }
        if ( ( *( fp_Temp1 + 2 ) ) != 0 )
        {
            fa_MaxAbs2[ 2 ] = *( fp_Abs2 + i * 4 + 2 );
            ia_MaxAbs2_Pos[ 1 ] = i * 2 + 1;
        }
        m128_Abs2Acc = _mm_add_ps( m128_Abs2Acc, *m128p_Abs2 );
    }

    if ( fa_MaxAbs2[ 0 ] >= fa_MaxAbs2[ 2 ] )
    {
        f_MaxAbs2 = fa_MaxAbs2[ 0 ];
        i_MaxAbs2_Pos = ia_MaxAbs2_Pos[ 0 ];
    }
    else
    {
        f_MaxAbs2 = fa_MaxAbs2[ 2 ];
        i_MaxAbs2_Pos = ia_MaxAbs2_Pos[ 1 ];
    }

    fp_Abs2Acc = ( float * )( &m128_Abs2Acc );
    f_AveAbs2 = ( ( *( fp_Abs2Acc ) ) + ( *( fp_Abs2Acc + 2 ) ) ) / i_DLength;
}

void Sync::AFC(  )
{
    __m128 m128_A, m128_B;
    __m128 * m128p_Q;
    float f_Q[ 4 ] __attribute__((aligned(16)));

    m128p_Q = ( __m128 * )( f_Q );
    m128_B = _mm_setr_ps( fp_Sum[ ia_ActivePos[ 0 ] * 2 ], fp_Sum[ ia_ActivePos[ 0 ] * 2 + 1 ],
                          fp_Sum[ ia_ActivePos[ 1 ] * 2 ], fp_Sum[ ia_ActivePos[ 1 ] * 2 + 1 ] );
    m128_A = _mm_setr_ps( fp_Sum[ ia_ActivePos[ 1 ] * 2 ], fp_Sum[ ia_ActivePos[ 1 ] * 2 + 1 ],
                          fp_Sum[ ia_ActivePos[ 2 ] * 2 ], fp_Sum[ ia_ActivePos[ 2 ] * 2 + 1 ] );
    *m128p_Q = CO.ComplexMulConj_2( m128_A, m128_B );
    m128_B = _mm_shuffle_ps( *m128p_Q, *m128p_Q, _MM_SHUFFLE( 1, 0, 3, 2 ) );
    *m128p_Q = _mm_add_ps( *m128p_Q, m128_B );

    /*
    	printf( "Pos 0: %f+j*%f\n", fp_Sum[ ia_ActivePos[ 0 ] * 2 ],  fp_Sum[ ia_ActivePos[ 0 ] * 2 + 1 ] );
    	printf( "Pos 1: %f+j*%f\n", fp_Sum[ ia_ActivePos[ 1 ] * 2 ],  fp_Sum[ ia_ActivePos[ 1 ] * 2 + 1 ] );
    	printf( "Pos 2: %f+j*%f\n", fp_Sum[ ia_ActivePos[ 2 ] * 2 ],  fp_Sum[ ia_ActivePos[ 2 ] * 2 + 1 ] );
    	printf( "Mul R: %f+j*%f\n", f_Q[ 0 ], f_Q[ 1 ] );
    */
    if ( f_Q[ 0 ] == 0 )
    {
        if ( f_Q[ 1 ] > 0 )
            f_Angle_Radian = pi / 2;
        else if ( f_Q[ 1 ] < 0 )
            f_Angle_Radian = ( -1.0 ) * pi / 2;
        else
            f_Angle_Radian = 0.0;
    }
    else if ( ( f_Q[ 0 ] > 0 ) && ( f_Q[ 1 ] >= 0 ) )
        f_Angle_Radian = atan( f_Q[ 1 ] / f_Q[ 0 ] );
    else if ( ( f_Q[ 0 ] < 0 ) && ( f_Q[ 1 ] >= 0 ) )
        f_Angle_Radian = atan( f_Q[ 1 ] / f_Q[ 0 ] ) + pi;
    else if ( ( f_Q[ 0 ] > 0 ) && ( f_Q[ 1 ] < 0 ) )
        f_Angle_Radian = atan( f_Q[ 1 ] / f_Q[ 0 ] );
    else
        f_Angle_Radian = atan( f_Q[ 1 ] / f_Q[ 0 ] ) - pi;

    b_Angle_Radian_Valid = true;
}
