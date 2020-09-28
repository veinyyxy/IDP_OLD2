//////////////////////////////////////////////////////////////////////////
//Product_501_BREF.cpp
//created by ChenXiang 20080429
//501号产品,组网算法,BREF
//////////////////////////////////////////////////////////////////////////
#include "Product_501_BREF.h"

Product_501_BREF::Product_501_BREF(void)
{
	
}
Product_501_BREF::~Product_501_BREF(void)
{
	
}
GHRESULT Product_501_BREF::Coustruct( gInt16 SiteNum,gInt16 NumberOfRows,gInt16 NumberOfCols,gInt16 LayerRepickTimes)
{
	GHRESULT Tempnam;

	Tempnam = FramALL1( SiteNum,LayerRepickTimes, 1,MULTI_BLOCKTYPE_GRID , NumberOfRows , NumberOfCols );
	GetMultiHead()->ProductCode = 501;

	if( Tempnam != GS_OK )

		return Tempnam;
	
	return GS_OK;
}

GHRESULT Product_501_BREF::AddMyLayer(gInt16 NumberOfRows,gInt16 NumberOfCols,gInt16 LayerRepickTimes)
{
	return AddLayer(LayerRepickTimes, 1, MULTI_BLOCKTYPE_GRID, NumberOfRows,NumberOfCols);
}
GHRESULT Product_501_BREF::AddMySite(const SiteInf & SiteToAdd)//suggest add site before any layer add.
{
	return AddSite(SiteToAdd);
}
MultiDataHead * Product_501_BREF::GetMyMultiHead()
{
	return GetMultiHead();
}
SiteInf * Product_501_BREF::GetMySite(gInt16 RadarIndex)
{
	return GetSite(RadarIndex);
}
MultiLayerHead * Product_501_BREF::GetMyLayer(gInt16 LayerIndex)
{
	return GetLayer(LayerIndex);
}
gInt16 * Product_501_BREF::GetMyLayerData(gInt16 LayerIndex)
{
	return GetBlockDataAsGrid_Data(LayerIndex, 0, 0);
}
gInt16 * Product_501_BREF::GetMyLineData(gInt16 LayerIndex, gInt16 RowIndex)
{
	return GetBlockDataAsGrid_Data(LayerIndex,0, RowIndex);
}

AreaInfo * Product_501_BREF::GetMyAreaInfo()
{
	return GetAreaInfo();
}
MultiDLIBlock * Product_501_BREF::GetMyBlock(gInt16 LayerIndex)
{
	return 	GetMultiBlock(LayerIndex,0);
}
MultiGridHead * Product_501_BREF::GetMyGrid_Head(gInt16 LayerIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,0);
}
MultiGridData * Product_501_BREF::GetMyGrid_Row(gInt16 LayerIndex, gInt32 row)
{
	return GetBlockDataAsGrid_RowData(LayerIndex, 0, row);
}
