//////////////////////////////////////////////////////
//Product_264_CRH.cpp 
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#include "Product_264_CRH.h"


Product_264_CRH::Product_264_CRH(void)
{
}

Product_264_CRH::~Product_264_CRH(void)
{
}
GHRESULT Product_264_CRH::ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( 1, ONCE_LAYER , 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 264;

	return GS_OK;

}

PDBlock  *Product_264_CRH::GetMyPDBlock( )
{
	return GetPDBlock( );	
}

DILayer  *Product_264_CRH::GetMyDILayer()
{
	return GetLayer(0);
}

DLIBlock *Product_264_CRH::GetMyDILBlock()
{
	return GetBlock(0,0);
}

RadialHead *Product_264_CRH::GetMyRadial_Head()
{
	return GetBlockDataAsRadial_Head(0,0);
}
RadialData *Product_264_CRH::GetMyRadial_RadialData( gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(0,0,RadialIndex);
}
gInt16 *Product_264_CRH::GetMyRadial_Data( gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(0,0,RadialIndex);

}

RadarInfor * Product_264_CRH::GetRadarInfor()
{
	return GetRadarInfo();
}