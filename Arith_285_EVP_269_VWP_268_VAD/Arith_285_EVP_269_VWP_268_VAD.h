//////////////////////////////////////////////////////////////////////////
//modify by zsc
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_285_EVP269_VWP_268_VAD_H
#define ARITH_285_EVP269_VWP_268_VAD_H
#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include <math.h>
#include "Product_285_EVP.h"
#include "Product_269_VWP.h"
#include "Product_268_VAD.h"
#include "Struct_285_EVP_269_VWP_268_VAD.h"

#define RADIUS_EARTH	6371            //km
#define RH0             1290				//kg/(km*km*km)

struct XML_Para
{
	gFloat Thr_Symmetry;     //km/hr
	gInt32 Thr_Velocity;     //km/hr
	gInt32 Thr_DataPoint;
	int Height_Num;
	gInt32 Height[10];            //10m
};

typedef struct Velocity_Azimuth_Display
{
	int tilt;
	int nDP;
	double Height;		//km
	double Difnear;		//10m
	int Vel[360];		//多普勒速度
	int RD[360];		//基本反射率因子
	double CF0;			//km/hr
	double FIT[360];	//km/hr    FIT=CF0-cos(Az-DW)*SPW;
	double DW;			//风;
	double SPW;         //速度
	double RMS;			//km/hr
	double SHW;			//水平速度  km/hr
	double HVAD;		//km
	double VPF;			//km/hr
	double DEC;			//km
	double AS;			//km*km
	double ASC;			//km
	double SVC;			//1/hr
	double SVW;			//km/hr
	double DIV;			//1/hr
	double stretch;
	double shear;
	bool Symmetry;
}VAD;

struct complex
{
	double real;
	double image;
};
class Arith_285_EVP_269_VWP_268_VAD:public IArith
{
public:
	Arith_285_EVP_269_VWP_268_VAD(void);
	~Arith_285_EVP_269_VWP_268_VAD(void);

	SYSMETHOD(Initialize)();
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);
	SYSMETHOD(OutPutData)(void * pValue);
	SYSMETHOD(Execute)();
	SYSMETHOD(UnInitialize)();
private:
	bool OnRadarEVP(int Num_Tilts, StandardRadarData *pGData,Product_285_EVP *Product,Product_269_VWP *Product1,
		Product_268_VAD *Product2, XML_Para parm);
	bool seg_init(int Dop_Gatelen, short El_Angle, double bh, double th);
	bool VAD_process(int tilt, StandardRadarData *pGData, VAD* vad, XML_Para parm);
	int InitX2(int Rj, int Tilt, int *X2, StandardRadarData *pGData);
	int InitX2(int Tilt, int *X2, StandardRadarData *pGData);
	bool Compute_Stretch_Shear(VAD* vad, int* X2, StandardRadarData* pGData, int tilt, int ihm, double R1);
	//void VAD_data_deal_with(VAD* vad);
	void VAD_mean(VAD* vad);
	inline complex Complex(double a, double b);
	inline complex ComplexPlus(complex a, complex b);
	inline complex ComplexMinus(complex a, complex b);
	inline complex ComplexMultiply(complex a, complex b);
	inline complex ComplexDivide(complex a, complex b);
private:
	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表

	double m_Sin_e1,m_Cos_e2;
	int m_StartBin,m_EndBin;
	//int m_cnt[1550];
	//float East_W_SW[1550];
	//float South_N_SW[1550];
	//float RMS[1550];
	//float m_ZyVAD[6][1550];
};

#endif
