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
	VortexType type;   //龙卷类型
	float BaseAzimuth;   //龙卷底方位角
	float BaseElev;      //龙卷底仰角
	float BaseRange;     //龙卷底斜距
	float BaseHeight;    //龙卷底高度
	float TopHeight;     //龙卷顶高度
	float Depth;         //龙卷厚度
	float BaseDeltaV;    //龙卷底速度差
	float AverDeltaV;    //平均速度差
	float MaxDeltaV;	 //最大速度差
	float HeightofMaxDeltaV;   //最大速度差的位置高度
	float MaximumShear;    //最大切变
	float HeightofMaxShear;    //最大切变高度
	float x;      //位置x，km
	float y;     //位置y，km，以雷达站为圆心。
	int StormCellID;   //风暴号
}TVSstruct;

#pragma pop(1)