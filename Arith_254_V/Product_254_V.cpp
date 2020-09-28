//////////////////////////////////////////////////////////////////////////
// Product_254_V.cpp
// created by ply 20080411
// 254号产品，基本谱宽，V
//////////////////////////////////////////////////////////////////////////
#include "Product_254_V.h"

GHRESULT Product_254_V::ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	GHRESULT TemRet;
	TemRet = FrameAll(NumberOfLayers,MUTI_LAYER,NumberOfLayers,1,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates);
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}
	GetPDBlock()->ProductCode=254;
	return GS_OK;
}

RadarInfor * Product_254_V::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_254_V::GetMyPDBlock()
{
	return GetPDBlock();
}
DILayer * Product_254_V::GetMyLayer(gInt16 NumberOfLayers)
{
	return GetLayer(NumberOfLayers);
}
DLIBlock * Product_254_V::GetMyBlock(gInt16 NumberOfLayers)
{
	return GetBlock(NumberOfLayers,0);
}
RadialHead * Product_254_V::GetMyRadial_Head(gInt16 NumberOfLayers)
{
	return GetBlockDataAsRadial_Head(NumberOfLayers,0);
}
RadialData * Product_254_V::GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(NumberOfLayers,0,RadialIndex);
}
gInt16 * Product_254_V::GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_Data(NumberOfLayers,0,RadialIndex);
}
