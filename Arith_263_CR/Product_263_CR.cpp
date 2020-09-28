//////////////////////////////////////////////////////
//Product_263_CR.cpp 
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#include "Product_263_CR.h"


Product_263_CR::Product_263_CR(void)
{
}

Product_263_CR::~Product_263_CR(void)
{
}
GHRESULT Product_263_CR::ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates )
{
	GHRESULT Tempnam;

	Tempnam = FrameAll( 1, ONCE_LAYER , 1, BLOCKTYPE_RADIAL , NumberOfRadials , NumberOfGates );
	
	if( Tempnam != GS_OK )
		
		return Tempnam;
	GetPDBlock()->ProductCode = 263;

	return GS_OK;

}

RadarInfor * Product_263_CR::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock  *Product_263_CR::GetMyPDBlock( )
{
	return GetPDBlock( );	
}

DILayer  *Product_263_CR::GetMyDILayer()
{
	return GetLayer(0);
}
DLIBlock *Product_263_CR::GetMyDILBlock()
{
	return GetBlock(0,0);
}

RadialHead *Product_263_CR::GetMyRadial_Head()
{
	return GetBlockDataAsRadial_Head(0,0);
}

RadialData *Product_263_CR::GetMyRadial_RadialData( gInt32 RadialIndex )
{
	return GetBlockDataAsRadial_RadialData(0,0,RadialIndex);
}
gInt16     *  Product_263_CR::GetMyRadial_Data( gInt32 RadialIndex )
{
	return   GetBlockDataAsRadial_Data(0,0,RadialIndex);

}
