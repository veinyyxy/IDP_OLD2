//////////////////////////////////////////////////////////////////////////
// AziSort.h
// created by zy 20080415
// 将方位角已排序的标准格式数据  低仰角（VCP31为3个低仰角，其他为2个）2层合为一层，并将每个仰角方位角数目差分为360
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

	SYSMETHOD(Initialize)();			//初始化对象
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);//设置算法输入数据
	SYSMETHOD(OutPutData)(void * pValue);//设置输出结果数据到pValue
	SYSMETHOD(Execute)();//执行算法
	SYSMETHOD(UnInitialize)();//反初始化

	GXList< GenerationData >  * m_pInputList;

private:
	void WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, GRADARDATAHEADER * pStInHead, gInt32 ElevationNumber );
	void WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, GELEVATIONDATAHEAD * pStInEleHead );
	void WriteRVW( StandardRadarData * pMySt, StandardRadarData * pMyStIn, gInt32 nElevationIndex, gInt32 nInEleNum, gInt32 nFlag, gInt32 nRefGatesA, gInt32 nDopGatesA, gInt16 nInAziNum);//nFlag为0写R 1块，为1写VW 2块，为2写RVW 3块
	//gInt16 GetDBZ( gUint8 nNum );
	//gInt16 GetVelData( gUint8 nNum, gInt16 VelocityResolution );
	//gInt16 GetSWData( gUint8 nNum );
	//gInt32 StrToNum( char * pStr, gInt32 nPrecision );//nPrecision表示单位为小数点后第几位，如0.0001则为4，精度为1则为0

private:
	//GXList< GenerationData > * m_pInputList;//测试修改
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;
	//int(* ReadP)(char * ,char * &);
	//char * FileName;
	//char * SiteCode;
};

#endif //AziSort_H