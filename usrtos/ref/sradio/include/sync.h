#ifndef _SYNC_H
#define _SYNC_H

#include "complexoperation.h"
#include <xmmintrin.h>

//输入数据（0～N）: D0_I D0_Q D1_I D1_Q ……
//输入、输出数据 __attribute__((aligned(16)))
//注意：PSC256结果应该除以256，PSC512结果应该除以512，PSC1024结果应该除以1024，程序中都没除，
//在设置有效径门限的时候应该注意

#define THRESHOLD				100
// 这一门限值是指某一点处相关值的模平方高于平均模平方值的倍数
// 高于此倍数，则认为是一个有效值

class Sync
{
public:
    Sync( int i_DInLength, int i_Ahead );//i_DInLength must be an even number.
    ~Sync( );

    void Synchronizer( float * fp_DIn );  // 在对一块数据用Synchronizer处理后，需要查询b_SyncStatus和i_RelativeOffset
    bool b_SyncStatus;									  // b_SyncStatus表明同步状态，i_RelativeOffset表明需要调整的相对偏移量
    int i_RelativeOffset;
    bool b_NextBlockFlag;									// Ture: Next; 当b_SyncStatus结果是false时，若偏移量不为0，则可根据
    // 此值判断下回应该送的数是哪块，True时送下一块
    // 无论送那块，如果偏移量不为0，都需要调整
    // 实际使用时是否判断此值，视具体情况而定
    unsigned char uc_ActivePosNum;

    bool b_Angle_Radian_Valid;						//参数有效标志，读取完成后置为false
    float f_Angle_Radian;									//AFC的角度

    float * fp_Sum;

    float f_MaxAbs2;
    float f_AveAbs2;
    ComplexOperation CO;

private:
    int ia_DelayVec[ 10 ];
    int ia_WeightVec[ 10 ];

    int i_DLength;
    int i_DLength_Div2;
    int i_Ahead_Pos;

    float * fp_Sub;

    void MatchFilter_PSC1024( float * fp_DIn );

    int i_MaxAbs2_Pos;
    float * fp_Abs2;
    void SearchMax( );

    int ia_ActivePos[ 255 ];

    void AFC(  );

};

#endif
