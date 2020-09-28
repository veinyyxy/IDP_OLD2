///////////////////////////////////////////////////////////////////////////////
//ProductRead.h 
//add by ljg 20080825
//classes for read product data .
///////////////////////////////////////////////////////////////////////////////
#ifndef PRODUCT_GENERAL_READ_H
#define PRODUCT_GENERAL_READ_H
#include "../GDataTypes/SingleProductBase.h"   
#include "../GDataTypes/MultiProductBase.h"  //add by ljg 20090220 for ������Ʒ��

class  ProductRead : public ProductBase
{
public:
	ProductRead();
	~ProductRead();
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
	//����������Ҫ�ĺ������������Щ�Լ���ӣ������ټӡ�
};
//add by ljg 20090220 for ������Ʒ��
class  ProductReadMosac : public MultiProductBase
{
public:
	ProductReadMosac();
	~ProductReadMosac();
public:
	MultiDataHead * GetHeadOfProduct();//��ȡ��Ʒ��Ϣͷ
	MultiLayerHead* GetHeadOfLayer(gInt32 LayerIndex);//��ȡ����Ϣͷ
	MultiDLIBlock* GetHeadOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ����Ϣͷ

	MultiGridHead* GetHeadOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ�ṹ����Ϣͷ
	MultiStructHead* GetHeadOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ�ṹ����Ϣͷ

	//void * GetDataOfBlock(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ������
	void * GetDataOfBlockStruct(gInt32 LayerIndex,gInt32 BlockIndex);//��ȡ�ṹ�������
	void * GetDataOfBlockGrid(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex);//��ȡ���������
	void * GetDataOfBlockRadial(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex);//��ȡ���������
	//����������Ҫ�ĺ������������Щ�Լ���ӣ������ټӡ�
};



#endif//#ifndef PRODUCT_GENERAL_READ_H