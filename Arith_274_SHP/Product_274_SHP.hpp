#ifndef _PRODUCT_274_SHP_HPP_
#define _PRODUCT_274_SHP_HPP_

#include "SingleProductBase.h"

class Product_274_SHP : public ProductBase
{
public:
	Product_274_SHP();
	~Product_274_SHP();

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