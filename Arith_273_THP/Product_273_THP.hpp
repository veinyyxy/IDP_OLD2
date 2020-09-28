#ifndef _PRODUCT_273_THP_HPP_
#define _PRODUCT_273_THP_HPP_

#include "SingleProductBase.h"

class Product_273_THP : public ProductBase
{
public:
	Product_273_THP();
	~Product_273_THP();

	GHRESULT ConstructIt(gInt16 width,
		gInt16 height);

	PDBlock    *  GetPDBlock();
	DILayer    *  GetLayer();
	DLIBlock   *  GetBlock();

	GridHead *  GetGrid_Head();

	GridRowData *  GetGrid_Row(gInt32 row);

	gInt16     *  GetGrid_Data(gInt32 row);

	RadarInfor * GetRadarInfor();
};


#endif