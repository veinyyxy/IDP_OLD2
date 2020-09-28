#pragma once
#include "singleproductbase.h"
class DLLEXP_GSYS CProductRead :
	public ProductBase
{
public:
	CProductRead(void);
	~CProductRead(void);
public:
	PDBlock* GetHeadOfProduct();//获取产品信息头
	DILayer* GetHeadOfLayer(gInt32 LayerIndex);//获取层信息头
	DLIBlock* GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//获取块信息头

	RadialData* GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex);  //add by ljg 20090724 for 获取径向小信息头 
	RadialHead* GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex);  //add by ljg 20090724 for 获取径向信息头 
	GridHead*	GetHeadOfBlockGrid(gInt32 LayerIndex, gInt32 BlockIndex);  //获取网格信息头
	StructHead* GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//获取结构体信息头

	//void * GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//获取块数据
	void * GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//获取结构体块数据
	void * GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);//获取网格块数据
	void * GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex);//获取径向块数据
};

