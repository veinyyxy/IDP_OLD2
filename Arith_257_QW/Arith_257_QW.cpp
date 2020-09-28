//////////////////////////////////////////////////////////////////////////
// Arith_257_QW.cpp
// created by ply 20080411
// 256号产品，基本谱宽，W
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include "Arith_257_QW.h"
#include "StandardRadarDataX.h"
#include "CJulMsGMT.h"
#include <fstream>
#include "IBase.h"
#include "GDef.h"

ARITH_257_QW::ARITH_257_QW()
{
	;
}
ARITH_257_QW::~ARITH_257_QW()
{
	;
}

GHRESULT ARITH_257_QW::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	return GS_OK;
}
GHRESULT ARITH_257_QW::LoadData(void *pValue,ReadParam* /*ReadParameter*/)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pInputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_257_QW::OutPutData(void * pValue)
{
	if(pValue == NULL)
		return GE_INVALIDARG;
	m_pOutputList=(GXList< GenerationData > *)pValue;
	return GS_OK;
}
GHRESULT ARITH_257_QW::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_257_QW.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"257_QW：  Started: "<<endl;
	logfile<<"257_QW：  Check m_pInputList's Count. "<<endl;

	if (m_pInputList->GetCount()!=1)
	{
		return GE_SHORTOFINDATA;
	}
	// int ElevationCount=((StandardRadarData*)(&(m_pInputList->GetHead())))->GetHeadP()->ElevationCount;

	logfile<<"257_QW：  Get and Set head Information from StandardRadarData. "<<endl;

	StandardRadarData* pRadar = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));
	GRADARDATAHEADER*    prdh = pRadar->GetHeadP();
	gInt16 ElevationCount = prdh->ElevationCount;

	m_pOutputList->AddTail();
	Product_257_QW* ppdt = &(*(m_pOutputList->GetTailAs<Product_257_QW>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(ElevationCount, pRadar->GetElevationHeadP(0)->AzimuthNumber, pRadar->GetElevationHeadP(0)->DopplerGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"257_QW：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
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
	logfile<<"257_QW：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8)* 2);
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8) * 4);
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
	//pdb->VCP=prdh->VCPMode;
	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	logfile<<"257_QW：  Set RadialData,ElevationHead and SpectrumWidth Data Information in each Elevation.\n";
	for (int i=0;i<ElevationCount;i++)
	{
		GELEVATIONDATAHEAD* pedh = pRadar->GetElevationHeadP(i);
		// if (((StandardRadarData*)(&(m_pInputList->GetHead())))->GetElevationHeadP(i)->DopplerGates<=0)
		if(pedh->DopplerGates <= 0)
		{
			continue;
		}
		//m_pOutputList->AddTail();
		//Product_257_QW* ppdt = &(*(m_pOutputList->GetTailAs<Product_257_QW>()));
		//ppdt->ConstructIt(pedh->AzimuthNumber, pedh->DopplerGates);
		//PDBlock* pdb = ppdt->GetMyPDBlock();
		//
		//pdb->ElevationNumber=(gInt16)i;
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
		//pdb->VCP=prdh->VCPMode;

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
			//((Product_251_R*)(&(m_pOutputList->GetTail())))->GetMyRadial_RadialData(j)->DeltaAngel=DeltaAngel;///平分全部角度做角差距

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
	logfile<<"257_QW：  Finished. "<<endl;
	logfile.close();

	return GS_OK;
}
GHRESULT ARITH_257_QW::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建ARITH_257_QW接口
{
	return new(::std::nothrow) ARITH_257_QW;
}
