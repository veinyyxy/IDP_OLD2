//////////////////////////////////////////////////////
//Arith_267_VILD.h  
//created by qcsun 20080403
//
//
//���㷨
//����
//StandardRadarData   * pGData��
//gInt16   EchoTopRefThreshold ; ����VIL��VILDʱ��ȡ�Ļز����ķ�����������ֵ����λdBZ,ȱʡֵΪ18dBZ
//gFloat   BeamWidth;  �״ﲨ����ȣ���λ0.01�ȣ�����95��ʾ0.95�ȣ���CINRAD-SA�״�Ĳ������Ϊ0.95��
//
//�������ֱҺ̬ˮ�����ܶ� ����λ0.01kg/m2
//
//
///////////////////////////////////////////////////////
#pragma once

#ifndef		Arith_267_VILD_H
#define		Arith_267_VILD_H

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "Product_267_VILD.h"
class Arith_267_VILD : public IArith
{
public:
	Arith_267_VILD(void);
	~Arith_267_VILD(void);
	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��

private:
	void  pArith_267_VILD(StandardRadarData *pGData , gInt16 EchoTopRefThreshold , Product_267_VILD * pRadialData, gInt16 MaxRefGateNumer ) ;//��ֱҺ̬ˮ�����ܶȲ�Ʒ���ɺ���

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam *m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
};

#endif
