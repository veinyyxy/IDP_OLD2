//////////////////////////////////////////////////////////////////
#ifndef STRUCT_277_SCI_H
#define STRUCT_277_SCI_H
#define MAX_TILTS_NUM     14

#include "../GDataTypes/GDef.h"

typedef struct tagStormCellComponentStructure//风暴分量结构体
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

typedef struct tagStormCellIDStructure//风暴体结构体
{
	gInt16 id ;      // 风暴体ID（从1开始）
	gInt16 NSC;      // 组成风暴体的风暴分量的数量
	gFloat MASS;     //
	gFloat vx;       //质心X坐标（单位 KM）
	gFloat vy;       //质心Y坐标（单位 KM）
	gFloat base;     //风暴体底部高度（单位 KM）
	gFloat top;      //风暴体顶部高度（单位 KM）
	gFloat r;        //风暴体距雷达距离（单位 KM）
	gFloat az;       //风暴体所在的方位角（单位：度）
	gFloat h;        
	gInt16 VIL;      //垂直液态水含量（单位： kg/m**2）
	gInt16 depth;    // 风暴体绝对高度 （单位：0.01km）顶高-底高所得; 
	gFloat maxZ;     //最大反射率(0.1DBZ)
	gFloat H_maxZ;   //最大反射率所在的高度（单位 KM）

} STORM_CELLID;

typedef struct tagStormCellAttributesStructure 
{
	STORM_CELLID sci;
	gInt16 NEL[MAX_TILTS_NUM];//风暴分量所在的层号
	STORM_CELLCOMPONENT NEC[MAX_TILTS_NUM];//组成风暴体的风暴分量
}STORM_CELLATTRIBUTES;

typedef struct tagSSHead 
{
	gInt32 NumberOfStormCells;//一个体扫内风暴体的个数
	gInt32 VolumeScanStartTime;//体扫起始时间
	//gInt32 VolumeScanEndTime;//体扫结束时间
	gInt32 LowThresholdOfReflectivity[15];//最小反射率阈值
	gInt32 ThresholdOfDropoutDifferenceReflectivity;//淘汰反射率差阈值
	gInt32 ThresholdOfDropoutCount;//淘汰点数阈值
	gFloat ThresholdOfSegmentLength;//风暴段长度阈值
	gInt32 ThresholdOfSeparatedAzimuth;//相邻风暴段方位间距阈值
	gInt32 ThresholdOfSegmentNumber;//最少风暴段个数阈值
	gFloat ThresholdOfOverlap;//相邻风暴段重叠距离阈值
	gFloat ThresholdOfComponentArea;//最小重叠面积阈值
	gFloat ThresholdOfSearchRadius[3];//最小关联距离阈值
	gInt32 ThresholdOfElevationMerge;//风暴合并的仰角阈值
	gFloat ThresholdOfHorizontalMerge;//判断风暴合并的水平距离阈值，km
	gFloat ThresholdOfHeightMerge;//判断风暴合并的高度阈值，km
	gFloat ThresholdOfHorizontalDelete;//相邻风暴体的水平删除阈值 单位是KM
	gInt32 ThresholdOfDepthDelete;//相邻风暴体的高度删除阈值
	gInt16 scanrang;  	 //雷达扫描范围，用以判断预测278产品的风暴体位置是否超出画图范围。单位千米
}TAGSSHEAD;

#endif