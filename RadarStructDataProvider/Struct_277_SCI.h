//////////////////////////////////////////////////////////////////
#ifndef STRUCT_277_SCI_H
#define STRUCT_277_SCI_H
#define MAX_TILTS_NUM     14

#include "../GDataTypes/GDef.h"

typedef struct tagStormCellComponentStructure//�籩�����ṹ��
{
	gInt16 Cell_id;   // the 3D-cell id number which the component be included.   
	gInt16 SC_id;     // the component id number of a storm, based 1
	gInt16 ICP;       // the i-th potential component (threshold(ITR) )
	gInt16  ITR;       // the i-th threshold reflectivity 
	gInt16  nexted;    // the correlated elevation(based on 1) 

	gInt16 AZbeg,AZend;
	gFloat RCbeg,RCend;
	gFloat maxZ;     //0.1dBz
	gFloat MASS;
	gFloat px,py;
	gFloat RC;
	gFloat az;
	gFloat hei;     //km
}STORM_CELLCOMPONENT;

typedef struct tagStormCellIDStructure//�籩��ṹ��
{
	gInt16 id ;      // �籩��ID����1��ʼ��
	gInt16 NSC;      // ��ɷ籩��ķ籩����������
	gFloat MASS;     //
	gFloat vx;       //����X���꣨��λ KM��
	gFloat vy;       //����Y���꣨��λ KM��
	gFloat base;     //�籩��ײ��߶ȣ���λ KM��
	gFloat top;      //�籩�嶥���߶ȣ���λ KM��
	gFloat r;        //�籩����״���루��λ KM��
	gFloat az;       //�籩�����ڵķ�λ�ǣ���λ���ȣ�
	gFloat h;        
	gInt16 VIL;      //��ֱҺ̬ˮ��������λ�� kg/m**2��
	gInt16 depth;    // �籩����Ը߶� ����λ��0.01km������-�׸�����; 
	gFloat maxZ;     //�������(0.1DBZ)
	gFloat H_maxZ;   //����������ڵĸ߶ȣ���λ KM��

} STORM_CELLID;

typedef struct tagStormCellAttributesStructure 
{
	STORM_CELLID sci;
	gInt16 NEL[MAX_TILTS_NUM];//�籩�������ڵĲ��
	STORM_CELLCOMPONENT NEC[MAX_TILTS_NUM];//��ɷ籩��ķ籩����
}STORM_CELLATTRIBUTES;

typedef struct tagSSHead 
{
	gInt32 NumberOfStormCells;//һ����ɨ�ڷ籩��ĸ���
	gInt32 VolumeScanStartTime;//��ɨ��ʼʱ��
	//gInt32 VolumeScanEndTime;//��ɨ����ʱ��
	gInt32 LowThresholdOfReflectivity[15];//��С��������ֵ
	gInt32 ThresholdOfDropoutDifferenceReflectivity;//��̭�����ʲ���ֵ
	gInt32 ThresholdOfDropoutCount;//��̭������ֵ
	gFloat ThresholdOfSegmentLength;//�籩�γ�����ֵ
	gInt32 ThresholdOfSeparatedAzimuth;//���ڷ籩�η�λ�����ֵ
	gInt32 ThresholdOfSegmentNumber;//���ٷ籩�θ�����ֵ
	gFloat ThresholdOfOverlap;//���ڷ籩���ص�������ֵ
	gFloat ThresholdOfComponentArea;//��С�ص������ֵ
	gFloat ThresholdOfSearchRadius[3];//��С����������ֵ
	gInt32 ThresholdOfElevationMerge;//�籩�ϲ���������ֵ
	gFloat ThresholdOfHorizontalMerge;//�жϷ籩�ϲ���ˮƽ������ֵ��km
	gFloat ThresholdOfHeightMerge;//�жϷ籩�ϲ��ĸ߶���ֵ��km
	gFloat ThresholdOfHorizontalDelete;//���ڷ籩���ˮƽɾ����ֵ ��λ��KM
	gInt32 ThresholdOfDepthDelete;//���ڷ籩��ĸ߶�ɾ����ֵ
	gInt16 scanrang;  	 //�״�ɨ�跶Χ�������ж�Ԥ��278��Ʒ�ķ籩��λ���Ƿ񳬳���ͼ��Χ����λǧ��
}TAGSSHEAD;

#endif