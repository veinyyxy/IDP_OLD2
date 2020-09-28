////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_277_SCI
//create by yangys 080417
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRODUCT_277_SCI_H
#define PRODUCT_277_SCI_H

#include "GDef.h"
#include "SingleProductBase.h"

class /*DLLEXP_GSYS*/ Product_277_SCI : public ProductBase
{
public:
	Product_277_SCI(void);
	~Product_277_SCI(void);

	GHRESULT  ConstructIt( gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2 );
	PDBlock    *  GetMyPDBlock();
	DILayer * GetMyLayer();
    DLIBlock * GetMyBlock(gInt16 NumofBlock);
    StructHead * GetBlockAsStruct_Head(gInt16 NumofBlock);
	void * GetBlockAsStruct_Data(gInt16 NumofBlock);	
	RadarInfor * GetRadarInfor();
};

#endif