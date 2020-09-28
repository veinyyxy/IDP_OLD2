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
	gInt16 VORTEXCnt; //vortex个数
	//gInt16 FEATRUECnt;
	gInt16 MESOCCnt; //中气旋个数
	gInt16 Height;
	gInt16 Width;
}MHead;

typedef struct  
{
	gFloat range;  //斜距
	gInt16 elev;   //仰角号
	gFloat azi;	   //方位角
}PostionStruct;

typedef struct  
{
	PostionStruct FCP;	
	gFloat HFC;		//   中心高度
	gFloat DAA;		//	 方位角方向半径	
	gFloat DAR;		//  径向半径
	gFloat SRTmaxavg;	// 最大平均切变
	gFloat SRTmax;		// 最大切变量
	gFloat Mavg;		// 平均角动量
}SRTmaxFeature;

typedef struct  
{
	gInt16 VORTEXTNo;		// 漩涡序号	
	VortexType VORTEXType;		//	漩涡类型
	gInt32 FEATRUECnt;		// 二维特征量个数
	SRTmaxFeature SMFeature; //切变量最大的二维特征量
	gInt16 CenterX;		//中心X
	gInt16 CenterY;		//中心Y
	gInt16 Radius;		//半径
	gFloat HighestHFC;		// 顶
	gFloat LowestHFC;		// 底
}VortexStruct;

typedef struct  
{
	gFloat FEATUREMaxAz;   //二维特征量的终止方位角
	gFloat FEATUREMinAz;	//其实方位角
	gFloat FEATUREMaxRs;	//起始斜距
	gFloat FEATUREMinRs;	//终止斜距
}Extrma;

typedef struct  
{
	gInt16 VORTEXTNo;		// 漩涡序号	
	//gFloat HighestHFC;		// 顶
	//gFloat LowestHFC;		// 底
	PostionStruct PM;		// 
	gFloat SPRavg;			// 平均转动速度
	Extrma	FE;//- FEATURE EXTREMA for each FEATURE in MESOCYCLONE ID
	gFloat SRTavg;			//平均切变
	gFloat HightestSRTmax;	// 最高SRTmax
}MesocStruct;

#endif