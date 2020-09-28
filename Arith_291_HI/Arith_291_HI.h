//////////////////////////////////////////////////////////////////////////
//created by zhaishuchao 081124
//////////////////////////////////////////////////////////////////////////
#pragma once 
#ifndef ARITH_291_HI
#define ARITH_291_HI

#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "struct_291_HI.h"
#include "Product_291_HI.h"
//#include "../Arith_277_SCI/Arith_277_SCI.h"
#include "../Arith_277_SCI/Struct_277_SCI.h"
#include "../Arith_277_SCI/Product_277_SCI.h"

#define MAX_CELLS       100
#define COEF_HKE_1		0.0005
#define COEF_HKE_2		0.084
#define COEF_HKE_3		10
#define COEF_POSH		29
#define OFFSET_POSH     50
//#define COEF_SHI_SIZE   0.1
//#define EXPO_SHI_SIZE   0.5
#define WT_COF          57.5		//100J/(m*m*s)
#define WT_OFS			-121		//100000J/(m*s)
#define HS_COF			2.54
#define HS_EXP			0.5

struct XML_Indexhi
{
	gFloat Threshold_HKE_RW_Lower;
	gFloat Threshold_HKE_RW_Upper;
	gFloat Threshold_POH_minR;
	gFloat HeightTop_0;         //km
	gFloat HeightTop_20;		//km
	gFloat POH_HD1;				//1.625km
	gFloat POH_HD2;				//1.875km
	gFloat POH_HD3;				//2.125km
	gFloat POH_HD4;				//2.375km
	gFloat POH_HD5;				//2.625km
	gFloat POH_HD6;				//2.925km;
	gFloat POH_HD7;				//3.3km
	gFloat POH_HD8;				//3.75km
	gFloat POH_HD9;				//4.5km;
	gFloat POH_HD10;			//5.5km

};

class Arith_291_HI : public IArith
{
public:
	Arith_291_HI(void);
	~Arith_291_HI(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;     //读入数据
	SYSMETHOD(Execute) ();             //执行算法
	SYSMETHOD(OutPutData)(void *) ;   //取出结果数据
	SYSMETHOD(UnInitialize)	() ;       //反初始化
private:
	GXList< GenerationData *> * m_pInputList;
	GXList< GenerationData > * m_pOutputList;
	ReadParam * m_pParameter;

	gFloat DH_POSH;
	gFloat WZ;
	gFloat HMC;
	gFloat VTWF;
	gFloat HKE;

	void ComHeightTop_C(int i, int j,STORM_CELLATTRIBUTES* stcIn, gInt16 NumOfComponent, gFloat* HeightTopC);
	void ComHailKE_C(gFloat WZ, gFloat maxRR, gFloat& HKE);
	void ComHeightDif_POH(gFloat* H_POH, int i, int j, STORM_CELLATTRIBUTES* stcIn, gFloat * maxR, XML_Indexhi index, int NumOfComponent);
	void ComPOH(int i, gFloat* DH_POH, XML_Indexhi index, gFloat * POH);
	void ComPOSH(int i, gFloat* SHI, gFloat* POSH, gFloat WTSM, gFloat* POH);
};

#endif