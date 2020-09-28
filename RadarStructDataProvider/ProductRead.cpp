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
};//获取产品信息头

DILayer* ProductRead::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//获取层信息头

DLIBlock* ProductRead::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlock(LayerIndex,BlockIndex);
};//获取块信息头
RadialHead* ProductRead::GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex)
{
	return GetBlockDataAsRadial_Head(LayerIndex,BlockIndex);
};  //add by ljg 20090724 for 获取径向信息头 
RadialData* ProductRead::GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,BlockIndex,RadialIndex);
};  //add by ljg 20090724 for 获取径向小信息头 

GridHead* ProductRead::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//获取网格信息头

StructHead* ProductRead::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//获取结构体信息头

//void * CProductRead::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//获取块数据

void *  ProductRead::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//获取结构体块数据

void *  ProductRead::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//获取网格块数据

void *  ProductRead::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//获取径向块数据




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
};//获取产品信息头

MultiLayerHead* ProductReadMosac::GetHeadOfLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
};//获取层信息头

MultiDLIBlock* ProductReadMosac::GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetMultiBlock(LayerIndex,BlockIndex);
};//获取块信息头

MultiGridHead* ProductReadMosac::GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
};//获取结构体信息头

MultiStructHead* ProductReadMosac::GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsStruct_Head(LayerIndex,BlockIndex);
};//获取结构体信息头

//void * CProductReadMosac::GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
//{
//	void* p=NULL;
//	p=GetBlockData(LayerIndex,BlockIndex);
//	return p;
//};//获取块数据

void *  ProductReadMosac::GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex)
{
	void* p=NULL;
	p=GetBlockDataAsStruct_Data(LayerIndex,BlockIndex);
	return p;
};//获取结构体块数据

void *  ProductReadMosac::GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	void* p=NULL;
	p=GetBlockDataAsGrid_Data(LayerIndex,BlockIndex,RowIndex);
	return p;
};//获取网格块数据

void *  ProductReadMosac::GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	void* p=NULL;
	//p=GetBlockDataAsRadial_Data(LayerIndex,BlockIndex,RadialIndex);
	return p;
};//获取径向块数据


