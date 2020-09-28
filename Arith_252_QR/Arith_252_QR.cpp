//////////////////////////////////////////////////////////////////////////
// Arith_252_QR.cpp
// created by ply 20080402
// 251号产品，基本反射率，R
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include "Arith_252_QR.h"
#include "StandardRadarDataX.h"
#include "CJulMsGMT.h"
#include <fstream>
#include "IBase.h"
#include "GDef.h"

ARITH_252_QR::ARITH_252_QR()
{
	;
}
ARITH_252_QR::~ARITH_252_QR()
{
	;
}

GHRESULT ARITH_252_QR::Initialize()
{
	return GS_OK;
}
GHRESULT ARITH_252_QR::LoadData(void *pValue,ReadParam* /*ReadParameter*/)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_252_QR::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_252_QR::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_252_QR.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"252_QR：  Started: "<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}
	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));

	logfile<<"252_QR：  pRadar->GetHeadP()"<<endl;

	GRADARDATAHEADER*  pRDHead = pRadar->GetHeadP();
	gInt16 ElevationCount = pRadar->GetHeadP()->ElevationCount;

	m_pOutputList->AddTail();
	Product_252_QR* ppdt = &(*(m_pOutputList->GetTailAs<Product_252_QR>())); 

	GHRESULT ConstrutHand;

	logfile<<"252_QR：  pOutData->ConstructIt()"<<endl;

	ConstrutHand = ppdt->ConstructIt(ElevationCount,pRadar->GetElevationHeadP(0)->AzimuthNumber,pRadar->GetElevationHeadP(0)->ReflectivityGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"252_QR：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"252_QR：  pOutData->GetRadarInfor() "<<endl;

	RadarInfor * pRadarInfor = ppdt->GetRadarInfor();
	memcpy(pRadarInfor->SiteCode, pRDHead->SiteCode, sizeof(gChar)*8);
	memcpy(pRadarInfor->RadarType, pRDHead->RadarType, sizeof(gInt8)*4);
	pRadarInfor->Latitude = pRDHead->RadarLat;
	pRadarInfor->Longitude = pRDHead->RadarLon;
	pRadarInfor->Altitude = pRDHead->RadarHeight;


	logfile<<"252_QR：  pOutData->GetMyPDBlock() "<<endl;

	PDBlock*       pdb = ppdt->GetMyPDBlock();

	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	pdb->ProductDate = pRDHead->VCPDate;
	pdb->ProductTime = pRDHead->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"252_QR：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	memcpy(pdb->OperationalMode, pRDHead->OperationMode, sizeof(gInt8)*2);
	/* pdb->ElevationNumber = (gInt16)i;*/
	//pdb->CinradType = pRDHead->RadarType;
	//CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
	//    pdb->GenerationTimeOfProduct);
	//pdb->VolumeScanDate = pRDHead->VCPDate;
	//pdb->VolumeScanTime = pRDHead->VCPTime;
	//pdb->Height = pRDHead->RadarHeight;
	//pdb->Latitude = pRDHead->RadarLat;
	//pdb->Longitude = pRDHead->RadarLon;
	//pdb->OperationalMode = pRDHead->ScanMode;
	//memcpy(pdb->SiteID, pRDHead->SiteCode, sizeof(gChar)*8);
	//memcpy(pdb->SiteName, pRDHead->SiteName, sizeof(gChar)*18);

	logfile<<"252_QR：  i=0-->ElevationCount "<<endl;
	logfile<<"252_QR：  {    pRadar->GetElevationHeadP(i) "<<endl;
	logfile<<"252_QR：       pRadar->GetRadarDataP(i,j)"<<endl;
	logfile<<"252_QR：       pOutData->GetMyRadial_RadialData(i,j) "<<endl;
	logfile<<"252_QR：       pOutData->GetMyRadial_Head(i) "<<endl;
	logfile<<"252_QR：  }"<<endl;
	for (int i=0;i<ElevationCount;i++)
	{
		if(pRadar->GetElevationHeadP(i)->ReflectivityGates <= 0)
		{
			continue;
		}
		/*   m_pOutputList->AddTail();                           //DE by sunqc 20080729
		Product_252_QR* ppdt = &(*(m_pOutputList->GetTailAs<Product_252_QR>()));    
		GELEVATIONDATAHEAD* pedh = pRadar->GetElevationHeadP(i);

		ppdt->ConstructIt(pedh->AzimuthNumber, pedh->ReflectivityGates);
		PDBlock*       pdb = ppdt->GetMyPDBlock();

		pdb->ElevationNumber = (gInt16)i;
		pdb->CinradType = pRDHead->RadarType;
		CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
		pdb->VolumeScanDate = pRDHead->VCPDate;
		pdb->VolumeScanTime = pRDHead->VCPTime;
		pdb->Height = pRDHead->RadarHeight;
		pdb->Latitude = pRDHead->RadarLat;
		pdb->Longitude = pRDHead->RadarLon;
		pdb->OperationalMode = pRDHead->ScanMode;
		memcpy(pdb->SiteID, pRDHead->SiteCode, sizeof(gChar)*8);
		memcpy(pdb->SiteName, pRDHead->SiteName, sizeof(gChar)*18);*/ 
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

	logfile<<"252_QR：  Finished. "<<endl;
	logfile.close();
	return GS_OK;
}
GHRESULT ARITH_252_QR::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_252_QR接口
{
	return new (::std::nothrow) ARITH_252_QR;
}
