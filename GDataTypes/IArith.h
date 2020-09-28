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

	SYSMETHOD(Initialize)() = 0;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) = 0;//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue) = 0;//�������������ݵ�pValue
	SYSMETHOD(Execute) () = 0;//ִ���㷨
	SYSMETHOD(UnInitialize)	() = 0;//����ʼ��

	char strName[20];//�㷨������
};

#endif
