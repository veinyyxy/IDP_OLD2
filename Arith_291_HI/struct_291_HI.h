#ifndef STRUCT_291_HI_H
#define STRUCT_291_HI_H

#include "GDef.h"

typedef struct tagHailIndex_Head
{
	gInt32 NumOfStormCells;
	gInt32 VolScanTime;
	gFloat POH_MinThreshold;		//冰雹最小显示阈值 （10%—100%），缺省30%     Modified by cm
	gFloat POH_Threshold;			//冰雹填充阈值 （10%—100%）缺省50%
	gFloat POSH_MinThreshold;		//强冰雹最小显示阈值 （10%—100%）缺省30%
	gFloat POSH_Threshold;			//强冰雹填充阈值 （10%—100%）缺省50%
}HAIL_HEAD;

typedef struct tagHailStructure
{
	gInt32 SerialNum;	//风暴体序列号
	gInt32 POH;			//冰雹概率
	gInt32 POSH;		//强冰雹概率
	gInt32 MEHS;		//冰雹尺寸 1mm //modified by cm 20090616
	gFloat r;		//风暴体距雷达距离（单位 KM）
	gFloat az;		//风暴体所在的方位角（单位：度）
}HAIL_INDEX;

#endif