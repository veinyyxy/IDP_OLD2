//////////////////////////////////////////////////////////////////////////
// Product_256_W.h
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_256_W_H
#define PRODUCT_256_W_H
#include "SingleProductBase.h"

class Product_256_W : public ProductBase
{
public:
	Product_256_W();
	~Product_256_W();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt16 NumberOfLayers);
	DLIBlock * GetMyBlock(gInt16 NumberOfLayers);
	RadialHead * GetMyRadial_Head(gInt16 NumberOfLayers);
	RadialData * GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex);

};

#endif //PRODUCT_256_W_H
