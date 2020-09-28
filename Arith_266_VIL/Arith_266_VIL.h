//////////////////////////////////////////////////////
//Arith_266_VIL.h  
//created by qcsun 20080402
//
//
//���㷨
//����
//StandardRadarData   * pGData��
//gInt16    EchoTopRefThreshold ; ����VIL��VILDʱ��ȡ�Ļز����ķ�����������ֵ����λdBZ,ȱʡֵΪ18dBZ
//gFloat   BeamWidth;  �״ﲨ����ȣ���λ0.01�ȣ�����95��ʾ0.95�ȣ���CINRAD-SA�״�Ĳ������Ϊ0.95��
//
//����� ��ֱҺ̬ˮ��������λ0.01kg/m2
//
//
///////////////////////////////////////////////////////
#pragma once

#ifndef		Arith_266_VIL_H
#define		Arith_266_VIL_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
//#include "PRM.h"
//#include <math.h>
#include "Product_266_VIL.h"
//#include <stdio.h>
#include "IBase.h"

class Arith_266_VIL : public IArith
{
public:
	Arith_266_VIL(void);
	~Arith_266_VIL(void);
	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��

private:
	void  pArith_266_VIL(StandardRadarData *pGData , gInt16 EchoTopRefThreshold , gFloat BeamWidth , Product_266_VIL * pRadialData,  gInt16 MaxRefGateNumer ) ;//��ֱҺ̬ˮ������Ʒ���ɺ���

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam * m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
};

#endif
