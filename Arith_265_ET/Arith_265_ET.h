//////////////////////////////////////////////////////
//Arith_265_ET.h  
//created by qcsun 20080401
//
//该算法
//输入：
//StandardRadarData * pGData；//
//gInt16   EchoTipRefThreshold;//回波顶高对应的反射率因子阈值，单位dBZ,缺省值为18dBZ
//gFloat BeamWidth;雷达波束宽度，单位0.01度（例如95表示0.95度），CINRAD-SA雷达的波束宽度为0.95度
//
//输出：//回波顶 ，单位0.01km
//
//
///////////////////////////////////////////////////////

#pragma once

#ifndef		Arith_265_ET_H
#define		Arith_265_ET_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "Product_265_ET.h"

class Arith_265_ET: public IArith
{
public:
	Arith_265_ET(void);
	~Arith_265_ET(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam *ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//取得结果大小以便申请内存
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化
	
private:
	
	void pArith_265_ET( StandardRadarData * pGData,gInt16  EchoTopRefThreshold,Product_265_ET * pRadialData, gInt16 MaxRefGateNumer);//回波顶高产品生成函数

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam *m_pParameter ;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
};

#endif
