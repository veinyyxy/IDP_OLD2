//////////////////////////////////////////////////////////////////////////
// IArith.h
// created by xm-zhang 20080311
//////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef IARITH_H
#define	IARITH_H

#include "GDef.h"
#include "IBase.h"
class ReadParam
{
public:
	virtual GHRESULT LoadParamFunc(char * pKey,char * & pValue)=0;
	inline GHRESULT  operator () (char * pKey,char * & pValue){return LoadParamFunc(pKey,pValue);};
};

class DLLEXP_GSYS  IArith
{

public:

	IArith(){};
	virtual ~IArith(){};

	SYSMETHOD(Initialize)() = 0;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) = 0;//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue) = 0;//设置输出结果数据到pValue
	SYSMETHOD(Execute) () = 0;//执行算法
	SYSMETHOD(UnInitialize)	() = 0;//反初始化

	char strName[20];//算法的名字
};

#endif
