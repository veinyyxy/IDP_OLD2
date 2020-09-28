//////////////////////////////////////////////////////
//Product_291_HI.h  
//created by zsc 20081124
///////////////////////////////////////////////////////

#pragma once 

#ifndef PRODUCT_291_HI
#define PRODUCT_291_HI

#include "SingleProductBase.h"

class Product_291_HI : public ProductBase
{
public:
	Product_291_HI(void);
	~Product_291_HI(void);

	GHRESULT  ConstructIt( gInt16 NumOfstruct1,gInt16 LengthOfStruct1,gInt16 NumOfstruct2,gInt16  LengthOfStruct2 );
	PDBlock    *  GetMyPDBlock();
	DILayer * GetMyLayer();
	DLIBlock * GetMyBlock(gInt16 NumofBlock);
	StructHead * GetBlockAsStruct_Head(gInt16 NumofBlock);
	void * GetBlockAsStruct_Data(gInt16 NumofBlock);
	RadarInfor * GetRadarInfor();
};
#endif