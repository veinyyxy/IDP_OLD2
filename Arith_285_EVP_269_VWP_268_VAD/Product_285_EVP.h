////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_285_EVP
//create by yangys 080417
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_285_EVP_H
#define PRODUCT_285_EVP_H
#include "SingleProductBase.h"

class Product_285_EVP : public ProductBase
{
public:
	Product_285_EVP(void);
	~Product_285_EVP(void);

	GHRESULT  ConstructIt( gInt16 NumOfstruct , gInt16 LengthOfstruct );
	PDBlock    *  GetMyPDBlock();
	DILayer * GetMyLayer();
    DLIBlock * GetMyBlock(gInt16 NumofBlock);
	StructHead * GetBlockAsStruct_Head();
	void * GetBlockAsStruct_Data();
	RadarInfor * GetRadarInfor();
};

#endif