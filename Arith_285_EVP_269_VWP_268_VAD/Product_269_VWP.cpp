//////////////////////////////////////////////////////////////////////////
//Product_269_VWP.CPP
//create by yangys 080417
//////////////////////////////////////////////////////////////////////////
#include "Product_269_VWP.h"
#include "SingleProductBase.h"
#include "CJulMsGMT.h"

Product_269_VWP::Product_269_VWP(void)
{

}
Product_269_VWP::~Product_269_VWP(void)
{

}
GHRESULT Product_269_VWP::ConstructIt( gInt16 NumOfstruct , gInt16 LengthOfstruct )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll(1, ONCE_LAYER ,1, BLOCKTYPE_STRUCT ,NumOfstruct , LengthOfstruct );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 269;

	return GS_OK;

}

PDBlock  *Product_269_VWP::GetMyPDBlock( )
{
	return GetPDBlock();	
}

DILayer * Product_269_VWP::GetMyLayer()
{
    return GetLayer(0);
}
DLIBlock * Product_269_VWP::GetMyBlock(gInt16 NumofBlock)
{
  return  GetBlock(0, NumofBlock);
}
StructHead * Product_269_VWP::GetBlockAsStruct_Head()
{
	return GetBlockDataAsStruct_Head( 0,0);
}
void *Product_269_VWP::GetBlockAsStruct_Data()
{
	return  GetBlockDataAsStruct_Data( 0,0);
}
//inline StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex);
//inline void * GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex);
//StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex, gInt32 BlockIndex)
//{
//	return GetBlockDataAsStruct_Head(0,0);
//}
RadarInfor * Product_269_VWP::GetRadarInfor()
{
	return GetRadarInfo();
}