//////////////////////////////////////////////////////////////////////////
// MultiProductBase.h
// created by ply 20080416
// 组网产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////

#pragma  once
#ifndef MULTIPRODUCTBASE_H
#define MULTIPRODUCTBASE_H

#pragma pack(push,1)


#include "GData.h"

typedef struct tagAreaInfo
{
		gChar	AreaCode[8];					//组网区域号
		gInt32	LongitudeOfWest;				//组网区域的西端经度（0.0001度）
		gInt32	LatitudeOfNorth;				//组网区域的北端纬度（0.0001度）
		gInt32	LongitudeOfEast;				//组网区域的东端经度（0.0001度）
		gInt32	LatitudeOfSouth;				//组网区域的南端纬度（0.0001度）
		gInt16	NumberOfRadars;					//实际组网站点数量
		gInt32	RadarSiteInfo[32];				//组网雷达站信息，包括组网雷达站及基数据信息
}AreaInfo;

typedef struct tagMultiDataHead					//组网产品数据头/产品信息
{
		gInt16 ProductCode;						//产品号
		gUint16 ProductDate;					//产品日期，儒略日（Julian）表示
		gInt32 ProductTime;						//产品时间，以毫秒为单位
		gUint16 GenerationDateOfProduct;		//产品生成日期，儒略日（Julian）表示
		gInt32 GenerationTimeOfProduct;			//产品生成时间，以毫秒为单位
		gInt16 ScaleOfData;						//产品数据缩放倍数
		gInt16 ProductDependentCount;			//产品参数个数
		gInt16 ProductDependent[20];			//产品参数
		gInt16 NumberOfLayers;					//数据层数
		gInt32 IndexOfLayers;					//数据层指针，相对于产品数据起始地址的偏移量
		gInt32 IndexOfTabular;					//表格块指针，相对于产品数据起始地址的偏移量
		gInt32 IndexOfGraphic;					//图形块指针，相对于产品数据起始地址的偏移量

		//gInt16	MessageCode;					//组网产品信息编号//249
		//gUint16	MosaicDate;						//拼图数据的Julian日期
		//gInt32	MosaicTime;						//拼图数据时间
		//gChar	RegionID[8];					//区域识别码
		//gChar	RegionName[16];					//区域名称（1-7个汉字）
		//gChar	ProductName[8];					//产品英文名称缩写（大写）
		//gUint16	GenerationDateOfMosaic;			//拼图处理Julian日期
		//gInt32	GenerationTimeOfMosaic;			//拼图处理时间
		//gInt16	NumberOfRadars;					//组网雷达个数//不要直接写//don't write it directly
		//gInt16	ProductCode;					//产品号
		//gInt32	LongitudeOfWest;				//拼图区域的西端经度（0.0001度）
		//gInt32	LatitudeOfNorth;				//拼图区域的北端纬度（0.0001度）
		//gInt32	LongitudeOfEast;				//拼图区域的东端经度（0.0001度）
		//gInt32	LatitudeOfSouth;				//拼图区域的南端纬度（0.0001度）
		//gInt32	LongitudeReso;					//经向间距（0.00001度）(经向分辨率)
		//gInt32	LatitudeReso;					//纬向间距（0.00001度）(纬向分辨率)
		//gInt16	NumberOfLayers;					//数据的层数//不要直接写//don't write it directly
		//gInt16	NumberOfRows;					//数据的行数
		//gInt16	NumberOfCols;					//数据的列数
		//gInt16	Scale;							//数据放大倍数
		//gInt32	OffsetBytesOfSiteInfo;			//站点信息偏移字节数
		//gInt32	OffsetBytesOfLayerOffset;		//层偏移量信息的偏移字节数
		//gChar	ProductCName[32];				//产品名称（1-15个汉字）
		//gInt16	Spare[3];						//留空
		//gInt32	SiteSign[32];					//共1024位。其中低1000位，每位表示一个雷达站编号。等同 z9后面的数字。比如北京z9010占用第11位(0位开始计算)//不要直接写//don't write it directly
} MultiDataHead;


//typedef struct tagSiteInf						//组网雷达标识信息
//{
//	char SiteID[8];								//雷达标识码
//} SiteInf;

//		gInt32 OffsetOfEachLayer[NumberOfLayers];	//每层的偏移量//不要直接写//don't write it directly

//		gInt32 Unused[n];						//不定长预留区。


//typedef struct tagMultiLayerHead						//组网雷达标识信息
//{
//		gInt16	BlockLayer;						//0xff
//		gInt32	LongitudeOfWest;				//本层区域的西端经度（0.0001度）
//		gInt32	LatitudeOfNorth;				//本层区域的北端纬度（0.0001度）
//		gInt32	LongitudeOfEast;				//本层区域的东端经度（0.0001度）
//		gInt32	LatitudeOfSouth;				//本层区域的南端纬度（0.0001度）
//		gInt16	NumberOfRows;					//数据的行数//不要直接写//don't write it directly
//		gInt16	NumberOfCols;					//数据的列数//不要直接写//don't write it directly
//		gInt16	Height;							//数据层高度值（10米）
//		//gInt16 Data[NumberOfRows*NumberOfCols]	//数据，从北到南逐行排放；每行数据从西到东存放
//} MultiLayerHead;

typedef struct tagMultiLayerHead
{
		gInt16	SNOfLayer;//数据层序号
		gInt32	LengthOfLayer;//数据层长度（字节数）
		gInt16	LayerDate;//层数据日期
		gInt32	LayerTime;//层数据时间
		gInt32	HeightOfElevation;//本层数据的高度或仰角度数，根据产品区分
		gInt16	NumberOfBlocks;//数据层中的数据块数
		gInt32	IndexOfBlocks;//数据块指针，相对于数据层起始地址的偏移量
}MultiLayerHead;

typedef struct tagMultiDLIBlock
{
		gInt16	SNOfBlock;					//当前数据层内数据块的序号
		gInt16	TypeOfDataPackets;			//数据块类型
		gInt32	LengthOfBlock;				//数据块长度
}MultiDLIBlock;

typedef struct tagMultiGridHead 
{
		gInt16 NumberOfRows;
		gInt16 NumberOfCols;
		gInt16 ResolutionOfRow;
		gInt16 ResolutionOfCol;
}MultiGridHead;

typedef struct tagMultiGridData
{
		gInt16 SNOfRow;
		//Data
}MultiGridData;

typedef struct  tagMultiStructHead 
{
		gInt16 LengthOfDataType;
		gInt16 NumbersOfData;
		//Data;
}MultiStructHead;

typedef struct tagSiteInf						//组网雷达标识信息
{
		gChar SiteCode[8];
		gChar RadarType[4];
		gInt32 Longitude;
		gInt32 Latitude;
		gInt32 Altitude;
		gInt8 OperationalMode[2] ;
		gInt8 VCPMode[4];
		gUint16 BaseDataDate;
		gInt32 BaseDataTime;
} SiteInf;

enum MultiBlockType
{
	MULTI_BLOCKTYPE_GRID=0x0B00,
	MULTI_BLOCKTYPE_LONLATGRID=0x0B01,
	MULTI_BLOCKTYPE_STRUCT=0x0C00,
	MULTI_BLOCKTYPE_UNDEFINED=0x0F00,
};

class DLLEXP_GSYS MultiProductBase:public GenerationData
{
public:
	inline void GetProductCode(gInt16& nProductCode);  //add by dqc 20081219
	//write functions
	GHRESULT CreateP();
	GHRESULT AddSiteP(const SiteInf & SiteToAdd);//suggest add site before any layer add.
	//GHRESULT AddLayerP(gInt32 LayerReplayTimes, gInt16 NumberOfBlocks,...);//suggest add layer after all sites added.

	//write/read functions
	MultiDataHead * GetMultiHeadP();
	SiteInf *GetSiteP(gInt16 RadarIndex);
	MultiLayerHead * GetLayerP(gInt16 LayerIndex);
	AreaInfo * GetAreaInfoP();
	MultiGridHead *	GetBlockDataAsGrid_HeadP(gInt32 LayerIndex,gInt32 BlockIndex);
	//gInt16 *GetLayerDataP(gInt16 LayerIndex);
	gInt16 *GetLineDataP(gInt16 LayerIndex,gInt16 RowIndex);

//private:  //del by chenxiang 20080512
protected:
	//write functions
	inline GHRESULT Create();
	inline GHRESULT AddSite(const SiteInf & SiteToAdd);//suggest add site before any layer add.
	//inline GHRESULT AddLayer(gInt16	NumberOfRows,gInt16	NumberOfCols,gInt16 LayerRepickTimes=1);//suggest add layer after all sites added.
	inline GHRESULT AddLayer(gInt32 LayerReplayTimes, gInt16 NumberOfBlocks,.../*BLOCKTYPE_RADIAL,gInt16 NumberOfRadials,gInt16 NumberOfGates...BLOCKTYPE_GRID,gInt16 NumberOfRows,gInt16 NumberOfCols...,MUTI_LAYER,gInt32 LayerReplayTimes,gInt16 NumberOfBlocks,gInt16 BLOCKTYPE_STRUCT,gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct,...*/);//suggest add layer after all sites added.
	//write/read functions
	inline AreaInfo * GetAreaInfo();
	inline gInt32 GetOffsetOfSiteInf();
	inline MultiDataHead * GetMultiHead();
	inline SiteInf *GetSite(gInt16 RadarIndex);
	inline MultiLayerHead * GetLayer(gInt16 LayerIndex);

	inline gInt32 GetSizeOfGridBlock(gInt16 NumberOfRows,gInt16 NumberOfCols);
	inline gInt32 GetSizeOfStructBlock(gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct);

	inline gInt32 * GetLayerOffsetIndex();
	inline gInt32 GetSizeBeforeOffsetIndex();
	inline gInt32 GetOffsetOfLayer(gInt32 LayerIndex);
	inline MultiDLIBlock * GetMultiBlock(gInt32 LayerIndex,gInt32 BlockIndex);
	inline gInt32 GetOffsetOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);
	inline void * GetMultiBlockData(gInt32 LayerIndex,gInt32 BlockIndex); 
	inline gInt32 GetOffsetOfBlockData(gInt32 LayerIndex,gInt32 BlockIndex);

	inline MultiGridHead * GetBlockDataAsGrid_Head(gInt32 LayerIndex,gInt32 BlockIndex);
	inline MultiGridData * GetBlockDataAsGrid_RowData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);
	inline gInt16 * GetBlockDataAsGrid_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);
GHRESULT FramALL1(gInt16 SiteNum,gInt32 LayerReplayTimes, gInt16 NumberOfBlocks,...);
	inline MultiStructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex);
	inline void * GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex);
	inline MultiBlockType GetMultiBlockType(gInt32 LayerIndex,gInt32 BlockIndex);

};


#pragma pack(pop)
#include "MultiProductBase.inl"
#endif //MULTIPRODUCTBASE_H