//////////////////////////////////////////////////////////////////////////
// SingleProductBase.h
// created by ply 20080401
// 产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCTBASE_H
#define PRODUCTBASE_H

#pragma pack(push,1)


#include "GData.h"

typedef struct tagRadarInfor
{
		gChar	SiteCode[8];
		gInt8	RadarType[4];
		gInt32	Longitude;
		gInt32	Latitude;
		gInt32	Altitude;
}RadarInfor;

typedef struct tagPDBlock						//产品数据头/产品信息
{
		gInt16	ProductCode;					//产品号
		gUint16 ProductDate;					//产品日期，儒略日（Julian）表示
		gInt32	ProductTime;					//产品时间，以毫秒为单位
		gInt8	OperationalMode[2];				//工作模式
		gInt8	VCPMode[4];						//体扫模式：雷达型号不同，体扫模式不同
		gUint16 GenerationDateOfProduct;		//产品生成日期，儒略日（Julian）表示
		gInt32	GenerationTimeOfProduct;		//产品生成时间，以毫秒为单位
		gInt16  ScaleOfData;					//产品数据缩放倍数
		gInt16	ProductDependentCount;			//产品参数个数
		gInt16	ProductDependent[20];				//产品参数
		gInt16	NumberOfLayers;					//数据层数
		gInt32	IndexOfLayers;					//数据层指针，相对于产品数据起始地址的偏移量
		gInt32  IndexOfTabular;                 //表格块指针，相对于产品数据起始地址的偏移量
		gInt32  IndexOfGraphic;					// 图形块指针，相对于产品数据起始地址的偏移量
} PDBlock;

//		gInt32 OffsetOfEachLayer[NumberOfLayers];	//每层的偏移量

typedef struct tagDILayer				//数据块
{
		gInt16	SNOfLayer;//数据层序号
		gInt32	LengthOfLayer;//数据层长度（字节数）
		gInt16	LayerDate;//层数据日期
		gInt32	LayerTime;//层数据时间
		gInt32	HeightOfElevation;//本层数据的高度或仰角度数，根据产品区分
		gInt16	NumberOfBlocks;//数据层中的数据块数
		gInt32	IndexOfBlocks;//数据块指针，相对于数据层起始地址的偏移量
} DILayer;

typedef struct tagDLIBlock 
{
		gInt16	SNOfBlock;					//当前数据层内数据块的序号
		gInt16	TypeOfDataPackets;			//数据块类型
		gInt32	LengthOfBlock;				//数据块长度
} DLIBlock;

typedef struct tagRadialHead 
{
		gInt16	DistanceOfFirstGate;//第一距离库的位置
		gInt16	LengthOfGate;				//距离库长度
		gInt16	NumberOfRadials;			//径向数据的个数
		gInt16	NumberOfGates;				//径向数据的距离库数
		gInt16	MaxVelocity;				//最大不模糊速度 （Nyquist速度或Nyquist速度×2×退模糊次数）
} RadialHead;

typedef struct tagRadialData 
{
		gUint16  AzimuthAngle;				//方位角
		gInt16	DeltaAngle;					//方位角间距
		//gInt16 Data;						//数据，数据个数=径向数据的个数×径向数据的距离库数
} RadialData;
typedef struct tagGridHead 
{
		gInt16	NumberOfRows;				//行数
		gInt16	NumberOfCols;				//列数
		gInt16	ResolutionOfRow;			//行分辨率
		gInt16	ResolutionOfCol;			//列分辨率
} GridHead;

typedef struct tagGridRowData 
{
		gInt16	SNOfRow;				//行序号
		//gInt16	Data;					//数据，数据个数=行数×列数
} GridRowData;

typedef struct tagStructHead 
{
		gInt16	LengthOfDataType;				//数据类型长度
		gInt16	NumbersOfData;					//数据个数
} StructHead;

enum BlockType
{
	BLOCKTYPE_RADIAL=0x0A00,
	BLOCKTYPE_GRID=0x0B00,
	BLOCKTYPE_LONLATGRID=0x0B01,
	BLOCKTYPE_STRUCT=0x0C00,
	BLOCKTYPE_UNDEFINED=0x0F00,

	//下面是给构造用的
	ONCE_LAYER=0x0D00,
	MUTI_LAYER=0x0E00,
};

/*
m_TemInt使用表

m_TemInt[0]
m_TemInt[1]
m_TemInt[2]
m_TemInt[3]
m_TemInt[4]
m_TemInt[5]
m_TemInt[6]
m_TemInt[7]
m_TemInt[8]
m_TemInt[9]
m_TemInt[10]
m_TemInt[11]
m_TemInt[12]
m_TemInt[13]
m_TemInt[14]
m_TemInt[15]
m_TemInt[16]
m_TemInt[17]
m_TemInt[18]
m_TemInt[19]

*/

class DLLEXP_GSYS ProductBase : public GenerationData
{
public:
	ProductBase();
	~ProductBase();
	inline void GetProductCode(gInt16& nProductCode);  //add by dqc 20080520

	//read functions
	inline void GetScanTime(gInt32& sTime);
	tagRadarInfor * GetRadarInfoP(); 
	inline void GetScanDate(gInt16& sDate);
		PDBlock * GetPDBlockP();

	//write functions
//protected:
public:
	//read functions
	//read/write functions
		inline PDBlock * GetPDBlock();
		inline gInt32 GetOffsetOfLayer(gInt32 LayerIndex);
		inline DILayer * GetLayer(gInt32 LayerIndex);
		inline gInt32 GetOffsetOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);
		inline gInt32 GetOffsetOfBlockData(gInt32 LayerIndex,gInt32 BlockIndex);
		inline DLIBlock * GetBlock(gInt32 LayerIndex,gInt32 BlockIndex);
		inline void * GetBlockData(gInt32 LayerIndex,gInt32 BlockIndex);
		inline BlockType GetBlockType(gInt32 LayerIndex,gInt32 BlockIndex);
		inline gInt32 * GetLayerOffsetIndex();

		inline RadialHead * GetBlockDataAsRadial_Head(gInt32 LayerIndex,gInt32 BlockIndex);
		inline RadialData * GetBlockDataAsRadial_RadialData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex);
		inline gInt16 * GetBlockDataAsRadial_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex);

		inline GridHead * GetBlockDataAsGrid_Head(gInt32 LayerIndex,gInt32 BlockIndex);
		inline GridRowData * GetBlockDataAsGrid_RowData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);
		inline gInt16 * GetBlockDataAsGrid_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);

		inline StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex);
		inline void * GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex);

		inline tagRadarInfor * GetRadarInfo();   //AS by sunx 20081202

	//write functions
		inline gInt32 GetSizeOfPDBlock();
		inline gInt32 GetSizeOfOffsetOfEachLayerBlock(gInt32 NumberOfLayers);
		inline gInt32 GetSizeOfRadialBlock(gInt16 NumberOfRadials,gInt16 NumberOfGates);
		inline gInt32 GetSizeOfGridBlock(gInt16 NumberOfRows,gInt16 NumberOfCols);
		inline gInt32 GetSizeOfStructBlock(gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct);

		inline gInt32 GetSizeOfRadarInfo();

		GHRESULT FrameAll(gInt16 NumberOfLayers,.../*ONCE_LAYER,gInt16 NumberOfBlocks,BLOCKTYPE_RADIAL,gInt16 NumberOfRadials,gInt16 NumberOfGates...BLOCKTYPE_GRID,gInt16 NumberOfRows,gInt16 NumberOfCols...,MUTI_LAYER,gInt32 LayerReplayTimes,gInt16 NumberOfBlocks,gInt16 BLOCKTYPE_STRUCT,gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct,...*/);

};


#include "SingleProductBase.inl"

#pragma pack(pop)
#endif //PRODUCTBASE_H