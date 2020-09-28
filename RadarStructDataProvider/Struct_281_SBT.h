#ifndef STRUCT_279_RCT_H
#define STRUCT_279_RCT_H

//#include "GDef.h"

//轮廓头的结构体；
typedef struct BoundHead
{
	gUint32 len; //轮廓数据的长度（数据点的个数）
	gInt16 lastIndex;	// 轮廓对0映上一个体扫中匹配的轮廓的序号
	gInt16 curIndex;   // 轮廓在当前体扫中的轮廓的序号
	gInt32 cen_lon;	// 轮廓中心  放大10000
	gInt32 cen_lat;	// 轮廓中心  放大10000
	gFloat azi;	// 轮廓中心方位( 度  )
	gFloat area;	// 轮廓面积( 点 )
	gUint32 OffsetOfPoint;
}BoundHead;

//一组轮廓的头信息；
typedef struct _BoundFileHeader
{
	gUint16 cntBound;	// 轮廓个数
	gUint16	VolumeScanDate;		//扫描日期
	gInt32	VolumeScanStartTime; //扫描时间
	gInt16 Width;	//图片宽度，单位：像素
	gInt16 Height;	//图片高度，单位：像素
	gInt16 ikuchang;//库长，单位：米
}tagBoundFileHeader;

//由一组轮廓组成的一副图的结构体；
//typedef struct _OutBound
//{
//	tagBoundHeader head;  //轮廓头
//	gUint32 X[MAX_POINTS];		//轮廓点X经度
//	gUint32 Y[MAX_POINTS];		//轮廓点Y纬度
//}OutBound;


typedef struct l_Point
{
	gInt32 lon;		//轮廓点X经度 放大10000
	gInt32 lat;		//轮廓点Y纬度 放大10000
}l_Point;

#endif
