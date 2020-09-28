//////////////////////////////////////////////////////
//Product_265_ET.h  
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#pragma once

#ifndef  PRODUCT_265_ET_H
#define  PRODUCT_265_ET_H

#include "SingleProductBase.h"

class Product_265_ET : public  ProductBase
{
public:
	Product_265_ET(void);
	~Product_265_ET(void);

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
