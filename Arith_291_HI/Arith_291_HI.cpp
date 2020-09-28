/////////////////////////////////////////
//Arith_291_HI
//created by zsc 
////////////////////////////////////////
//实现冰雹探测算法
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_291_HI.h"
#include "CJulMsGMT.h"
#include "math.h"
#include "GDef.h"
#include <fstream>

Arith_291_HI::Arith_291_HI()
{
	;
}
Arith_291_HI::~Arith_291_HI()
{
	;
}
GHRESULT Arith_291_HI::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;

	return GS_OK;
}
GHRESULT Arith_291_HI::LoadData(void * pValue, ReadParam* ReadParameter)
{
	if (pValue == NULL)
	{
		return GE_INVALIDARG;
	}
	m_pInputList = (GXList<GenerationData *>*)pValue;
	m_pParameter = ReadParameter;

	return GS_OK;
}
GHRESULT Arith_291_HI::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_291_HI.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"291_HI：  Started: "<<endl;

	logfile<<"291_HI：  Get and Check the m_pInputList's Count."<<endl;

	if (m_pInputList->GetCount()<=0)
	{
		logfile<<"291_HI： The m_pInputList's Count < 1 and Return GE_SHORTOFINDATA."<<endl;
		logfile.close();
		return GE_SHORTOFINDATA;
	}

	logfile<<"291_HI：  Send in and Read Parameters."<<endl;
	XML_Indexhi index;
	index.Threshold_HKE_RW_Lower = 40;    //dBz
	index.Threshold_HKE_RW_Upper = 50;    //dBz
	index.Threshold_POH_minR = 45;         //dBz
	index.HeightTop_0 = 4.8;         //3~5.5km
	index.HeightTop_20 = 8;		//6.5~8.5km
	index.POH_HD1 = 1.625;				//1.625km
	index.POH_HD2 = 1.875;				//1.875km
	index.POH_HD3 = 2.125;				//2.125km
	index.POH_HD4 = 2.375;				//2.375km
	index.POH_HD5 = 2.625;				//2.625km
	index.POH_HD6 = 2.925;				//2.925km;
	index.POH_HD7 = 3.3;				//3.3km
	index.POH_HD8 = 3.75;				//3.75km
	index.POH_HD9 = 4.5;				//4.5km;
	index.POH_HD10 = 5.5;				//5.5km

	logfile<<"291_HI：  Send In Product_277_SCI and Create Output Product_291_HI."<<endl;

	Product_277_SCI* pGData = &(*(m_pInputList->GetHeadAs<Product_277_SCI >()));
	m_pOutputList->AddTail();
	Product_291_HI* Product = &(*(m_pOutputList->GetTailAs<Product_291_HI>()));

	logfile<<"291_HI：  Get Send-in Product's PDBlock."<<endl;
	PDBlock * pdbIn = pGData->GetMyPDBlock();
	TAGSSHEAD* sscIn = (TAGSSHEAD*)pGData->GetBlockAsStruct_Data(0);//modified by cm 20090616
	if (NULL == sscIn) return GE_FAILOFNEWPRODUMEM;
	gInt32 NumOfStorm = sscIn->NumberOfStormCells;
	GHRESULT ConstrutHand = NULL ;
	ConstrutHand = Product->ConstructIt(1, sizeof(HAIL_HEAD), NumOfStorm, sizeof(HAIL_INDEX));
	if(ConstrutHand != GS_OK)
	{
		logfile<<"291_HI：  Create New Product_291_HI Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"291_HI：  Get and Set RadarInfor Information."<<endl;
	PDBlock * pdbPdt = Product->GetMyPDBlock();

	Product->GetRadarInfor()->Altitude = pGData->GetRadarInfor()->Altitude;
	Product->GetRadarInfor()->Latitude = pGData->GetRadarInfor()->Latitude;
	Product->GetRadarInfor()->Longitude = pGData->GetRadarInfor()->Longitude;
	memcpy(Product->GetRadarInfor()->RadarType, pGData->GetRadarInfor()->RadarType, sizeof(gInt8)*4);
	memcpy(Product->GetRadarInfor()->SiteCode, pGData->GetRadarInfor()->SiteCode, sizeof(gInt8)*8);

	pdbPdt->ProductCode = 291;
	pdbPdt->ProductDate = pdbIn->ProductDate;
	pdbPdt->ProductTime = pdbIn->ProductTime;

	CJulMsGMT::Jul2Greg(pdbPdt->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdbPdt->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"291_HI：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	memcpy(pdbPdt->OperationalMode, pdbIn->OperationalMode, sizeof(gInt8)*2);
	memcpy(pdbPdt->VCPMode, pdbIn->VCPMode, sizeof(gInt8)*4);
	CJulMsGMT::GetLocalJulMs(pdbPdt->GenerationDateOfProduct, pdbPdt->GenerationTimeOfProduct);
	memcpy(pdbPdt->ProductDependent,pdbIn->ProductDependent,sizeof(gInt16)*20);
	pdbPdt->ScaleOfData = 1;
	pdbPdt->IndexOfTabular = -1;
	pdbPdt->IndexOfGraphic = -1;

	logfile<<"291_HI：  Get TAGSSHEAD and HAIL_HEAD To Set Information."<<endl;
	//TAGSSHEAD* sscIn = (TAGSSHEAD*)pGData->GetBlockAsStruct_Data(0);
	HAIL_HEAD* pdthead = (HAIL_HEAD*)Product->GetBlockAsStruct_Data(0);
	pdthead->NumOfStormCells = NumOfStorm;
	pdthead->VolScanTime = sscIn->VolumeScanStartTime;
	pdthead->POH_MinThreshold = 0.3;		//冰雹最小显示阈值 （10%—100%），缺省30%     Modified by cm 20090617
	pdthead->POH_Threshold = 0.5;			//冰雹填充阈值 （10%—100%）缺省50%
	pdthead->POSH_MinThreshold = 0.3;		//强冰雹最小显示阈值 （10%—100%）缺省30%
	pdthead->POSH_Threshold = 0.5;		//强冰雹填充阈值 （10%—100%）缺省50%
	//gInt32 NumOfStorm = sscIn->NumberOfStormCells;

	StructHead* sthIn = pGData->GetBlockAsStruct_Head(1);
	STORM_CELLATTRIBUTES* stcIn = (STORM_CELLATTRIBUTES*)pGData->GetBlockAsStruct_Data(1);
	HAIL_INDEX* pdtHI = (HAIL_INDEX*)Product->GetBlockAsStruct_Data(1);

	logfile<<"291_HI：  Allocate New Memory Variables."<<endl;
	gFloat * SHI = NULL;
	SHI = new gFloat[NumOfStorm];
	memset(SHI, 0, NumOfStorm*sizeof(gFloat));

	gFloat * H_POH = NULL;
	H_POH = new gFloat[NumOfStorm];
	memset(H_POH, 0, NumOfStorm*sizeof(gFloat));

	gFloat * DH_POH = NULL;
	DH_POH = new gFloat[NumOfStorm];
	memset(DH_POH, 0, NumOfStorm*sizeof(gFloat));

	gFloat * POH = NULL;
	POH = new gFloat[NumOfStorm];
	memset(POH, 0, NumOfStorm*sizeof(gFloat));

	gFloat * POSH = NULL;
	POSH = new gFloat[NumOfStorm];
	memset(POSH, 0, NumOfStorm*sizeof(gFloat));

	gFloat * MEHS = NULL;
	MEHS = new gFloat[NumOfStorm];
	memset(MEHS, 0, NumOfStorm*sizeof(gFloat));

	gFloat WTSM = WT_COF*index.HeightTop_0+WT_OFS;   //100000 J/(m*s)
	if (WTSM < 20)
	{
		WTSM = 20;
	}

	gFloat * HTC = NULL;
	gFloat * HBC = NULL;
	gFloat * maxR = NULL;

	logfile<<"291_HI：  Loop : i=0-->NumOfStorm("<<NumOfStorm<<")"<<endl;
	for (int i=0; i<NumOfStorm; i++)
	{
		logfile<<"291_HI：  i="<<i<<":"<<endl;
		gInt16 NumOfComponent = stcIn[i].sci.NSC;
		HTC = new gFloat[NumOfComponent];
		memset(HTC, 0, NumOfComponent*sizeof(gFloat));
		HBC = new gFloat[NumOfComponent];
		memset(HBC, 0, NumOfComponent*sizeof(gFloat));    //0或者默认值
		maxR = new gFloat[NumOfComponent];

		for (int j=0; j<NumOfComponent; j++)
		{
			maxR[j] = stcIn[i].NEC[j+1].maxZ/10;
		}

		logfile<<"291_HI：   Loop : j=0-->NumOfComponent("<<NumOfComponent<<") and Call Func ComHeightTop_C(),ComHailKE_C(),ComHeightDif_POH()."<<endl;
		for (int j=0; j<NumOfComponent; j++)
		{
			logfile<<"291_HI：   Loop : j="<<j<<" and will come into if(1)."<<endl;
			if (maxR[j]>index.Threshold_HKE_RW_Lower)
			{
				logfile<<"291_HI：   Loop : j="<<j<<" and come into if(1)."<<endl;
				ComHeightTop_C(i, j, stcIn, NumOfComponent, HTC);
				logfile<<"291_HI：   Loop : j="<<j<<" and Called Func ComHeightTop_C()."<<endl;

				logfile<<"291_HI：   Loop : j="<<j<<" and will come into if(2)."<<endl;
				if ((HTC[j]>index.HeightTop_0 && index.HeightTop_0>stcIn[i].NEC[j+1].hei && maxR[j]>index.Threshold_HKE_RW_Lower)//maxR[j+1]下标有误
					||stcIn[i].NEC[j+1].hei>=index.HeightTop_0)
				{
					logfile<<"291_HI：   Loop : j="<<j<<" and come into if(2)."<<endl;
					if (j!=0)
					{
						HBC[j] = (stcIn[i].NEC[j+1].hei+stcIn[i].NEC[j].hei)/2;
					}

					DH_POSH = (index.HeightTop_0>HBC[j])?HTC[j]-index.HeightTop_0:HTC[j]-HBC[j];
									
					WZ =(maxR[j]>=index.Threshold_HKE_RW_Upper)?1:(maxR[j]-index.Threshold_HKE_RW_Lower)/(index.Threshold_HKE_RW_Upper-index.Threshold_HKE_RW_Lower);
					
					HMC = (HTC[j]+HBC[j])/2;

					if (index.HeightTop_0 > HBC[j])
					{
						HMC = (HTC[j]+index.HeightTop_0)/2;
					}
					else if ((j!=0)&&(j!=NumOfComponent-1)&&(!((index.HeightTop_0>HBC[j-1])&&(HTC[j-1]>index.HeightTop_0)&&(maxR[j-1]<=index.Threshold_HKE_RW_Lower))))
					{
						HMC = (stcIn[i].NEC[j].hei+stcIn[i].NEC[j+2].hei)/2;
					}

					VTWF = HMC >= index.HeightTop_20?1:(HMC-index.HeightTop_0)/(index.HeightTop_20-index.HeightTop_0);					

					logfile<<"291_HI：   Loop : j="<<j<<" and before Called Func ComHailKE_C()."<<endl;
					ComHailKE_C(WZ,  maxR[j], HKE);
					logfile<<"291_HI：   Loop : j="<<j<<" and Called Func ComHailKE_C()."<<endl;

					SHI[i] += HKE*DH_POSH*VTWF;       //100000J/(m*s)
				}
				else
				{
					logfile<<"291_HI：   Loop : j="<<j<<" and does not come into if(2)."<<endl;
				}
			}
			else
			{
				logfile<<"291_HI：   Loop : j="<<j<<" and does not come into if(1)."<<endl;
			}


			logfile<<"291_HI：   Loop : j="<<j<<" and will come into if(3)."<<endl;
			if (stcIn[i].NEC[j+1].hei>=index.HeightTop_0 && maxR[j]>=index.Threshold_POH_minR)
			{
				logfile<<"291_HI：   Loop : j="<<j<<" and come into if(3)."<<endl;
				ComHeightDif_POH(H_POH, i, j, stcIn, maxR, index, NumOfComponent);
				logfile<<"291_HI：   Loop : j="<<j<<" and Called Func ComHeightDif_POH()."<<endl;
				DH_POH[i] = H_POH[i] - index.HeightTop_0;
			}
			else
			{
				logfile<<"291_HI：   Loop : j="<<j<<" and does not come into if(3)."<<endl;
			}
		}
		logfile<<"291_HI：   Loop j Finished."<<endl;
		logfile<<"291_HI：   Call Function: ComPOH()."<<endl;
		ComPOH(i, DH_POH, index, POH);
		logfile<<"291_HI：   Call Function: ComPOSH()."<<endl;
		ComPOSH(i, SHI, POSH, WTSM, POH);
		MEHS[i] = HS_COF*pow(SHI[i], float(HS_EXP));

		pdtHI[i].SerialNum = i;//modified by cm 20090617
		pdtHI[i].POH = POH[i];
		pdtHI[i].POSH = POSH[i];
		pdtHI[i].MEHS = MEHS[i]+0.5;// (MEHS[i]+0.5)*10
		pdtHI[i].az = stcIn[i].sci.az; // added by cm 20090617
		pdtHI[i].r = stcIn[i].sci.r;
		printf("%d    %d    %d    %d \n",i,pdtHI[i].POH ,pdtHI[i].POSH,pdtHI[i].MEHS);

		if (HTC != NULL)
		{
			delete []HTC;
			HTC = NULL;
		}
		if (HBC != NULL)
		{
			delete []HBC;
			HBC = NULL;
		}
		if (maxR != NULL)
		{
			delete []maxR;
			maxR = NULL;
		}
	}
	logfile<<"291_HI：  Loop i Finished."<<endl;

	logfile<<"291_HI：  Release Dynamic Memory Variables."<<endl;
	if (SHI != NULL)
	{
		delete []SHI;
		SHI = NULL;
	}
	if (H_POH != NULL)
	{
		delete []H_POH;
		H_POH = NULL;
	}
	if (DH_POH != NULL)
	{
		delete []DH_POH;
		DH_POH = NULL;
	}
	if (POH != NULL)
	{
		delete []POH;
		POH = NULL;
	}
	if (POSH != NULL)
	{
		delete[]POSH;
		POSH = NULL;
	}
	if (MEHS != NULL)
	{
		delete []MEHS;
		MEHS = NULL;
	}
	logfile<<"291_HI：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
void Arith_291_HI::ComHeightTop_C(int i, int j,STORM_CELLATTRIBUTES* stcIn, gInt16 NumOfComponent, gFloat* HTC)
{
	if (j == NumOfComponent-1)
	{
		HTC[j] = stcIn[i].NEC[j+1].hei+(stcIn[i].NEC[j+1].hei-stcIn[i].NEC[j].hei)/2;
	}
	else
	{
		HTC[j] = (stcIn[i].NEC[j+1].hei+stcIn[i].NEC[j+2].hei)/2;
	}
}
void Arith_291_HI::ComHailKE_C(gFloat WZ, gFloat  maxRR, gFloat& HKE)
{
	HKE = COEF_HKE_1*WZ*pow(COEF_HKE_3, COEF_HKE_2*maxRR);    //100J/(m*m*s)
}
void Arith_291_HI::ComHeightDif_POH(gFloat* H_POH, int i, int j, STORM_CELLATTRIBUTES* stcIn, gFloat * maxR, XML_Indexhi index, int NumOfComponent)
{
	gFloat * DD = NULL;
	DD = new gFloat[6];

	if (j == NumOfComponent-1)
	{
		DD[0] = maxR[j-1] - maxR[j];
		DD[1] = maxR[j-1] - index.Threshold_POH_minR;
		DD[2] = stcIn[i].NEC[j+1].hei - stcIn[i].NEC[j].hei;
		H_POH[i] = stcIn[i].NEC[j+1].hei+(stcIn[i].NEC[j+1].hei-stcIn[i].NEC[j].hei)/2;
		gFloat H_INT = DD[1]*DD[2]/DD[0] + stcIn[i].NEC[j].hei;
		if (DD[0] <= 0)
		{
			if (maxR[j]-index.Threshold_POH_minR<0.0001 && maxR[j]-index.Threshold_POH_minR>-0.0001)
			{
				H_POH[i] = stcIn[i].NEC[j+1].hei;
			} 
		}
		else if (H_INT < H_POH[i])
		{
			H_POH[i] = H_INT;
		}
	}
	else
	{
		DD[3] = maxR[j] - index.Threshold_POH_minR;
		DD[4] = (1.0 > maxR[j]-maxR[j+1])?1.0:maxR[j]-maxR[j+1];
		DD[5] = stcIn[i].NEC[j+2].hei - stcIn[i].NEC[j+1].hei;
		if (maxR[j+1] < index.Threshold_POH_minR)
		{
			H_POH[i] = DD[3]*DD[5]/DD[4] + stcIn[i].NEC[j+1].hei;
		}
	}
	if (DD != NULL)
	{
		delete []DD;
		DD = NULL;
	}
}
void Arith_291_HI::ComPOH(int i, gFloat* DH_POH, XML_Indexhi index, gFloat * POH)
{
	if (DH_POH[i] <= index.POH_HD1)
	{
		POH[i] = 0;
	}
	else if (DH_POH[i] <= index.POH_HD2)
	{
		POH[i] = 10;
	}
	else if (DH_POH[i] <= index.POH_HD3)
	{
		POH[i] = 20;
	}
	else if (DH_POH[i] <= index.POH_HD4)
	{
		POH[i] = 30;
	}
	else if (DH_POH[i] <= index.POH_HD5)
	{
		POH[i] = 40;
	}
	else if (DH_POH[i] <= index.POH_HD6)
	{
		POH[i] = 50;
	}
	else if (DH_POH[i] <= index.POH_HD7)
	{
		POH[i] = 60;
	}
	else if (DH_POH[i] <= index.POH_HD8)
	{
		POH[i] = 70;
	}
	else if (DH_POH[i] <= index.POH_HD9)
	{
		POH[i] = 80;
	}
	else if (DH_POH[i] <= index.POH_HD10)
	{
		POH[i] = 90;
	}
	else
	{
		POH[i] = 100;
	}
}
void Arith_291_HI::ComPOSH(int i, gFloat* SHI, gFloat* POSH, gFloat WTSM, gFloat* POH)
{
	if (SHI[i] > 0)
	{
		POSH[i] = COEF_POSH*(log(SHI[i]/WTSM))+OFFSET_POSH;
	}
	else
	{
		POSH[i] = 0;
	}
	if (POSH[i] > 100)
	{
		POSH[i] = 100;
	}
	else if (POSH[i] < 0)
	{
		POSH[i] = 0;
	}

	if (POH[i] < POSH[i])
	{
		POH[i] = POSH[i];
	}
}
GHRESULT Arith_291_HI::OutPutData(void * pValue)
{
	if (pValue == NULL)
	{
		return GE_INVALIDARG;
	}
	m_pOutputList = (GXList<GenerationData>*)pValue;
	return GS_OK;
}
GHRESULT Arith_291_HI::UnInitialize()
{
	return GS_OK;
}
extern "C"  DLLEXP_GSYS IArith * Create_arith(void)
{
	return  new(::std::nothrow) Arith_291_HI;
}
