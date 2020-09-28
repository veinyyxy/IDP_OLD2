//////////////////////////////////////////////////////////////////////////
// Arith_257_QW.h
// created by ply 20080411
// 256�Ų�Ʒ�������׿�W
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

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

GHRESULT DLLEXP_GSYS ExecuteArithQW( GXList< GenerationData> *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);


#endif //ARITH_257_QW_H

