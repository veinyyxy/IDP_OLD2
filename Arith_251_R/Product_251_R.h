//////////////////////////////////////////////////////////////////////////
// Product_251_R.h
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_251_R_H
#define PRODUCT_251_R_H

#include "../GDataTypes/SingleProductBase.h"

class Product_251_R : public ProductBase
{
public:
	Product_251_R();
	~Product_251_R();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt32 LayerIndex);
	DLIBlock * GetMyBlock(gInt32 LayerIndex);
	RadialHead * GetMyRadial_Head(gInt32 LayerIndex);
	RadialData * GetMyRadial_RadialData(gInt32 LayerIndex,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt32 LayerIndex,gInt32 RadialIndex);
};

#endif //PRODUCT_251_R_H