#pragma once
#ifndef  PRODUCT_292_M_H
#define  PRODUCT_292_M_H

#include "singleproductbase.h"

class Product_292_M :
	public ProductBase
{
public:
	Product_292_M(void);
	~Product_292_M(void);

	GHRESULT  ConstructIt(gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2, 
		gInt16 NumOfstruct3,gInt16 LengthOfStruct3/*,gInt16 NumOfstruct4,gInt16  LengthOfStruct4*/);
	PDBlock    *  GetMyPDBlock();
	DILayer * GetMyLayer();
	DLIBlock * GetMyBlock(gInt16 BlockIndex);
	StructHead * GetMyBlockDataAsStruct_Head(gInt32 BlockIndex);
	void * GetBlockAsStruct_Data(gInt32 BlockIndex);
	RadarInfor * GetRadarInfor();
};

#endif
