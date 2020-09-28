////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_258_CAP
//create by yangys 080417
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef Product_258_CAP_H
#define Product_258_CAP_H
#include "SingleProductBase.h"


class Product_258_CAP : public ProductBase
{
public:
	Product_258_CAP(void);
	~Product_258_CAP(void);

	GHRESULT  ConstructIt( gInt16 LayerIndex,gInt16 NumberOfRadials , gInt16 NumberOfGates );
	PDBlock    *  GetMyPDBlock();
	DILayer    *  GetMyDILayer(int i);
	DLIBlock   *  GetMyDILBlock(gInt32 LayerIndex);
	RadialHead *  GetMyRadial_Head(gInt32 LayerIndex);
	RadialData *  GetMyRadial_RadialData( gInt32 LayerIndex,gInt32 RadialIndex );
	gInt16     *  GetMyRadial_Data( gInt32 LayerIndex,gInt32 RadialIndex );
	RadarInfor *  GetRadarInfor();
};

#endif