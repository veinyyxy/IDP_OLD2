//////////////////////////////////////////////////////////////////////////
// Arith_256_W.h
// created by ply 20080411
// 256�Ų�Ʒ�������׿�W
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

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam *ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
};

#endif //ARITH_256_W_H

