#include "Product_291_HI.h"
#include "SingleProductBase.h"

Product_291_HI::Product_291_HI(void)
{
	;
}
Product_291_HI::~Product_291_HI(void)
{
	;
}
GHRESULT Product_291_HI::ConstructIt( gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2)
{
	GHRESULT Tempnam;

	Tempnam = FrameAll(1, ONCE_LAYER ,2, BLOCKTYPE_STRUCT ,  NumOfstruct1, LengthOfStruct1,BLOCKTYPE_STRUCT, NumOfstruct2, LengthOfStruct2 );

	if( Tempnam != GS_OK )
		return Tempnam;
	GetPDBlock()->ProductCode = 291;
	return GS_OK;
}
DILayer * Product_291_HI::GetMyLayer()
{
	return  GetLayer(0);
}
DLIBlock * Product_291_HI::GetMyBlock(gInt16 NumofBlock)
{
	return GetBlock(0, NumofBlock);
}

PDBlock  *Product_291_HI::GetMyPDBlock( )
{
	return GetPDBlock();	
}
StructHead * Product_291_HI::GetBlockAsStruct_Head(gInt16 NumofBlock)
{
	return GetBlockDataAsStruct_Head( 0,NumofBlock);
}
void *Product_291_HI::GetBlockAsStruct_Data(gInt16 NumofBlock)
{
	return  GetBlockDataAsStruct_Data(0,NumofBlock);
}
RadarInfor * Product_291_HI::GetRadarInfor()
{
	return GetRadarInfo();
}