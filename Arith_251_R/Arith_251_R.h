//////////////////////////////////////////////////////////////////////////
// Arith_251_R.h
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_251_R_H
#define ARITH_251_R_H

#include <math.h>

#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"

#include "Product_251_R.h"


class ARITH_251_R: public IArith
{
public:
	ARITH_251_R();
	~ARITH_251_R();

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData *> * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

#endif //ARITH_251_R_H

