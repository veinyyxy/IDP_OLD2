//////////////////////////////////////////////////////////////////////////
// Arith_254_V.h
// created by ply 20080411
// 254�Ų�Ʒ�������ٶȣ�V
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

#endif //ARITH_254_V_H

