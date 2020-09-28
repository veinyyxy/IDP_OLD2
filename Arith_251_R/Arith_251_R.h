//////////////////////////////////////////////////////////////////////////
// Arith_251_R.h
// created by ply 20080402
// 251�Ų�Ʒ�����������ʣ�R
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

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData *> * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

#endif //ARITH_251_R_H

