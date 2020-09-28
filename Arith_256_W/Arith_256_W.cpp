//////////////////////////////////////////////////////////////////////////
// Arith_256_W.cpp
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include "Arith_256_W.h"
#include "StandardRadarDataX.h"
#include "CJulMsGMT.h"
#include <fstream>
#include "IBase.h"
#include "GDef.h"

ARITH_256_W::ARITH_256_W()
{
	;
}
ARITH_256_W::~ARITH_256_W()
{
	;
}

GHRESULT ARITH_256_W::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	return GS_OK;
}
GHRESULT ARITH_256_W::LoadData(void *pValue,ReadParam* /*ReadParameter*/)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_256_W::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_256_W::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_256_W.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"256_W：  Started: "<<endl;
	logfile<<"256_W：  Check m_pInputList's Count. "<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}

	logfile<<"256_W：  Get and Set head Information from StandardRadarData. "<<endl;

	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));
	GRADARDATAHEADER*    prdh = pRadar->GetHeadP();
	gInt16 ElevationCount = prdh->ElevationCount;

	m_pOutputList->AddTail();
	Product_256_W* ppdt = &(*(m_pOutputList->GetTailAs<Product_256_W>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(ElevationCount, pRadar->GetElevationHeadP(0)->AzimuthNumber, pRadar->GetElevationHeadP(0)->DopplerGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"256_W：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	RadarInfor * pRadarInfor = ppdt->GetRadarInfor();
	memcpy(pRadarInfor->SiteCode, prdh->SiteCode, sizeof(gChar)*8);
	memcpy(pRadarInfor->RadarType, prdh->RadarType, sizeof(gInt8)* 4);
	pRadarInfor->Latitude = prdh->RadarLat;
	pRadarInfor->Longitude = prdh->RadarLon;
	pRadarInfor->Altitude = prdh->RadarHeight;

	PDBlock* pdb = ppdt->GetMyPDBlock();
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	pdb->ProductDate = prdh->VCPDate;
	pdb->ProductTime = prdh->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"256_W：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8)* 2);
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8) * 4);		
	//pdb->VCP=prdh->VCPMode;
	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	logfile<<"256_W：  Set RadialData,ElevationHead and SpectrumWidth Data Information in each Elevation.\n";
	for (int i=0;i<ElevationCount;i++)
	{
		GELEVATIONDATAHEAD* pedh = pRadar->GetElevationHeadP(i);
		if(pedh->DopplerGates <= 0)
		{
			continue;
		}
		ppdt->GetMyLayer(i)->HeightOfElevation = pedh->Elevation;
		ppdt->GetMyLayer(i)->LayerDate=pRadar->GetRadarDataP(i,0)->Date;
		ppdt->GetMyLayer(i)->LayerTime=pRadar->GetRadarDataP(i,0)->Time;
		ppdt->GetMyRadial_Head(i)->DistanceOfFirstGate=pRadar->GetElevationHeadP(i)->FirstGateRangeOfDoppler;
		ppdt->GetMyRadial_Head(i)->LengthOfGate=pRadar->GetHeadP()->DopplerGateSize;
		ppdt->GetMyRadial_Head(i)->NumberOfGates=pRadar->GetElevationHeadP(i)->DopplerGates;

		gInt16 DeltaAngel=36000/pRadar->GetElevationHeadP(i)->AzimuthNumber;
		for (int j=0;j< pedh->AzimuthNumber;j++)
		{
			ppdt->GetMyRadial_RadialData(i,j)->AzimuthAngle=pRadar->GetRadarDataP(i,j)->Azimuth;

			//s这段是根据下一个角读角差距
			if (j== pedh->AzimuthNumber-1)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=(pRadar->GetRadarDataP(i,0)->Azimuth+36000-pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			else
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=(pRadar->GetRadarDataP(i,j+1)->Azimuth+36000-pRadar->GetRadarDataP(i,j)->Azimuth)%36000;
			}
			if (ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle<DeltaAngel||ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle>2*DeltaAngel)
			{
				ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=DeltaAngel;
			}


			//e这段是根据下一个角读角差距
			for (int k=0;k< pedh->DopplerGates; k++)
			{
				ppdt->GetMyRadial_Data(i,j)[k]=pRadar->GetRadarDataW(i,j)[k];
			}
		}
	}
	logfile<<"256_W：  Finished. "<<endl;
	logfile.close();

	return GS_OK;
}
GHRESULT ARITH_256_W::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_256_W接口
{
	return new(::std::nothrow) ARITH_256_W;
}
