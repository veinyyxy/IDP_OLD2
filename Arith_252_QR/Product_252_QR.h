//////////////////////////////////////////////////////////////////////////
// Product_252_QR.h
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_252_QR_H
#define PRODUCT_252_QR_H
#include "SingleProductBase.h"

class Product_252_QR : public ProductBase
{
public:
	Product_252_QR();
	~Product_252_QR();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt32 LayerIndex);
	DLIBlock * GetMyBlock(gInt32 LayerIndex);
	RadialHead * GetMyRadial_Head(gInt32 LayerIndex);
	RadialData * GetMyRadial_RadialData(gInt32 LayerIndex,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt32 LayerIndex,gInt32 RadialIndex);
};

#endif //PRODUCT_252_QR_H
