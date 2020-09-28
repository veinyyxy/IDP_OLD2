//////////////////////////////////////////////////////
//Product_266_VIL.cpp 
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#include "Product_266_VIL.h"


Product_266_VIL::Product_266_VIL(void)
{
}

Product_266_VIL::~Product_266_VIL(void)
{
}
GHRESULT Product_266_VIL::ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( 1, ONCE_LAYER , 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );

	if( Tempnam != GS_OK )

		return Tempnam;
	GetPDBlock()->ProductCode = 266;

	return GS_OK;

}

PDBlock  *Product_266_VIL::GetMyPDBlock( )
{
	return GetPDBlock( );	
}

DILayer  *Product_266_VIL::GetMyDILayer()
{
	return GetLayer(0);
}

DLIBlock *Product_266_VIL::GetMyDILBlock()
{
	return GetBlock(0,0);
}

RadialHead *Product_266_VIL::GetMyRadial_Head()
{
	return GetBlockDataAsRadial_Head(0,0);
}
RadialData *Product_266_VIL::GetMyRadial_RadialData( gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(0,0,RadialIndex);
}
gInt16     *  Product_266_VIL::GetMyRadial_Data( gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(0,0,RadialIndex);

}

RadarInfor * Product_266_VIL::GetRadarInfor()
{
	return GetRadarInfo();
}