//////////////////////////////////////////////////////
//Arith_266_VIL.cpp 
//created by qcsun  20080402
//
///////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_266_VIL.h"
#include <malloc.h>
#include <memory.h>
#include "CJulMsGMT.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

Arith_266_VIL::Arith_266_VIL(void)
{
}

Arith_266_VIL::~Arith_266_VIL(void)
{
}
GHRESULT Arith_266_VIL::Initialize()
{
	m_pGenerateProductList	= NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;
	return GS_OK;
}

GHRESULT Arith_266_VIL::UnInitialize()
{
	/*if (m_pGenerateProductList->GetCount() != 0)  //delete by dqc 20080522
	{
	m_pGenerateProductList->RemoveAll();
	}
	if(m_pOutputProductList->GetCount() != 0)
	{
	m_pOutputProductList->RemoveAll(); 
	}	*/
	return GS_OK;
}

GHRESULT Arith_266_VIL::LoadData(void *pValue,ReadParam * ReadParameter)
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

GHRESULT Arith_266_VIL::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_266_VIL.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"266_VIL：  Started: "<<endl;
	logfile<<"266_VIL：  Get Send-in StandardRadarData. "<<endl;

	StandardRadarData* pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));	
	gInt16   EchoTopRefThreshold = 18;
	gFloat   BeamWidth =  95/200;
	char * pTempName = NULL;

	logfile<<"266_VIL：  Read Parameter: EchoTopRefThreshold and BeamWidth. "<<endl;

	gInt16 ScanArea = 256;//默认256km
	logfile<<"266_VIL：  Create Send-out Product: Product_266_VIL."<<endl;

	gInt16  AzimuthNumber = 0;
	gInt16 MaxRefGateNumer = 0;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	gInt16 TempMaxRefGateNumer = 0;

	logfile<<"266_VIL：  Find Max Reference Gates Number. "<<endl;
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
	Product_266_VIL* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_266_VIL>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(pGData->GetElevationHeadP(0)->AzimuthNumber,TempMaxRefGateNumer/*pGData->GetElevationHeadP(0)->ReflectivityGates/2*/);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"266_VIL：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"266_VIL：  Initial Radial Data as RADAR_DATA_NOECHO."<<endl;
	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			(ppdt->GetMyRadial_Data(j))[k] = RADAR_DATA_NOECHO;
		}
	}

	logfile<<"266_VIL：  Call Function: pArith_266_VIL()."<<endl;

	//调用垂直液态水生成函数，
	pArith_266_VIL( pGData , EchoTopRefThreshold ,  BeamWidth , ppdt, MaxRefGateNumer);

	//将得到的产品加到 m_pOutputProductList中
	PDBlock* pdb = ppdt->GetMyPDBlock();
	GRADARDATAHEADER* prdh = pGData->GetHeadP();	

	logfile<<"266_VIL：  Get and set RadarInfor and Radial_Head Information."<<endl;
	//pdb->ElevationNumber = 0;
	memcpy(ppdt->GetRadarInfor()->RadarType, prdh->RadarType, sizeof(gInt8)*4);
	ppdt->GetRadarInfor()->Altitude = prdh->RadarHeight;
	ppdt->GetRadarInfor()->Latitude = prdh->RadarLat;
	ppdt->GetRadarInfor()->Longitude = prdh->RadarLon;
	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8) * 4);
	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8) *2);
	pdb->ProductDate = prdh->VCPDate;
	pdb->ProductTime = prdh->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"266_VIL：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	memcpy(ppdt->GetRadarInfor()->SiteCode,prdh->SiteCode,sizeof(gChar)*8);
	//memcpy(pdb->SiteName,prdh->SiteName,sizeof(gChar)*18);
	ppdt->GetMyDILayer()->HeightOfElevation=0;
	//ppdt->GetMyRadial_Head()->ScaleOfData = 100;
	ppdt->GetMyRadial_Head()->DistanceOfFirstGate = pGData->GetElevationHeadP(0)->FirstGateRangeOfRef;
	ppdt->GetMyRadial_Head()->LengthOfGate = pGData->GetHeadP()->ReflectivityGateSize;
	gUint16 RadialAzimuth = 0;
	gInt16 AzimuthNumberBound = AzimuthNumber - 1;

	logfile<<"266_VIL：  Loop: i=0-->AzimuthNumber(360) to set Radial_Data Angle Information."<<endl;
	for( int i = 0 ; i < AzimuthNumber ; i++ )
	{
		RadialAzimuth = pGData->GetRadarDataP(0, i)->Azimuth;
		ppdt->GetMyRadial_RadialData(i)->AzimuthAngle = RadialAzimuth;
		//if ( i == AzimuthNumberBound )
		//{
		//	ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,0)->Azimuth + 36000 - RadialAzimuth )%36000;
		//}
		//else
		//{
		//	ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
		//}
		ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( i == AzimuthNumberBound )? ( pGData->GetRadarDataP(0,0)->Azimuth + 36000 - RadialAzimuth )%36000\
			:( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
	}

	logfile<<"266_VIL：  Loop Finished."<<endl;
	logfile<<"266_VIL：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

GHRESULT Arith_266_VIL::OutPutData(void * pValue)
{
	if( pValue == NULL ) 
		return GE_INVALIDARG;
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return  GS_OK;
}

void  Arith_266_VIL::pArith_266_VIL( StandardRadarData *pGData , gInt16 EchoTopRefThreshold , gFloat BeamWidth ,Product_266_VIL * pRadialData, gInt16 MaxRefGateNumer )
{	
	gInt16 AzimuthNumber = 0;
	//gInt16 MaxRefGateNumer = 0;
	gInt16 ElevationCount = 0;
	gFloat *ElevationSin= NULL;
	gInt16 * RadialDataR = NULL;
	gFloat TempSin = 0;
	gFloat Exponential =(gFloat)0.57142857; //(4.0/7.0)
	gInt16  ReflectivityGateSize = 0;
	ReflectivityGateSize = pGData->GetHeadP()->ReflectivityGateSize;
	TempSin = (gFloat)sin(BeamWidth * M_PI/36000.0 );
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	//MaxRefGateNumer = pGData->GetHeadP()->maxRefgatenum/2;
	ElevationCount =  pGData->GetHeadP()->ElevationCount;
	ElevationSin =  new(::std::nothrow) gFloat[ElevationCount];
	RadialDataR =  new(::std::nothrow) gInt16[ElevationCount];
	gFloat *Temp =  new(::std::nothrow) gFloat[ElevationCount];
	gFloat ElevationCos =(gFloat) cos(pGData->GetElevationHeadP(0)->Elevation*M_PI/18000.0);
	gInt16 *ReflectivityGates =new(::std::nothrow) gInt16[ElevationCount];
	for( gInt16  i = 0 ; i < ElevationCount ; i++  )
	{
		ElevationSin[i] = (gFloat)sin((pGData->GetElevationHeadP(i)->Elevation) *M_PI/18000.0) ;
	}
	for( gInt16  i =  0 ; i < ElevationCount; i++ )
	{
		ReflectivityGates[i] = pGData->GetElevationHeadP(i)->ReflectivityGates;
		Temp[i] =(gFloat)( ElevationCos /cos(pGData->GetElevationHeadP(i)->Elevation*M_PI/18000.0) );
	}

	for( gInt16 j = 0 ; j < AzimuthNumber; j++ )  //num_Beam每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k< MaxRefGateNumer ; k++ )  // 径向距离库数
		{

			/*---------以下求给定阈值的回波顶、回波底及其所在仰角和回波厚度---------------*/
			gInt32  TempVIL = 0;		
			/*gFloat  TempVIL = 0;*/
			gInt16  EchoTipEvl = 0 ;    //回波顶高所在仰角
			gInt16  EchoBottomEvl = 0 ;    //回波底高所在仰角	
			for( gInt16  i = 0 ; i < ElevationCount ; i++)
			{
				gInt16 k1=(gInt16)(k*Temp[i]);
				RadialDataR[i] = (j < pGData->GetElevationHeadP(i)->AzimuthNumber && k1 < ReflectivityGates[i]) \
					? pGData->GetRadarDataR(i,j)[k1]/100 : RADAR_DATA_NOECHO;
			}
			for( gInt16  i = 0 ; i < ElevationCount ; i++)
			{		
				if( RadialDataR[i] > EchoTopRefThreshold )
				{
					EchoBottomEvl = i;   //求回波底所在仰角								
					break;
				}
			}
			for( gInt16 i = (ElevationCount - 1) ;  i >= 0; i-- )
			{
				if( RadialDataR[i]  > EchoTopRefThreshold )
				{
					EchoTipEvl = i ;  //求回波顶所在仰角			
					break;
				}
			}	
			//--------以下求VIL----------------------
			gInt32  EchoBottomVIL = 0 ;
			gInt32	EchoTipVIL = 0 ;
			gFloat  NumOne = 0 ;
			gFloat  NumTwo = 0 ;
			for( gInt16  i = 0 ; i <  (ElevationCount - 1) ; i++ )
			{
				if( ( RadialDataR[i]  > EchoTopRefThreshold ) && ( RadialDataR[i+1] > EchoTopRefThreshold ) )
				{			
					NumOne = (gFloat)pow(  10.0 ,RadialDataR[i]/ 10) ;
					NumTwo = (gFloat)pow(  10.0 , RadialDataR[i+1] / 10 ) ;

					//----------回波底对应的仰角取半个波束宽度计算VIL--------
					if( i == EchoBottomEvl )
					{		
						EchoBottomVIL = ( gInt32 )(344* pow ( NumOne , Exponential ) *k*Temp[i] * TempSin/1000)*ReflectivityGateSize;
					}
					//----------回波顶对应的仰角取半个波束宽度计算VIL--------
					if( i == EchoTipEvl )
					{
						EchoTipVIL = ( gInt32 )(344* pow ( NumTwo , Exponential ) *k*Temp[i] * TempSin/1000 )*ReflectivityGateSize;
					}  
					//------------计算回波顶和回波底之间的VIL--------------	
					if( EchoTipEvl  > EchoBottomEvl && i >= EchoBottomEvl && i < EchoTipEvl )
					{
						TempVIL += ( gInt32 ) ( 344 * pow ( ( NumOne + NumTwo ) / 2 , Exponential ) * (k*Temp[i+1] * ElevationSin[i+1]- k*Temp[i]* ElevationSin[i]) /1000)*ReflectivityGateSize;
					}
				}				
			}
			if(TempVIL!= 0||EchoTipVIL!=0||EchoBottomVIL!= 0)
				pRadialData->GetMyRadial_Data(j)[k] = ( gInt16 )((TempVIL +EchoTipVIL + EchoBottomVIL )/1000);	
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


extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_266_VIL接口
{
	return  new(::std::nothrow) Arith_266_VIL;
}
