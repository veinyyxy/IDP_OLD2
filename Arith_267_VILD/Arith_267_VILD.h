//////////////////////////////////////////////////////
//Arith_267_VILD.h  
//created by qcsun 20080403
//
//
//该算法
//输入
//StandardRadarData   * pGData；
//gInt16   EchoTopRefThreshold ; 计算VIL和VILD时所取的回波顶的反射率因子阈值，单位dBZ,缺省值为18dBZ
//gFloat   BeamWidth;  雷达波束宽度，单位0.01度（例如95表示0.95度），CINRAD-SA雷达的波束宽度为0.95度
//
//输出：垂直液态水含量密度 ，单位0.01kg/m2
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
	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//取得结果大小以便申请内存
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:
	void  pArith_267_VILD(StandardRadarData *pGData , gInt16 EchoTopRefThreshold , Product_267_VILD * pRadialData, gInt16 MaxRefGateNumer ) ;//垂直液态水含量密度产品生成函数

private:
	GXList< GenerationData > * m_pGenerateProductList;
	ReadParam *m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
};

#endif
