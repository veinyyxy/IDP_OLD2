//////////////////////////////////////////////////////////////////////////
// Product_257_QW.cpp
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#include "Product_257_QW.h"

GHRESULT Product_257_QW::ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	GHRESULT TemRet;
	TemRet = FrameAll(NumberOfLayers,MUTI_LAYER,NumberOfLayers,1,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates);
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}
	GetPDBlock()->ProductCode=257;
	return GS_OK;
}

RadarInfor * Product_257_QW::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_257_QW::GetMyPDBlock()
{
	return GetPDBlock();
}
DILayer * Product_257_QW::GetMyLayer(gInt16 NumberOfLayers)
{
	return GetLayer(NumberOfLayers);
}
DLIBlock * Product_257_QW::GetMyBlock(gInt16 NumberOfLayers)
{
	return GetBlock(NumberOfLayers,0);
}
RadialHead * Product_257_QW::GetMyRadial_Head(gInt16 NumberOfLayers)
{
	return GetBlockDataAsRadial_Head(NumberOfLayers,0);
}
RadialData * Product_257_QW::GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(NumberOfLayers,0,RadialIndex);
}
gInt16 * Product_257_QW::GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_Data(NumberOfLayers,0,RadialIndex);
}
