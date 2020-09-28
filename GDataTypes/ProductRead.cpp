#include "ProductRead.h"


CProductRead::CProductRead(void)
{
}


CProductRead::~CProductRead(void)
{
}

PDBlock* CProductRead::GetHeadOfProduct()
{
	return GetPDBlock();
};//��ȡ��Ʒ��Ϣͷ

DILayer* CProductRead::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//��ȡ����Ϣͷ

DLIBlock* CProductRead::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlock(LayerIndex,BlockIndex);
};//��ȡ����Ϣͷ
RadialHead* CProductRead::GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex)
{
	return GetBlockDataAsRadial_Head(LayerIndex,BlockIndex);
};  //add by ljg 20090724 for ��ȡ������Ϣͷ 
RadialData* CProductRead::GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,BlockIndex,RadialIndex);
};  //add by ljg 20090724 for ��ȡ����С��Ϣͷ 

GridHead* CProductRead::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//��ȡ������Ϣͷ

StructHead* CProductRead::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//��ȡ�ṹ����Ϣͷ

//void * CProductRead::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//��ȡ������

void *  CProductRead::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//��ȡ�ṹ�������

void *  CProductRead::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//��ȡ���������

void *  CProductRead::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//��ȡ���������