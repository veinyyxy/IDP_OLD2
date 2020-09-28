//////////////////////////////////////////////////////////////////////////
// Arith_251_R.cpp
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include <fstream>
#include "../GDataTypes/StandardRadarDataX.h"
#include "../GDataTypes/CJulMsGMT.h"
#include "Arith_251_R.h"

ARITH_251_R::ARITH_251_R()
{
	;
}
ARITH_251_R::~ARITH_251_R()
{
	;
}

GHRESULT ARITH_251_R::Initialize()
{
	return GS_OK;
}
GHRESULT ARITH_251_R::LoadData(void *pValue,ReadParam* /*ReadParameter*/)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData *> *)pValue;
	return GS_OK;
}
GHRESULT ARITH_251_R::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_251_R::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_251_R.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"251_R：  Started: "<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}
	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));

	logfile<<"251_R：  pRadar->GetHeadP()"<<endl;

	GRADARDATAHEADER*  pRDHead = pRadar->GetHeadP();
	gInt16 ElevationCount = pRadar->GetHeadP()->ElevationCount;

	m_pOutputList->AddTail();
	Product_251_R* ppdt = &(*(m_pOutputList->GetTailAs<Product_251_R>())); 

	GHRESULT ConstrutHand;

	logfile<<"251_R：  pOutData->ConstructIt()"<<endl;

	ConstrutHand = ppdt->ConstructIt(ElevationCount,pRadar->GetElevationHeadP(0)->AzimuthNumber,pRadar->GetElevationHeadP(0)->ReflectivityGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"251_R：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"251_R：  pOutData->GetRadarInfor() "<<endl;

	RadarInfor * pRadarInfor = ppdt->GetRadarInfor();
	memcpy(pRadarInfor->SiteCode, pRDHead->SiteCode, sizeof(gChar)*8);
	memcpy(pRadarInfor->RadarType, pRDHead->RadarType, sizeof(gInt8)*4);
	pRadarInfor->Latitude = pRDHead->RadarLat;
	pRadarInfor->Longitude = pRDHead->RadarLon;
	pRadarInfor->Altitude = pRDHead->RadarHeight;


	logfile<<"251_R：  pOutData->GetMyPDBlock() "<<endl;

	PDBlock*       pdb = ppdt->GetMyPDBlock();

	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	pdb->ProductDate = pRDHead->VCPDate;
	pdb->ProductTime = pRDHead->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"251_R：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	memcpy(pdb->OperationalMode, pRDHead->OperationMode, sizeof(gInt8)*2);

	logfile<<"251_R：  i=0-->ElevationCount "<<endl;
	logfile<<"251_R：  {    pRadar->GetElevationHeadP(i) "<<endl;
	logfile<<"251_R：       pRadar->GetRadarDataP(i,j)"<<endl;
	logfile<<"251_R：       pOutData->GetMyRadial_RadialData(i,j) "<<endl;
	logfile<<"251_R：       pOutData->GetMyRadial_Head(i) "<<endl;
	logfile<<"251_R：  }"<<endl;
	for (int i=0;i<ElevationCount;i++)
	{
		if(pRadar->GetElevationHeadP(i)->ReflectivityGates <= 0)
		{
			continue;
		}

		GELEVATIONDATAHEAD* pedh = pRadar->GetElevationHeadP(i);

		DILayer* pLayer = NULL;

		memcpy(pdb->VCPMode, pRDHead->VCPMode, sizeof(gInt8)*4);
		pLayer = ppdt->GetMyLayer(i);

		pLayer->HeightOfElevation = pedh->Elevation;

		pLayer->LayerDate = pRadar->GetRadarDataP(i,0)->Date;

		pLayer->LayerTime = pRadar->GetRadarDataP(i,0)->Time;

		RadialHead* prhd = ppdt->GetMyRadial_Head(i);
		prhd->DistanceOfFirstGate = pedh->FirstGateRangeOfRef;
		prhd->LengthOfGate = pRDHead->ReflectivityGateSize;
		prhd->NumberOfGates = pedh->ReflectivityGates;
		prhd->MaxVelocity = -1;
		gInt16 DeltaAngel = 36000/pedh->AzimuthNumber;
		for(int j = 0; j < pedh->AzimuthNumber; j++)
		{
			ppdt->GetMyRadial_RadialData(i,j)->AzimuthAngle = pRadar->GetRadarDataP(i,j)->Azimuth;
			//s这段是根据下一个角读角差距
			if(j == pedh->AzimuthNumber - 1)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle = (pRadar->GetRadarDataP(i,0)->Azimuth + 36000 - pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			else
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle = (pRadar->GetRadarDataP(i, j + 1)->Azimuth + 36000 - pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			//ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle = (j == pedh->AzimuthNumber - 1)? (pRadar->GetRadarDataP(i,0)->Azimuth + 36000 - \
			//	pRadar->GetRadarDataP(i,j)->Azimuth)%36000 : (pRadar->GetRadarDataP(i, j + 1)->Azimuth + 36000 - pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			if(ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle < DeltaAngel ||
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle > 2 * DeltaAngel)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle = DeltaAngel;
			}

			//e这段是根据下一个角读角差距

			for(int k = 0; k < pedh->ReflectivityGates; k++)
			{
				(ppdt->GetMyRadial_Data(i,j))[k] = (pRadar->GetRadarDataR(i,j))[k];
			}
		}
	}

	logfile<<"251_R：  Finished. "<<endl;
	logfile.close();
	return GS_OK;
}
GHRESULT ARITH_251_R::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_251_R接口
{
	return new (::std::nothrow) ARITH_251_R;
}
