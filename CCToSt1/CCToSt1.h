//////////////////////////////////////////////////////////////////////////
// SCToSt1.h
// created by zy 20080415
// SC雷达数据转写成标准格式，只排序，未合并和差分
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

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化

private:
	gInt32 WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSCDataHead, gInt32 ElevationNumber );
	void WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSCDataHead, gInt32 nAziNum );
	void WriteRVW( StandardRadarData * pMySt, CCProductPretreatment1 * pMySc, gInt32 nSCDataNum, gInt32 nElevationIndex,gInt32 nScAziNum, gInt32 nRefGates, gInt32 nDopGates);//nFlag为0写R 1块，为1写VW 2块，为2写RVW 3块
	gInt16 GetDBZ( gUint8 nNum );
	gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 NyquistVelocity );
	gInt16 GetSWData( gUint8 nNum, gInt16 NyquistVelocity  );
	gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision表示单位为小数点后第几位，如0.0001则为4，精度为1则为0
	gInt32 GetEleNumber(CCProductPretreatment1 * pMySc);
private:
	GXList< GenerationData > * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	int m_SecondTimeZone;

};

GHRESULT DLLEXP_GSYS ExecuteArithCCToSt1( GXList< GenerationData>  *Datas,  ReadParam* pParam,  GXList< GenerationData > * pOutputList);


#endif //CCTOST1_H