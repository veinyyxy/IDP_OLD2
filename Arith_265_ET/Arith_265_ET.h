//////////////////////////////////////////////////////
//Arith_265_ET.h  
//created by qcsun 20080401
//
//���㷨
//���룺
//StandardRadarData * pGData��//
//gInt16   EchoTipRefThreshold;//�ز����߶�Ӧ�ķ�����������ֵ����λdBZ,ȱʡֵΪ18dBZ
//gFloat BeamWidth;�״ﲨ����ȣ���λ0.01�ȣ�����95��ʾ0.95�ȣ���CINRAD-SA�״�Ĳ������Ϊ0.95��
//
//�����//�ز��� ����λ0.01km
//
//
///////////////////////////////////////////////////////

#pragma once

#ifndef		Arith_265_ET_H
#define		Arith_265_ET_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "Product_265_ET.h"

class Arith_265_ET: public IArith
{
public:
	Arith_265_ET(void);
	~Arith_265_ET(void);

	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam *ReadParameter) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��
	
private:
	
	void pArith_265_ET( StandardRadarData * pGData,gInt16  EchoTopRefThreshold,Product_265_ET * pRadialData, gInt16 MaxRefGateNumer);//�ز����߲�Ʒ���ɺ���

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam *m_pParameter ;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
};

#endif
