//////////////////////////////////////////////////////
//Product_265_ET.cpp 
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#include "Product_265_ET.h"


Product_265_ET::Product_265_ET(void)
{
}

Product_265_ET::~Product_265_ET(void)
{
}
GHRESULT Product_265_ET::ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( 1, ONCE_LAYER , 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 265;

	return GS_OK;
}
PDBlock  *Product_265_ET::GetMyPDBlock( )
{
	return GetPDBlock( );	
}

DILayer  *Product_265_ET::GetMyDILayer()
{
	return GetLayer(0);
}
DLIBlock *Product_265_ET::GetMyDILBlock()
{
	return GetBlock(0,0);
}
RadialHead *Product_265_ET::GetMyRadial_Head()
{
	return GetBlockDataAsRadial_Head(0,0);
}
RadialData *Product_265_ET::GetMyRadial_RadialData( gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(0,0,RadialIndex);
}
gInt16     *  Product_265_ET::GetMyRadial_Data( gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(0,0,RadialIndex);
}

RadarInfor * Product_265_ET::GetRadarInfor()
{
	return GetRadarInfo();
}
