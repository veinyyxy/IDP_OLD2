//////////////////////////////////////////////////////////////////////////
// MultiProductBase.h
// created by ply 20080416
// ������Ʒ����ͨ�û��࣬GData������
//////////////////////////////////////////////////////////////////////////

#pragma  once
#ifndef MULTIPRODUCTBASE_H
#define MULTIPRODUCTBASE_H

#pragma pack(push,1)


#include "GData.h"

typedef struct tagAreaInfo
{
		gChar	AreaCode[8];					//���������
		gInt32	LongitudeOfWest;				//������������˾��ȣ�0.0001�ȣ�
		gInt32	LatitudeOfNorth;				//��������ı���γ�ȣ�0.0001�ȣ�
		gInt32	LongitudeOfEast;				//��������Ķ��˾��ȣ�0.0001�ȣ�
		gInt32	LatitudeOfSouth;				//����������϶�γ�ȣ�0.0001�ȣ�
		gInt16	NumberOfRadars;					//ʵ������վ������
		gInt32	RadarSiteInfo[32];				//�����״�վ��Ϣ�����������״�վ����������Ϣ
}AreaInfo;

typedef struct tagMultiDataHead					//������Ʒ����ͷ/��Ʒ��Ϣ
{
		gInt16 ProductCode;						//��Ʒ��
		gUint16 ProductDate;					//��Ʒ���ڣ������գ�Julian����ʾ
		gInt32 ProductTime;						//��Ʒʱ�䣬�Ժ���Ϊ��λ
		gUint16 GenerationDateOfProduct;		//��Ʒ�������ڣ������գ�Julian����ʾ
		gInt32 GenerationTimeOfProduct;			//��Ʒ����ʱ�䣬�Ժ���Ϊ��λ
		gInt16 ScaleOfData;						//��Ʒ�������ű���
		gInt16 ProductDependentCount;			//��Ʒ��������
		gInt16 ProductDependent[20];			//��Ʒ����
		gInt16 NumberOfLayers;					//���ݲ���
		gInt32 IndexOfLayers;					//���ݲ�ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����
		gInt32 IndexOfTabular;					//����ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����
		gInt32 IndexOfGraphic;					//ͼ�ο�ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����

		//gInt16	MessageCode;					//������Ʒ��Ϣ���//249
		//gUint16	MosaicDate;						//ƴͼ���ݵ�Julian����
		//gInt32	MosaicTime;						//ƴͼ����ʱ��
		//gChar	RegionID[8];					//����ʶ����
		//gChar	RegionName[16];					//�������ƣ�1-7�����֣�
		//gChar	ProductName[8];					//��ƷӢ��������д����д��
		//gUint16	GenerationDateOfMosaic;			//ƴͼ����Julian����
		//gInt32	GenerationTimeOfMosaic;			//ƴͼ����ʱ��
		//gInt16	NumberOfRadars;					//�����״����//��Ҫֱ��д//don't write it directly
		//gInt16	ProductCode;					//��Ʒ��
		//gInt32	LongitudeOfWest;				//ƴͼ��������˾��ȣ�0.0001�ȣ�
		//gInt32	LatitudeOfNorth;				//ƴͼ����ı���γ�ȣ�0.0001�ȣ�
		//gInt32	LongitudeOfEast;				//ƴͼ����Ķ��˾��ȣ�0.0001�ȣ�
		//gInt32	LatitudeOfSouth;				//ƴͼ������϶�γ�ȣ�0.0001�ȣ�
		//gInt32	LongitudeReso;					//�����ࣨ0.00001�ȣ�(����ֱ���)
		//gInt32	LatitudeReso;					//γ���ࣨ0.00001�ȣ�(γ��ֱ���)
		//gInt16	NumberOfLayers;					//���ݵĲ���//��Ҫֱ��д//don't write it directly
		//gInt16	NumberOfRows;					//���ݵ�����
		//gInt16	NumberOfCols;					//���ݵ�����
		//gInt16	Scale;							//���ݷŴ���
		//gInt32	OffsetBytesOfSiteInfo;			//վ����Ϣƫ���ֽ���
		//gInt32	OffsetBytesOfLayerOffset;		//��ƫ������Ϣ��ƫ���ֽ���
		//gChar	ProductCName[32];				//��Ʒ���ƣ�1-15�����֣�
		//gInt16	Spare[3];						//����
		//gInt32	SiteSign[32];					//��1024λ�����е�1000λ��ÿλ��ʾһ���״�վ��š���ͬ z9��������֡����籱��z9010ռ�õ�11λ(0λ��ʼ����)//��Ҫֱ��д//don't write it directly
} MultiDataHead;


//typedef struct tagSiteInf						//�����״��ʶ��Ϣ
//{
//	char SiteID[8];								//�״��ʶ��
//} SiteInf;

//		gInt32 OffsetOfEachLayer[NumberOfLayers];	//ÿ���ƫ����//��Ҫֱ��д//don't write it directly

//		gInt32 Unused[n];						//������Ԥ������


//typedef struct tagMultiLayerHead						//�����״��ʶ��Ϣ
//{
//		gInt16	BlockLayer;						//0xff
//		gInt32	LongitudeOfWest;				//������������˾��ȣ�0.0001�ȣ�
//		gInt32	LatitudeOfNorth;				//��������ı���γ�ȣ�0.0001�ȣ�
//		gInt32	LongitudeOfEast;				//��������Ķ��˾��ȣ�0.0001�ȣ�
//		gInt32	LatitudeOfSouth;				//����������϶�γ�ȣ�0.0001�ȣ�
//		gInt16	NumberOfRows;					//���ݵ�����//��Ҫֱ��д//don't write it directly
//		gInt16	NumberOfCols;					//���ݵ�����//��Ҫֱ��д//don't write it directly
//		gInt16	Height;							//���ݲ�߶�ֵ��10�ף�
//		//gInt16 Data[NumberOfRows*NumberOfCols]	//���ݣ��ӱ����������ŷţ�ÿ�����ݴ����������
//} MultiLayerHead;

typedef struct tagMultiLayerHead
{
		gInt16	SNOfLayer;//���ݲ����
		gInt32	LengthOfLayer;//���ݲ㳤�ȣ��ֽ�����
		gInt16	LayerDate;//����������
		gInt32	LayerTime;//������ʱ��
		gInt32	HeightOfElevation;//�������ݵĸ߶Ȼ����Ƕ��������ݲ�Ʒ����
		gInt16	NumberOfBlocks;//���ݲ��е����ݿ���
		gInt32	IndexOfBlocks;//���ݿ�ָ�룬��������ݲ���ʼ��ַ��ƫ����
}MultiLayerHead;

typedef struct tagMultiDLIBlock
{
		gInt16	SNOfBlock;					//��ǰ���ݲ������ݿ�����
		gInt16	TypeOfDataPackets;			//���ݿ�����
		gInt32	LengthOfBlock;				//���ݿ鳤��
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

typedef struct tagSiteInf						//�����״��ʶ��Ϣ
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