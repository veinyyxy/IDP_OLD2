//////////////////////////////////////////////////////////////////////////
// Product_256_W.cpp
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#include "Product_256_W.h"

GHRESULT Product_256_W::ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	GHRESULT TemRet;
	TemRet = FrameAll(NumberOfLayers,MUTI_LAYER,NumberOfLayers,1,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates);
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}
	GetPDBlock()->ProductCode=256;
	return GS_OK;
}

RadarInfor * Product_256_W::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_256_W::GetMyPDBlock()
{
	return GetPDBlock();
}
DILayer * Product_256_W::GetMyLayer(gInt16 NumberOfLayers)
{
	return GetLayer(NumberOfLayers);
}
DLIBlock * Product_256_W::GetMyBlock(gInt16 NumberOfLayers)
{
	return GetBlock(NumberOfLayers,0);
}
RadialHead * Product_256_W::GetMyRadial_Head(gInt16 NumberOfLayers)
{
	return GetBlockDataAsRadial_Head(NumberOfLayers,0);
}
RadialData * Product_256_W::GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(NumberOfLayers,0,RadialIndex);
}
gInt16 * Product_256_W::GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_Data(NumberOfLayers,0,RadialIndex);
}
