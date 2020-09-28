//////////////////////////////////////////////////////////////////////////
// Arith_254_V.cpp
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include "Arith_254_V.h"
#include "StandardRadarDataX.h"
#include "CJulMsGMT.h"
#include <fstream>

ARITH_254_V::ARITH_254_V()
{
	;
}
ARITH_254_V::~ARITH_254_V()
{
	;
}

GHRESULT ARITH_254_V::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	return GS_OK;
}
GHRESULT ARITH_254_V::LoadData(void *pValue,ReadParam *ReadParameter)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	if(ReadParameter == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData > *)pValue;
	m_pParameter=ReadParameter;
	return GS_OK;
}
GHRESULT ARITH_254_V::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_254_V::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_254_V.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);

	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"254_V：  Started: "<<endl;
	logfile<<"254_V：  Check m_pInputList and m_pParameter."<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}
	if(m_pParameter == NULL)
	{
		return GE_INVALIDARG;
	}

	logfile<<"254_V：  Read Parameter: ProductDependentFisrt."<<endl;
	gInt16 ProductDependentFisrt = 0;
	ProductDependentFisrt = 1;

	logfile<<"254_V：  Get Information of pRadar and Set Product_254_V."<<endl;
	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));
	
	GRADARDATAHEADER*    prdh = pRadar->GetHeadP();
	GELEVATIONDATAHEAD* pedh = NULL;

	gInt16  ElevationCount = prdh->ElevationCount;
	m_pOutputList->AddTail(); 
	Product_254_V* ppdt = &(*(m_pOutputList->GetTailAs<Product_254_V>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(ElevationCount,pRadar->GetElevationHeadP(0)->AzimuthNumber,pRadar->GetElevationHeadP(0)->DopplerGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"254_V：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}
	RadarInfor * pRadarInfor = ppdt->GetRadarInfor();
	memcpy(pRadarInfor->SiteCode, prdh->SiteCode, sizeof(gChar)*8);
	memcpy(pRadarInfor->RadarType, prdh->RadarType, sizeof(gInt8)*4);
	pRadarInfor->Latitude = prdh->RadarLat;
	pRadarInfor->Longitude = prdh->RadarLon;
	pRadarInfor->Altitude = prdh->RadarHeight;

	PDBlock* pdb = ppdt->GetMyPDBlock();

	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	pdb->ProductDate = prdh->VCPDate;
	pdb->ProductTime = prdh->VCPTime;
	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8)*2);

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"254_V：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8)* 4);
	pdb->ProductDependent[0]=ProductDependentFisrt;	   
	pdb->ProductDependent[1]=pRadar->GetElevationHeadP(0)->NyquistVelocity;//NyquistVelocity;// 最大不模糊速度(0.01m/s)//added by ply 20080611
	pdb->ProductDependentCount= 2;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	logfile<<"254_V：  Set RadialData,ElevationHead and Velocity Data Information in each Elevation.\n";
	for (int i=0;i<ElevationCount;i++)
	{
		pedh = pRadar->GetElevationHeadP(i);
		if(pedh->DopplerGates <= 0)
			continue;

		DILayer* pLayer = NULL;
		pLayer = ppdt->GetMyLayer(i);
		pLayer->HeightOfElevation = pedh->Elevation;
		pLayer->LayerDate = pRadar->GetRadarDataP(i,0)->Date;
		pLayer->LayerTime = pRadar->GetRadarDataP(i,0)->Time;  

		RadialHead* prhd = ppdt->GetMyRadial_Head(i);
		prhd->DistanceOfFirstGate = pedh->FirstGateRangeOfRef;
		prhd->LengthOfGate = prdh->DopplerGateSize;
		prhd->NumberOfGates = pedh->DopplerGates;
		//prhd->ScaleOfData = 10;   

		gInt16 DeltaAngel=36000/pedh->AzimuthNumber;
		for (int j=0;j<pedh->AzimuthNumber;j++)
		{
			ppdt->GetMyRadial_RadialData(i,j)->AzimuthAngle=pRadar->GetRadarDataP(i,j)->Azimuth;
			//((Product_251_R*)(&(m_pOutputList->GetTail())))->GetMyRadial_RadialData(j)->DeltaAngel=DeltaAngel;///平分全部角度做角差距

			//s这段是根据下一个角读角差距
			if (j==pedh->AzimuthNumber-1)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=(pRadar->GetRadarDataP(i,0)->Azimuth+36000-pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			else
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=(pRadar->GetRadarDataP(i,j+1)->Azimuth+36000-pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			if (ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle < DeltaAngel ||
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle > 2*DeltaAngel)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=DeltaAngel;
			}


			//e这段是根据下一个角读角差距
			for (int k=0;k<pedh->DopplerGates;k++)
			{
				(ppdt->GetMyRadial_Data(i,j))[k] = (pRadar->GetRadarDataV(i,j))[k];
			}
		}
	}

	logfile<<"254_V：  Finished.\n";
	logfile.close();

	return GS_OK;
}
GHRESULT ARITH_254_V::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_254_V接口
{
	return new (::std::nothrow)ARITH_254_V;
}
