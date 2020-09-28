//////////////////////////////////////////////////////////////////////////
// Arith_255_QV.h
// created by ply 20080411
// 254�Ų�Ʒ�������ٶȣ�V
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

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam *m_pParameter;
};

GHRESULT DLLEXP_GSYS ExecuteArithQV( GXList< GenerationData> *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam);

#endif //ARITH_255_QV_H

