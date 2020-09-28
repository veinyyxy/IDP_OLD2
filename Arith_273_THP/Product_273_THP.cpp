#include "Product_273_THP.hpp"

Product_273_THP::Product_273_THP()
{
};
Product_273_THP::~Product_273_THP()
{
};

GHRESULT Product_273_THP::ConstructIt(gInt16 width,
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

PDBlock * Product_273_THP::GetPDBlock()
{
	return ProductBase::GetPDBlock();
}
DILayer * Product_273_THP::GetLayer()
{
	return ProductBase::GetLayer(0);
}
DLIBlock * Product_273_THP::GetBlock()
{
	return ProductBase::GetBlock(0, 0);
}

GridHead * Product_273_THP::GetGrid_Head()
{
	return ProductBase::GetBlockDataAsGrid_Head(0,0);
}
GridRowData * Product_273_THP::GetGrid_Row(gInt32 row)
{
	return ProductBase::GetBlockDataAsGrid_RowData(0, 0, row);
}
gInt16 * Product_273_THP::GetGrid_Data(gInt32 row)
{
	return ProductBase::GetBlockDataAsGrid_Data(0, 0, row);
}
RadarInfor * Product_273_THP::GetRadarInfor()
{
	return GetRadarInfo();
}