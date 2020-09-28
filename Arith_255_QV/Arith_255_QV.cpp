//////////////////////////////////////////////////////////////////////////
// Arith_255_QV.cpp
// created by ply 20080411
// 254号产品，基本速度，V
//////////////////////////////////////////////////////////////////////////
#define IARITHMS_LIB

#include "Arith_255_QV.h"
#include "StandardRadarDataX.h"
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include "CJulMsGMT.h"
#include <fstream>
#include "IBase.h"
#include "GDef.h"

ARITH_255_QV::ARITH_255_QV()
{
	;
}
ARITH_255_QV::~ARITH_255_QV()
{
	;
}

GHRESULT ARITH_255_QV::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	return GS_OK;
}
GHRESULT ARITH_255_QV::LoadData(void *pValue,ReadParam *ReadParameter)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	if(ReadParameter == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData > *)pValue;
	m_pParameter=ReadParameter;
	return GS_OK;
}
GHRESULT ARITH_255_QV::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_255_QV::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_255_QV.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);

	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"255_QV：  Started: "<<endl;
	logfile<<"255_QV：  Check m_pInputList and m_pParameter."<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}
	if(m_pParameter == NULL)
	{
		return GE_INVALIDARG;
	}

	gInt16 ProductDependentFisrt = 0;
	gInt16 NyquistVelocity=0;

	logfile<<"255_QV：  Get Information of pRadar and Set Product_255_QV."<<endl;
	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));
	GRADARDATAHEADER*    prdh = pRadar->GetHeadP();

	GELEVATIONDATAHEAD* pedh = NULL;

	gInt16  ElevationCount = prdh->ElevationCount;

	m_pOutputList->AddTail(); 
	Product_255_QV* ppdt = &(*(m_pOutputList->GetTailAs<Product_255_QV>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(ElevationCount,pRadar->GetElevationHeadP(0)->AzimuthNumber,pRadar->GetElevationHeadP(0)->DopplerGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"255_QV：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
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
	logfile<<"255_QV：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;
	//pdb->CinradType=prdh->RadarType;
	//CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);//获取当前儒略日毫秒
	//pdb->VolumeScanDate=prdh->VCPDate;
	//pdb->VolumeScanTime=prdh->VCPTime;
	//pdb->Height=prdh->RadarHeight;   	  
	//pdb->Latitude=prdh->RadarLat;
	//pdb->Longitude=prdh->RadarLon;
	//pdb->OperationalMode=prdh->ScanMode;
	//memcpy(pdb->SiteID,prdh->SiteCode,sizeof(gChar)*8);
	//memcpy(pdb->SiteName,prdh->SiteName,sizeof(gChar)*18);	
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8)* 4);
	//pdb->ProductDependent[0]=ProductDependentFisrt;	   
	pdb->ProductDependent[1]=pRadar->GetElevationHeadP(0)->NyquistVelocity;//NyquistVelocity;// 最大不模糊速度(0.01m/s)//added by ply 20080611
	pdb->ProductDependentCount= 2;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	logfile<<"255_QV：  Set RadialData,ElevationHead and Velocity Data Information in each Elevation.\n";
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
		//ppdt->GetMyRadial_Head(i)->DistanceOfFirstGate=pedh->FirstGateRangeOfDoppler;
		//ppdt->GetMyRadial_Head(i)->LengthOfGate=pedh->DopplerGateSize;
		//ppdt->GetMyRadial_Head(i)->NumberOfGates=pedh->DopplerGates;
		//ppdt->GetMyRadial_Head(i)->ScaleOfData=10;

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

	logfile<<"255_QV：  Finished.\n";
	logfile.close();

	return GS_OK;
}
GHRESULT ARITH_255_QV::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_255_QV接口
{
	return new (::std::nothrow)ARITH_255_QV;
}
