////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_268_VAD
//create by zsc 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_268_VAD_H
#define PRODUCT_268_VAD_H
#include "SingleProductBase.h"

class Product_268_VAD : public ProductBase
{
public:
	Product_268_VAD(void);
	~Product_268_VAD(void);

	GHRESULT  ConstructIt( gInt16 layerNum,gInt16 NumOfstruct , gInt16 LengthOfstruct );
	PDBlock    *  GetMyPDBlock();
	DILayer *GetMyLayer(gInt16 layerNum);
	DLIBlock * GetMyBlock(gInt16 layerNum,gInt16 NumofBlock);
	StructHead * GetBlockAsStruct_Head(gInt16 layerNum);
	void * GetBlockAsStruct_Data(gInt16 layerNum);
	RadarInfor * GetRadarInfor();
};

#endif