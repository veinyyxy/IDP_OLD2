//////////////////////////////////////////////////////////////////////////
// SAToSt.h
// created by zy 20080415
// SA�״�����תд�ɱ�׼��ʽ��ֻ����δ�ϲ��Ͳ��
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SATOST_H
#define SATOST_H
#include "StandardRadarDataX.h"
#include "SAProductPretreatment.h"
#include "SAProductWSR.h"
#include "IArith.h"
#include "GXList.h"
#include "GData.h"
#include "GDef.h"
#include <math.h>
#include <string.h>

#define Ignore_Radar_Data 100

class SAToSt: public IArith
{
public:
	SAToSt();
	~SAToSt();

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��

private:
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSADataHead, gInt32 ElevationNumber );
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE2 * pSADataHead, gInt16 ElevationNumber,gInt16 iSwapon);
	gInt32 WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSADataHead, gInt32 nAziNum );
	gInt16 WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE2 * pSADataHead, gInt32 nAziNum,gInt16 iSwapon);
	gInt32 WriteRVW( StandardRadarData * pMySt, SAProductPretreatment * pMySa, gInt32 nSADataNum, gInt32 nElevationIndex,gInt32 nSaAziNum, gInt32 nRefGatesA, gInt32 nDopGatesA);//nFlagΪ0дR 1�飬Ϊ1дVW 2�飬Ϊ2дRVW 3��
	gInt16 GetDBZ( gUint8 nNum );
	gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 NyquistVelocity );
	gInt16 GetSWData( gUint8 nNum, gInt16 NyquistVelocity  );
	gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision��ʾ��λΪС�����ڼ�λ����0.0001��Ϊ4������Ϊ1��Ϊ0
	gInt32 GetEleNumber(SAProductPretreatment * pMySa);
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	int m_SecondTimeZone;
	gInt32 m_DBZStatistics[11];

};

#endif //SAToSt_H