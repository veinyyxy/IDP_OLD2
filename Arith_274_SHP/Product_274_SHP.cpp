#include "Product_274_SHP.hpp"

Product_274_SHP::Product_274_SHP()
{
};
Product_274_SHP::~Product_274_SHP()
{
};

GHRESULT Product_274_SHP::ConstructIt(gInt16 width,
									  gInt16 height)
{
	// ATTENTION: Confirm this call before debugging.
	GHRESULT ret = FrameAll(1, ONCE_LAYER, 1, BLOCKTYPE_GRID, width, height);

	if(ret != GS_OK)
	{
		return ret;
	}

	ProductBase::GetPDBlock()->ProductCode = 273;

	return GS_OK;
};

PDBlock * Product_274_SHP::GetPDBlock()
{
	return ProductBase::GetPDBlock();
}
DILayer * Product_274_SHP::GetLayer()
{
	return ProductBase::GetLayer(0);
}
DLIBlock * Product_274_SHP::GetBlock()
{
	return ProductBase::GetBlock(0, 0);
}

GridHead * Product_274_SHP::GetGrid_Head()
{
	return ProductBase::GetBlockDataAsGrid_Head(0,0);
}
GridRowData * Product_274_SHP::GetGrid_Row(gInt32 row)
{
	return ProductBase::GetBlockDataAsGrid_RowData(0, 0, row);
}
gInt16 * Product_274_SHP::GetGrid_Data(gInt32 row)
{
	return ProductBase::GetBlockDataAsGrid_Data(0, 0, row);
}
RadarInfor * Product_274_SHP::GetRadarInfor()
{
	return GetRadarInfo();
}