//////////////////////////////////////////////////////////////////////////
//Product_268_VAD.CPP
//create by zsc 
//////////////////////////////////////////////////////////////////////////
#include "Product_268_VAD.h"
#include "SingleProductBase.h"
#include "CJulMsGMT.h"

Product_268_VAD::Product_268_VAD(void)
{

}
Product_268_VAD::~Product_268_VAD(void)
{

}
GHRESULT Product_268_VAD::ConstructIt(  gInt16 layerNum,gInt16 NumOfstruct , gInt16 LengthOfstruct )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll(layerNum, MUTI_LAYER ,layerNum,1, BLOCKTYPE_STRUCT ,NumOfstruct , LengthOfstruct );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 268;

	return GS_OK;

}

PDBlock  *Product_268_VAD::GetMyPDBlock( )
{
	return GetPDBlock();	
}

DILayer * Product_268_VAD::GetMyLayer(gInt16 layerNum)
{
	return GetLayer(layerNum);
}
DLIBlock * Product_268_VAD::GetMyBlock(gInt16 layerNum,gInt16 NumofBlock)
{
	return  GetBlock(layerNum, NumofBlock);
}
StructHead * Product_268_VAD::GetBlockAsStruct_Head(gInt16 layerNum)
{
	return GetBlockDataAsStruct_Head( layerNum,0);
}
void *Product_268_VAD::GetBlockAsStruct_Data(gInt16 layerNum)
{
	return  GetBlockDataAsStruct_Data( layerNum,0);
}
//inline StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex);
//inline void * GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex);
//StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex, gInt32 BlockIndex)
//{
//	return GetBlockDataAsStruct_Head(0,0);
//}
RadarInfor * Product_268_VAD::GetRadarInfor()
{
	return GetRadarInfo();
}