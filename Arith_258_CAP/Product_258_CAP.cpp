//////////////////////////////////////////////////////////////////////////
//Product_258_CAP.CPP
//create by yangys 080417
//////////////////////////////////////////////////////////////////////////
#include "Product_258_CAP.h"

Product_258_CAP::Product_258_CAP(void)
{

}
Product_258_CAP::~Product_258_CAP(void)
{

}
GHRESULT Product_258_CAP::ConstructIt( gInt16 LayerIndex ,gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( LayerIndex, MUTI_LAYER ,LayerIndex, 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 258;

	return GS_OK;

}

PDBlock  *Product_258_CAP::GetMyPDBlock( )
{
	return GetPDBlock();	
}

DILayer  *Product_258_CAP::GetMyDILayer(int i)
{
	return GetLayer(i);
}
DLIBlock *Product_258_CAP::GetMyDILBlock( gInt32 LayerIndex )
{
	return GetBlock(LayerIndex,0);
}

RadialHead *Product_258_CAP::GetMyRadial_Head( gInt32 LayerIndex )
{
	return GetBlockDataAsRadial_Head(LayerIndex,0);
}

RadialData *Product_258_CAP::GetMyRadial_RadialData( gInt32 LayerIndex,gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,0,RadialIndex);
}
gInt16     *  Product_258_CAP::GetMyRadial_Data( gInt32 LayerIndex,gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(LayerIndex,0,RadialIndex);

}

RadarInfor * Product_258_CAP::GetRadarInfor()
{
	return GetRadarInfo();
}