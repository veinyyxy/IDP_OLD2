#include "Product_271_OHP.hpp"

CommonProduct::CommonProduct()
{
};

CommonProduct::~CommonProduct()
{
};

PDBlock* CommonProduct::GetPDBlockP()
{
  return ProductBase::GetPDBlockP();
};

gInt16* CommonProduct::GetGridDataP()
{
    return ProductBase::GetBlockDataAsGrid_Data(0,0,0);
};

Product_271_OHP::Product_271_OHP()
{
};
Product_271_OHP::~Product_271_OHP()
{
};

GHRESULT Product_271_OHP::ConstructIt(gInt16 width,
                                      gInt16 height)
{
    // ATTENTION: Confirm this call before debugging.
    GHRESULT ret = FrameAll(1, ONCE_LAYER, 1, BLOCKTYPE_GRID, width, height);

    if(ret != GS_OK)
    {
        return ret;
    }

    ProductBase::GetPDBlock()->ProductCode = 271;//edited by ply. 20080611

    return GS_OK;
};

PDBlock * Product_271_OHP::GetPDBlock()
{
    return ProductBase::GetPDBlock();
}
DILayer * Product_271_OHP::GetLayer()
{
    return ProductBase::GetLayer(0);
}
DLIBlock * Product_271_OHP::GetBlock()
{
  return ProductBase::GetBlock(0, 0);
}

GridHead * Product_271_OHP::GetGrid_Head()
{
    return ProductBase::GetBlockDataAsGrid_Head(0,0);
}
GridRowData * Product_271_OHP::GetGrid_Row(gInt32 row)
{
    return ProductBase::GetBlockDataAsGrid_RowData(0, 0, row);
}
gInt16 * Product_271_OHP::GetGrid_Data(gInt32 row)
{
    return ProductBase::GetBlockDataAsGrid_Data(0, 0, row);
}
RadarInfor * Product_271_OHP::GetRadarInfor()
{
	return GetRadarInfo();
}