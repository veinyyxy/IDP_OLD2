//////////////////////////////////////////////////////
//Arith_264_CRH.cpp 
//created by qcsun  20080403
//
///////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_264_CRH.h"
#include <malloc.h>
#include <memory.h>
#include "CJulMsGMT.h"
#include <stdio.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

Arith_264_CRH::Arith_264_CRH(void)
{

}

Arith_264_CRH::~Arith_264_CRH(void)
{
}

GHRESULT Arith_264_CRH::Initialize()
{
	m_pGenerateProductList	= NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;

	return GS_OK;
}

GHRESULT Arith_264_CRH::UnInitialize()
{
	/*if (m_pGenerateProductList->GetCount() != 0) //delete by dqc 20080522
	{
	m_pGenerateProductList->RemoveAll();
	}
	if(m_pOutputProductList->GetCount() != 0)
	{
	m_pOutputProductList->RemoveAll(); 
	}*/
	return GS_OK;
}

GHRESULT Arith_264_CRH::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if (pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pGenerateProductList = (GXList< GenerationData > *)pValue;//取得输入数据列表
	if (ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pParameter = ReadParameter;	//取得参数函数指针
	return GS_OK;
}

GHRESULT Arith_264_CRH::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_264_CRH.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"264_CRH：  Started: "<<endl;

	logfile<<"264_CRH：  Read Parameter of CRThreshold."<<endl;
	gInt16 CRThreshold = 0;
	gInt16 ScanArea = 256;//默认256km
	logfile<<"264_CRH：  Get Send-in Data."<<endl;

	StandardRadarData* pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	//处理数据
	m_pOutputProductList->AddTail();
	Product_264_CRH* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_264_CRH>()));

	logfile<<"264_CRH：  Create Send-out Data. "<<endl;

	gInt16  AzimuthNumber = 0;
	gInt16 TempMaxRefGateNumer = 0;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;

	logfile<<"264_CRH：  Loop form 0-->ElevationCount: Find MaxRefGateNumber."<<endl;
	for(gInt16 i = 0; i< pGData->GetHeadP()->ElevationCount; i++)
	{
		gInt16 TempNumOfGates = pGData->GetElevationHeadP(i)->ReflectivityGates;
		if(TempMaxRefGateNumer < TempNumOfGates)
		{		
			TempMaxRefGateNumer = TempNumOfGates;
		}
	}
	gInt32 TempGateNum =0;
	TempGateNum = ScanArea*1000/(pGData->GetHeadP()->ReflectivityGateSize);
	TempMaxRefGateNumer = TempGateNum < (TempMaxRefGateNumer)? TempGateNum:TempMaxRefGateNumer;

	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(pGData->GetElevationHeadP(0)->AzimuthNumber,
		TempMaxRefGateNumer/*pGData->GetElevationHeadP(0)->ReflectivityGates/2*/);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"264_CRH：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"264_CRH：  Loop Finished."<<endl;

	gInt16 MaxRefGateNumer = TempMaxRefGateNumer;

	logfile<<"264_CRH：  Initial Product Radial Data as RADAR_DATA_NOECHO."<<endl;
	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			(ppdt->GetMyRadial_Data(j))[k] = RADAR_DATA_NOECHO;
		}
	}

	//调用回波顶产品生成函数，
	logfile<<"264_CRH：  Call Function: pArith_264_CRH( )."<<endl;

	pArith_264_CRH( pGData, ppdt,CRThreshold, MaxRefGateNumer); 

	logfile<<"264_CRH：  Get and set Radar and Radial Head Information."<<endl;
	PDBlock* pdb = ppdt->GetMyPDBlock();
	GRADARDATAHEADER* prdh = pGData->GetHeadP();
	//pdb->ElevationNumber = 0;
	memcpy(ppdt->GetRadarInfor()->RadarType, prdh->RadarType, sizeof(gInt8) * 4);
	ppdt->GetRadarInfor()->Altitude = prdh->RadarHeight;
	ppdt->GetRadarInfor()->Latitude = prdh->RadarLat;
	ppdt->GetRadarInfor()->Longitude = prdh->RadarLon;
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8) * 4);
	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8)* 2);
	pdb->ProductDate = prdh->VCPDate;
	pdb->ProductTime = prdh->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"264_CRH：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	memcpy(ppdt->GetRadarInfor()->SiteCode,prdh->SiteCode,sizeof(gChar)*8);
	/*  memcpy(pdb->SiteName,prdh->SiteName,sizeof(gChar)*18);*/	
	ppdt->GetMyDILayer()->HeightOfElevation = 0;
	//ppdt->GetMyRadial_Head()->ScaleOfData = 100 ;
	ppdt->GetMyRadial_Head()->DistanceOfFirstGate = pGData->GetElevationHeadP(0)->FirstGateRangeOfRef;
	ppdt->GetMyRadial_Head()->LengthOfGate = pGData->GetHeadP()->ReflectivityGateSize;
	gUint16 RadialAzimuth = 0;
	gInt16 AzimuthNumberBound = AzimuthNumber - 1;

	logfile<<"264_CRH：  Loop: i=0-->AzimuthNumber(360) to set Radial_Data Angle Information."<<endl;
	for( int i = 0 ; i < AzimuthNumber ; i++ )
	{
		RadialAzimuth = pGData->GetRadarDataP(0, i)->Azimuth;
		ppdt->GetMyRadial_RadialData(i)->AzimuthAngle = RadialAzimuth;
		//if ( i == AzimuthNumberBound )
		//{
		// ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,0)->Azimuth + 36000 - RadialAzimuth )%36000;
		//}
		//else
		//{
		// ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
		//}
		ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( i == AzimuthNumberBound )? ( pGData->GetRadarDataP(0,0)->Azimuth + 36000 \
			- RadialAzimuth )%36000 : ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
	}
	logfile<<"264_CRH：  Loop Finished."<<endl;
	//end of 处理

	logfile<<"264_CRH：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

GHRESULT Arith_264_CRH::OutPutData(void * pValue)
{
	if( pValue == NULL ) 
		return GE_INVALIDARG;
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return GS_OK;
}

void  Arith_264_CRH::pArith_264_CRH( StandardRadarData * pGData , Product_264_CRH * pRadialData, gInt16 CRThreshold, gInt16 MaxRefGateNumer )
{	
	gInt16 k1 = 0;
	gFloat temDou = 0;
	gInt16 AzimuthNumber = 0;
	//gInt16 MaxRefGateNumer = 0;
	gInt16 ElevationCount = 0;
	gInt16 *ReflectivityGates = NULL;
	gFloat *Temp = NULL;
	gInt16 * RadialDataR = NULL;
	gFloat *ElevationSin= NULL;
	gFloat ElevationCos = 0;
	gInt16 RadarHeight = 0;
	gInt16 ReflectivityGateSize =0;
	RadarHeight = pGData->GetHeadP()->RadarHeight/10;
	ReflectivityGateSize = pGData->GetHeadP()->ReflectivityGateSize;
	ElevationCos = (gFloat)cos(pGData->GetElevationHeadP(0)->Elevation*M_PI/18000.0);
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	//MaxRefGateNumer = pGData->GetHeadP()->maxRefgatenum/2 ;
	ElevationCount =  pGData->GetHeadP()->ElevationCount;
	ReflectivityGates = new (::std::nothrow)gInt16[ElevationCount];
	Temp = new (::std::nothrow)gFloat[ElevationCount];
	RadialDataR = new (::std::nothrow)gInt16[ElevationCount];
	ElevationSin = new (::std::nothrow)gFloat[ElevationCount];
	for( gInt16  i = 0 ; i < ElevationCount; i++ )
	{
		ReflectivityGates[i] = pGData->GetElevationHeadP(i)->ReflectivityGates;
		Temp[i] =(gFloat)( ElevationCos /cos(pGData->GetElevationHeadP(i)->Elevation*M_PI/18000.0) );
		ElevationSin[i] =(gFloat) sin((pGData->GetElevationHeadP(i)->Elevation) * M_PI/18000.0) ;
	}
	for( gInt16 j = 0 ; j < AzimuthNumber; j++ ) 
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  
		{	
			gInt16 MaxRadarDataR = RADAR_DATA_NOECHO;
			gInt16 MaxI =0;
			for( gInt16  i = ElevationCount - 1 ; i >=0; i-- )
			{
				temDou=(gFloat)((k*Temp[i]));	
				if ( temDou < ReflectivityGates[i] && j < pGData->GetElevationHeadP(i)->AzimuthNumber\
					&& k < pGData->GetElevationHeadP(i)->ReflectivityGates)
				{
					k1=(gInt16)temDou;

					RadialDataR [i] = pGData->GetRadarDataR(i,j)[k1] ;
					if( ( RadialDataR [i] ) > RADAR_DATA_NOECHO )
					{
						if( ( RadialDataR [i] ) > MaxRadarDataR )
						{
							MaxRadarDataR = RadialDataR [i] ;
							MaxI = i;
						}		
					}
				}
			}
			if(MaxRadarDataR != RADAR_DATA_NOECHO && MaxRadarDataR/100 >= CRThreshold)
				pRadialData->GetMyRadial_Data(j)[k]  = ( gInt16 )(((RadarHeight/10 + k * ( ReflectivityGateSize ) *ElevationSin[MaxI] 
			+ k * (ReflectivityGateSize) * k * ( ReflectivityGateSize/1000)/17000))/10); 		
		}		
	}
	if(ReflectivityGates != NULL)
	{
		delete[] ReflectivityGates;
		ReflectivityGates = NULL;
	}
	if(Temp != NULL)
	{
		delete[] Temp;
		Temp = NULL;
	}
	if(RadialDataR != NULL)
	{
		delete[] RadialDataR;
		RadialDataR = NULL;
	}
	if(ElevationSin != NULL)
	{
		delete[] ElevationSin;
		ElevationSin = NULL;
	}
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_264_CRH接口
{
	return new (::std::nothrow)Arith_264_CRH;
}
