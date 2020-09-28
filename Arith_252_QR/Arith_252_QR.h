//////////////////////////////////////////////////////////////////////////
// Arith_252_QR.h
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_252_QR_H
#define ARITH_252_QR_H
#include "Product_252_QR.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "IBase.h"


class ARITH_252_QR: public IArith
{
public:
	ARITH_252_QR();
	~ARITH_252_QR();

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};


#endif //ARITH_252_QR_H

