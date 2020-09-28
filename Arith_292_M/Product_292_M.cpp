#include "Product_292_M.h"

Product_292_M::Product_292_M(void)
{
}

Product_292_M::~Product_292_M(void)
{
}

GHRESULT Product_292_M::ConstructIt(gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2, 
									gInt16 NumOfstruct3,gInt16 LengthOfStruct3/*,gInt16 NumOfstruct4,gInt16  LengthOfStruct4*/)
{
	GHRESULT Tempnam;
	Tempnam = FrameAll(1, ONCE_LAYER ,3, BLOCKTYPE_STRUCT ,  NumOfstruct1, LengthOfStruct1,BLOCKTYPE_STRUCT, NumOfstruct2, LengthOfStruct2, 
		BLOCKTYPE_STRUCT, NumOfstruct3, LengthOfStruct3/*,BLOCKTYPE_STRUCT, NumOfstruct4, LengthOfStruct4*/);

	if( Tempnam != GS_OK )
		return Tempnam;

	GetPDBlock()->ProductCode = 292;
	return GS_OK;

}

PDBlock  *Product_292_M::GetMyPDBlock()
{
	return GetPDBlock();	
}

DILayer * Product_292_M::GetMyLayer()
{
	return  GetLayer(0);
}

DLIBlock * Product_292_M::GetMyBlock(gInt16 NumofBlock)
{
	return GetBlock(0, NumofBlock);
}
StructHead * Product_292_M::GetMyBlockDataAsStruct_Head(gInt32 BlockIndex)
{
	return	GetBlockDataAsStruct_Head(0,BlockIndex);
}

void *Product_292_M::GetBlockAsStruct_Data(gInt32 BlockIndex)
{
	return  GetBlockDataAsStruct_Data(0,BlockIndex);
}

RadarInfor * Product_292_M::GetRadarInfor()
{
	return GetRadarInfo();
}
