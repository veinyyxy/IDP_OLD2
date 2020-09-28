//////////////////////////////////////////////////////
//Product_264_CRH.h  
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#pragma once

#ifndef  PRODUCT_264_CRH_H
#define  PRODUCT_264_CRH_H

#include "SingleProductBase.h"

class Product_264_CRH : public  ProductBase
{
public:
	Product_264_CRH(void);
	~Product_264_CRH(void);

	GHRESULT  ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates );
	PDBlock    *  GetMyPDBlock();
	DILayer    *  GetMyDILayer();
	DLIBlock   *  GetMyDILBlock();
	RadialHead *  GetMyRadial_Head();
	RadialData *  GetMyRadial_RadialData( gInt32 RadialIndex );
	gInt16     *  GetMyRadial_Data( gInt32 RadialIndex );	
	RadarInfor *  GetRadarInfor();
};

#endif;
