#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "CCToSt2.h"
#include "StandardRadarDataX.h"
#include <ctype.h>
GHRESULT CCToSt2::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	
	//FileName= NULL;
	//SiteCode=NULL;
	return GS_OK;
}
GHRESULT CCToSt2::LoadData(void *pValue,ReadParam * ReadParameter)
{
	//(*ReadParameter)("filename",FileName);
	//(*ReadParameter)("sitecode",SiteCode);
	//ReadP=ReadParameter;

	m_pParameter=ReadParameter;
	m_pInputList=(GXList< GenerationData > *)pValue;
	if (m_pInputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT CCToSt2::OutPutData(void * pValue)
{
	m_pOutputList=(GXList< GenerationData > *)pValue;
	if (m_pOutputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}

GHRESULT CCToSt2::Execute()
{
	CCProductPretreatment2 *pCCData = &(*m_pInputList->GetHeadAs<CCProductPretreatment2>());
	gInt32 ElevationNumber = pCCData->GetLayerNubP();
	RADDATAHEADER * pCChead = pCCData->GetTArchiveCCRadDataHeaderP();
	m_pOutputList->AddTail();
	StandardRadarData *pData = &(*m_pOutputList->GetHeadAs<StandardRadarData>());
	
	GRADARDATAHEADER *pDataHead = new GRADARDATAHEADER;
	
	char cStationNumber[10]={'0'};
	//strcpy(cStationNumber,pCChead->cStationNumber);
	pDataHead->ElevationCount = ElevationNumber;
	
	pDataHead->RadarLat = pCChead->LatitudeValue;
	pDataHead->RadarLon =  pCChead->LongitudeValue;
	pDataHead->RadarHeight = pCChead->Height/10;
	pDataHead->ReflectivityGateSize = pCChead->LayerParam[0].ZBinWidth/10;
	pDataHead->DopplerGateSize = pCChead->LayerParam[0].VBinWidth/10;

	pDataHead->OperationMode[0]=0;
	pDataHead->OperationMode[1]=1;
	if(pCChead->SType == 1)//RHI
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x05;
	}

	else if(pCChead->SType == 10)//PPI
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x02;
	}
	else if(pCChead->SType > 100)//VCP
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x06;
	}

	pDataHead->SectorCode=360;//ÔÝ¶¨

	char * pTempName = NULL;
	(*m_pParameter)("SiteCode",pTempName);
	pDataHead->SiteCode[0]=pDataHead->SiteCode[1]=pDataHead->SiteCode[4]='0';
	pDataHead->SiteCode[2]=pTempName[0];
	pDataHead->SiteCode[3]=pTempName[1];
	pDataHead->SiteCode[5]=pTempName[2];
	pDataHead->SiteCode[6]=pTempName[3];
	pDataHead->SiteCode[7]=pTempName[4];

	pDataHead->ElevationCount=ElevationNumber;//

	pDataHead->SizeOfSector=360;
	pDataHead->VCPDate = DateToJulian(pCChead->SYear,pCChead->SMonth,pCChead->SDay);
	pDataHead->VCPTime = TimeToSeconds(pCChead->SHour,pCChead->SMinute,pCChead->SSecond);


	(*m_pParameter)("TimeZone",pTempName);
	int m_SecondTimeZone = atoi(pTempName);

	if (m_SecondTimeZone > 28800)
	{
		gInt32 TimeDiff = (m_SecondTimeZone - 28800) * 1000;
		pDataHead->VCPTime-=TimeDiff;
		if (pDataHead->VCPTime < 0)
		{
			pDataHead->VCPDate--;
			pDataHead->VCPTime+=24*3600*1000;
		}
	} 
	else if ( m_SecondTimeZone < 28800)
	{
		gInt32 TimeDiff = (28800 - m_SecondTimeZone) * 1000;
		pDataHead->VCPTime+=TimeDiff;
		if (pDataHead->VCPTime >= 24*3600*1000)
		{
			pDataHead->VCPDate++;
			pDataHead->VCPTime-=24*3600*1000;
		}
	} 
	else
	{
		;
	}
	//pData->WriteHead(pDataHead);

	GELEVATIONDATAHEAD *pElevHead = new GELEVATIONDATAHEAD[ElevationNumber];
	RVWDATAHEAD * pRVWhead = NULL;

	for(int i=0;i < pDataHead->ElevationCount ; i++)
	{
		pElevHead[i].AzimuthNumber = pCChead->LayerParam[i].RecordNumber;
		pElevHead[i].DopplerGates = pCChead->LayerParam[i].VBinNumber;
		pElevHead[i].Elevation = pCChead->LayerParam[i].SwpAngles;
		pElevHead[i].FirstGateRangeOfDoppler = 0;//meiyou
		pElevHead[i].FirstGateRangeOfRef =0; //meiyou
		pElevHead[i].NyquistVelocity =pCChead->LayerParam[i].MaxV;
		pElevHead[i].ReflectivityGates = pCChead->LayerParam[i].ZBinNumber;
		pElevHead[i].ReflectivityResolution = 0;//meiyou
		pElevHead[i].SNOfElevationData = i+1;
		pElevHead[i].SpectrumWidthOfVelocityResolution =0;//meiyou
		pElevHead[i].UnambiguousRange = pCChead->LayerParam[i].MaxL;
		pElevHead[i].VelocityResolution=0;//meiyou
	}

	GHRESULT gResult;
	if((gResult = pData->WriteHeadAndElevationHead( pDataHead,pElevHead,ElevationNumber)) != GS_OK)
	{
		delete pDataHead;
		delete []pElevHead;
		return gResult;
	}

//CorZ+UnZ+ V+ W
	RADIALHEADER * ppDataCC = NULL;
	unsigned char * ppDataR = NULL;
	unsigned char * ppDataV = NULL;
	unsigned char * ppDataW = NULL;

	gInt16 * pDataR = NULL;
	gInt16 * pDataV = NULL;
	gInt16 * pDataW = NULL;
	for(int i=0;i < pDataHead->ElevationCount ; i++)
	{
		//pData->WriteElevationHead(i,pElevHead);
		
		for(int j = 0; j< pElevHead[i].AzimuthNumber ; j++)
		{
			ppDataCC = pCCData->GetTArchiveCCRVWP(i, j);
			pRVWhead = pData->GetRadarDataP(i,j);
			pRVWhead->Azimuth =(gUint16)ppDataCC->Azimuth;
			pRVWhead->Date = 0;
			pRVWhead->Time =0 ;
			pRVWhead->Elevation = ppDataCC->Elev;
			pRVWhead->SNOfRadialData = j+1;

			ppDataR = pCCData->GetTArchiveCCRP(i, j);
			ppDataV = pCCData->GetTArchiveCCVP(i, j);
			ppDataW = pCCData->GetTArchiveCCWP(i, j);

			pDataR = pData->GetRadarDataR(i,j);
			pDataV = pData->GetRadarDataV(i,j);
			pDataW = pData->GetRadarDataW(i,j);
			for(int k =0; k< pElevHead[i].ReflectivityGates; k++)
			{
				gInt16 m_Temp = (gInt16)ppDataR[k];
				if( m_Temp != 0 )
					pDataR[k] = (m_Temp-64)*50;
				else 
					pDataR[k] = RADAR_DATA_NOECHO;
			}

			for(int k =0; k< pElevHead[i].DopplerGates; k++)
			{
				gInt16 m_Temp =  (gInt16)ppDataV[k];
				if( m_Temp != 0 )
					pDataV[k] = (m_Temp-128)*pElevHead[i].NyquistVelocity/127;		
				else 
					 pDataV[k]  = RADAR_DATA_NOECHO;
					
				m_Temp = (gInt16)ppDataW[k];
				if( m_Temp != 0 )
					pDataW[k] = m_Temp*pElevHead[i].NyquistVelocity/512;	
				else 
					pDataW[k] = RADAR_DATA_NOECHO;

			//	printf("R= %d, v = %d , w = %d \n", pDataR[k],pDataV[k],pDataW[k]);
				
			}
			
		}
		
	}

	delete pDataHead;
	delete []pElevHead;

	return GS_OK;
}
int CCToSt2::DateToJulian(int year, int mon, int day)
{
	int JulianDate = day-32075 + 1461*(year+4800+(mon-14)/12)/4  \
		+ 367*(mon-2-(mon-14)/12*12)/12 - 3*((year+4900+(mon-14)/12)/100)/4;

	return (JulianDate-2440587) ;
}
int CCToSt2::TimeToSeconds(short hour, short minute, short second)
{
	int ulSeconds =(hour*3600+minute*60+second)*1000 ;

	return ulSeconds;
}
GHRESULT CCToSt2::UnInitialize()
{
	return GS_OK;
}

GHRESULT DLLEXP_GSYS ExecuteArithCCToSt2( GXList< GenerationData>  *Datas,  ReadParam* pParam,  GXList< GenerationData > * pOutputList)
{
	if (Datas == NULL || pOutputList == NULL)
	{
		return GS_FALSE;
	}

/*	GXList<GenerationData>* InList = new 	GXList<GenerationData>;

	for (int i = 0; i < num ; i++)
	{
		InList.push_back(Datas[i]);
	}*/
	CCToSt2 *Arith = new CCToSt2();
	if (pOutputList == NULL || pParam == NULL)
	{
		return GE_INVALIDARG;
	}
	GHRESULT tempret;
	tempret = Arith->Initialize();
	if(tempret != GS_OK)
	{
		return 0;
	}

//	tempret = Arith->LoadData(InList,pParam);
//	Arith->m_pInputList=Datas;
	
	tempret = Arith->LoadData(Datas,pParam);
	if(tempret != GS_OK)
	{
		return 0;
	}
	
	tempret = Arith->OutPutData(pOutputList);
	if(tempret != GS_OK)
	{
		return 0;
	}
	printf("begin CCToSt2 Execute\n");
	tempret = Arith->Execute();
	if(tempret != GS_OK)
	{
		return 0;
	}
	printf("end Execute\n");
	tempret = Arith->UnInitialize();
	if(tempret != GS_OK)
	{
		return 0;
	}

	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)
{
	return new CCToSt2;
}