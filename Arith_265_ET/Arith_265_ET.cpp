//////////////////////////////////////////////////////
//Arith_265_ET.cpp 
//created by qcsun  20080401
//
///////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_265_ET.h"
#include <malloc.h>
#include <memory.h>
#include "CJulMsGMT.h"
#include <stdio.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

Arith_265_ET::Arith_265_ET(void)
{	
}
Arith_265_ET::~Arith_265_ET(void)
{
}

GHRESULT Arith_265_ET::Initialize()
{
	m_pGenerateProductList	= NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;
	return GS_OK;
}

GHRESULT Arith_265_ET::UnInitialize()
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

GHRESULT Arith_265_ET::LoadData(void *pValue,ReadParam *ReadParameter)
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

GHRESULT Arith_265_ET::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_265_ET.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"265_ET：  Started: "<<endl;
	logfile<<"265_ET：  Get Send-in StandardRadarData. "<<endl;

	StandardRadarData* pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	//处理数据
	logfile<<"265_ET：  Read Parameter: EchoTopRefThreshold. "<<endl;

	gInt16  EchoTopRefThreshold = 18 ;//默认回波顶阀值
	gInt16 ScanArea = 256;//默认256km

	gInt16  AzimuthNumber = 0;
	gInt16 MaxRefGateNumer = 0;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	gInt16 TempMaxRefGateNumer = 0;

	logfile<<"265_ET：  Find Max Reference Gates Number. "<<endl;
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
	MaxRefGateNumer = TempMaxRefGateNumer;

	//存放回波顶数据
	m_pOutputProductList->AddTail();
	Product_265_ET* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_265_ET>()));
	GHRESULT ConstrutHand;

	logfile<<"265_ET：  Create Send-out Product. "<<endl;

	ConstrutHand = ppdt->ConstructIt(pGData->GetElevationHeadP(0)->AzimuthNumber, 
		TempMaxRefGateNumer/*pGData->GetElevationHeadP(0)->ReflectivityGates/2*/);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"265_ET：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}


	logfile<<"265_ET：  Initial Radial Data as RADAR_DATA_NOECHO."<<endl;
	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			(ppdt->GetMyRadial_Data(j))[k] = RADAR_DATA_NOECHO;
		}
	}		
	//调用回波顶产品生成函数，

	logfile<<"265_ET：  Call Function: pArith_265_ET()."<<endl;

	pArith_265_ET(pGData, EchoTopRefThreshold, ppdt, MaxRefGateNumer);
	//将得到的产品加到 m_pOutputProductList中

	logfile<<"265_ET：  Get and Set RadarInfor and Radial_Head Information."<<endl;
	PDBlock* pdb = ppdt->GetMyPDBlock();	
	//pdb->ElevationNumber = 0;
	GRADARDATAHEADER* prdh = pGData->GetHeadP();
	memcpy(ppdt->GetRadarInfor()->RadarType, prdh->RadarType, sizeof(gInt8) * 4);
	ppdt->GetRadarInfor()->Altitude = prdh->RadarHeight;
	ppdt->GetRadarInfor()->Latitude = prdh->RadarLat;
	ppdt->GetRadarInfor()->Longitude = prdh->RadarLon;
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8) * 4);
	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8) * 2);
	pdb->ProductDate = prdh->VCPDate;
	pdb->ProductTime = prdh->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"265_ET：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	memcpy(ppdt->GetRadarInfor()->SiteCode, prdh->SiteCode, sizeof(gChar)*8);
	//memcpy(pdb->SiteName, prdh->SiteName, sizeof(gChar)*18);
	ppdt->GetMyDILayer()->HeightOfElevation = 0;
	//ppdt->GetMyRadial_Head()->ScaleOfData = 100 ;
	ppdt->GetMyRadial_Head()->DistanceOfFirstGate = pGData->GetElevationHeadP(0)->FirstGateRangeOfRef;
	ppdt->GetMyRadial_Head()->LengthOfGate = pGData->GetHeadP()->ReflectivityGateSize;
	gUint16 RadialAzimuth = 0;
	gInt16 AzimuthNumberBound = AzimuthNumber - 1;

	logfile<<"265_ET：  Loop: i=0-->AzimuthNumber(360) to set Radial_Data Angle Information."<<endl;
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
		ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( i == AzimuthNumberBound )? ( pGData->GetRadarDataP(0,0)->Azimuth + 36000\
			- RadialAzimuth )%36000 : ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
	}
	//end of 处理
	logfile<<"265_ET：  Loop Finished."<<endl;
	logfile<<"265_ET：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

GHRESULT Arith_265_ET::OutPutData(void * pValue)
{
	if( pValue == NULL ) 
		return GE_INVALIDARG;
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return GS_OK;
}

void  Arith_265_ET::pArith_265_ET( StandardRadarData * pGData,gInt16  EchoTopRefThreshold,Product_265_ET * pRadialData, gInt16 MaxRefGateNumer)
{
	gInt16 AzimuthNumber = 0;
	//gInt16 MaxRefGateNumer = 0;
	gInt16 ElevationCount = 0;
	gInt16 ReflectivityGateSize =0;
	gInt16 RadarHeight = 0;
	gFloat *ElevationSin= NULL;
	gInt16 * RadialDataR = NULL;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	//MaxRefGateNumer = pGData->GetHeadP()->maxRefgatenum/2 ;
	ReflectivityGateSize = pGData->GetHeadP()->ReflectivityGateSize;
	RadarHeight = pGData->GetHeadP()->RadarHeight;
	ElevationCount =  pGData->GetHeadP()->ElevationCount;
	ElevationSin =  new(::std::nothrow) gFloat[ElevationCount];
	RadialDataR =  new(::std::nothrow) gInt16[ElevationCount];
	for( gInt16  i = 0  ; i < ElevationCount; i++ )
	{
		ElevationSin[i] =(gFloat) sin((pGData->GetElevationHeadP(i)->Elevation) * M_PI/18000.0) ;
	}

	for( gInt16 j = 0 ; j<AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			//----------以下求回波顶高------------------------------------		
			gInt16 EechoTopElevationNum = 0;	
			for( gInt16  i = 0 ; i < ElevationCount ; i++)
			{
				RadialDataR[i] = (j <pGData->GetElevationHeadP(i)->AzimuthNumber && k <pGData->GetElevationHeadP(i)->ReflectivityGates)?\
					pGData->GetRadarDataR(i,j)[k]:RADAR_DATA_NOECHO;
			} 
			for( gInt16 i = ( ElevationCount-1 ) ; i>=0 ; i-- )
			{
				if( RadialDataR[i]/100  >=  EchoTopRefThreshold)
				{
					EechoTopElevationNum = i + 1;		
					break;
				}
			}
			//以下求回波顶高（以反射率因子为权重进行了垂直内插）-----------		
			gFloat EchoTopOne=0;
			gFloat  EchoTopTwo=0;
			gFloat NumOne = 0;
			gFloat  NumTwo = 0;
			if( EechoTopElevationNum ==  ElevationCount )  //如果最高仰角的反射率因子大于ETTresRef，那么回波顶高等于最高仰角波束轴线所对应的高度，这个高度不是真正的云顶高度
			{	
				pRadialData->GetMyRadial_Data(j)[k]  = ( gInt16 ) ((RadarHeight/100.0 +
					k * ( ReflectivityGateSize) *ElevationSin[EechoTopElevationNum-1] 
				+ k * ( ReflectivityGateSize) * k * ( ReflectivityGateSize /1000.0)/17000.0))/10.0;	
			}
			else if( EechoTopElevationNum == 0 ) 

				pRadialData->GetMyRadial_Data(j)[k]  = RADAR_DATA_NOECHO;	
			else
			{
				if(RadialDataR[EechoTopElevationNum-1] == RADAR_DATA_NOECHO &&( RadialDataR[EechoTopElevationNum-1] -RadialDataR[EechoTopElevationNum]) == 0)
					pRadialData->GetMyRadial_Data(j)[k]  = RADAR_DATA_NOECHO ;
				else
				{
					EchoTopOne =(gInt16)( RadarHeight/100.0 + k * (ReflectivityGateSize)* ElevationSin[EechoTopElevationNum-1]
					+ k * (ReflectivityGateSize) * k * (ReflectivityGateSize/1000.0) /17000.0);
					EchoTopTwo =(gInt16)(RadarHeight/100 + k * (ReflectivityGateSize) * ElevationSin[EechoTopElevationNum]
					+ k * (ReflectivityGateSize)  * k * (ReflectivityGateSize/1000.0) /17000.0);
					NumOne   = (( RadialDataR[EechoTopElevationNum-1]/100.0 - EchoTopRefThreshold)*10000.0/ ( RadialDataR[EechoTopElevationNum-1]/100.0 -RadialDataR[EechoTopElevationNum]/100.0 ));			
					NumTwo   = (( EchoTopRefThreshold - RadialDataR[EechoTopElevationNum]/100.0)*10000.0/( RadialDataR[EechoTopElevationNum-1]/100.0 -RadialDataR[EechoTopElevationNum]/100.0 ));			
					pRadialData->GetMyRadial_Data(j)[k]  = ( gInt16 )( 100 * (EchoTopTwo/1000.0*NumOne/10000.0 + EchoTopOne/1000.0*NumTwo/10000.0));	
				}
			}	
		}
	}
	if(ElevationSin != NULL) 
	{
		delete[] ElevationSin;
		ElevationSin =NULL;
	}
	if(RadialDataR != NULL)
	{
		delete[] RadialDataR;
		RadialDataR = NULL;
	}
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_265_ET接口
{
	return  new(::std::nothrow) Arith_265_ET;
}
