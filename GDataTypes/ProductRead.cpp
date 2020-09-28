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
};//获取产品信息头

DILayer* CProductRead::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//获取层信息头

DLIBlock* CProductRead::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlock(LayerIndex,BlockIndex);
};//获取块信息头
RadialHead* CProductRead::GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex)
{
	return GetBlockDataAsRadial_Head(LayerIndex,BlockIndex);
};  //add by ljg 20090724 for 获取径向信息头 
RadialData* CProductRead::GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,BlockIndex,RadialIndex);
};  //add by ljg 20090724 for 获取径向小信息头 

GridHead* CProductRead::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//获取网格信息头

StructHead* CProductRead::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//获取结构体信息头

//void * CProductRead::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//获取块数据

void *  CProductRead::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//获取结构体块数据

void *  CProductRead::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//获取网格块数据

void *  CProductRead::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//获取径向块数据