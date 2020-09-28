#pragma once
#ifndef CCTOST_H
#define CCTOST_H

#include "CCProductPretreatment.h"
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"
#include <math.h>
#include <string.h>
class CCToSt: public IArith
{
public:
	CCToSt(){};
	~CCToSt(){};
	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;

	
private:
int DateToJulian(int year, int mon, int day);
	
int TimeToSeconds(short hour, short minute, short second);
};

#endif

