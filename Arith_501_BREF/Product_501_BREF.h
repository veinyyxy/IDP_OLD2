////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Product_501_BREF.h
//created by ChenXiang 20080429
//501号产品,组网算法,BREF
////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef PRODUCT_501_BREF_H
#define PRODUCT_501_BREF_H

#pragma pack(push,1)

//#include "SingleProductBase.h"
//#include "DataTypes/StandardRadarData.h"
#include "MultiProductBase.h"

class Product_501_BREF : public MultiProductBase
{
public:
	Product_501_BREF(void);
	~Product_501_BREF(void);

	//write functions
	//GHRESULT		Create();
	GHRESULT Coustruct( gInt16 SiteNum,gInt16 NumberOfRows,gInt16 NumberOfCols,gInt16 LayerRepickTimes);
	GHRESULT		AddMySite(const SiteInf & SiteToAdd);//suggest add site before any layer add.
	//GHRESULT		AddLayer(gInt16	NumberOfRows,gInt16	NumberOfCols,gInt16 LayerRepickTimes=1);//suggest add layer after all sites added.
	GHRESULT		AddMyLayer(gInt16 NumberOfRows,gInt16 NumberOfCols,gInt16 LayerRepickTimes=1);
	//write/read functions
	
	MultiDataHead	* GetMyMultiHead();
	SiteInf			* GetMySite(gInt16 RadarIndex);
	MultiLayerHead	* GetMyLayer(gInt16 LayerIndex);
	gInt16			* GetMyLayerData(gInt16 LayerIndex);
	gInt16			* GetMyLineData(gInt16 LayerIndex,gInt16 RowIndex);

	AreaInfo * GetMyAreaInfo();
	MultiDLIBlock * GetMyBlock(gInt16 LayerIndex);
	MultiGridHead * GetMyGrid_Head(gInt16 LayerIndex);
	MultiGridData * GetMyGrid_Row(gInt16 LayerIndex, gInt32 row);
};

#pragma pack(pop)
#endif //PRODUCT_501_BREF_H
