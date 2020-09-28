//////////////////////////////////////////////////////
//Arith_263_CR.h  
//created by qcsun 20080403
//
//该算法
//输入：
//StandardRadarData * pGData；//
//
//输出：;//最大反射率 ，单位0.01km
//
//
///////////////////////////////////////////////////////

#pragma once

#ifndef		Arith_263_CR_H
#define		Arith_263_CR_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include <math.h>
#include "Product_263_CR.h"

class Arith_263_CR: public IArith
{
public:
	Arith_263_CR(void);
	~Arith_263_CR(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam* ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//取得结果大小以便申请内存
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:

	void pArith_263_CR( StandardRadarData * pGData , Product_263_CR * pRadialData, gInt16 maxRefgatenum);//最大反射率因子生成函数

private:
	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam* m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
};

#endif