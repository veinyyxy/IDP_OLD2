//////////////////////////////////////////////////////////////////////////
// SAToSt.h
// created by zy 20080415
// SA雷达数据转写成标准格式，只排序，未合并和差分
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

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化

private:
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSADataHead, gInt32 ElevationNumber );
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE2 * pSADataHead, gInt16 ElevationNumber,gInt16 iSwapon);
	gInt32 WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSADataHead, gInt32 nAziNum );
	gInt16 WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE2 * pSADataHead, gInt32 nAziNum,gInt16 iSwapon);
	gInt32 WriteRVW( StandardRadarData * pMySt, SAProductPretreatment * pMySa, gInt32 nSADataNum, gInt32 nElevationIndex,gInt32 nSaAziNum, gInt32 nRefGatesA, gInt32 nDopGatesA);//nFlag为0写R 1块，为1写VW 2块，为2写RVW 3块
	gInt16 GetDBZ( gUint8 nNum );
	gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 NyquistVelocity );
	gInt16 GetSWData( gUint8 nNum, gInt16 NyquistVelocity  );
	gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision表示单位为小数点后第几位，如0.0001则为4，精度为1则为0
	gInt32 GetEleNumber(SAProductPretreatment * pMySa);
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	int m_SecondTimeZone;
	gInt32 m_DBZStatistics[11];

};

#endif //SAToSt_H