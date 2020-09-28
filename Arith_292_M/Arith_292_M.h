#pragma once
#ifndef		Arith_292_M_H
#define		Arith_292_M_H

#include "iarith.h"
#include "GXList.h"
#include "GData.h"
#include <list>
#include <vector>
#include "Struct_292_M.h"

typedef struct PatV 
{
	bool InFeature;
	//gFloat SRbeg;
	//gFloat SRend;
	gFloat RS;
	gFloat AZbegpv;
	gFloat AZendpv;
	gFloat VDbeg;
	gFloat VDend;
	gFloat SRT;
	gFloat MA;
}PatV;

typedef struct
{
	bool Ismesoc;
	gInt32 index;
	gInt32 VortexNo;
	gFloat MaxAZ;
	gFloat MinAZ;
	gFloat MaxRS;
	gFloat MinRS;
	gFloat HFC;
	gInt16 elev;
	gFloat RSFEcen;
	gFloat AZcenfe;
	gFloat DAA;
	gFloat DAR;
	gFloat SRTavg;
	gFloat Mavg;
	gFloat SRTmax;
	gFloat DMA;
}FEATUREStruct;

typedef struct  
{
	gInt32 FeatureCnt;
	//gInt32 MesocCnt;
	VortexType VortexType;
	gFloat SRTmaxavg; 
	gFloat SRTmax;
	gFloat DAA;
	gFloat DAR;
	gFloat Mavg;
	gFloat HFC;
	gInt16 elev;
	gFloat RSFEcen;
	gFloat AZcenfe;
	gFloat HighestHFC;
	gFloat LowestHFC;
	gFloat SRTavg;
	gFloat SPRavg;
	gFloat HightestSRTmax;
	gFloat FEATUREMaxAz;
	gFloat FEATUREMinAz;
	gFloat FEATUREMaxRs;
	gFloat FEATUREMinRs;
	gInt16 CenterX;
	gInt16 CenterY;
	gInt16 Radius;
}VORTEXStruct;

typedef struct  
{
	gFloat TFHeight;
	gFloat THMomen;	
	gFloat TRadiDis;
	gFloat TMesoHighS;
	gFloat TLMoment;
	gFloat TMesoLowS; 
	gFloat TMesoAzi;
	gFloat TPV;
	gFloat TFMaxRat;
	gFloat TFMinRat;
	gFloat TMaxRat;
	gFloat TMinRat;
	gFloat TRange;
}Param;

typedef list<PatV> PVL;
typedef vector<FEATUREStruct> FEAV;

class Arith_292_M :
	public IArith
{
public:
	Arith_292_M(void);
	~Arith_292_M(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:
	void PatternVector(GenerationData * p, gInt16 layer, vector<PatV> &PVector/* ,gFloat DeltaAngel, gFloat &VDbeg, gFloat &VDend, gFloat &AZbeg, gFloat &AZend*/);
	void Feature(/*GenerationData * p,*/ vector<PatV> &PVector, vector<PVL> &FEATUREs/*, gInt16 layer, gFloat DeltaAngel*/);
	void CharacterizeFeature(vector<PVL> &FEATUREs, vector<FEATUREStruct>& FEATUREStructs, FEAV * TOTALFEATUREs, gInt16 layer,
		gFloat angle, gInt16 LenOfGate/*, gFloat &VDbeg, gFloat &VDend, gFloat &AZbeg, gFloat &AZend*/);
	void Vortex(GenerationData * p, gInt16 NumOfLayer, FEAV * TOTALFEATUREs, vector<FEAV> & VortexV);
	void CharacterizeMesocyclone(vector<FEAV> & VortexV, VORTEXStruct * Vortices);
	Param m_Param;

private:
	GXList<GenerationData *>* m_pInputList;
	GXList<GenerationData>* m_pOutputList;
	ReadParam *       m_fnParam;
};

#endif

