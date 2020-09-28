//////////////////////////////////////////////////////////////////////////
// SingleProductBase.h
// created by ply 20080401
// ��Ʒ����ͨ�û��࣬GData������
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

typedef struct tagPDBlock						//��Ʒ����ͷ/��Ʒ��Ϣ
{
		gInt16	ProductCode;					//��Ʒ��
		gUint16 ProductDate;					//��Ʒ���ڣ������գ�Julian����ʾ
		gInt32	ProductTime;					//��Ʒʱ�䣬�Ժ���Ϊ��λ
		gInt8	OperationalMode[2];				//����ģʽ
		gInt8	VCPMode[4];						//��ɨģʽ���״��ͺŲ�ͬ����ɨģʽ��ͬ
		gUint16 GenerationDateOfProduct;		//��Ʒ�������ڣ������գ�Julian����ʾ
		gInt32	GenerationTimeOfProduct;		//��Ʒ����ʱ�䣬�Ժ���Ϊ��λ
		gInt16  ScaleOfData;					//��Ʒ�������ű���
		gInt16	ProductDependentCount;			//��Ʒ��������
		gInt16	ProductDependent[20];				//��Ʒ����
		gInt16	NumberOfLayers;					//���ݲ���
		gInt32	IndexOfLayers;					//���ݲ�ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����
		gInt32  IndexOfTabular;                 //����ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����
		gInt32  IndexOfGraphic;					// ͼ�ο�ָ�룬����ڲ�Ʒ������ʼ��ַ��ƫ����
} PDBlock;

//		gInt32 OffsetOfEachLayer[NumberOfLayers];	//ÿ���ƫ����

typedef struct tagDILayer				//���ݿ�
{
		gInt16	SNOfLayer;//���ݲ����
		gInt32	LengthOfLayer;//���ݲ㳤�ȣ��ֽ�����
		gInt16	LayerDate;//����������
		gInt32	LayerTime;//������ʱ��
		gInt32	HeightOfElevation;//�������ݵĸ߶Ȼ����Ƕ��������ݲ�Ʒ����
		gInt16	NumberOfBlocks;//���ݲ��е����ݿ���
		gInt32	IndexOfBlocks;//���ݿ�ָ�룬��������ݲ���ʼ��ַ��ƫ����
} DILayer;

typedef struct tagDLIBlock 
{
		gInt16	SNOfBlock;					//��ǰ���ݲ������ݿ�����
		gInt16	TypeOfDataPackets;			//���ݿ�����
		gInt32	LengthOfBlock;				//���ݿ鳤��
} DLIBlock;

typedef struct tagRadialHead 
{
		gInt16	DistanceOfFirstGate;//��һ������λ��
		gInt16	LengthOfGate;				//����ⳤ��
		gInt16	NumberOfRadials;			//�������ݵĸ���
		gInt16	NumberOfGates;				//�������ݵľ������
		gInt16	MaxVelocity;				//���ģ���ٶ� ��Nyquist�ٶȻ�Nyquist�ٶȡ�2����ģ��������
} RadialHead;

typedef struct tagRadialData 
{
		gUint16  AzimuthAngle;				//��λ��
		gInt16	DeltaAngle;					//��λ�Ǽ��
		//gInt16 Data;						//���ݣ����ݸ���=�������ݵĸ������������ݵľ������
} RadialData;
typedef struct tagGridHead 
{
		gInt16	NumberOfRows;				//����
		gInt16	NumberOfCols;				//����
		gInt16	ResolutionOfRow;			//�зֱ���
		gInt16	ResolutionOfCol;			//�зֱ���
} GridHead;

typedef struct tagGridRowData 
{
		gInt16	SNOfRow;				//�����
		//gInt16	Data;					//���ݣ����ݸ���=����������
} GridRowData;

typedef struct tagStructHead 
{
		gInt16	LengthOfDataType;				//�������ͳ���
		gInt16	NumbersOfData;					//���ݸ���
} StructHead;

enum BlockType
{
	BLOCKTYPE_RADIAL=0x0A00,
	BLOCKTYPE_GRID=0x0B00,
	BLOCKTYPE_LONLATGRID=0x0B01,
	BLOCKTYPE_STRUCT=0x0C00,
	BLOCKTYPE_UNDEFINED=0x0F00,

	//�����Ǹ������õ�
	ONCE_LAYER=0x0D00,
	MUTI_LAYER=0x0E00,
};

/*
m_TemIntʹ�ñ�

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