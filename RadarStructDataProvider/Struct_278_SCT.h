/////////////////////////////////////////////////////////////////////////////////
//Struct_278_SCT.h
//by ljg 20080806 for  part contents seperated from Arith_278_SCT.h.
//
#ifndef STRUCT_278_SCT_H
#define STRUCT_278_SCT_H

#include "../GDataTypes/GDef.h"

typedef struct tagStorm_CTHead
{
	gInt32 NumberOfScanFiles ;     //��ɨ�ļ����к�
	gInt32 NumberOfStormCells;     //��ɨ�籩�����
	gInt32 VolScanTime;			   //��ɨ��ʼʱ�� ms
	gInt32 NumberOfLines;
	gInt16 scanrang;  	 //�״�ɨ�跶Χ�������ж�Ԥ��278��Ʒ�ķ籩��λ���Ƿ񳬳���ͼ��Χ����λǧ��
}STORM_CTHEAD;

typedef struct{
	gInt32 SerialNum;  ///�籩�����к�
	bool NewStorm;   ///�Ƿ�Ϊ��������
	gInt32 VolIndex;    ///������ĵڼ�����ɨ
	gFloat MASS;         //�籩������
	gFloat HeightOfBase;    //�籩��ײ��߶ȣ���λ KM��
	gFloat HeightOfTop;     //�籩�嶥���߶ȣ���λ KM��
	gFloat Height;			//�籩�����ĸ߶�
	gInt16 VIL;				//��ֱҺ̬ˮ��������λ�� kg/m**2��
	gInt16 Depth;    // �籩����Ը߶� ����λ��0.01km������-�׸�����;
	gFloat MaxZ;     //�������(0.1DBZ)
	gFloat H_MaxZ;   //����������ڵĸ߶ȣ���λ KM��

	gInt32 IndexPre;     ///ǰһ����ɨʱƥ��ķ籩��ԭ��ɨ�籩�е����
	gFloat ASPre;    ///ǰһ����ɨʱ�籩�����ķ�λ����
	gFloat RSPre;    ///ǰһ����ɨʱ�籩�����ľ��룬km

	gInt32 IndexCur;	    ///��ǰ��ɨʱƥ��ķ籩�ڵ�ǰ��ɨ�籩�е����
	gFloat ASCur;    ///��ǰ��ɨʱƥ��ķ籩�����ķ�λ����
	gFloat RSCur;    ///��ǰ��ɨʱƥ��ķ籩�����ľ��룬km

}STORMCELLTRACK; //�籩�������Ϣ�ṹ��

typedef struct{
	gInt32 SerialNum;  ///�籩�����к�

	float VelStorm;		////�籩������,m/s
	float DirStorm;		////�籩������,��

	float XSC;      ///�籩������Ȩ���������ڵ�X���꣬km
	float YSC;      ///�籩������Ȩ���������ڵ�Y���꣬km

	float ASCur;    ///�籩���ڵ����ķ�λ����
	float RSCur;    ///�籩���ڵ����ľ��룬km

	float ASF[4];    ///Ԥ����1Сʱ�ڷ籩���ķ�λ(ÿ��15����һ��ֵ)����
	float RSF[4];    ///Ԥ����1Сʱ�ڷ籩���ľ���(ÿ��15����һ��ֵ)��km
 
}STORMCELLFORECAST; //�籩��Ԥ����Ϣ�ṹ�� 

typedef struct tagStormCellTrack
{
	gInt32 SerialNum;  ///�籩�����к�
	STORMCELLTRACK  sct;
	STORMCELLFORECAST  scf;
}STORM_TRACK;

#endif//#ifndef STRUCT_278_SCT_H
