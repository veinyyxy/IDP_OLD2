//////////////////////////////////////////////////////
//Arith_266_VIL.h  
//created by qcsun 20080402
//
//
//该算法
//输入
//StandardRadarData   * pGData；
//gInt16    EchoTopRefThreshold ; 计算VIL和VILD时所取的回波顶的反射率因子阈值，单位dBZ,缺省值为18dBZ
//gFloat   BeamWidth;  雷达波束宽度，单位0.01度（例如95表示0.95度），CINRAD-SA雷达的波束宽度为0.95度
//
//输出： 垂直液态水含量，单位0.01kg/m2
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
	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//取得结果大小以便申请内存
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:
	void  pArith_266_VIL(StandardRadarData *pGData , gInt16 EchoTopRefThreshold , gFloat BeamWidth , Product_266_VIL * pRadialData,  gInt16 MaxRefGateNumer ) ;//垂直液态水含量产品生成函数

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam * m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
};

#endif
