///////////////////////////////////////////////////////////////////////////////
//ProductRead.cpp
//add by ljg 20080825
//classes for read product data.
///////////////////////////////////////////////////////////////////////////////
#include "ProductRead.h"
/////////////////////////////////////////////////
//class CProductRead construction
/////////////////////////////////////////////////

ProductRead::ProductRead()
{

}

ProductRead::~ProductRead()
{

}

/////////////////////////////////////////////////
//class CProductRead Implation
/////////////////////////////////////////////////
PDBlock* ProductRead::GetHeadOfProduct()
{
	return GetPDBlock();
};//��ȡ��Ʒ��Ϣͷ

DILayer* ProductRead::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//��ȡ����Ϣͷ

DLIBlock* ProductRead::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlock(LayerIndex,BlockIndex);
};//��ȡ����Ϣͷ
RadialHead* ProductRead::GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex)
{
	return GetBlockDataAsRadial_Head(LayerIndex,BlockIndex);
};  //add by ljg 20090724 for ��ȡ������Ϣͷ 
RadialData* ProductRead::GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,BlockIndex,RadialIndex);
};  //add by ljg 20090724 for ��ȡ����С��Ϣͷ 

GridHead* ProductRead::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//��ȡ������Ϣͷ

StructHead* ProductRead::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//��ȡ�ṹ����Ϣͷ

//void * CProductRead::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//��ȡ������

void *  ProductRead::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//��ȡ�ṹ�������

void *  ProductRead::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//��ȡ���������

void *  ProductRead::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//��ȡ���������




/////////////////////////////////////////////////
//class CProductRead construction
/////////////////////////////////////////////////

ProductReadMosac::ProductReadMosac()
{

};

ProductReadMosac::~ProductReadMosac()
{
};

/////////////////////////////////////////////////
//class CProductReadMosac Implation
/////////////////////////////////////////////////
MultiDataHead * ProductReadMosac::GetHeadOfProduct()
{
	return  GetMultiHeadP();
};//��ȡ��Ʒ��Ϣͷ

MultiLayerHead* ProductReadMosac::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//��ȡ����Ϣͷ

MultiDLIBlock* ProductReadMosac::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetMultiBlock(LayerIndex,BlockIndex);
};//��ȡ����Ϣͷ

MultiGridHead* ProductReadMosac::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//��ȡ�ṹ����Ϣͷ

MultiStructHead* ProductReadMosac::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//��ȡ�ṹ����Ϣͷ

//void * CProductReadMosac::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//��ȡ������

void *  ProductReadMosac::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//��ȡ�ṹ�������

void *  ProductReadMosac::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//��ȡ���������

void *  ProductReadMosac::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	//p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//��ȡ���������


