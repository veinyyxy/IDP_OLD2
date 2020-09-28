//////////////////////////////////////////////////////
//Product_266_VIL.h  
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#pragma once

#ifndef  PRODUCT_266_VIL_H
#define  PRODUCT_266_VIL_H

#include "SingleProductBase.h"

class Product_266_VIL : public  ProductBase
{
public:
	Product_266_VIL(void);
	~Product_266_VIL(void);

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
