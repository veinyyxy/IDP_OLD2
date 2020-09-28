//////////////////////////////////////////////////////
//Product_267_VILD.cpp 
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#include "Product_267_VILD.h"


Product_267_VILD::Product_267_VILD(void)
{
}

Product_267_VILD::~Product_267_VILD(void)
{
}
GHRESULT Product_267_VILD::ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( 1, ONCE_LAYER , 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 267;

	return GS_OK;

}

PDBlock  *Product_267_VILD::GetMyPDBlock( )
{
	return GetPDBlock( );	
}

DILayer  *Product_267_VILD::GetMyDILayer()
{
	return GetLayer(0);
}
DLIBlock *Product_267_VILD::GetMyDILBlock()
{
	return GetBlock(0,0);
}
RadialHead *Product_267_VILD::GetMyRadial_Head()
{
	return GetBlockDataAsRadial_Head(0,0);
}
RadialData *Product_267_VILD::GetMyRadial_RadialData( gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(0,0,RadialIndex);
}
gInt16     *  Product_267_VILD::GetMyRadial_Data( gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(0,0,RadialIndex);
}

RadarInfor * Product_267_VILD::GetRadarInfor()
{
	return GetRadarInfo();
}

