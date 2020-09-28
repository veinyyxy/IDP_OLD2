//////////////////////////////////////////////////////////////////////////
// Arith_255_QV.h
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_255_QV_H
#define ARITH_255_QV_H
#include "Product_255_QV.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "IBase.h"
#include "GDef.h"


class ARITH_255_QV: public IArith
{
public:
	ARITH_255_QV();
	~ARITH_255_QV();

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam *m_pParameter;
};

GHRESULT DLLEXP_GSYS ExecuteArithQV( GXList< GenerationData> *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);

#endif //ARITH_255_QV_H

