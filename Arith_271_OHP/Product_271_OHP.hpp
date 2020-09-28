#ifndef _PRODUCT_271_OHP_HPP_
#define _PRODUCT_271_OHP_HPP_

#include "SingleProductBase.h"

class CommonProduct : public ProductBase
{
public:
    CommonProduct();
    ~CommonProduct();

    PDBlock* GetPDBlockP();
    gInt16*  GetGridDataP();
};

class Product_271_OHP : public ProductBase
{
public:
  Product_271_OHP();
  ~Product_271_OHP();

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
