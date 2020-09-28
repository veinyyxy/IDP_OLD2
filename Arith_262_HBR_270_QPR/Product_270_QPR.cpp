#include "Product_270_QPR.h"

Product_270_QPR::Product_270_QPR()
{
};
Product_270_QPR::~Product_270_QPR()
{
};

GHRESULT Product_270_QPR::ConstructIt(gInt16 width,
                                      gInt16 height)
{
    // ATTENTION: Confirm this call before debugging.
    GHRESULT ret = FrameAll(1, ONCE_LAYER, 1, BLOCKTYPE_GRID, width, height);

    if(ret != GS_OK)
    {
        return ret;
    }

    ProductBase::GetPDBlock()->ProductCode = 270;

    return GS_OK;
};

RadarInfor * Product_270_QPR::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_270_QPR::GetPDBlock()
{
    return ProductBase::GetPDBlock();
}
DILayer * Product_270_QPR::GetLayer()
{
    return ProductBase::GetLayer(0);
}
DLIBlock * Product_270_QPR::GetBlock()
{
  return ProductBase::GetBlock(0, 0);
}

GridHead * Product_270_QPR::GetGrid_Head()
{
    return ProductBase::GetBlockDataAsGrid_Head(0,0);
}
GridRowData * Product_270_QPR::GetGrid_Row(gInt32 row)
{
    return ProductBase::GetBlockDataAsGrid_RowData(0, 0, row);
}
gInt16 * Product_270_QPR::GetGrid_Data(gInt32 row)
{
    return ProductBase::GetBlockDataAsGrid_Data(0, 0, row);
}
