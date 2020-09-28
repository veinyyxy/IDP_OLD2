////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_269_VWP
//create by yangys 080417
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_269_VWP_H
#define PRODUCT_269_VWP_H
#include "SingleProductBase.h"

class Product_269_VWP : public ProductBase
{
public:
	Product_269_VWP(void);
	~Product_269_VWP(void);

	GHRESULT  ConstructIt( gInt16 NumOfstruct , gInt16 LengthOfstruct );
	PDBlock    *  GetMyPDBlock();
	DILayer *GetMyLayer();
    DLIBlock * GetMyBlock(gInt16 NumofBlock);
	StructHead * GetBlockAsStruct_Head();
	void * GetBlockAsStruct_Data();
	RadarInfor * GetRadarInfor();
};

#endif