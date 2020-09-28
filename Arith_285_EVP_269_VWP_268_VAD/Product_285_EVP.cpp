//////////////////////////////////////////////////////////////////////////
//Product_285_EVP.CPP
//create by yangys 080417
//////////////////////////////////////////////////////////////////////////
#include "Product_285_EVP.h"
#include "SingleProductBase.h"

Product_285_EVP::Product_285_EVP(void)
{

}
Product_285_EVP::~Product_285_EVP(void)
{

}
GHRESULT Product_285_EVP::ConstructIt( gInt16 NumOfstruct , gInt16 LengthOfstruct )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll(1, ONCE_LAYER ,1, BLOCKTYPE_STRUCT ,NumOfstruct , LengthOfstruct );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = /*268*/285; //modify by dqc 20081117

	return GS_OK;

}

PDBlock  *Product_285_EVP::GetMyPDBlock( )
{
	return GetPDBlock();	
}

DILayer * Product_285_EVP::GetMyLayer()
{
    return GetLayer(0);
}
DLIBlock * Product_285_EVP::GetMyBlock(gInt16 NumofBlock)
{
  return GetBlock(0, NumofBlock);
}
StructHead * Product_285_EVP::GetBlockAsStruct_Head()
{
	return GetBlockDataAsStruct_Head( 0,0);
}
void *Product_285_EVP::GetBlockAsStruct_Data()
{
	return  GetBlockDataAsStruct_Data( 0,0);
}
//inline StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex);
//inline void * GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex);
//StructHead * GetBlockDataAsStruct_Head(gInt32 LayerIndex, gInt32 BlockIndex)
//{
//	return GetBlockDataAsStruct_Head(0,0);
//}
RadarInfor * Product_285_EVP::GetRadarInfor()
{
	return GetRadarInfo();
}