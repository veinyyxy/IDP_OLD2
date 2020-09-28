//////////////////////////////////////////////////////
//Product_263_CR.h  
//created by qcsun 20080408
//
///////////////////////////////////////////////////////

#pragma once

#ifndef  PRODUCT_263_CR_H
#define  PRODUCT_263_CR_H

#include "SingleProductBase.h"

class Product_263_CR : public  ProductBase
{
public:
	Product_263_CR(void);
	~Product_263_CR(void);
	
	GHRESULT  ConstructIt( gInt16 NumberOfRadials , gInt16 NumberOfGates );
	RadarInfor * GetRadarInfor();
	PDBlock    *  GetMyPDBlock();
	DILayer    *  GetMyDILayer();
	DLIBlock   *  GetMyDILBlock();
	RadialHead *  GetMyRadial_Head();
	RadialData *  GetMyRadial_RadialData( gInt32 RadialIndex );
	gInt16     *  GetMyRadial_Data( gInt32 RadialIndex );

};
#endif;