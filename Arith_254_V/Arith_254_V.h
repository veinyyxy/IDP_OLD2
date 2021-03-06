//////////////////////////////////////////////////////////////////////////
// Arith_254_V.h
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_254_V_H
#define ARITH_254_V_H
#include "Product_254_V.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "GDef.h"
#include "IBase.h"


class ARITH_254_V: public IArith
{
public:
	ARITH_254_V();
	~ARITH_254_V();

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

#endif //ARITH_254_V_H

