//////////////////////////////////////////////////////
//Arith_263_CR.cpp 
//created by qcsun  20080401
//
///////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_263_CR.h"
#include <malloc.h>
#include <memory.h>
#include "CJulMsGMT.h"
#include "IBase.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

Arith_263_CR::Arith_263_CR(void)
{

}

Arith_263_CR::~Arith_263_CR(void)
{

}

GHRESULT Arith_263_CR::Initialize()
{
	m_pGenerateProductList	= NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;

	return GS_OK;
}

GHRESULT Arith_263_CR::UnInitialize()
{
	/*if (m_pGenerateProductList->GetCount() != 0)  //delete by dqc 20080522
	{
	m_pGenerateProductList->RemoveAll();
	}
	if(m_pOutputProductList->GetCount() != 0)
	{
	m_pOutputProductList->RemoveAll(); 
	}*/
	return GS_OK;
}

GHRESULT Arith_263_CR::LoadData(void *pValue,ReadParam* ReadParameter) 
{
	if (pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;//取得输入数据列表
	if (ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pParameter = ReadParameter;	//取得参数函数指针
	return GS_OK;
}

GHRESULT Arith_263_CR::Execute()
{	
	ofstream logfile;
	logfile.open("./Log/Arith_263_CR.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"263_CR：  Started: "<<endl;
	logfile<<"263_CR：  Get Head and Elevation Head Information from StandardRadarData. "<<endl;

	StandardRadarData* pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	//处理数据
	//存放回波顶数据
	m_pOutputProductList->AddTail();
	Product_263_CR* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_263_CR>()));
	gInt16 maxRefgatenum = 0;

	logfile<<"263_CR：  Find MaxRefgatenum. "<<endl;
	for(gInt16 i = 0; i< pGData->GetHeadP()->ElevationCount; i++)
	{
		gInt16 TempNumOfGates = pGData->GetElevationHeadP(i)->ReflectivityGates;
		if(maxRefgatenum < TempNumOfGates)
		{		
			maxRefgatenum = TempNumOfGates;
		}
	}

	logfile<<"263_CR：  ConstructIt(). "<<endl;
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(pGData->GetElevationHeadP(0)->AzimuthNumber, maxRefgatenum );	
	if(ConstrutHand != GS_OK)
	{
		logfile<<"263_CR：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	//调用回波顶产品生成函数，
	gInt16  AzimuthNumber = 0;
	gInt16 MaxRefGateNumer = 0;
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	MaxRefGateNumer = maxRefgatenum;

	logfile<<"263_CR：  Initialize Radial_Data to RADAR_DATA_NOECHO. "<<endl;
	for( gInt16 j = 0 ; j < AzimuthNumber ; j++ )  //每个仰角层的径向数
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  // 径向距离库数的一半
		{
			(ppdt->GetMyRadial_Data(j))[k] = RADAR_DATA_NOECHO;
		}
	}
	logfile<<"263_CR：  pArith_263_CR(). "<<endl;
	pArith_263_CR( pGData, ppdt, maxRefgatenum);

	logfile<<"263_CR：  Get and set Radar Head Information. "<<endl;
	RadarInfor * pRadarInfor = ppdt->GetRadarInfor();
	memcpy(pRadarInfor->SiteCode, pGData->GetHeadP()->SiteCode, sizeof(gChar)*8);
	memcpy(pRadarInfor->RadarType, pGData->GetHeadP()->RadarType, sizeof(gInt8) * 4);
	pRadarInfor->Latitude = pGData->GetHeadP()->RadarLat;
	pRadarInfor->Longitude = pGData->GetHeadP()->RadarLon;
	pRadarInfor->Altitude = pGData->GetHeadP()->RadarHeight;

	PDBlock* pdb = ppdt->GetMyPDBlock();

	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	pdb->ProductDate = pGData->GetHeadP()->VCPDate;
	pdb->ProductTime = pGData->GetHeadP()->VCPTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"263_CR：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	memcpy(pdb->OperationalMode, pGData->GetHeadP()->OperationMode, sizeof(gInt8) * 2);
	memcpy(pdb->VCPMode, pGData->GetHeadP()->VCPMode, sizeof(gInt8) * 4);
	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	//将得到的产品加到 m_pOutputProductList中
	ppdt->GetMyDILayer()->HeightOfElevation=0;
	ppdt->GetMyDILayer()->LayerDate=0;
	ppdt->GetMyDILayer()->LayerTime=0;
	//ppdt->GetMyRadial_Head()->ScaleOfData = 10;
	ppdt->GetMyRadial_Head()->DistanceOfFirstGate = pGData->GetElevationHeadP(0)->FirstGateRangeOfRef;
	ppdt->GetMyRadial_Head()->LengthOfGate = pGData->GetHeadP()->ReflectivityGateSize;

	gUint16 RadialAzimuth = 0;
	gInt16 AzimuthNumberBound = AzimuthNumber - 1;

	logfile<<"263_CR：  Loop from i=0-->AzimuthNumber(360) and set RadialData Angle Information"<<endl;
	for( int i = 0 ; i < AzimuthNumber ; i++ )
	{
		RadialAzimuth = pGData->GetRadarDataP(0, i)->Azimuth;
		ppdt->GetMyRadial_RadialData(i)->AzimuthAngle = RadialAzimuth;
		if ( i == AzimuthNumberBound )
		{
			ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,0)->Azimuth + 36000 - RadialAzimuth )%36000;
		}
		else
		{
			ppdt->GetMyRadial_RadialData(i)->DeltaAngle = ( pGData->GetRadarDataP(0,i+1)->Azimuth+36000 - RadialAzimuth )%36000;
		}
	}
	logfile<<"263_CR：  Loop Finished."<<endl;
	logfile<<"263_CR：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

GHRESULT Arith_263_CR::OutPutData(void * pValue)
{
	if( pValue == NULL ) 
		return GE_INVALIDARG;
	m_pOutputProductList = (GXList< GenerationData > *)pValue;

	return GS_OK;
}
void  Arith_263_CR::pArith_263_CR( StandardRadarData * pGData , Product_263_CR * pRadialData, gInt16 maxRefgatenum)
{	
	gInt16 k1 = 0;
	gFloat temDou = 0;
	gInt16 AzimuthNumber = 0;
	gInt16 MaxRefGateNumer = 0;
	gInt16 ElevationCount = 0;
	gInt16 *ReflectivityGates = NULL;
	gFloat *Temp = NULL;
	gInt16 * RadialDataR = NULL;
	gFloat ElevationCos = 0;
	ElevationCos =(gFloat) cos(pGData->GetElevationHeadP(0)->Elevation*M_PI/18000.0);
	AzimuthNumber = pGData->GetElevationHeadP(0)->AzimuthNumber;
	MaxRefGateNumer = maxRefgatenum;
	ElevationCount =  pGData->GetHeadP()->ElevationCount;
	ReflectivityGates = new (::std::nothrow)gInt16[ElevationCount];
	Temp = new (::std::nothrow)gFloat[ElevationCount];
	RadialDataR = new (::std::nothrow)gInt16[ElevationCount];
	for( gInt16  i =  0 ; i < ElevationCount; i++ )
	{
		ReflectivityGates[i] = pGData->GetElevationHeadP(i)->ReflectivityGates;
		Temp[i] =(gFloat)( ElevationCos /cos(pGData->GetElevationHeadP(i)->Elevation*M_PI/18000.0) );
	}
	for( gInt16 j = 0 ; j < AzimuthNumber; j++ ) 
	{
		for( gInt16 k = 0 ; k < MaxRefGateNumer ; k++ )  
		{	
			gInt16 MaxRadarDataR = RADAR_DATA_NOECHO;
			for( gInt16  i = ElevationCount - 1 ; i >=0; i-- )
			{
				temDou=(gFloat)((k*Temp[i]));	
				if ( temDou < ReflectivityGates[i] )
				{
					k1=(gInt16)temDou;
					RadialDataR [i] = pGData->GetRadarDataR(i,j)[k1];
					if( ( RadialDataR [i] ) > RADAR_DATA_NOECHO && j < pGData->GetElevationHeadP(i)->AzimuthNumber\
						&& k < pGData->GetElevationHeadP(i)->ReflectivityGates)
					{
						if( ( RadialDataR [i] ) > MaxRadarDataR )
						{
							MaxRadarDataR = RadialDataR [i] ;
						}		
					}
				}
			}
			pRadialData->GetMyRadial_Data(j)[k]  = MaxRadarDataR;
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
}
extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_263_CR接口
{
	return new (::std::nothrow)Arith_263_CR;
}
