#ifndef _PRODUCT_270_QPR_H_
#define _PRODUCT_270_QPR_H_

#include "SingleProductBase.h"

class Product_270_QPR : public ProductBase
{
public:
  Product_270_QPR();
  ~Product_270_QPR();

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
