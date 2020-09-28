//////////////////////////////////////////////////////////////////////////
// Arith_257_QW.h
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_257_QW_H
#define ARITH_257_QW_H
#include "Product_257_QW.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "IBase.h"
#include "GDef.h"


class ARITH_257_QW: public IArith
{
public:
	ARITH_257_QW();
	~ARITH_257_QW();

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

GHRESULT DLLEXP_GSYS ExecuteArithQW( GXList< GenerationData> *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);


#endif //ARITH_257_QW_H

