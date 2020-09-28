#include "Product_262_HBR.h"

Product_262_HBR::Product_262_HBR()
{
};
Product_262_HBR::~Product_262_HBR()
{
};

GHRESULT Product_262_HBR::ConstructIt(gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
    // ATTENTION: Confirm this call before debugging.
  GHRESULT ret = FrameAll(1, ONCE_LAYER, 1, BLOCKTYPE_RADIAL, NumberOfRadials, NumberOfGates);

    if(ret != GS_OK)
    {
        return ret;
    }

    ProductBase::GetPDBlock()->ProductCode = 262;

    return GS_OK;
};

RadarInfor * Product_262_HBR::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_262_HBR::GetPDBlock()
{
    return ProductBase::GetPDBlock();
}
DILayer * Product_262_HBR::GetLayer()
{
    return ProductBase::GetLayer(0);
}
DLIBlock * Product_262_HBR::GetBlock()
{
  return ProductBase::GetBlock(0, 0);
}

RadialHead * Product_262_HBR::GetRadial_Head()
{
    return ProductBase::GetBlockDataAsRadial_Head(0,0);
}
RadialData * Product_262_HBR::GetRadial_RadialData(gInt32 RadialIndex)
{
  return ProductBase::GetBlockDataAsRadial_RadialData(0, 0, RadialIndex);
}
gInt16 * Product_262_HBR::GetRadial_Data(gInt32 RadialIndex)
{
  return ProductBase::GetBlockDataAsRadial_Data(0, 0, RadialIndex);
}
