#pragma once

#pragma pack(1)

typedef struct TVSprohead
{
	unsigned short DV_thresholdnum;
	unsigned short max3dnum_onethreshold;
}TVSprohead;

typedef struct TVShead 
{
	float DV_threshold;
	short thresholdindex;
	unsigned int TVSnum;
	unsigned int ETVSnum;
}TVShead;

enum VortexType
{
	NOTYPE,
	TVS,
	ETVS,
};

typedef struct TVSstruct
{
	VortexType type;   //��������
	float BaseAzimuth;   //����׷�λ��
	float BaseElev;      //���������
	float BaseRange;     //�����б��
	float BaseHeight;    //����׸߶�
	float TopHeight;     //�����߶�
	float Depth;         //������
	float BaseDeltaV;    //������ٶȲ�
	float AverDeltaV;    //ƽ���ٶȲ�
	float MaxDeltaV;	 //����ٶȲ�
	float HeightofMaxDeltaV;   //����ٶȲ��λ�ø߶�
	float MaximumShear;    //����б�
	float HeightofMaxShear;    //����б�߶�
	float x;      //λ��x��km
	float y;     //λ��y��km�����״�վΪԲ�ġ�
	int StormCellID;   //�籩��
}TVSstruct;

#pragma pop(1)