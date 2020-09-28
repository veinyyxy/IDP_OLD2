//////////////////////////////////////////////////////
//Arith_263_CR.h  
//created by qcsun 20080403
//
//���㷨
//���룺
//StandardRadarData * pGData��//
//
//�����;//������� ����λ0.01km
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

	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam* ReadParameter) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��

private:

	void pArith_263_CR( StandardRadarData * pGData , Product_263_CR * pRadialData, gInt16 maxRefgatenum);//��������������ɺ���

private:
	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam* m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
};

#endif