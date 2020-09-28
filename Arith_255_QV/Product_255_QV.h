//////////////////////////////////////////////////////////////////////////
// Product_255_QV.h
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_255_QV_H
#define PRODUCT_255_QV_H
#include "SingleProductBase.h"

class Product_255_QV : public ProductBase
{
public:
	Product_255_QV();
	~Product_255_QV();

	GHRESULT ConstructIt(gInt16 NumberOfLayers,gInt16 NumberOfRadials,gInt16 NumberOfGates);
	RadarInfor * GetRadarInfor();
	PDBlock * GetMyPDBlock();
	DILayer * GetMyLayer(gInt16 NumberOfLayers);
	DLIBlock * GetMyBlock(gInt16 NumberOfLayers);
	RadialHead * GetMyRadial_Head(gInt16 NumberOfLayers);
	RadialData * GetMyRadial_RadialData(gInt16 NumberOfLayers,gInt32 RadialIndex);
	gInt16 * GetMyRadial_Data(gInt16 NumberOfLayers,gInt32 RadialIndex);

};

#endif //PRODUCT_255_QV_H

