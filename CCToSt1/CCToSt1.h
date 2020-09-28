//////////////////////////////////////////////////////////////////////////
// SCToSt1.h
// created by zy 20080415
// SC�״�����תд�ɱ�׼��ʽ��ֻ����δ�ϲ��Ͳ��
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CCTOST1_H
#define CCTOST1_H
#include "StandardRadarDataX.h"
#include "CCProductPretreatment1.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include "GDef.h"
#include <math.h>
#include <string.h>

#define Ignore_Radar_Data 100

class DLLEXP_GSYS CCToSt1: public IArith
{
public:
	CCToSt1();
	~CCToSt1();

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��

private:
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSCDataHead, gInt32 ElevationNumber );
	void WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSCDataHead, gInt32 nAziNum );
	void WriteRVW( StandardRadarData * pMySt, CCProductPretreatment1 * pMySc, gInt32 nSCDataNum, gInt32 nElevationIndex,gInt32 nScAziNum, gInt32 nRefGates, gInt32 nDopGates);//nFlagΪ0дR 1�飬Ϊ1дVW 2�飬Ϊ2дRVW 3��
	gInt16 GetDBZ( gUint8 nNum );
	gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 NyquistVelocity );
	gInt16 GetSWData( gUint8 nNum, gInt16 NyquistVelocity  );
	gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision��ʾ��λΪС�����ڼ�λ����0.0001��Ϊ4������Ϊ1��Ϊ0
	gInt32 GetEleNumber(CCProductPretreatment1 * pMySc);
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	int m_SecondTimeZone;

};

GHRESULT DLLEXP_GSYS ExecuteArithCCToSt1( GXList< GenerationData>  *Datas,  ReadParam* pParam,  GXList< GenerationData > * pOutputList);


#endif //CCTOST1_H