//////////////////////////////////////////////////////////////////////////
// Product_257_QW.h
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_257_QW_H
#define PRODUCT_257_QW_H
#include "SingleProductBase.h"

class Product_257_QW : public ProductBase
{
public:
	Product_257_QW();
	~Product_257_QW();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt16 NumberOfLayers);
	DLIBlock * GetMyBlock(gInt16 NumberOfLayers);
	RadialHead * GetMyRadial_Head(gInt16 NumberOfLayers);
	RadialData * GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex);

};

#endif //PRODUCT_257_QW_H
