#ifndef STRUCT_516_MSBT_H
#define STRUCT_516_MSBT_H

#include "..\GDataTypes\GDef.h"
#define MAX_POINTS 3000
//轮廓头的结构体；
typedef struct _BoundHeader
{
	gUint32 len; //轮廓数据的长度（数据点的个数）
	gInt16 lastIndex;	// 轮廓对0映上一个体扫中匹配的轮廓的序号
	gInt16 curIndex;   // 轮廓在当前体扫中的轮廓的序号
	gInt32 cenX;	// 轮廓中心 X
	gInt32 cenY;	// 轮廓中心 Y
	//gFloat dis;	// 轮廓中心距离( 单:km )
	//gFloat azi;	// 轮廓中心方位( 度  )
	gFloat area;	// 轮廓面积( 点 )
	gFloat d;	// 傅立叶描绘子的匹配值
	gFloat p;	// 相位（）
	gInt32 iVol;
	//tm time;	// bound的时间
}tagBoundHeader;

//一组轮廓的头信息；
typedef struct _BoundFileHeader
{
	gUint16 cntBound;	// 轮廓个数
	gUint16	VolumeScanDate;		//扫描日期
	gInt32	VolumeScanStartTime; //扫描时间
	gInt16 Width;	//图片宽度，单位：像素
	gInt16 Height;	//图片高度，单位：像素
}tagBoundFileHeader;

//由一组轮廓组成的一副图的结构体；
typedef struct _OutBound
{
	tagBoundHeader head;  //轮廓头
	gInt16 X[MAX_POINTS];		//轮廓点X坐标
	gInt16 Y[MAX_POINTS];		//轮廓点Y坐标
}OutBound;

#endif
