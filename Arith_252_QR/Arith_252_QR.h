//////////////////////////////////////////////////////////////////////////
// Arith_252_QR.h
// created by ply 20080402
// 251�Ų�Ʒ�����������ʣ�R
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

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};


#endif //ARITH_252_QR_H

