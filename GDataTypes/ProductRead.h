#pragma once
#include "singleproductbase.h"
class DLLEXP_GSYS CProductRead :
	public ProductBase
{
public:
	CProductRead(void);
	~CProductRead(void);
public:
	PDBlock* GetHeadOfProduct();//��ȡ��Ʒ��Ϣͷ
	DILayer* GetHeadOfLayer(gInt32 LayerIndex);//��ȡ����Ϣͷ
	DLIBlock* GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ����Ϣͷ

	RadialData* GetHeadOfBlockRadialData(gInt32 LayerIndex, gInt32 BlockIndex,gInt32 RadialIndex);  //add by ljg 20090724 for ��ȡ����С��Ϣͷ 
	RadialHead* GetHeadOfBlockRadial(gInt32 LayerIndex, gInt32 BlockIndex);  //add by ljg 20090724 for ��ȡ������Ϣͷ 
	GridHead*	GetHeadOfBlockGrid(gInt32 LayerIndex, gInt32 BlockIndex);  //��ȡ������Ϣͷ
	StructHead* GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ�ṹ����Ϣͷ

	//void * GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ������
	void * GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ�ṹ�������
	void * GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);//��ȡ���������
	void * GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex);//��ȡ���������
};

