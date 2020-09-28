//////////////////////////////////////////////////////////////////////////
//modify by zsc
//////////////////////////////////////////////////////////////////////////
#ifndef ARITH_285_EVP_269_VWP_268_VAD_CPP
#define ARITH_285_EVP_269_VWP_268_VAD_CPP
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_285_EVP_269_VWP_268_VAD.h"
#include <cmath>
#include <cstdio>
#include "CJulMsGMT.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream> 
#include <fstream>

Arith_285_EVP_269_VWP_268_VAD::Arith_285_EVP_269_VWP_268_VAD()
{
}
Arith_285_EVP_269_VWP_268_VAD::~Arith_285_EVP_269_VWP_268_VAD()
{
}
GHRESULT Arith_285_EVP_269_VWP_268_VAD::Initialize()
{
	m_pGenerateProductList = NULL;
	m_pOutputProductList = NULL;
	return GS_OK;
}
GHRESULT Arith_285_EVP_269_VWP_268_VAD::UnInitialize()
{
	return GS_OK;
}
GHRESULT Arith_285_EVP_269_VWP_268_VAD::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if ( ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pParameter=ReadParameter;
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;
	return GS_OK;
}
extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_285_EVP_269_VWP_268_VAD接口 add by dqc
{
	return new Arith_285_EVP_269_VWP_268_VAD;
}

GHRESULT Arith_285_EVP_269_VWP_268_VAD::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_285_EVP_269_VWP_268_VAD.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"285_EVP_269_VWP_268_VAD：  Started: "<<endl;
	
	logfile<<"285_EVP_269_VWP_268_VAD：  Send in and Read Parameters."<<endl;
	StandardRadarData* pRadar = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	XML_Para parm;
	parm.Thr_Symmetry = 25.2;      //km/hr
	parm.Thr_DataPoint = 25;
	parm.Thr_Velocity = 18;        //km/hr
	parm.Height_Num = 3;
	parm.Height[0] = 10;          //10m
	parm.Height[1] = 20;          //10m
	parm.Height[2] = 80;

	logfile<<"285_EVP_269_VWP_268_VAD：  Add and Create Send-out Product: Product_268_VAD."<<endl;
	//产品VAD
	m_pOutputProductList->AddTail();
	Product_268_VAD* ppdt2 = &(*(m_pOutputProductList->GetTailAs<Product_268_VAD>()));

	GHRESULT ConstrutHand;
	ConstrutHand = ppdt2->ConstructIt(parm.Height_Num,1,sizeof(VAD_ELEMENTS));
	if(ConstrutHand != GS_OK)
	{
		logfile<<"285_EVP_269_VWP_268_VAD：  Create New Product_268_VAD Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"285_EVP_269_VWP_268_VAD：  Get and Set RadarInfor Information."<<endl;
	ppdt2->GetRadarInfor()->Altitude = pRadar->GetHeadP()->RadarHeight;
	ppdt2->GetRadarInfor()->Latitude = pRadar->GetHeadP()->RadarLat;
	ppdt2->GetRadarInfor()->Longitude = pRadar->GetHeadP()->RadarLon;
	memcpy(ppdt2->GetRadarInfor()->RadarType, pRadar->GetHeadP()->RadarType, sizeof(gInt8)*4);
	memcpy(ppdt2->GetRadarInfor()->SiteCode, pRadar->GetHeadP()->SiteCode, sizeof(gChar)*8);

	for( int i=0; i< parm.Height_Num ; i++)
		ppdt2->GetMyLayer(i)->HeightOfElevation = parm.Height[i] ;
	

	PDBlock* pdb2 = ppdt2->GetMyPDBlock();
	CJulMsGMT::GetLocalJulMs(pdb2->GenerationDateOfProduct,pdb2->GenerationTimeOfProduct);//获取当前儒略日毫秒
	memcpy(pdb2->VCPMode, pRadar->GetHeadP()->VCPMode, sizeof(gInt8)*4);
	memcpy(pdb2->OperationalMode, pRadar->GetHeadP()->OperationMode, sizeof(gInt8)*2);
	pdb2->ProductDate = pRadar->GetHeadP()->VCPDate;
	pdb2->ProductTime = pRadar->GetHeadP()->VCPTime;

	CJulMsGMT::Jul2Greg(pdb2->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb2->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"285_EVP_269_VWP_268_VAD：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb2->IndexOfGraphic = -1;
	pdb2->IndexOfLayers = -1;

	logfile<<"285_EVP_269_VWP_268_VAD：  Add and Create Send-out Product: Product_269_VWP."<<endl;
	//产品VWP
	m_pOutputProductList->AddTail();
	Product_269_VWP* ppdt1 = &(*(m_pOutputProductList->GetTailAs<Product_269_VWP>()));
	ppdt1->ConstructIt(31,sizeof(VWP_ELEMENTS));

	logfile<<"285_EVP_269_VWP_268_VAD：  Get and Set RadarInfor Information."<<endl;
	ppdt1->GetRadarInfor()->Altitude = pRadar->GetHeadP()->RadarHeight;
	ppdt1->GetRadarInfor()->Latitude = pRadar->GetHeadP()->RadarLat;
	ppdt1->GetRadarInfor()->Longitude = pRadar->GetHeadP()->RadarLon;
	memcpy(ppdt1->GetRadarInfor()->RadarType, pRadar->GetHeadP()->RadarType, sizeof(gInt8)*4);
	memcpy(ppdt1->GetRadarInfor()->SiteCode, pRadar->GetHeadP()->SiteCode, sizeof(gChar)*8);

	PDBlock *pdb1 = ppdt1->GetMyPDBlock();
	CJulMsGMT::GetLocalJulMs(pdb1->GenerationDateOfProduct,pdb1->GenerationTimeOfProduct);//获取当前儒略日毫秒
	memcpy(pdb1->VCPMode, pRadar->GetHeadP()->VCPMode, sizeof(gInt8)*4);
	memcpy(pdb1->OperationalMode, pRadar->GetHeadP()->OperationMode, sizeof(gInt8)*2);
	pdb1->ProductDate = pRadar->GetHeadP()->VCPDate;
	pdb1->ProductTime = pRadar->GetHeadP()->VCPTime;
	pdb1->IndexOfGraphic = -1;
	pdb1->IndexOfLayers = -1;

	logfile<<"285_EVP_269_VWP_268_VAD：  Add and Create Send-out Product: Product_285_EVP."<<endl;
	//产品EVP
	m_pOutputProductList->AddTail();
	Product_285_EVP* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_285_EVP>()));
	ppdt->ConstructIt(1550,sizeof(EVP_ELEMENTS));

	logfile<<"285_EVP_269_VWP_268_VAD：  Get and Set RadarInfor Information."<<endl;
	ppdt->GetRadarInfor()->Altitude = pRadar->GetHeadP()->RadarHeight;
	ppdt->GetRadarInfor()->Latitude = pRadar->GetHeadP()->RadarLat;
	ppdt->GetRadarInfor()->Longitude = pRadar->GetHeadP()->RadarLon;
	memcpy(ppdt->GetRadarInfor()->RadarType, pRadar->GetHeadP()->RadarType, sizeof(gInt8)*4);
	memcpy(ppdt->GetRadarInfor()->SiteCode, pRadar->GetHeadP()->SiteCode, sizeof(gChar)*8);

	PDBlock* pdb = ppdt->GetMyPDBlock();
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);//获取当前儒略日毫秒
	memcpy(pdb->VCPMode, pRadar->GetHeadP()->VCPMode, sizeof(gInt8)*4);
	memcpy(pdb->OperationalMode, pRadar->GetHeadP()->OperationMode, sizeof(gInt8)*2);
	pdb->ProductDate = pRadar->GetHeadP()->VCPDate;
	pdb->ProductTime = pRadar->GetHeadP()->VCPTime;

	pdb->IndexOfGraphic = -1;
	pdb->IndexOfLayers = -1;

	logfile<<"285_EVP_269_VWP_268_VAD：  Call Function: OnRadarEVP(Temp_Num,pRadar,ppdt,ppdt1, ppdt2, parm)."<<endl;
	int Temp_Num = pRadar->GetHeadP()->ElevationCount;
	OnRadarEVP(Temp_Num,pRadar,ppdt,ppdt1, ppdt2, parm);
	logfile<<"285_EVP_269_VWP_268_VAD：  Function OnRadarEVP( ) Finished."<<endl;
	logfile<<"285_EVP_269_VWP_268_VAD：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
GHRESULT Arith_285_EVP_269_VWP_268_VAD::OutPutData(void * pValue)
{
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return GS_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数，通过循环调用其它函数实现速度范围显示
bool Arith_285_EVP_269_VWP_268_VAD::
OnRadarEVP(int Num_Tilts, StandardRadarData *pGData,Product_285_EVP *Product,Product_269_VWP *Product1, Product_268_VAD *Product2, XML_Para parm)
{
	int Dop_Gatelen = pGData->GetHeadP()->DopplerGateSize;      //m
	bool Flat = false;
	double bh, th;

	VAD *vad = new VAD[1550];
	for (int i=0; i<1550; i++)
	{
		vad[i].nDP = 0;
		vad[i].DW = 0;
		vad[i].SPW = 0;
		vad[i].SHW = 0;
		vad[i].RMS = 0;
		vad[i].Difnear = 1;
		vad[i].Symmetry = false;
		vad[i].HVAD = 0;
		vad[i].VPF = 0;
		vad[i].DEC = RADAR_DATA_NOECHO;
		vad[i].AS = 0;
		vad[i].ASC = 0;
		vad[i].SVW = RADAR_DATA_NOECHO;
		vad[i].SVC = 0;
		vad[i].DIV = 0;
		vad[i].shear = RADAR_DATA_NOECHO;
		vad[i].stretch = RADAR_DATA_NOECHO;
		vad[i].CF0 = 0;
		for (int j=0; j<360; j++)
		{
			vad[i].Vel[j] = RADAR_DATA_NOECHO;
			vad[i].FIT[j] = RADAR_DATA_NOECHO;
			vad[i].RD[j] = RADAR_DATA_NOECHO;
		}
	}

	for (int tilt = 0; tilt < Num_Tilts; tilt++)
	{		
		if (Dop_Gatelen == 0|| pGData->GetElevationHeadP(tilt)->DopplerGates < 50)
		{
			continue;
		}
		if (pGData->GetElevationHeadP(tilt)->Elevation <= 200)  
		{
			bh = 0.00;
			th = 1.5;
		}
		else if (pGData->GetElevationHeadP(tilt)->Elevation < 400)
		{
			bh = 1.0;
			th = 3.0;
		}
		else if(pGData->GetElevationHeadP(tilt)->Elevation < 600)
		{
			bh = 2.0;
			th = 5.25;
		}
		else if (pGData->GetElevationHeadP(tilt)->Elevation < 700)
		{
			bh = 3.0;
			th = 7.25;
		}
		else if (pGData->GetElevationHeadP(tilt)->Elevation < 1050)
		{
			bh = 5.0;
			th = 9.5;
		}
		else if (pGData->GetElevationHeadP(tilt)->Elevation < 1650)
		{
			bh = 6.5;
			th = 12.5;
		}
		else
		{
			bh = 7.5;
			th = 15.5;
		}
		seg_init(Dop_Gatelen, pGData->GetElevationHeadP(tilt)->Elevation, bh, th);
		VAD_process(tilt, pGData, vad, parm);
	}
	//VAD_data_deal_with(vad);
	VAD_mean(vad);
	//////////////////////////////////////////////////////////////////////////
	VAD_ELEMENTS *pTemp = NULL;
	for (int i = 0; i < parm.Height_Num; i++)
	{
		pTemp = (VAD_ELEMENTS *)Product2->GetBlockAsStruct_Data(i);
		int j = int(parm.Height[i] + 0.5);
		pTemp[0].hight = j;
		pTemp[0].CF0 = RADAR_DATA_NODATA;
		pTemp[0].U = RADAR_DATA_NODATA;
		pTemp[0].Uh = RADAR_DATA_NODATA;
		pTemp[0].Du = RADAR_DATA_NODATA;
		for (int ii = 0; ii < 360; ii++)
		{
			//pTemp[i].FIT[ii] = RADAR_DATA_NODATA;
			pTemp[0].Vel[ii] = RADAR_DATA_NODATA;
			pTemp[0].RD[ii] = RADAR_DATA_NODATA;
		}
		if (vad[j].SHW>0)
		{
			pTemp[0].CF0 = gInt16(vad[j].CF0*100./3.6);
			pTemp[0].U = gInt16(vad[j].SPW*100./3.6);
			pTemp[0].Uh = gInt16(vad[j].SHW*100./3.6);
			pTemp[0].Du = gInt16(vad[j].DW*10);
			//int x=0;
			for (int ii = 0; ii < 360; ii++)
			{
				//pTemp[i].FIT[ii] = gInt16(vad[j].FIT[ii]*100/3.6);
				pTemp[0].Vel[ii] = gInt16(vad[j].Vel[ii]);
				pTemp[0].RD[ii] = gInt16(vad[j].RD[ii]);
				/*if (pTemp[i].Vel[ii]>RADAR_DATA_OUTOFRANGE)
				{
				printf("%d     %d    ",pTemp[i].Vel[ii],pTemp[i].RD[ii]);
				x++;
				if (x==4)
				{
				printf("\n");
				x=0;
				}
				}*/
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	EVP_ELEMENTS *pTemp1 = (EVP_ELEMENTS *)Product->GetBlockAsStruct_Data();
	for (int i = 0;i < 1550; i++)
	{
		pTemp1[i].hight = (short)(vad[i].Height*100+0.5);
		pTemp1[i].div = RADAR_DATA_NODATA;
		pTemp1[i].Du = RADAR_DATA_NODATA;
		pTemp1[i].Uh = RADAR_DATA_NODATA;
		pTemp1[i].stretch = RADAR_DATA_NODATA;
		pTemp1[i].shear = RADAR_DATA_NODATA;
		if((vad[i].Height>0)&&(vad[i].SHW>0))
		{
			pTemp1[i].div = (short)(vad[i].DIV * 1000000/3600.0);
			pTemp1[i].Uh = (short)(vad[i].SHW*100./3.6);
			pTemp1[i].Du = (short)(vad[i].DW*10);
			pTemp1[i].stretch = (short)(vad[i].stretch*1000000);
			pTemp1[i].shear = (short)(vad[i].shear*1000000);
		}
		else
		{
			pTemp1[i].hight = i;
		}
	}
	//for (int i=0; i<1550; i++)
	//{
	//	if((vad[i].Height>0)&&(vad[i].SHW>0))
	//	{
	//	cout<<pTemp1[i].hight<<"    "<<pTemp1[i].div<<"     "<<pTemp1[i].Du
	//		<<"    "<<pTemp1[i].Uh<<"    "<<pTemp1[i].stretch<<"     "<<pTemp1[i].shear<<"      "<<(short)(vad[i].RMS*100./3.6)<<endl;
	//	if (pTemp1[i].Du > 3600)
	//	{
	//		cout<<endl;
	//	}
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////
	VWP_ELEMENTS *pTemp2 = (VWP_ELEMENTS *)Product1->GetBlockAsStruct_Data();
	for (int i = 0; i < 31; i++)//初始化
	{
		pTemp2[i].time = pGData->GetVCPTime();
		pTemp2[i].hight = RADAR_DATA_NODATA;
		pTemp2[i].Uh = RADAR_DATA_NODATA;
		pTemp2[i].Du = RADAR_DATA_NODATA;
		pTemp2[i].RMS = RADAR_DATA_NODATA;	
	}
	for (int i = 0; i < 19; i++)//
	{
		pTemp2[i].time = pGData->GetVCPTime();
		pTemp2[i].hight = (i+1)*30;
		pTemp2[i].Uh = pTemp1[(i+1)*30].Uh;
		pTemp2[i].Du = pTemp1[(i+1)*30].Du;
		pTemp2[i].RMS = (short)(vad[(i+1)*30].RMS*100./3.6);
		if (pTemp2[i].Uh == RADAR_DATA_NODATA)
		{
			pTemp2[i].Du = RADAR_DATA_NODATA;
			pTemp2[i].RMS = RADAR_DATA_NODATA;	
		}
	}
	for (int i = 19; i < 24; i++)
	{
		int g = 600+(i-19)*60;
		pTemp2[i].time = pGData->GetVCPTime();
		pTemp2[i].hight = g;
		pTemp2[i].Uh = pTemp1[g].Uh;
		pTemp2[i].Du = pTemp1[g].Du;
		pTemp2[i].RMS = (short)(vad[g].RMS*100./3.6);	
		if (pTemp2[i].Uh == RADAR_DATA_NODATA)
		{
			pTemp2[i].Du = RADAR_DATA_NODATA;
			pTemp2[i].RMS = RADAR_DATA_NODATA;	
		}
	}
	for (int i = 24; i < 31; i++)
	{
		int g = 900+(i-24)*100;
		pTemp2[i].time = pGData->GetVCPTime();
		pTemp2[i].hight = g;
		pTemp2[i].Uh = pTemp1[g].Uh;
		pTemp2[i].Du = pTemp1[g].Du;
		pTemp2[i].RMS = (short)(vad[g].RMS*100./3.6);	
		if (pTemp2[i].Uh == RADAR_DATA_NODATA)
		{
			pTemp2[i].Du = RADAR_DATA_NODATA;
			pTemp2[i].RMS = RADAR_DATA_NODATA;	
		}
	}
	//for (int i=0; i<31; i++)
	//{
	//	cout<<pTemp2[i].hight<<"      "<<pTemp2[i].Uh<<"    "<<pTemp2[i].Du<<"      "<<pTemp2[i].RMS<<endl;
	//}
	if (vad != NULL)
	{
		delete []vad;
		vad = NULL;
	}
	Flat = true;
	return Flat;
}
bool Arith_285_EVP_269_VWP_268_VAD::seg_init(int Dop_Gatelen, short El_Angle, double bh, double th)
{
	double Ele_Temp,b_temp,c_temp, bL,tL;
	Ele_Temp = El_Angle * M_PI/18000.0;
	m_Sin_e1 = sin(Ele_Temp);
	m_Cos_e2 = cos(Ele_Temp)*cos(Ele_Temp);
	//求m_StartBin
	b_temp = 17008.0/m_Cos_e2;
	c_temp = bh * b_temp;
	b_temp = b_temp * m_Sin_e1*0.5;
	bL = sqrt(b_temp * b_temp + c_temp) - b_temp;
	m_StartBin = (int)(bL * 1000/Dop_Gatelen);
	m_StartBin = m_StartBin/4*4;
	//求m_EndBin
	b_temp = 17008.0/m_Cos_e2;
	c_temp = th * b_temp;
	b_temp = b_temp * m_Sin_e1 * 0.5;
	tL = sqrt(b_temp * b_temp + c_temp) - b_temp;
	if (tL > 75)
	{
		tL = 75;
	}
	m_EndBin = (int)(tL * 1000/Dop_Gatelen);
	m_EndBin = m_EndBin/4*4;
	return true;
}
int Arith_285_EVP_269_VWP_268_VAD::InitX2(int Rj, int Tilt, int *X2, StandardRadarData *pGData)
{
	short nrays = pGData->GetElevationHeadP(Tilt)->AzimuthNumber;
	int ab_1,ab_2,bb_1,bb_2,count = 0;
	for (int i = 0; i < nrays; i++)
	{
		short temp_RJ = pGData->GetRadarDataV(Tilt,i)[Rj];            //0.1m/s
		if (temp_RJ <= RADAR_DATA_OUTOFRANGE)
		{
			X2[i] = RADAR_DATA_NOECHO;
		}
		else
		{
			X2[i] = temp_RJ;
			count++;
		}
	}
	if (count < 25)
	{
		return count;
	}
	int *dX2 = new int[nrays];
	for (int i = 0; i < nrays; i++)
	{
		ab_1 = i - 1;
		if (ab_1 < 0)
		{
			ab_1 += nrays;
		}
		ab_2 = i - 2;
		if (ab_2 < 0)
		{
			ab_2 += nrays;
		}
		bb_1 = i + 1;
		if (bb_1 >= nrays)
		{
			bb_1 -= nrays;
		}
		bb_2 = i + 2;
		if (bb_2 >= nrays)
		{
			bb_2 -= nrays;
		}
		dX2[i] = 0;
		count = 0;
		if ( X2[ab_2] != RADAR_DATA_NOECHO)
		{
			dX2[i] += X2[ab_2];
			count++;
		}
		if (X2[ab_1] != RADAR_DATA_NOECHO)
		{
			dX2[i] += X2[ab_1];
			count++;
		}
		if (X2[i] != RADAR_DATA_NOECHO)
		{
			dX2[i] += X2[i];
			count++;
		}
		if (X2[bb_1] != RADAR_DATA_NOECHO)
		{
			dX2[i] += X2[bb_1];
			count++;
		}
		if (X2[bb_2] != RADAR_DATA_NOECHO)
		{
			dX2[i] += X2[bb_2];
			count++;
		}
		if (count >= 3)
		{
			dX2[i] = dX2[i]/count;
		}
		else
		{
			dX2[i] = RADAR_DATA_NOECHO;
		}
	}
	for (int i = 0; i < nrays; i++)
	{
		X2[i] =  dX2[i];
	}

	if (dX2 != NULL)
	{
		delete []dX2;
		dX2 = NULL;
	}

	count = 0;
	for (int i = 0; i < nrays; i++)
	{
		if (X2[i] != RADAR_DATA_NOECHO)
		{
			count++;
		}
	}

	return count;
}
int Arith_285_EVP_269_VWP_268_VAD::InitX2(int Tilt, int *X2, StandardRadarData *pGData)
{
	short nrays = pGData->GetElevationHeadP(Tilt)->AzimuthNumber;
	int count = 0;

	count = 0;
	for (int i = 0; i < nrays; i++)
	{
		if (X2[i] != RADAR_DATA_NOECHO)
		{
			count++;
		}
	}

	return count;
}
bool Arith_285_EVP_269_VWP_268_VAD::VAD_process(int tilt, StandardRadarData* pGData, VAD* vad, XML_Para parm)
{
	int NumOfRadial = pGData->GetElevationHeadP(tilt)->AzimuthNumber;
	int Mode1 = (int)pGData->GetHeadP()->OperationMode[1];
	double El = pGData->GetElevationHeadP(tilt)->Elevation*M_PI/18000.0;
	double R1 = m_StartBin * pGData->GetHeadP()->DopplerGateSize * 0.001+0.5;//开始高度与雷达的距离，单位KM
	double dh = R1 * m_Sin_e1 + R1 * R1 * m_Cos_e2 / 17008.0;//R1点的垂直高度＋水平高度的二次方
	int ihm = (int)(dh * 100 + 0.5);
	if (ihm >= 1550 || ihm<=0)
	{
		return false;
	}
	//double *East_W_SW = new double [1550];
	//double *South_N_SW = new double [1550];
	//int *m_cnt = new int[1550];
	for (int j = m_StartBin; j <= m_EndBin; j++)
	{
		R1 = j*pGData->GetHeadP()->DopplerGateSize*0.001+0.5;
		dh = R1 * m_Sin_e1 + R1*R1*m_Cos_e2/17008.0;
		ihm = (int)(dh * 100 + 0.5);
		if (ihm >= 1550)
		{
			return false;
		}

		int *X2 = new int[NumOfRadial];
		memset(X2, 0, NumOfRadial*sizeof(int));
		complex *Q = new complex [5];
		for (int m=0; m<5; m++)
		{
			Q[m].real = 0;
			Q[m].image = 0;
		}
		double *CF = new double [3];
		memset(CF, 0, 3*sizeof(double));

		int count = InitX2(j, tilt, X2, pGData);             //X2   0.01m/s
		if (count < 25)
		{
			if (Q != NULL)
			{
				delete []Q;
				Q = NULL;
			}
			if (CF != NULL)
			{
				delete []CF;
				CF = NULL;
			}
			if (X2 != NULL)
			{
				delete []X2;
				X2 = NULL;
			}
			continue;
		}
		Compute_Stretch_Shear(vad, X2, pGData, tilt, ihm, R1);

		if (abs(dh*100-ihm)<vad[ihm].Difnear)
		{
			vad[ihm].Difnear = abs(dh*100 - ihm);
		}
		else
		{
			if (Q != NULL)
			{
				delete []Q;
				Q = NULL;
			}
			if (CF != NULL)
			{
				delete []CF;
				CF = NULL;
			}
			if (X2 != NULL)
			{
				delete []X2;
				X2 = NULL;
			}
			continue;
		}
		//memset(East_W_SW, 0, 1550*sizeof(double));
		//memset(South_N_SW, 0, 1550*sizeof(double));
		//memset(m_cnt, 0, 1550*sizeof(int));
		for (int FitTest=0; FitTest<2; FitTest++)
		{
			double SAsin = 0;
			double SA2sin = 0;
			double SAcos = 0;
			double SA2cos = 0;
			double SVsin = 0;
			double SVcos = 0;
			double SumV = 0;

			for (int IndAzi=0; IndAzi<NumOfRadial; IndAzi++)
			{
				if (X2[IndAzi] != RADAR_DATA_NOECHO)
				{
					float Azi = float(IndAzi*M_PI/180.0);
					SAsin += sin(Azi); 
					SA2sin += sin(2.0*Azi);
					SAcos += cos(Azi);
					SA2cos += cos(2.0*Azi);
					SVsin += X2[IndAzi]*sin(Azi)*0.036;			//km/hr
					SVcos += X2[IndAzi]*cos(Azi)*0.036;			//km/hr
					SumV += X2[IndAzi]*0.036;					//km/hr
				}
			}
			Q[2].real = double(SVcos/count);                
			Q[2].image = double((-1)*SVsin/count);          
			Q[3].real = double(SAcos/(2*count));
			Q[3].image = double(/*(-1)**/SAsin/(2*count));
			Q[4].real = double(SA2cos/(2*count));
			Q[4].image = (-1)*double(SA2sin/(2*count));
			complex ComQ3 = Complex(Q[3].real, (-1)*Q[3].image);
			Q[0] = ComplexDivide(ComplexMinus(Complex(SumV/count, 0),ComplexDivide(Q[2], Complex(2*ComQ3.real, 2*ComQ3.image))),
				ComplexMinus(Q[3], ComplexDivide(Complex(1, 0), Complex(4*ComQ3.real, 4*ComQ3.image))));
			Q[1] = ComplexDivide(ComplexMinus(ComQ3, ComplexDivide(Q[4], Complex(2*ComQ3.real, 2*ComQ3.image))),
				ComplexMinus(Q[3], ComplexDivide(Complex(1, 0), Complex(4*ComQ3.real, 4*ComQ3.image))));

			complex ComQ0 = Complex(Q[0].real, (-1)*Q[0].image);
			complex ComQ1 = Complex(Q[1].real, (-1)*Q[1].image);
			complex temp1, temp2, temp3;
			temp1 = ComplexDivide(ComplexMinus(Q[0], ComplexMultiply(Q[1], ComQ0)), Complex(1-(Q[1].real*Q[1].real+Q[1].image*Q[1].image), 0));
			temp2 = Complex(temp1.real, (-1)*temp1.image);
			temp3 = ComplexMultiply(temp1, Q[3]);

			CF[0] = SumV/count-2*temp3.real;			//km/hr
			CF[1] = temp1.real;							//km/hr	
			CF[2] = temp1.image;						//km/hr

			vad[ihm].tilt = tilt;
			vad[ihm].nDP = count;
			vad[ihm].Height = (float)dh;                //高度    km
			vad[ihm].SPW = sqrt(CF[1]*CF[1]+CF[2]*CF[2]);               //速度km/hr                        
			vad[ihm].DW = M_PI-atan2(CF[2],CF[1]);
			if (vad[ihm].DW < 0)
			{
				vad[ihm].DW += M_PI*2;
			}
			if (vad[ihm].DW >= M_PI*2)
			{
				vad[ihm].DW -= M_PI*2;
			}
			vad[ihm].CF0 = CF[0];

			double tempRMS = 0;
			for (int IndAzi=0; IndAzi<NumOfRadial; IndAzi++)
			{
				if (X2[IndAzi] != RADAR_DATA_NOECHO)
				{
					float Azi = float(IndAzi*M_PI/180.0);
					tempRMS += pow(CF[0]-X2[IndAzi]*0.036-cos(Azi-vad[ihm].DW)*sqrt(CF[1]*CF[1]+CF[2]*CF[2]),2);
				}
			}
			vad[ihm].RMS = sqrt(tempRMS/count);                //km/hr

			for(int IndAzi=0; IndAzi<NumOfRadial; IndAzi++)
			{
				if (X2[IndAzi]!=RADAR_DATA_NOECHO)
				{
					float Azi = float(IndAzi*M_PI/180.0);
					vad[ihm].FIT[IndAzi] = CF[0]-cos(Azi-vad[ihm].DW)*vad[ihm].SPW;        //km/hr
					if (vad[ihm].FIT[IndAzi]>0)
					{
						if (vad[ihm].FIT[IndAzi]-X2[IndAzi]*0.036>vad[ihm].RMS)
						{
							X2[IndAzi] = RADAR_DATA_NOECHO;
						}
					}
					else
					{
						if (X2[IndAzi]*0.036-vad[ihm].FIT[IndAzi]>vad[ihm].RMS)
						{
							X2[IndAzi] = RADAR_DATA_NOECHO;
						}
					}
				}
			}
			if (FitTest==0)
			{
				count = InitX2(tilt, X2, pGData);
			}
			else
			{
				for (int ii=0; ii<360; ii++)
				{
					vad[ihm].Vel[ii] = X2[ii];
					if (vad[ihm].Vel[ii] != RADAR_DATA_NOECHO)
					{
						vad[ihm].RD[ii] = pGData->GetRadarDataR(tilt, ii)[int(j/4+0.5)];
					}
				}
			}
		}

		//if ((vad[ihm].SHW>0)&&(vad[ihm].SHW<1000)&&(vad[ihm].DW>0))
		//{
		//	East_W_SW[ihm] += -vad[ihm].SHW*sin(vad[ihm].DW);
		//	South_N_SW[ihm] += -vad[ihm].SHW*cos(vad[ihm].DW);
		//	RMS[ihm] += vad[ihm].RMS;
		//	m_cnt[ihm] ++;
		//}
		if ((abs(CF[0])<parm.Thr_Symmetry)&&(abs(CF[0])-vad[ihm].SPW<=0))
		{
			vad[ihm].Symmetry = true;
		}

		vad[ihm].HVAD = (R1*R1+RADIUS_EARTH*R1*sin(El)*8.0/3.0)/(RADIUS_EARTH*8.0/3.0);
		if ((vad[ihm].RMS<parm.Thr_Velocity)&&(vad[ihm].Symmetry)&&(count>parm.Thr_DataPoint))
		{
			vad[ihm].SHW = sqrt(CF[1]*CF[1]+CF[2]*CF[2])/(cos(El));       //水平风速  km/r
			double RData = 0;
			int cnt = 0;
			for (int jj=0; jj<360; jj++)
			{
				if (pGData->GetRadarDataR(tilt, jj)[int(j/4+0.5)]>RADAR_DATA_OUTOFRANGE)
				{
					//vad[ihm].RD[jj] = pGData->GetRadarDataR(tilt, jj)[j];
					RData += pGData->GetRadarDataR(tilt, jj)[int(j/4+0.5)]*0.01;
					cnt++;
				}
			}
			RData = RData/cnt;
			if (cnt == 0)
			{
				RData = 0;
			}
			double ZE = pow(10, RData/10.0);

			switch(Mode1)
			{
			case 1:
				vad[ihm].VPF = 9.54*pow(ZE, 0.114)*(1.01091+(vad[ihm].HVAD+vad[ihm].Height)*0.02863
					+(vad[ihm].HVAD+vad[ihm].Height)*(vad[ihm].HVAD+vad[ihm].Height)*0.00259);
				break;
			case 2:
				vad[ihm].VPF = 0;
				break;
			default:
				if (Q != NULL)
				{
					delete []Q;
					Q = NULL;
				}
				if (CF != NULL)
				{
					delete []CF;
					CF = NULL;
				}
				if (X2 != NULL)
				{
					delete []X2;
					X2 = NULL;
				}
				return false;
			}

			double tempE = RADIUS_EARTH*4.0/3.0;
			vad[0].DEC = RADIUS_EARTH;
			vad[0].SVW = 0;
			if(ihm!=0)
			{
				vad[ihm].DEC = RADIUS_EARTH-tempE+tempE*sqrt(1+(R1/tempE)*(R1/tempE)+2*R1*sin(El)/tempE);
			}
			double Q6 = acos((tempE*tempE+(vad[ihm].DEC+RADIUS_EARTH/3.0)*(vad[ihm].DEC+RADIUS_EARTH/3.0)-R1*R1)
				/(2*tempE*(vad[ihm].DEC+RADIUS_EARTH/3.0)))*4.0/3.0;
			vad[ihm].AS = 2*M_PI*(vad[ihm].DEC*vad[ihm].DEC)*(1-cos(Q6));
			vad[ihm].ASC = 2*vad[ihm].AS/vad[ihm].DEC-(vad[ihm].DEC*vad[ihm].DEC*sin(Q6)/(RADIUS_EARTH*sin(Q6*3.0/4.0))
				*(1+(R1*R1-tempE*tempE)/((vad[ihm].DEC+RADIUS_EARTH/3.0)*(vad[ihm].DEC+RADIUS_EARTH/3.0))));

			double RH = RH0*exp(-dh/10.0);
			double DRH = RH/(-10.0);

			int num1=1;
			int num2=1;
			while (vad[ihm-num1].SVW==RADAR_DATA_NOECHO)
			{
				if (ihm-num1==0)
				{
					break;
				}
				num1++;
			}
			while (vad[ihm-num2].DEC==RADAR_DATA_NOECHO)
			{
				if (ihm-num2==0)
				{
					break;
				}
				num2++;
			}
			vad[ihm].SVC = R1*(vad[ihm].DEC+RADIUS_EARTH/3.0)*vad[ihm].CF0/(RADIUS_EARTH*vad[ihm].AS)*(-3.0)/2.0
				-(vad[ihm].ASC/vad[ihm].AS+DRH/RH)*vad[ihm-num1].SVW;
			vad[ihm].SVW = vad[ihm-num1].SVW+vad[ihm].SVC*(vad[ihm].DEC-vad[ihm-num2].DEC);
			double CFI = vad[ihm].CF0-vad[ihm].SVW*sin(El);
			vad[ihm].DIV = 2*CFI/(R1*cos(El))+2*vad[ihm].VPF*tan(El)/R1;
		}
		if (Q != NULL)
		{
			delete []Q;
			Q = NULL;
		}
		if (CF != NULL)
		{
			delete []CF;
			CF = NULL;
		}
		if (X2 != NULL)
		{
			delete []X2;
			X2 = NULL;
		}
	}

	return true;
}
bool Arith_285_EVP_269_VWP_268_VAD::Compute_Stretch_Shear(VAD* vad, int* X2, StandardRadarData* pGData, int tilt, int ihm, double R1)
{
	double Temp_V[5][6];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Temp_V[i][j] = 0;
		}
	}
	int nrays = pGData->GetElevationHeadP(tilt)->AzimuthNumber;
	int count = 0;
	for (int i = 0;i < nrays; i++)
	{
		if (X2[i] != RADAR_DATA_NOECHO)
		{
			Temp_V[0][0] += X2[i]*0.01;
			double azm = i * M_PI/180.0;
			Temp_V[1][0] += sin(azm);
			Temp_V[2][0] += cos(azm);
			Temp_V[3][0] += sin(2.0*azm);
			Temp_V[4][0] += cos(2.0*azm);
			count++;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		Temp_V[i][0] /= count;
	}
	for (int i = 0; i < nrays; i++)
	{
		if (X2[i] == RADAR_DATA_NOECHO)
		{
			continue;
		}
		double azm = i * M_PI/180.0;
		double sina = sin(azm);
		double cosa = cos(azm);
		double sin2a = sin(2.0*azm);
		double cos2a = cos(2.0*azm);
		Temp_V[1][1] += (X2[i]*0.01 - Temp_V[0][0])*(sina - Temp_V[1][0]);
		Temp_V[2][1] += (X2[i]*0.01 - Temp_V[0][0])*(cosa - Temp_V[2][0]);
		Temp_V[3][1] += (X2[i]*0.01 - Temp_V[0][0])*(sin2a - Temp_V[3][0]);
		Temp_V[4][1] += (X2[i]*0.01 - Temp_V[0][0])*(cos2a - Temp_V[4][0]);

		Temp_V[1][2] += (sina - Temp_V[1][0])*(sina - Temp_V[1][0]);
		Temp_V[2][2] += (sina - Temp_V[1][0])*(cosa - Temp_V[2][0]);
		Temp_V[3][2] += (sina - Temp_V[1][0])*(sin2a - Temp_V[3][0]);
		Temp_V[4][2] += (sina - Temp_V[1][0])*(cos2a - Temp_V[4][0]);

		Temp_V[2][3] += (cosa-Temp_V[2][0]) * (cosa-Temp_V[2][0]);
		Temp_V[3][3] += (cosa-Temp_V[2][0]) * (sin2a-Temp_V[3][0]);
		Temp_V[4][3] += (cosa-Temp_V[2][0]) * (cos2a-Temp_V[4][0]);

		Temp_V[3][4] += (sin2a-Temp_V[3][0]) * (sin2a-Temp_V[3][0]);
		Temp_V[4][4] += (sin2a-Temp_V[3][0]) * (cos2a-Temp_V[4][0]);

		Temp_V[4][5] += (cos2a-Temp_V[4][0]) * (cos2a-Temp_V[4][0]);
	}
	for (int i = 1; i < 5; i++)
	{
		for (int j = 1; j < 6; j++)
		{
			Temp_V[i][j] /= count; 
		}
	} 
	double Temp_V11 = Temp_V[1][2] * Temp_V[2][1] - (Temp_V[1][1] * Temp_V[2][2]); /* A */
	double Temp_V21 = Temp_V[1][2] * Temp_V[3][1] - (Temp_V[1][1] * Temp_V[3][2]); /* B */
	double Temp_V31 = Temp_V[1][2] * Temp_V[4][1] - (Temp_V[1][1] * Temp_V[4][2]); /* C */
	double Temp_V41 = Temp_V[1][2] * Temp_V[2][3] - (Temp_V[2][2] * Temp_V[2][2]); /* M */
	double Temp_V51 = Temp_V[1][2] * Temp_V[3][3] - (Temp_V[2][2] * Temp_V[3][2]); /* N */
	double Temp_V12 = Temp_V[1][2] * Temp_V[4][3] - (Temp_V[4][2] * Temp_V[2][2]); /* P */
	double Temp_V22 = Temp_V[1][2] * Temp_V[4][4] - (Temp_V[4][2] * Temp_V[3][2]); /* Q */
	double Temp_V32 = Temp_V[1][2] * Temp_V[3][4] - (Temp_V[3][2] * Temp_V[3][2]); /* R */
	double Temp_V42 = Temp_V[1][2] * Temp_V[4][5] - (Temp_V[4][2] * Temp_V[4][2]); /* S */

	double v1 = Temp_V42*Temp_V41-(Temp_V12*Temp_V12); /* S*M-P*P */
	double v2 = Temp_V22*Temp_V41-(Temp_V12*Temp_V51); /* Q*M-P*N */
	double v3 = Temp_V32*Temp_V41-(Temp_V51*Temp_V51); /* R*M-N*N */

	double dvv = v1*v3 - v2*v2;
	if (fabs(dvv) <= 0.0000005)
	{
		Temp_V[0][5] = 0.0;
	}
	else
	{
		Temp_V[0][5] = ((Temp_V41*Temp_V31-Temp_V11*Temp_V12)*v3 - (Temp_V41*Temp_V21-Temp_V11*Temp_V51)*v2)/dvv;
	}
	if (fabs(v3) <= 0.0000005)
	{
		Temp_V[0][4] = 0.0;
	}
	else
	{
		Temp_V[0][4] = (Temp_V41 * Temp_V21 - Temp_V11 * Temp_V51 - Temp_V[0][5] * v2) / v3;
	}
	if (fabs(Temp_V41) <= 0.0000005)
	{
		Temp_V[0][3] = 0.0;
	}
	else
	{
		Temp_V[0][3] = ( Temp_V11 - Temp_V[0][4] * Temp_V51 - Temp_V[0][5] * Temp_V12 ) / Temp_V41;
	}
	if (fabs(Temp_V[1][2]) <= 0.0000005)
	{
		Temp_V[0][2] = 0.0;
	}
	else
	{
		Temp_V[0][2] = (Temp_V[1][1]-Temp_V[0][3]*Temp_V[1][3]- Temp_V[0][4]*Temp_V[1][4] -Temp_V[0][3]*Temp_V[1][3]) / Temp_V[1][2];
	}

	Temp_V[0][1] = Temp_V[0][0] -Temp_V[0][2]*Temp_V[1][0] - Temp_V[0][3]*Temp_V[2][0] - 
		Temp_V[0][4]*Temp_V[3][0] - Temp_V[0][5]*Temp_V[4][0];

	vad[ihm].stretch = (float)((2.0*Temp_V[0][4])/(R1*1000*m_Cos_e2));
	vad[ihm].shear = (float)((2.0*Temp_V[0][5])/(R1*1000*m_Cos_e2));	

	return true;
}
//void Arith_285_EVP_269_VWP::VAD_data_deal_with(VAD* vad)
//{
//
//	for(int i=0; i<1550; i++)
//	{
//		if (m_cnt[i]!=0)
//		{
//			float East = East_W_SW[i]/m_cnt[i];
//			float South = South_N_SW[i]/m_cnt[i];
//			vad[i].SHW = sqrt(East*East+South*South);
//			vad[i].DW = atan2(East, South)+M_PI;
//			vad[i].RMS = RMS[i]/m_cnt[i];
//		}
//	}
//	 
//}
void Arith_285_EVP_269_VWP_268_VAD::VAD_mean(VAD* vad)
{
	int k, bret;
	float TempV1,TempV2,TempV3,TempV4;

	float Eest_W_W[1550], South_N_W[1550];
	memset(Eest_W_W, 0, 1550*sizeof(float));
	memset(South_N_W, 0, 1550*sizeof(float));

	double **ZyVAD = NULL;
	ZyVAD = new double* [7];
	for (int i=0; i<7; i++)
	{
		ZyVAD[i] = new double[1550];
		for (int j=0; j<1550; j++)
		{
			ZyVAD[i][j] = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 先排序，得到点数
	k = 0;
	for (int i = 0; i < 1550; i++)
	{
		if (vad[i].DW > 0&& vad[i].SHW > 0 )
		{
			ZyVAD[0][k] = i;
			ZyVAD[1][k] = vad[i].DIV;
			ZyVAD[2][k] = vad[i].SHW;
			ZyVAD[3][k] = vad[i].DW;
			ZyVAD[4][k] = vad[i].RMS;
			ZyVAD[5][k] = vad[i].stretch;
			ZyVAD[6][k] = vad[i].shear;
			Eest_W_W[k]=float(-vad[i].SHW * sin(vad[i].DW));
			South_N_W[k] = float(-vad[i].SHW * cos(vad[i].DW));
			k++;
		}
	}
	bret = k;

	for(int i=1; i<3; i++)
	{
		if (i == 1)
		{
			for (int j=0; j<bret; j++)
			{
				TempV4=0;
				TempV3=0;
				TempV2=0;
				TempV1=0;
				k=0;
				for (int m=0; m<31; m++)
				{
					int temp = j-5+m;
					if (temp < 0)
					{
						continue;
					}
					if (temp >= bret)
					{
						break;
					}
					TempV1 += ZyVAD[1][temp];
					TempV2 += ZyVAD[4][temp];
					TempV3 += ZyVAD[5][temp];
					TempV4 += ZyVAD[6][temp];
					k++;
				}

				TempV1 = (float)TempV1/k;
				TempV2 = (float)TempV2/k;				
				TempV3 = (float)TempV3/k;				
				TempV4 = (float)TempV4/k;
				ZyVAD[1][j] = TempV1;			
				ZyVAD[4][j] = TempV2;				
				ZyVAD[5][j] = TempV3;				
				ZyVAD[6][j] = TempV4;
				int n = int(ZyVAD[0][j]);
				vad[n].DIV = ZyVAD[1][j];
				vad[n].RMS = ZyVAD[4][j];
				vad[n].stretch = ZyVAD[5][j];
				vad[n].shear = ZyVAD[6][j];
			}
		}
		if (i == 2)
		{
			for (int j=0; j<bret; j++)
			{
				TempV1=0;
				TempV2=0;
				k=0;
				for (int m=0; m<31; m++)
				{
					int temp = j-5+m;
					if (temp < 0)
					{
						continue;
					}
					if (temp >= bret)
					{
						break;
					}
					TempV1 += Eest_W_W[temp];
					TempV2 += South_N_W[temp];
					k++;
				}

				TempV1 = (float)TempV1/k;
				TempV2 = (float)TempV2/k;
				int n = int(ZyVAD[0][j]);
				ZyVAD[2][j] = sqrt(TempV1*TempV1+TempV2*TempV2);
				ZyVAD[3][j] = float(atan2(TempV1,TempV2)*180/M_PI+180);
				if (ZyVAD[3][j] < 0)
				{
					ZyVAD[3][j] += 360;
				}
				if (ZyVAD[3][j] >= 360)
				{
					ZyVAD[3][j] -= 360;
				}
				vad[n].SHW = ZyVAD[2][j];
				vad[n].DW = ZyVAD[3][j];
			}
		}
	}
	for (int i=0; i<7; i++)
	{
		delete[] ZyVAD[i];
	}
	delete []ZyVAD;
	ZyVAD = NULL;
}
inline complex Arith_285_EVP_269_VWP_268_VAD::Complex(double a, double b)
{
	complex c;
	c.real = a;
	c.image = b;
	return c;
}
inline complex Arith_285_EVP_269_VWP_268_VAD::ComplexPlus(complex a, complex b)
{
	complex c;
	c.real = a.real+b.real;
	c.image = a.image+b.image;
	return c;
}
inline complex Arith_285_EVP_269_VWP_268_VAD::ComplexMinus(complex a, complex b)
{
	complex c;
	c.real = a.real-b.real;
	c.image = a.image-b.image;
	return c;
}
inline complex Arith_285_EVP_269_VWP_268_VAD::ComplexMultiply(complex a, complex b)
{
	complex c;
	c.real = a.real*b.real - a.image*b.image;
	c.image = a.real*b.image + a.image*b.real;
	return c;
}
inline complex Arith_285_EVP_269_VWP_268_VAD::ComplexDivide(complex a, complex b)
{
	complex c;
	c.real=(a.real*b.real+a.image*b.image)/(b.real*b.real+b.image*b.image);
	c.image=(a.real*b.image*(-1)+a.image*b.real)/(b.real*b.real+b.image*b.image);
	return c;
} 
#endif