#ifndef STRUCT_291_HI_H
#define STRUCT_291_HI_H

#include "GDef.h"

typedef struct tagHailIndex_Head
{
	gInt32 NumOfStormCells;
	gInt32 VolScanTime;
	gFloat POH_MinThreshold;		//������С��ʾ��ֵ ��10%��100%����ȱʡ30%     Modified by cm
	gFloat POH_Threshold;			//���������ֵ ��10%��100%��ȱʡ50%
	gFloat POSH_MinThreshold;		//ǿ������С��ʾ��ֵ ��10%��100%��ȱʡ30%
	gFloat POSH_Threshold;			//ǿ���������ֵ ��10%��100%��ȱʡ50%
}HAIL_HEAD;

typedef struct tagHailStructure
{
	gInt32 SerialNum;	//�籩�����к�
	gInt32 POH;			//��������
	gInt32 POSH;		//ǿ��������
	gInt32 MEHS;		//�����ߴ� 1mm //modified by cm 20090616
	gFloat r;		//�籩����״���루��λ KM��
	gFloat az;		//�籩�����ڵķ�λ�ǣ���λ���ȣ�
}HAIL_INDEX;

#endif