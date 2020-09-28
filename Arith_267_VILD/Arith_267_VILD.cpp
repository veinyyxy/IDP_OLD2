/////////////////////////////////////////////////////
//Arith_267_VILD.cpp 
//created by qcsun  20080402
//
///////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_267_VILD.h"
#include <malloc.h>
#include <memory.h>
#include "CJulMsGMT.h"
#include <stdio.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

Arith_267_VILD::Arith_267_VILD(void)
{
}

Arith_267_VILD::~Arith_267_VILD(void)
{
}
GHRESULT Arith_267_VILD::Initialize()
{
	m_pGenerateProductList	= NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;
	return GS_OK;
}

GHRESULT Arith_267_VILD::UnInitialize()
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

GHRESULT Arith_267_VILD::LoadData(void *pValue,ReadParam * ReadParameter)
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

GHRESULT Arith_267_VILD::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_267_VILD.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"267_VILD：  Started: "<<endl;
	logfile<<"267_VILD：  Get Send-in StandardRadarData. "<<endl;

	StandardRadarData* pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	gInt16   EchoTopRefThreshold = 18;

	logfile<<"267_VILD：  Read Parameter: EchoTopRefThreshold. "<<endl;
	gInt16 ScanArea = 256;//默认256km
	logfile<<"267_VILD：  Create Send-out Product: Product_267_VILD. "<<endl;

	gInt16  AzimuthNumber = 0;
	gInt16 MaxRefGateNumer = 0;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	gInt16 TempMaxRefGateNumer = 0;

	logfile<<"267_VILD：  Find Max Reference Gates Number. "<<endl;
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

	m_pOutputProductList->AddTail(); 
	Product_267_VILD* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_267_VILD>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(pGData->GetElevationHeadP(0)->AzimuthNumber,MaxRefGateNumer);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"267_VILD：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}
	logfile<<"267_VILD：  Initial Radial Data as RADAR_DATA_NOECHO."<<endl;
	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			(ppdt->GetMyRadial_Data(j))[k] = RADAR_DATA_NOECHO;
		}
	}

	logfile<<"267_VILD：  Call Function: pArith_267_VILD()."<<endl;
	//调用垂直液态水含量密度生成函数，
	pArith_267_VILD( pGData,  EchoTopRefThreshold,  ppdt, MaxRefGateNumer);
	//pTempName=NULL;
	//将得到的产品加到 m_pOutputProductList中

	logfile<<"267_VILD：  Get and Set RadarInfor and Radial_Head Information."<<endl;
	PDBlock* pdb = ppdt->GetMyPDBlock();
	//pdb->ElevationNumber = 0;
	memcpy(ppdt->GetRadarInfor()->RadarType, pGData->GetHeadP()->RadarType, sizeof(gInt8) * 4);
	ppdt->GetRadarInfor()->Altitude = pGData->GetHeadP()->RadarHeight;
	ppdt->GetRadarInfor()->Latitude = pGData->GetHeadP()->RadarLat;
	ppdt->GetRadarInfor()->Longitude = pGData->GetHeadP()->RadarLon;
	memcpy(pdb->VCPMode, pGData->GetHeadP()->VCPMode, sizeof(gInt8) * 4);
	memcpy(pdb->OperationalMode, pGData->GetHeadP()->OperationMode, sizeof(gInt8) * 2);
	pdb->ProductDate = pGData->GetHeadP()->VCPDate;
	pdb->ProductTime = pGData->GetHeadP()->VCPTime;	

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"267_VILD：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ProductCode = 267;
	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	memcpy(ppdt->GetRadarInfor()->SiteCode,pGData->GetHeadP()->SiteCode,sizeof(gChar)*8);
	//memcpy(pdb->SiteName,pGData->GetHeadP()->SiteName,sizeof(gChar)*18);
	ppdt->GetMyDILayer()->HeightOfElevation=0;
	//ppdt->GetMyRadial_Head()->ScaleOfData = 100;
	ppdt->GetMyRadial_Head()->DistanceOfFirstGate = pGData->GetElevationHeadP(0)->FirstGateRangeOfRef;
	ppdt->GetMyRadial_Head()->LengthOfGate = pGData->GetHeadP()->ReflectivityGateSize;	
	gUint16 RadialAzimuth = 0;
	gInt16 AzimuthNumberBound = AzimuthNumber - 1;

	logfile<<"267_VILD：  Loop: i=0-->AzimuthNumber(360) to set Radial_Data Angle Information."<<endl;
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
		ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( i == AzimuthNumberBound )? ( pGData->GetRadarDataP(0,0)->Azimuth + \
			36000 - RadialAzimuth )%36000: ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
	}
	//end of 处理
	logfile<<"267_VILD：  Loop Finished."<<endl;
	logfile<<"267_VILD：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

GHRESULT Arith_267_VILD::OutPutData(void * pValue)
{
	if( pValue == NULL ) 
		return GE_INVALIDARG;
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	//return GE_NOTIMPL;
	return GS_OK;  //changed by dqc 20080522
}

void  Arith_267_VILD::pArith_267_VILD( StandardRadarData *pGData , gInt16 EchoTopRefThreshold  , Product_267_VILD * pRadialData, gInt16 MaxRefGateNumer )
{
	gInt16 AzimuthNumber = 0;
	//gInt16 MaxRefGateNumer = 0;
	gInt16 ElevationCount = 0;
	gInt16 ElevationCountBound = 0;
	gFloat *ElevationSin= NULL;
	gInt16 * RadialDataR = NULL;
	gInt16 ReflectivityGateSize =0;
	gInt16 RadarHeight = 0;
	gFloat Exponential =(gFloat)0.57142857; //(4.0/7.0)
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	RadarHeight = pGData->GetHeadP()->RadarHeight/10;
	ReflectivityGateSize = pGData->GetHeadP()->ReflectivityGateSize/1000;
	//MaxRefGateNumer = pGData->GetHeadP()->maxRefgatenum/2;
	ElevationCount =  pGData->GetHeadP()->ElevationCount;
	ElevationSin = new (::std::nothrow)gFloat[ElevationCount];
	RadialDataR = new (::std::nothrow)gInt16[ElevationCount];
	ElevationCountBound = ElevationCount - 1;
	for( gInt16  i = 0 ; i < ElevationCount ; i++ )
	{
		ElevationSin[i] = (gFloat)sin((pGData->GetElevationHeadP(i)->Elevation/100) * M_PI/180.0) ;
	}

	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //num_Beam每个仰角层的径向数
	{
		for( gInt16  k=0 ; k< MaxRefGateNumer ; k++ )  // 径向距离库数
		{
			//---------以下求给定阈值的回波顶、回波底及其所在仰角和回波厚度---------------
			gFloat  TempVIL = 0;
			gInt16  EchoTip = 0 ;
			gInt16  EchoBottom = 0 ;
			gInt16  EchoTipEvl = 0 ;    //回波顶高所在仰角
			gInt16  EchoBottomEvl = 0 ;    //回波底高所在仰角
			gInt16 EchoThickness =0; //雷达探测的回波厚度	
			for( gInt16  i = 0 ; i < ElevationCount ; i++)
			{
				RadialDataR[i] = (j < pGData->GetElevationHeadP(i)->AzimuthNumber && k < pGData->GetElevationHeadP(i)->ReflectivityGates) \
					? pGData->GetRadarDataR(i,j)[k]/100 : RADAR_DATA_NOECHO;
			}
			for( gInt16  i = 0 ; i < ElevationCount ; i++) 
			{
				if( RadialDataR[i] >EchoTopRefThreshold )
				{
					EchoBottom = ( gInt16 )((RadarHeight/10000+k * ( ReflectivityGateSize) * ElevationSin[i]+ k * ( ReflectivityGateSize ) * k * (ReflectivityGateSize) /17000));  //求回波底
					EchoBottomEvl = i;   //求回波底所在仰角
					break;
				}
			}
			for( gInt16 i = ElevationCountBound ;  i >= 0; i-- )
			{		
				if( RadialDataR[i] > EchoTopRefThreshold )	
				{
					EchoTip =(gInt16)((RadarHeight/10000+ k * (ReflectivityGateSize ) * ElevationSin[i] +k*k/17000));  //求回波顶
					EchoTipEvl = i ;  //求回波顶所在仰角
					break;
				}
			}
			EchoThickness = EchoTip - EchoBottom ;   
			gFloat  NumOne = 0 ;
			gFloat  NumTwo = 0 ;
			for( gInt16  i = 0 ; i < ElevationCountBound ; i++ )
			{
				if( ( RadialDataR[i] > EchoTopRefThreshold ) && (RadialDataR[i+1] > EchoTopRefThreshold ) )
				{

					NumOne =(gFloat) pow( 10.0 , RadialDataR[i]/10) ; 
					NumTwo =(gFloat) pow( 10.0 , RadialDataR[i+1] /10) ;			
					if( EchoTipEvl  > EchoBottomEvl && i >= EchoBottomEvl && i < EchoTipEvl )
					{
						TempVIL +=  ( 344  * pow ( (NumOne + NumTwo)/ 2, Exponential ) * k * ( ElevationSin[i+1] - ElevationSin[i] ))*ReflectivityGateSize;
					}
				}
			}
			//--------以下求VILD----------------------				
			//if ( EchoThickness <= 0 || ( EchoTipEvl - EchoBottomEvl ) < 2 ) 			
			//		pRadialData->GetMyRadial_Data(j)[k] = RADAR_DATA_NOECHO;
			//else 
			//	pRadialData->GetMyRadial_Data(j)[k] = ( gInt16 ) ( 100 * (TempVIL/100000)/(EchoThickness)+ 50 );
			pRadialData->GetMyRadial_Data(j)[k] = ( EchoThickness <= 0 || ( EchoTipEvl - EchoBottomEvl ) < 2 )? RADAR_DATA_NOECHO\
				: ( gInt16 ) ( 100 * (TempVIL/100000)/(EchoThickness)+ 50 );
		}
	}
	if(RadialDataR != NULL)
	{
		delete[] RadialDataR;
		RadialDataR = NULL;
	}
	if(ElevationSin != NULL)
	{
		delete[] ElevationSin;
		ElevationSin =NULL;
	}
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_267_VILD接口
{
	return new(::std::nothrow) Arith_267_VILD;
}
