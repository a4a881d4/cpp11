#ifndef _SYNC_H
#define _SYNC_H

#include "complexoperation.h"
#include <xmmintrin.h>

//�������ݣ�0��N��: D0_I D0_Q D1_I D1_Q ����
//���롢������� __attribute__((aligned(16)))
//ע�⣺PSC256���Ӧ�ó���256��PSC512���Ӧ�ó���512��PSC1024���Ӧ�ó���1024�������ж�û����
//��������Ч�����޵�ʱ��Ӧ��ע��

#define THRESHOLD				100
// ��һ����ֵ��ָĳһ�㴦���ֵ��ģƽ������ƽ��ģƽ��ֵ�ı���
// ���ڴ˱���������Ϊ��һ����Чֵ

class Sync
{
public:
    Sync( int i_DInLength, int i_Ahead );//i_DInLength must be an even number.
    ~Sync( );

    void Synchronizer( float * fp_DIn );  // �ڶ�һ��������Synchronizer�������Ҫ��ѯb_SyncStatus��i_RelativeOffset
    bool b_SyncStatus;									  // b_SyncStatus����ͬ��״̬��i_RelativeOffset������Ҫ���������ƫ����
    int i_RelativeOffset;
    bool b_NextBlockFlag;									// Ture: Next; ��b_SyncStatus�����falseʱ����ƫ������Ϊ0����ɸ���
    // ��ֵ�ж��»�Ӧ���͵������Ŀ飬Trueʱ����һ��
    // �������ǿ飬���ƫ������Ϊ0������Ҫ����
    // ʵ��ʹ��ʱ�Ƿ��жϴ�ֵ���Ӿ����������
    unsigned char uc_ActivePosNum;

    bool b_Angle_Radian_Valid;						//������Ч��־����ȡ��ɺ���Ϊfalse
    float f_Angle_Radian;									//AFC�ĽǶ�

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
