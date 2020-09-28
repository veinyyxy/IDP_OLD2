//////////////////////////////////////////////////////
//Arith_264_CRH.h  
//created by qcsun 20080403
//
//���㷨
//���룺
//StandardRadarData * pGData��//
//
//�����//������ʸ߶� ����λ0.01km
//
//
///////////////////////////////////////////////////////

#pragma once

#ifndef		Arith_264_CRH_H
#define		Arith_264_CRH_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
//#include "PRM.h"
#include "Product_264_CRH.h"
#include "IBase.h"
class Arith_264_CRH: public IArith
{
public:
	Arith_264_CRH(void);
	~Arith_264_CRH(void);

	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue, ReadParam * ReadParameter) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��

private:

	void pArith_264_CRH(StandardRadarData * pGData , Product_264_CRH * pRadialData, gInt16 CRThreshold, gInt16 MaxRefGateNumer );//��������������ɺ���

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam *m_pParameter ;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
};

#endif
