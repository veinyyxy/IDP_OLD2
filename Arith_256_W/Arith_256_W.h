//////////////////////////////////////////////////////////////////////////
// Arith_256_W.h
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_256_W_H
#define ARITH_256_W_H
#include "Product_256_W.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include <math.h>
#include "GDef.h"
#include "IBase.h"


class ARITH_256_W: public IArith
{
public:
	ARITH_256_W();
	~ARITH_256_W();

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

#endif //ARITH_256_W_H

