//////////////////////////////////////////////////////////////////////////
// Product_254_V.h
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_254_V_H
#define PRODUCT_254_V_H
#include "SingleProductBase.h"

class Product_254_V : public ProductBase
{
public:
	Product_254_V();
	~Product_254_V();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt16 NumberOfLayers);
	DLIBlock * GetMyBlock(gInt16 NumberOfLayers);
	RadialHead * GetMyRadial_Head(gInt16 NumberOfLayers);
	RadialData * GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex);

};

#endif //PRODUCT_254_V_H

