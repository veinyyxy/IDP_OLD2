//////////////////////////////////////////////////////////////////////////
// Product_255_QV.cpp
// created by ply 20080411
// 254号产品，基本谱宽，V
//////////////////////////////////////////////////////////////////////////
#include "Product_255_QV.h"

GHRESULT Product_255_QV::ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	GHRESULT TemRet;
	TemRet = FrameAll(NumberOfLayers,MUTI_LAYER,NumberOfLayers,1,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates);
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}
	GetPDBlock()->ProductCode=255;
	return GS_OK;
}

RadarInfor * Product_255_QV::GetRadarInfor()
{
	return GetRadarInfo();
}

PDBlock * Product_255_QV::GetMyPDBlock()
{
	return GetPDBlock();
}
DILayer * Product_255_QV::GetMyLayer(gInt16 NumberOfLayers)
{
	return GetLayer(NumberOfLayers);
}
DLIBlock * Product_255_QV::GetMyBlock(gInt16 NumberOfLayers)
{
	return GetBlock(NumberOfLayers,0);
}
RadialHead * Product_255_QV::GetMyRadial_Head(gInt16 NumberOfLayers)
{
	return GetBlockDataAsRadial_Head(NumberOfLayers,0);
}
RadialData * Product_255_QV::GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_RadialData(NumberOfLayers,0,RadialIndex);
}
gInt16 * Product_255_QV::GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex)
{
	return GetBlockDataAsRadial_Data(NumberOfLayers,0,RadialIndex);
}
