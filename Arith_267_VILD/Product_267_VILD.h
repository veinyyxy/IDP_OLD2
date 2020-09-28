//////////////////////////////////////////////////////
//Product_267_VILD.h  
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#pragma once

#ifndef  Product_267_VILD_H
#define  PRODUCT_267_VILD_H

#include "SingleProductBase.h"

class Product_267_VILD : public  ProductBase
{
public:
	Product_267_VILD(void);
	~Product_267_VILD(void);

	GHRESULT  ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates );
	PDBlock    *  GetMyPDBlock();
	DILayer    *  GetMyDILayer();
	DLIBlock   *  GetMyDILBlock();
	RadialHead *  GetMyRadial_Head();
	RadialData *  GetMyRadial_RadialData( gInt32 RadialIndex );
	gInt16     *  GetMyRadial_Data( gInt32 RadialIndex );
	RadarInfor * GetRadarInfor();
};

#endif;
