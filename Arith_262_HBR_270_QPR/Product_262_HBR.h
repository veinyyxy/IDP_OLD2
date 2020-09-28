#ifndef _PRODUCT_262_HBR_H_
#define _PRODUCT_262_HBR_H_

#include "SingleProductBase.h"

class Product_262_HBR : public ProductBase
{
public:
    Product_262_HBR();
    ~Product_262_HBR();

    GHRESULT ConstructIt(gInt16 rardialCnt, gInt16 gateCnt);

    PDBlock    *    GetPDBlock();
    DILayer    *    GetLayer();
    DLIBlock   *    GetBlock();

    RadialHead *      GetRadial_Head();

    RadialData *   GetRadial_RadialData(gInt32 RadialIndex);

    gInt16 *        GetRadial_Data(gInt32 RadialIndex);

	RadarInfor * GetRadarInfor();
};

#endif
