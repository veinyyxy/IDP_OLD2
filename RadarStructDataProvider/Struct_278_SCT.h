/////////////////////////////////////////////////////////////////////////////////
//Struct_278_SCT.h
//by ljg 20080806 for  part contents seperated from Arith_278_SCT.h.
//
#ifndef STRUCT_278_SCT_H
#define STRUCT_278_SCT_H

#include "../GDataTypes/GDef.h"

typedef struct tagStorm_CTHead
{
	gInt32 NumberOfScanFiles ;     //体扫文件序列号
	gInt32 NumberOfStormCells;     //体扫风暴体个数
	gInt32 VolScanTime;			   //体扫开始时间 ms
	gInt32 NumberOfLines;
	gInt16 scanrang;  	 //雷达扫描范围，用以判断预测278产品的风暴体位置是否超出画图范围。单位千米
}STORM_CTHEAD;

typedef struct{
	gInt32 SerialNum;  ///风暴体序列号
	bool NewStorm;   ///是否为新生单体
	gInt32 VolIndex;    ///程序处理的第几个体扫
	gFloat MASS;         //风暴体质量
	gFloat HeightOfBase;    //风暴体底部高度（单位 KM）
	gFloat HeightOfTop;     //风暴体顶部高度（单位 KM）
	gFloat Height;			//风暴体质心高度
	gInt16 VIL;				//垂直液态水含量（单位： kg/m**2）
	gInt16 Depth;    // 风暴体绝对高度 （单位：0.01km）顶高-底高所得;
	gFloat MaxZ;     //最大反射率(0.1DBZ)
	gFloat H_MaxZ;   //最大反射率所在的高度（单位 KM）

	gInt32 IndexPre;     ///前一次体扫时匹配的风暴在原体扫风暴中的序号
	gFloat ASPre;    ///前一次体扫时风暴的中心方位，度
	gFloat RSPre;    ///前一次体扫时风暴的中心距离，km

	gInt32 IndexCur;	    ///当前体扫时匹配的风暴在当前体扫风暴中的序号
	gFloat ASCur;    ///当前体扫时匹配的风暴的中心方位，度
	gFloat RSCur;    ///当前体扫时匹配的风暴的中心距离，km

}STORMCELLTRACK; //风暴体跟踪信息结构体

typedef struct{
	gInt32 SerialNum;  ///风暴体序列号

	float VelStorm;		////风暴的移速,m/s
	float DirStorm;		////风暴的移向,度

	float XSC;      ///风暴体质量权重中心所在的X坐标，km
	float YSC;      ///风暴体质量权重中心所在的Y坐标，km

	float ASCur;    ///风暴现在的中心方位，度
	float RSCur;    ///风暴现在的中心距离，km

	float ASF[4];    ///预报的1小时内风暴中心方位(每隔15分钟一个值)，度
	float RSF[4];    ///预报的1小时内风暴中心距离(每隔15分钟一个值)，km
 
}STORMCELLFORECAST; //风暴体预报信息结构体 

typedef struct tagStormCellTrack
{
	gInt32 SerialNum;  ///风暴体序列号
	STORMCELLTRACK  sct;
	STORMCELLFORECAST  scf;
}STORM_TRACK;

#endif//#ifndef STRUCT_278_SCT_H
