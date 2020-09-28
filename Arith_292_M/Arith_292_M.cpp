#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_292_M.h"
#include "Product_292_M.h"
#include "../Arith_254_V/Product_254_V.h"
#include "CJulMsGMT.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

#define RE 6371

Arith_292_M::Arith_292_M(void)
{
}

Arith_292_M::~Arith_292_M(void)
{
}

GHRESULT Arith_292_M::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;

	return GS_OK;
};
GHRESULT Arith_292_M::UnInitialize()
{
	return GS_OK;
};

GHRESULT Arith_292_M::LoadData(void* pdata, ReadParam * Paramter)
{
	if(pdata == NULL) return GE_INVALIDARG;
	if(!Paramter) return GE_INVALIDARG;

	m_pInputList = (GXList<GenerationData *>*)pdata;

	m_fnParam = Paramter;

	return GS_OK;
};

GHRESULT Arith_292_M::OutPutData(void* pdata)
{
	if(pdata == NULL) return GE_INVALIDARG;

	m_pOutputList = (GXList<GenerationData>*)pdata;

	return GS_OK;
};

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_292_M接口
{
	return new (::std::nothrow) Arith_292_M;
}

GHRESULT Arith_292_M::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_292_M.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"292_M：  Started: "<<endl;

	logfile<<"292_M：  Get and Check the m_pInputList's Count."<<endl;

	if(m_pInputList->GetCount()< 1)
	{
		logfile<<"292_M：  m_pInputList's Count<1 and Return GE_SHORTOFINDATA."<<endl;
		logfile.close();
		return GE_SHORTOFINDATA;
	}

	m_Param.TFHeight = 8.0f;
	m_Param.THMomen = 540.0f;	
	m_Param.TRadiDis = 0.75f;
	m_Param.TMesoHighS = 14.4f;
	m_Param.TLMoment = 180.0f;
	m_Param.TMesoLowS = 7.2f; 
	m_Param.TMesoAzi = 0.034f;
	m_Param.TPV = 8.0f;
	m_Param.TFMaxRat = 4.0f;
	m_Param.TFMinRat = 1.6f;
	m_Param.TMaxRat = 2.0f;
	m_Param.TMinRat = 0.5f;
	m_Param.TRange = 140.0f;

	logfile<<"292_M：  Create Send-out Product: Product_292_M."<<endl;
	m_pOutputList->AddTail();
	Product_292_M* pProduct = &(*(m_pOutputList->GetTailAs<Product_292_M>()));
	GHRESULT ConstrutHand;
	ConstrutHand = pProduct->ConstructIt(1,sizeof(MHead),50,sizeof(VortexStruct), 50, sizeof(MesocStruct));
	if(ConstrutHand != GS_OK)
	{
		logfile<<"292_M：  Create Product_292_M failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}
	PDBlock * pPdbOut = pProduct->GetMyPDBlock();

	Product_254_V * pPtIn = &(*(m_pInputList->GetHeadAs<Product_254_V >()));

	//pPdbIn->GetMyRadial_RadialData();
	logfile<<"292_M：  Get and Set RadarInfor Information."<<endl;
	PDBlock * pPdbIn = pPtIn->GetMyPDBlock();
	gInt16 NumOfLayer = pPdbIn->NumberOfLayers;

	//pPdbOut->ElevationNumber = pPdbIn->ElevationNumber;
	memcpy(pProduct->GetRadarInfor()->RadarType, pPtIn->GetRadarInfor()->RadarType, sizeof(gInt8)*4);
	pProduct->GetRadarInfor()->Altitude = pPtIn->GetRadarInfor()->Altitude;
	pProduct->GetRadarInfor()->Latitude = pPtIn->GetRadarInfor()->Latitude;
	pProduct->GetRadarInfor()->Longitude = pPtIn->GetRadarInfor()->Longitude;
	pPdbOut->ProductCode = 292;
	memcpy(pPdbOut->VCPMode, pPdbIn->VCPMode, sizeof(gInt8)*4);
	memcpy(pPdbOut->OperationalMode, pPdbIn->OperationalMode, sizeof(gInt8)*2);
	pPdbOut->ProductDate = pPdbIn->ProductDate;
	pPdbOut->ProductTime = pPdbIn->ProductTime;

	CJulMsGMT::Jul2Greg(pPdbOut->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pPdbOut->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"292_M：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pPdbOut->ScaleOfData = 1; 
	pPdbOut->IndexOfTabular = -1;
	pPdbOut->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pPdbOut->GenerationDateOfProduct,pPdbOut->GenerationTimeOfProduct);
	memcpy(pProduct->GetRadarInfor()->SiteCode, pPtIn->GetRadarInfor()->SiteCode,sizeof(gChar)*8);
	//memcpy(pPdbOut->SiteName, pPdbIn->SiteName,sizeof(gChar)*18);
	//pPdbOut->ProductDependent[1] = pPtIn->ProductDependent[1];
	RadialHead *pRadHead = pPtIn->GetMyRadial_Head(0);
	if (NULL == pRadHead) return GS_FALSE;
	gInt16 IndexOfFirstRangeB = pRadHead->DistanceOfFirstGate;
	gInt16 NumberOfGates = pPtIn->GetMyRadial_Head(0)->NumberOfGates;
	gInt16 LengthOfGate = pPtIn->GetMyRadial_Head(0)->LengthOfGate;

	pPdbOut->ProductDependent[1] = (gInt16)((NumberOfGates*LengthOfGate + IndexOfFirstRangeB)/1000);	

	DILayer * pLayer = pProduct->GetMyLayer();
	pLayer->LayerDate = pPdbIn->ProductDate;	
	pLayer->LayerTime = pPdbIn->ProductTime;	
	pLayer->HeightOfElevation = 0;	

	FEAV * TOTALFEATUREs = new (::std::nothrow) FEAV[NumOfLayer];
	vector<FEAV> VortexV;
	logfile<<"292_M：  Loop:layer=0-->NumOfLayer("<<NumOfLayer<<")."<<endl;
	for(gInt16 layer = 0; layer < NumOfLayer; layer ++)
	{
		logfile<<"292_M：  layer="<<layer<<":"<<endl;
		gInt16 LenOfGate = pPtIn->GetMyRadial_Head(layer)->LengthOfGate;
		vector<PatV> PVector;
		vector<PVL> FEATUREs;

		logfile<<"292_M：   Call Function: PatternVector()."<<endl;
		PatternVector(pPtIn, layer, PVector/*, DeltaAngel, VDbeg, VDend, AZbeg, AZend*/);
		
		logfile<<"292_M：   Call Function: Feature()."<<endl;
		Feature(/*pPtIn, */PVector, FEATUREs/*, layer,DeltaAngel*/);
		
		vector<FEATUREStruct> FEATUREStructs;

		DILayer * pLayer = pPtIn->GetMyLayer(layer);
		gFloat angle = (gFloat)(pLayer->HeightOfElevation * 0.01 * M_PI/180.0);

		logfile<<"292_M：   Call Function: CharacterizeFeature()."<<endl;
		CharacterizeFeature(FEATUREs, FEATUREStructs, TOTALFEATUREs, layer, angle, LenOfGate/*, VDbeg, VDend, AZbeg, AZend*/);
	}
	logfile<<"292_M：  Loop layer Finished."<<endl;

	logfile<<"292_M：  Call Function Vortex()."<<endl;
	Vortex(pPtIn, NumOfLayer, TOTALFEATUREs, VortexV);
	VORTEXStruct * Vortices = new (::std::nothrow) VORTEXStruct[VortexV.size()];

	logfile<<"292_M：  Call Function: CharacterizeMesocyclone(VortexV, Vortices)."<<endl;
	CharacterizeMesocyclone(VortexV, Vortices);
	logfile<<"292_M：  Function Finished."<<endl;

	MHead *pMHead = (MHead*)pProduct->GetBlockAsStruct_Data(0);
	pMHead->VORTEXCnt = (gInt16)VortexV.size();	
	pMHead->Height = NumberOfGates * 2;
	pMHead->Width = NumberOfGates * 2;
	VortexStruct * VortexS = (VortexStruct*)pProduct->GetBlockAsStruct_Data(1);	
	MesocStruct * MesocS = (MesocStruct*)pProduct->GetBlockAsStruct_Data(2);
	gInt16 i = 0;

	logfile<<"292_M：  Loop: cnt=0-->VORTEXCnt"<<endl;
	for (gInt16 cnt = 0; cnt < pMHead->VORTEXCnt; cnt ++)
	{
		VortexS[cnt].VORTEXTNo = cnt;
		VortexS[cnt].VORTEXType = Vortices[cnt].VortexType;
		VortexS[cnt].FEATRUECnt = Vortices[cnt].FeatureCnt;
		VortexS[cnt].SMFeature.FCP.range = Vortices[cnt].RSFEcen;
		VortexS[cnt].SMFeature.FCP.elev = Vortices[cnt].elev;
		VortexS[cnt].SMFeature.FCP.azi = Vortices[cnt].AZcenfe;
		VortexS[cnt].SMFeature.HFC = Vortices[cnt].HFC;;
		VortexS[cnt].SMFeature.DAA = Vortices[cnt].DAA;
		VortexS[cnt].SMFeature.DAR = Vortices[cnt].DAR;
		VortexS[cnt].SMFeature.SRTmaxavg = Vortices[cnt].SRTmaxavg;
		VortexS[cnt].SMFeature.SRTmax = Vortices[cnt].SRTmax;
		VortexS[cnt].SMFeature.Mavg = Vortices[cnt].Mavg;
		VortexS[cnt].CenterX = Vortices[cnt].CenterX;
		VortexS[cnt].CenterY = Vortices[cnt].CenterY;
		VortexS[cnt].Radius = Vortices[cnt].Radius;
		VortexS[cnt].HighestHFC = Vortices[cnt].HighestHFC;
		VortexS[cnt].LowestHFC = Vortices[cnt].LowestHFC;
		if (Vortices[cnt].VortexType == _MESOCYCLONE)
		{
			MesocS[i].VORTEXTNo = cnt;
			//MesocS[i].HighestHFC = Vortices[cnt].HighestHFC;
			//MesocS[i].LowestHFC = Vortices[cnt].LowestHFC;
			MesocS[i].PM.azi = Vortices[cnt].AZcenfe;
			MesocS[i].PM.elev = Vortices[cnt].elev;
			MesocS[i].PM.range = Vortices[cnt].RSFEcen;
			MesocS[i].SPRavg = Vortices[cnt].SPRavg;
			MesocS[i].SRTavg = Vortices[cnt].SRTavg;
			MesocS[i].HightestSRTmax = cnt;
			MesocS[i].FE.FEATUREMaxAz = Vortices[cnt].FEATUREMaxAz;
			MesocS[i].FE.FEATUREMaxAz = Vortices[cnt].FEATUREMaxAz;
			MesocS[i].FE.FEATUREMaxRs = Vortices[cnt].FEATUREMaxRs;
			MesocS[i].FE.FEATUREMinRs = Vortices[cnt].FEATUREMinRs;
			i++;
		}
	}
	logfile<<"292_M：  Loop cnt Finished."<<endl;

	pMHead->MESOCCnt = i;

	if (TOTALFEATUREs != NULL)
	{
		delete [] TOTALFEATUREs;
	}
	if (Vortices != NULL)
	{
		delete [] Vortices;
	}
	logfile<<"292_M：  Finished."<<endl;
	logfile.close();

	if (pMHead->VORTEXCnt == 0)
	{
		m_pOutputList->clear();
		return GE_SHORTOFINDATA;
	}

	return GS_OK;
}

void Arith_292_M::PatternVector(GenerationData * p, gInt16 layer, vector<PatV> &PVector/*, gFloat DeltaAngel, 
								gFloat &VDbeg, gFloat &VDend, gFloat &AZbeg, gFloat &AZend*/)
{
	gFloat VDbeg = 0.0;
	gFloat VDend = 0.0; 
	gFloat AZbeg = 0.0;
	gFloat AZend = 0.0;
	Product_254_V * pPtIn = (Product_254_V *)p;
	RadialHead * pHead = pPtIn->GetMyRadial_Head(layer);

	gInt16 nRadial = pHead->NumberOfRadials;
	gInt16 NumberOfGates = pHead->NumberOfGates;
	gInt16 LengthOfGate = pHead->LengthOfGate;

	gInt16 ** pInData = new (::std::nothrow) gInt16*[nRadial];
	for (gInt16 n = 0; n < nRadial; n ++)
	{
		pInData[n] = new (::std::nothrow) gInt16[NumberOfGates];
		gInt16 *pT =  pPtIn->GetMyRadial_Data(layer, n);
		for (gInt16 m = 0; m < NumberOfGates; m ++)
		{
			pInData[n][m] = (pT[m] < RADAR_DATA_OUTOFRANGE)? RADAR_DATA_OUTOFRANGE: pT[m];
		}
	}

	int * TCY1;
	int * TCY2;
	TCY1 = new (::std::nothrow) int[NumberOfGates];
	TCY2 = new (::std::nothrow) int[NumberOfGates];

	for(gInt16 gate = 0; gate < NumberOfGates; gate ++)
	{
		TCY1[gate] = pInData[1][gate] - pInData[0][gate];
	}

	for(gInt16 gate = 0; gate < NumberOfGates; gate ++)
	{
		gInt16 End = 1;
		for(gInt16 Azi = 2; Azi != End; Azi ++)
		{
			if (TCY1[gate] <= 0 && End == 1)
			{
				End = Azi;
			}
			
			if (Azi == nRadial)
			{
				Azi = 0;
			}

			if (Azi > 0)
			{			
				TCY2[gate] = pInData[Azi][gate] - pInData[Azi - 1][gate];
			}
			else
			{
				TCY2[gate] = pInData[Azi][gate] - pInData[nRadial - 1][gate];
			}

			if(TCY1[gate] <= 0 && TCY2[gate] > 0)
			{
				RadialData * pRadialData = NULL;

				VDbeg = Azi >0 ? ((pInData[Azi-1][gate] > RADAR_DATA_OUTOFRANGE)? (gFloat)(pInData[Azi - 1][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData): 
					(gFloat)(pInData[Azi][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData)) : ((pInData[nRadial - 1][gate] > RADAR_DATA_OUTOFRANGE)? 
					(gFloat)(pInData[nRadial - 1][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData): (gFloat)(pInData[0][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData));
				pRadialData = Azi > 0 ? ((pInData[Azi-1][gate] > RADAR_DATA_OUTOFRANGE)? (pPtIn->GetMyRadial_RadialData(layer, Azi - 1)):
					(pPtIn->GetMyRadial_RadialData(layer, Azi))): ((pInData[nRadial - 1][gate] > RADAR_DATA_OUTOFRANGE)? 
					(pPtIn->GetMyRadial_RadialData(layer, nRadial - 1)): (pRadialData = pPtIn->GetMyRadial_RadialData(layer, 0)));

				AZbeg = (gFloat)(pRadialData->AzimuthAngle /** DeltaAngel*//100.0);	 
			}

			if(TCY1[gate] > 0 && TCY2[gate] <= 0 && End != 1)
			{
				RadialData * pRadialData = NULL;
				if (Azi > 0)
				{				
					VDend = (gFloat)(pInData[Azi - 1][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData);				
					pRadialData = pPtIn->GetMyRadial_RadialData(layer, Azi-1);
				}
				else
				{
					VDend = (gFloat)(pInData[nRadial - 1][gate] * 3600.0 / 1000.0 / pPtIn->GetMyPDBlock()->ScaleOfData);	
					pRadialData = pPtIn->GetMyRadial_RadialData(layer, nRadial-1);
				}
				AZend = (gFloat)(pRadialData->AzimuthAngle /** DeltaAngel*//100.0);	

				if(AZend < AZbeg)
				{
					AZend = (gFloat)360.0 + AZend;
				}

				gFloat RS = (gFloat)(gate * LengthOfGate/1000.0);
				gFloat M = abs((VDend - VDbeg)* RS * (AZend - AZbeg));
				gFloat SRT = abs((VDend - VDbeg) / (RS * (AZend - AZbeg)));
				if ((M > m_Param.TLMoment && SRT >= m_Param.TMesoHighS) || (M > m_Param.THMomen && SRT > m_Param.TMesoLowS))
				{
					PatV PV;
					PV.AZbegpv = AZbeg;
					PV.AZendpv = AZend;
					PV.MA = M;
					PV.SRT = SRT;
					PV.RS = RS;
					PV.VDbeg = VDbeg;
					PV.VDend = VDend;
					PV.InFeature = false;
					PVector.push_back(PV);
				}
			}
			TCY1[gate] = TCY2[gate];
		}
	}

	for (gInt16 n = 0; n < nRadial; n ++)
	{
		delete [] pInData[n];
	}
	delete [] pInData; 

	if (TCY1 != NULL)
	{
		delete [] TCY1;
	}

	if (TCY2 != NULL)
	{
		delete [] TCY2;
	}
}

void Arith_292_M::Feature(/*GenerationData * p,*/ vector<PatV> &PVector, vector<PVL> &FEATUREs/*, gInt16 layer, gFloat DeltaAngel*/)
{
	int PVsize = PVector.size();
	//Product_254_V * pPtIn = (Product_254_V *)p;
	//gFloat DeltaAngel = pPtIn->GetMyRadial_RadialData(layer, 0)->DeltaAngel/100.0;
	for (gInt16 cnt = 0; cnt < PVsize; cnt ++)
	{
		if (PVector[cnt].InFeature == false)
		{
			list<PatV> FEATURE;	
			PVector[cnt].InFeature = true;
			FEATURE.push_back(PVector[cnt]);
			for (list<PatV>::iterator it = FEATURE.begin(); it != FEATURE.end(); it++)
			{
				for (gInt16 j = cnt + 1; j < PVsize; j++ )
				{
					if (PVector[j].InFeature == false)
					{
						gFloat AzibegDif = abs(PVector[j].AZbegpv - (*it).AZbegpv) > 180.0f ? abs(360.0f - abs(PVector[j].AZbegpv - (*it).AZbegpv))
							: abs(PVector[j].AZbegpv - (*it).AZbegpv);
						gFloat AziendDif = abs(PVector[j].AZendpv - (*it).AZendpv) > 180.0f ? abs(360.0f - abs(PVector[j].AZendpv - (*it).AZendpv))
							: abs(PVector[j].AZendpv - (*it).AZendpv);
						gFloat AzibegendDif1 = abs(PVector[j].AZbegpv - (*it).AZendpv) > 180.0f ? abs(360.0f - abs(PVector[j].AZbegpv - (*it).AZendpv))
							: abs(PVector[j].AZbegpv - (*it).AZendpv);
						gFloat AzibegendDif2 = abs(PVector[j].AZendpv - (*it).AZbegpv) > 180.0f ? abs(360.0f - abs(PVector[j].AZendpv - (*it).AZbegpv))
							: abs(PVector[j].AZendpv - (*it).AZbegpv);

						gFloat AziCen1 = (PVector[j].AZbegpv + PVector[j].AZendpv)/2;
						gFloat AziCen2 = ((*it).AZbegpv + (*it).AZendpv)/2;	
						gFloat AziCenDif = abs(AziCen1 - AziCen2) > 180.0f ? abs(360.0f - abs(AziCen1 - AziCen2)) : abs(AziCen1 - AziCen2);

						gFloat AziDif1 = AzibegDif < AziendDif ? AzibegDif : AziendDif;
						gFloat AziDif2 = AzibegendDif1 < AzibegendDif2 ? AzibegendDif1 : AzibegendDif2;
						gFloat AziDif3 = AziDif1 < AziDif2 ? AziDif1 : AziDif2;

						gFloat AziDif = (gFloat)((AziDif3 <AziCenDif ? AziDif3: AziCenDif) * M_PI/180.0f);

						gFloat RangDif = abs(PVector[j].RS - (*it).RS);
						if (AziDif <= m_Param.TMesoAzi && RangDif <= m_Param.TRadiDis)
						{
							PVector[j].InFeature = true;
							FEATURE.push_back(PVector[j]);
						}
					}
				}
			}
			FEATUREs.push_back(FEATURE);
		}
	}
}

void Arith_292_M::CharacterizeFeature(vector<PVL> &FEATUREs, vector<FEATUREStruct>& FEATUREStructs, FEAV * TOTALFEATUREs, gInt16 layer,
									  gFloat angle, gInt16 LenOfGate/*, gFloat &VDbeg, gFloat &VDend, gFloat &AZbeg, gFloat &AZend*/)
{
	for (size_t cnt = 0; cnt <FEATUREs.size(); cnt ++)
	{
		if (FEATUREs[cnt].size()>= m_Param.TPV)
		{
			gFloat SUM1 = 0.0;
			gFloat SUM2 = 0.0;
			gFloat SUM3 = 0.0;
			gFloat SUM4 = 0.0;
			gFloat SUMDAA1 = 0.0;
			gFloat SUMDAA2 = 0.0;
			gFloat SUMDMA1 = 0.0;
			gFloat SUMDMA2 = 0.0;

			gFloat RSFEend = 0.0;
			gFloat RSFEbeg = (*FEATUREs[cnt].begin()).RS;
			gFloat AZFEbeg = (*FEATUREs[cnt].begin()).AZbegpv;
			gFloat AZFEend = 0.0;

			gFloat SUMSRTavg = 0.0;
			gFloat SRTmax = 0.0;
			gFloat SUMMavg = 0.0;

			FEATUREStruct FEATUREValue;

			list<PatV>::iterator itr_PV;
			for (itr_PV = FEATUREs[cnt].begin(); itr_PV != FEATUREs[cnt].end(); itr_PV++)
			{
				gFloat AZbegpvn = (*itr_PV).AZbegpv;
				gFloat AZendpvn = (*itr_PV).AZendpv;
				gFloat VDbegpvn = (*itr_PV).VDbeg;
				gFloat VDendpvn = (*itr_PV).VDend;
				gFloat RSPV = (*itr_PV).RS;
				gFloat RSPVn = (*itr_PV).RS;
				gFloat RS = (*itr_PV).RS;

				gFloat DAZ = abs(AZbegpvn - AZendpvn);
				gFloat VM = abs(DAZ * (VDendpvn - VDbegpvn));

				SUM1 += (AZbegpvn + DAZ/2) * VM;
				SUM2 += VM;

				SUM3 += abs((VDendpvn - VDbegpvn) * (AZendpvn - AZbegpvn) * RSPV);
				SUM4 += abs((VDendpvn - VDbegpvn) * (AZendpvn - AZbegpvn));

				SUMDAA1 += (AZendpvn - AZbegpvn) * (AZendpvn - AZbegpvn);
				SUMDAA2 += abs(AZendpvn - AZbegpvn);

				SUMDMA1 += abs((AZendpvn - AZbegpvn) * (AZendpvn - AZbegpvn) * (VDendpvn - VDbegpvn));
				SUMDMA2 += abs((VDendpvn - VDbegpvn) * (AZendpvn - AZbegpvn));

				SUMSRTavg += abs((VDendpvn - VDbegpvn) / (RSPVn * (AZendpvn - AZbegpvn)));
				gFloat SRT = abs((VDendpvn - VDbegpvn) / (RS * (AZendpvn - AZbegpvn)));
				if (SRT > SRTmax)
				{
					SRTmax = SRT;
				}

				SUMMavg += abs((VDendpvn - VDbegpvn) * RSPVn * (AZendpvn - AZbegpvn));

				if(RSFEbeg > (*itr_PV).RS)
				{
					RSFEbeg = (*itr_PV).RS;
				}

				if(RSFEend < (*itr_PV).RS)
				{
					RSFEend = (*itr_PV).RS;
				}

				if (AZFEbeg > (*itr_PV).AZbegpv)
				{
					AZFEbeg = (*itr_PV).AZbegpv;
				}

				if (AZFEend < (*itr_PV).AZendpv)
				{
					AZFEend = (*itr_PV).AZendpv;
				}
			}
			gFloat AZcenfe = SUM1/SUM2;
			gFloat RSFEcen = SUM3/SUM4;

			gFloat DAA = (gFloat)((SUMDAA1 * RSFEcen)/(SUMDAA2 * M_PI/4.0));
			gFloat DMA = (gFloat)(3.0* M_PI/8.0 * (SUMDMA1 * RSFEcen)/SUMDMA2);
			gFloat DAR = (RSFEend - RSFEbeg> 0)? RSFEend - RSFEbeg: (gFloat)(LenOfGate/2.0/1000.0);
			gFloat ROD = DAA/DAR;
			gFloat RMD = DMA/DAR;

			if (RMD > m_Param.TMinRat && RMD < m_Param.TMaxRat)
			{
				FEATUREValue.Ismesoc = true;
			}
			else if (RSFEbeg >= m_Param.TRange && ROD > m_Param.TFMinRat && ROD < m_Param.TFMaxRat)
			{
				FEATUREValue.Ismesoc = true;
			}
			else
			{
				FEATUREValue.Ismesoc = false;
			}

			gFloat SRTavg = SUMSRTavg/FEATUREs[cnt].size();
			gFloat HFC = (RSFEcen * RSFEcen + 2 * (4/3 * RE * RSFEcen * sin(angle)))/(2* 4 /3 * RE);
			gFloat Mavg = SUMMavg/FEATUREs[cnt].size();

			FEATUREValue.index = cnt;
			FEATUREValue.MaxAZ = AZFEend;
			FEATUREValue.MinAZ = AZFEbeg;
			FEATUREValue.MaxRS = RSFEend;
			FEATUREValue.MinRS = RSFEbeg;
			FEATUREValue.HFC = HFC;
			FEATUREValue.DAA = DAA;
			FEATUREValue.DAR = DAR;
			FEATUREValue.VortexNo = -1;
			FEATUREValue.SRTavg = SRTavg;
			FEATUREValue.Mavg = Mavg;
			FEATUREValue.SRTmax = SRTmax;
			FEATUREValue.DMA = DMA;
			FEATUREValue.AZcenfe = AZcenfe;
			FEATUREValue.RSFEcen = RSFEcen;
			FEATUREValue.elev = layer;
			FEATUREStructs.push_back(FEATUREValue);
		}
	}
	TOTALFEATUREs[layer] = FEATUREStructs;
}

void Arith_292_M::Vortex(GenerationData * p, gInt16 NumOfLayer, FEAV * TOTALFEATUREs, vector<FEAV> & VortexV)
{
	Product_254_V * pPtIn = (Product_254_V *)p;
	for (gInt16 layer = 1; layer < NumOfLayer; layer ++)
	{
		DILayer * pLayer = pPtIn->GetMyLayer(layer - 1);
		gFloat angle1 = (gFloat)(pLayer->HeightOfElevation * 0.01 * M_PI/180.0);
		pLayer = pPtIn->GetMyLayer(layer);
		gFloat angle2 = (gFloat)(pLayer->HeightOfElevation * 0.01 * M_PI/180);
		for (size_t index = 0; index < TOTALFEATUREs[layer].size(); index ++)
		{
			if (TOTALFEATUREs[layer][index].HFC <= m_Param.TFHeight)
			{
				bool flag = false;
				for (size_t index1 = 0; index1 < TOTALFEATUREs[layer - 1].size(); index1 ++)
				{
					if (TOTALFEATUREs[layer - 1][index1].HFC <= m_Param.TFHeight)
					{
						gFloat XFCP1 = (gFloat)(TOTALFEATUREs[layer - 1][index1].RSFEcen * sin(TOTALFEATUREs[layer - 1][index1].AZcenfe* 0.01 * M_PI/180.0) * cos(angle1));
						gFloat YFCP1 = (gFloat)(TOTALFEATUREs[layer - 1][index1].RSFEcen * cos(TOTALFEATUREs[layer - 1][index1].AZcenfe* 0.01 * M_PI/180) * cos(angle1));
						gFloat XFCP2 = (gFloat)(TOTALFEATUREs[layer][index].RSFEcen * sin(TOTALFEATUREs[layer][index].AZcenfe* 0.01 * M_PI/180) * cos(angle2));
						gFloat YFCP2 = (gFloat)(TOTALFEATUREs[layer][index].RSFEcen * cos(TOTALFEATUREs[layer][index].AZcenfe* 0.01 * M_PI/180) * cos(angle2));

						gFloat DIF = (gFloat)sqrt((XFCP1 - XFCP2) * (XFCP1 - XFCP2) + (YFCP1 - YFCP2)* (YFCP1 - YFCP2));

						gFloat DAA1 = TOTALFEATUREs[layer - 1][index1].DAA;
						gFloat DAR1 = TOTALFEATUREs[layer - 1][index1].DAR;
						gFloat DAA2 = TOTALFEATUREs[layer][index].DAA;
						gFloat DAR2 = TOTALFEATUREs[layer][index].DAR;

						gFloat MFD = ((DAA2 + DAR2)/2 + (DAA1 + DAR1)/2)/2;

						if (DIF <= MFD)
						{
							gFloat rs1 = TOTALFEATUREs[layer - 1][index1].RSFEcen;
							gFloat rs2 = TOTALFEATUREs[layer][index].RSFEcen;
							gFloat az1 = TOTALFEATUREs[layer - 1][index1].AZcenfe;
							gFloat az2 = TOTALFEATUREs[layer][index].AZcenfe;
							if (TOTALFEATUREs[layer - 1][index1].VortexNo != -1)
							{
								TOTALFEATUREs[layer][index].VortexNo = TOTALFEATUREs[layer - 1][index1].VortexNo;
								VortexV[TOTALFEATUREs[layer - 1][index1].VortexNo].push_back(TOTALFEATUREs[layer][index]);
							}
							else
							{
								FEAV V;
								TOTALFEATUREs[layer - 1][index1].VortexNo = VortexV.size();
								TOTALFEATUREs[layer][index].VortexNo = VortexV.size();
								V.push_back(TOTALFEATUREs[layer - 1][index1]);
								V.push_back(TOTALFEATUREs[layer][index]);
								VortexV.push_back(V);
							}

							flag = true;
							break;
						}
					}
				}
				
				if(flag == false && layer != 1)
				{
					for (size_t index1 = 0; index1 < TOTALFEATUREs[layer - 2].size(); index1 ++)
					{
						if (TOTALFEATUREs[layer - 2][index1].HFC <= m_Param.TFHeight)
						{
							gFloat XFCP1 = (gFloat)(TOTALFEATUREs[layer - 2][index1].RSFEcen * sin(TOTALFEATUREs[layer - 2][index1].AZcenfe* 0.01 * M_PI/180) * cos(angle1));
							gFloat YFCP1 = (gFloat)(TOTALFEATUREs[layer - 2][index1].RSFEcen * cos(TOTALFEATUREs[layer - 2][index1].AZcenfe* 0.01 * M_PI/180) * cos(angle1));
							gFloat XFCP2 = (gFloat)(TOTALFEATUREs[layer][index].RSFEcen * sin(TOTALFEATUREs[layer][index].AZcenfe* 0.01 * M_PI/180) * cos(angle2));
							gFloat YFCP2 = (gFloat)(TOTALFEATUREs[layer][index].RSFEcen * cos(TOTALFEATUREs[layer][index].AZcenfe* 0.01 * M_PI/180) * cos(angle2));

							gFloat DIF = (gFloat)sqrt((XFCP1 - XFCP2) * (XFCP1 - XFCP2) + (YFCP1 - YFCP2)* (YFCP1 - YFCP2));

							gFloat DAA1 = TOTALFEATUREs[layer - 2][index1].DAA;
							gFloat DAR1 = TOTALFEATUREs[layer - 2][index1].DAR;
							gFloat DAA2 = TOTALFEATUREs[layer][index].DAA;
							gFloat DAR2 = TOTALFEATUREs[layer][index].DAR;

							gFloat MFD = ((DAA2 + DAR2)/2 + (DAA1 + DAR1)/2)/2;

							if (DIF <= MFD)
							{
								if (TOTALFEATUREs[layer - 2][index1].VortexNo != -1)
								{
									TOTALFEATUREs[layer][index].VortexNo = TOTALFEATUREs[layer - 2][index1].VortexNo;
									VortexV[TOTALFEATUREs[layer - 2][index1].VortexNo].push_back(TOTALFEATUREs[layer][index]);
								}
								else
								{
									FEAV V;
									TOTALFEATUREs[layer - 2][index1].VortexNo = VortexV.size();
									TOTALFEATUREs[layer][index].VortexNo = VortexV.size();
									V.push_back(TOTALFEATUREs[layer - 2][index1]);
									V.push_back(TOTALFEATUREs[layer][index]);
									VortexV.push_back(V);
								}
								break;
							}
						}
					}
				}
			}
		}
	}

	for (gInt16 layer = 1; layer < NumOfLayer; layer ++)
	{
		for (size_t index = 0; index < TOTALFEATUREs[layer].size(); index ++)
		{
			if (TOTALFEATUREs[layer][index].HFC <= m_Param.TFHeight)
			{
				if (TOTALFEATUREs[layer][index].VortexNo == - 1)
				{
					FEAV V;
					TOTALFEATUREs[layer][index].VortexNo = VortexV.size();
					V.push_back(TOTALFEATUREs[layer][index]);
					VortexV.push_back(V);
				}
			}
		}
	}
}

void Arith_292_M::CharacterizeMesocyclone(vector<FEAV> & VortexV, VORTEXStruct * Vortices)
{
	for (size_t i = 0; i< VortexV.size(); i++)
	{
		if (VortexV[i].size() > 1)
		{
			gInt32 MesocCnt = 0;
			for (size_t j = 0; j< VortexV[i].size(); j++)
			{
				if (VortexV[i][j].Ismesoc == true)
				{
					MesocCnt ++;
				}			
			}

			if (MesocCnt > 1)
			{
				Vortices[i].VortexType = _MESOCYCLONE;
			}
			else
			{
				Vortices[i].VortexType = _3D_SHEAR;
			}
		}
		else
		{
			Vortices[i].VortexType = _UNCORRELATED_SHEAR;
		}

		Vortices[i].FeatureCnt = VortexV[i].size();

		gInt32 index = 0;
		gInt32 index1 = 0;
		gInt32 index2 = 0;
		gFloat SRTmaxavg = 0;
		gFloat MaxAz = 0.0;
		gFloat MinAz = VortexV[i][0].MinAZ;
		gFloat MaxRS = 0.0;
		gFloat MinRS = VortexV[i][0].MinRS;
		gFloat LowestHFC = VortexV[i][0].HFC;
		gFloat HighestHFC = VortexV[i][0].HFC;
		gInt32 indexLowHFC = 0;
		gInt32 indexHighHFC = 0;
		for (size_t j = 0; j< VortexV[i].size(); j++)
		{
			if (SRTmaxavg < VortexV[i][j].SRTavg)
			{
				SRTmaxavg = VortexV[i][j].SRTavg;
				index = j;
			}
			if (MaxAz < VortexV[i][j].MaxAZ)
			{
				MaxAz = VortexV[i][j].MaxAZ;
				index1 = j;
			}
			if (MinAz > VortexV[i][j].MinAZ)
			{
				MinAz = VortexV[i][j].MinAZ;
				index2 = j;
			}
			if (MaxRS < VortexV[i][j].MaxRS)
			{
				MaxRS = VortexV[i][j].MaxRS;
			}
			if (MinRS > VortexV[i][j].MinRS)
			{
				MinRS = VortexV[i][j].MinRS;
			}
			if (LowestHFC > VortexV[i][j].HFC)
			{
				LowestHFC = VortexV[i][j].HFC;
				indexLowHFC = j;
			}

			if (HighestHFC < VortexV[i][j].HFC)
			{
				HighestHFC = VortexV[i][j].HFC;
				indexHighHFC = j;
			}
		}
		
		//gFloat rs = VortexV[i][index].RSFEcen;
		//Vortices[i].Radius = (gInt16)abs(MaxRS* tan((MaxAz - MinAz)/2));
		gFloat rs = VortexV[i][index].MaxRS;
		Vortices[i].Radius = (gInt16)(abs(rs * sin((MaxAz - MinAz)/2))> (MaxRS- MinRS)/2? (gInt16)abs(rs * sin((MaxAz - MinAz)/2)): (MaxRS- MinRS)/2);
		gFloat DisOfCenter = /*abs(rs / cos((MaxAz - MinAz)/2))*/(MaxRS + MinRS)/2;
		Vortices[i].CenterX = (gInt16)(DisOfCenter * cos((MaxAz + MinAz)/2));
		Vortices[i].CenterY = (gInt16)(DisOfCenter * sin((MaxAz + MinAz)/2));;

		Vortices[i].SRTmaxavg = SRTmaxavg;
		Vortices[i].DAA = VortexV[i][index].DAA;
		Vortices[i].DAR = VortexV[i][index].DAR;
		Vortices[i].HFC = VortexV[i][index].HFC;
		Vortices[i].SRTmax = VortexV[i][index].SRTmax;
		Vortices[i].Mavg = VortexV[i][index].Mavg;
		Vortices[i].elev = VortexV[i][index].elev;
		Vortices[i].AZcenfe = VortexV[i][index].AZcenfe;
		Vortices[i].RSFEcen = VortexV[i][index].RSFEcen;
		Vortices[i].LowestHFC = LowestHFC;
		Vortices[i].HighestHFC = HighestHFC;

		if (Vortices[i].VortexType == _MESOCYCLONE)
		{	
			gFloat MaxSRTmax = 0.0;
			gInt32 indexMaxSRT = 0;			
			gFloat LowestHFC = VortexV[i][0].HFC;
			gFloat HighestHFC = VortexV[i][0].HFC;
			gInt32 indexLowHFC = 0;
			gInt32 indexHighHFC = 0;
			for (size_t j = 0; j < VortexV[i].size(); j++)
			{
				if (MaxSRTmax < VortexV[i][j].SRTmax)
				{
					MaxSRTmax = VortexV[i][j].SRTmax;
					indexMaxSRT = j;
				}
			}

			gFloat SPRavg = VortexV[i][indexHighHFC].SRTavg * VortexV[i][indexHighHFC].DMA / 2;
			Vortices[i].SRTavg = VortexV[i][indexHighHFC].SRTavg;
			Vortices[i].FEATUREMaxAz = VortexV[i][indexHighHFC].MaxAZ;
			Vortices[i].FEATUREMinAz = VortexV[i][indexHighHFC].MinAZ;
			Vortices[i].FEATUREMaxRs = VortexV[i][indexHighHFC].MaxRS;
			Vortices[i].FEATUREMinRs = VortexV[i][indexHighHFC].MinRS;
			Vortices[i].SPRavg = SPRavg;
			Vortices[i].HightestSRTmax = MaxSRTmax;
		}
	}
}