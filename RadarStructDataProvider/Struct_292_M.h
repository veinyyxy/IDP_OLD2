#ifndef STRUCT_292_M_H
#define STRUCT_292_M_H

#include "../GDataTypes/GDef.h"

enum VortexType
{
	_MESOCYCLONE = 0,
	_3D_SHEAR,
	_UNCORRELATED_SHEAR
};

typedef struct 
{
	gInt16 VORTEXCnt; //vortex����
	//gInt16 FEATRUECnt;
	gInt16 MESOCCnt; //����������
	gInt16 Height;
	gInt16 Width;
}MHead;

typedef struct  
{
	gFloat range;  //б��
	gInt16 elev;   //���Ǻ�
	gFloat azi;	   //��λ��
}PostionStruct;

typedef struct  
{
	PostionStruct FCP;	
	gFloat HFC;		//   ���ĸ߶�
	gFloat DAA;		//	 ��λ�Ƿ���뾶	
	gFloat DAR;		//  ����뾶
	gFloat SRTmaxavg;	// ���ƽ���б�
	gFloat SRTmax;		// ����б���
	gFloat Mavg;		// ƽ���Ƕ���
}SRTmaxFeature;

typedef struct  
{
	gInt16 VORTEXTNo;		// �������	
	VortexType VORTEXType;		//	��������
	gInt32 FEATRUECnt;		// ��ά����������
	SRTmaxFeature SMFeature; //�б������Ķ�ά������
	gInt16 CenterX;		//����X
	gInt16 CenterY;		//����Y
	gInt16 Radius;		//�뾶
	gFloat HighestHFC;		// ��
	gFloat LowestHFC;		// ��
}VortexStruct;

typedef struct  
{
	gFloat FEATUREMaxAz;   //��ά����������ֹ��λ��
	gFloat FEATUREMinAz;	//��ʵ��λ��
	gFloat FEATUREMaxRs;	//��ʼб��
	gFloat FEATUREMinRs;	//��ֹб��
}Extrma;

typedef struct  
{
	gInt16 VORTEXTNo;		// �������	
	//gFloat HighestHFC;		// ��
	//gFloat LowestHFC;		// ��
	PostionStruct PM;		// 
	gFloat SPRavg;			// ƽ��ת���ٶ�
	Extrma	FE;//- FEATURE EXTREMA for each FEATURE in MESOCYCLONE ID
	gFloat SRTavg;			//ƽ���б�
	gFloat HightestSRTmax;	// ���SRTmax
}MesocStruct;

#endif