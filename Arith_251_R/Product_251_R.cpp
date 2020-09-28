//////////////////////////////////////////////////////////////////////////
// Product_251_R.cpp
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#include "Product_251_R.h"

GHRESULT Product_251_R::ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	GHRESULT TemRet;
	TemRet = FrameAll(NumberOfLayers,MUTI_LAYER,NumberOfLayers,1,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates);
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}
	GetPDBlock()->ProductCode=251;
	return GS_OK;
}

RadarInfor * Product_251_R::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_251_R::GetMyPDBlock()
{
	return GetPDBlock();
}
DILayer * Product_251_R::GetMyLayer(gInt32 LayerIndex)
{
	return GetLayer(LayerIndex);
}
DLIBlock * Product_251_R::GetMyBlock(gInt32 LayerIndex)
{
	return GetBlock(LayerIndex,0);
}
RadialHead * Product_251_R::GetMyRadial_Head(gInt32 LayerIndex)
{
	return GetBlockDataAsRadial_Head(LayerIndex,0);
}
RadialData * Product_251_R::GetMyRadial_RadialData(gInt32 LayerIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(LayerIndex,0,RadialIndex);
}
gInt16 * Product_251_R::GetMyRadial_Data(gInt32 LayerIndex,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_Data(LayerIndex,0,RadialIndex);
}
