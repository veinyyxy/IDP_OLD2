//////////////////////////////////////////////////////////////////////////
//Product_277_SCI.CPP
//create by yangys 080417
//////////////////////////////////////////////////////////////////////////
#include "Product_277_SCI.h"
#include "SingleProductBase.h"

Product_277_SCI::Product_277_SCI(void)
{

}
Product_277_SCI::~Product_277_SCI(void)
{

}
GHRESULT Product_277_SCI::ConstructIt( gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2)
{
	GHRESULT Tempnam;

	Tempnam = FrameAll(1, ONCE_LAYER ,2, BLOCKTYPE_STRUCT ,  NumOfstruct1, LengthOfStruct1,BLOCKTYPE_STRUCT, NumOfstruct2, LengthOfStruct2 );

	if( Tempnam != GS_OK )
	return Tempnam;
	GetPDBlock()->ProductCode = 277;
	return GS_OK;
}
DILayer * Product_277_SCI::GetMyLayer()
{
    return  GetLayer(0);
}
DLIBlock * Product_277_SCI::GetMyBlock(gInt16 NumofBlock)
{
  return GetBlock(0, NumofBlock);
}

PDBlock  *Product_277_SCI::GetMyPDBlock( )
{
	return GetPDBlock();	
}
StructHead * Product_277_SCI::GetBlockAsStruct_Head(gInt16 NumofBlock)
{
	return GetBlockDataAsStruct_Head( 0,NumofBlock);
}
void *Product_277_SCI::GetBlockAsStruct_Data(gInt16 NumofBlock)
{
	return  GetBlockDataAsStruct_Data(0,NumofBlock);
}
RadarInfor * Product_277_SCI::GetRadarInfor()
{
	return GetRadarInfo();
}
