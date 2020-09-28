//////////////////////////////////////////////////////////////////////////
// AziSort.h
// created by zy 20080415
// ����λ��������ı�׼��ʽ����  �����ǣ�VCP31Ϊ3�������ǣ�����Ϊ2����2���Ϊһ�㣬����ÿ�����Ƿ�λ����Ŀ���Ϊ360
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef AZISORT_H
#define AZISORT_H
#include <math.h>
#include <string.h>

#include "../GDataTypes/StandardRadarDataX.h"
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"



class AziSort: public IArith
{
public:
	AziSort();
	~AziSort();

	SYSMETHOD(Initialize)();			//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//�����㷨��������
	SYSMETHOD(OutPutData)(void * pValue);//�������������ݵ�pValue
	SYSMETHOD(Execute)();//ִ���㷨
	SYSMETHOD(UnInitialize)();//����ʼ��

	GXList< GenerationData >  * m_pInputList;

private:
	void WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, GRADARDATAHEADER * pStInHead, gInt32 ElevationNumber );
	void WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, GELEVATIONDATAHEAD * pStInEleHead );
	void WriteRVW( StandardRadarData * pMySt, StandardRadarData * pMyStIn, gInt32 nElevationIndex, gInt32 nInEleNum, gInt32 nFlag, gInt32 nRefGatesA, gInt32 nDopGatesA, gInt16 nInAziNum);//nFlagΪ0дR 1�飬Ϊ1дVW 2�飬Ϊ2дRVW 3��
	//gInt16 GetDBZ( gUint8 nNum );
	//gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution );
	//gInt16 GetSWData( gUint8 nNum );
	//gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision��ʾ��λΪС�����ڼ�λ����0.0001��Ϊ4������Ϊ1��Ϊ0

private:
	//GXList< GenerationData > * m_pInputList;//�����޸�
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	//int(* ReadP)(char * ,char * &);
	//char * FileName;
	//char * SiteCode;
};

#endif //AziSort_H