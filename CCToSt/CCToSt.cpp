#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "CCToSt.h"
#include "../GDataTypes//StandardRadarDataX.h"
#include <ctype.h>
GHRESULT CCToSt::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	
	//FileName= NULL;
	//SiteCode=NULL;
	return GS_OK;
}
GHRESULT CCToSt::LoadData(void *pValue,ReadParam * ReadParameter)
{
	m_pParameter=ReadParameter;
	m_pInputList=(GXList< GenerationData > *)pValue;
	if (m_pInputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT CCToSt::OutPutData(void * pValue)
{
	m_pOutputList=(GXList< GenerationData > *)pValue;
	if (m_pOutputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}

GHRESULT CCToSt::Execute()
{
	CCProductPretreatment *pCCData = &(*m_pInputList->GetHeadAs<CCProductPretreatment>());
	CC_DATAHEADER * pCChead = pCCData->GetTArchiveCCHeadP();
	m_pOutputList->AddTail();
	StandardRadarData *pData = &(*m_pOutputList->GetHeadAs<StandardRadarData>());
	
	GRADARDATAHEADER *pDataHead = new GRADARDATAHEADER;
	gInt32 ElevationNumber = pCCData->GetLayerNubP();
	
	char cStationNumber[10]={'0'};
	//strcpy(cStationNumber,pCChead->cStationNumber);
	pDataHead->ElevationCount = ElevationNumber;
	
	pDataHead->RadarLat = (pCChead->lLatitudeValue*1.0/3600000)*1000;
	pDataHead->RadarLon =  (pCChead->lLongitudeValue*1.0/3600000)*1000;
	pDataHead->RadarHeight = pCChead->lHeight/10;
	pDataHead->ReflectivityGateSize = pCChead->vsp[0].usBindWidth*2;
	pDataHead->DopplerGateSize = pCChead->vsp[0].usBindWidth*2;
	//pDataHead->RadarType =
	/*pDataHead->ConstantOfSystemCorrect =*/
	pDataHead->OperationMode[0]=0;
	pDataHead->OperationMode[1]=1;
	if(pCChead->ucScanMode == 1)//RHI
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x05;
	}

	else if(pCChead->ucScanMode == 10)//PPI
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x02;
	}
	else if(pCChead->ucScanMode > 100)//VCP
	{
		pDataHead->ScanMode[0] = 0;
		pDataHead->ScanMode[1] = 0x06;
	}

	pDataHead->SectorCode=360;//ÔÝ¶¨

	char * SideCode = "Z9439";

	char * pTempName = NULL;
//	(*m_pParameter)("SiteCode",pTempName);
    pTempName = SideCode;

	pDataHead->SiteCode[0]=pDataHead->SiteCode[1]=pDataHead->SiteCode[4]='0';
	pDataHead->SiteCode[2]=pTempName[0];
	pDataHead->SiteCode[3]=pTempName[1];
	pDataHead->SiteCode[5]=pTempName[2];
	pDataHead->SiteCode[6]=pTempName[3];
	pDataHead->SiteCode[7]=pTempName[4];

	pDataHead->ElevationCount=ElevationNumber;//

	pDataHead->SizeOfSector=360;
	pDataHead->VCPDate = DateToJulian(pCChead->ucSYear1*100+pCChead->ucSYear2,pCChead->ucSMonth,pCChead->ucSDay);
	pDataHead->VCPTime = TimeToSeconds(pCChead->ucSHour,pCChead->ucSMinute,pCChead->ucSSecond);

    int m_SecondTimeZone = 0;

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
	gInt16 * pDataR = NULL;
	gInt16 * pDataV = NULL;
	gInt16 * pDataW = NULL;
	DATA_CC * ppDataCC = NULL;
	gInt16 pbig =0;
	pbig =(pCCData->GetDataSize() -sizeof(CC_DATAHEADER))/(512*sizeof(DATA_CC));
	int sizoffset = sizeof(CC_DATAHEADER);

	for(int i=0;i < pDataHead->ElevationCount ; i++)
	{
		pElevHead[i].AzimuthNumber = pCChead->vsp[i].usRecordNumber;
		pElevHead[i].DopplerGates = pCChead->vsp[i].usBinNumber;
		pElevHead[i].Elevation = pCChead->vsp[i].usAngle;
		pElevHead[i].FirstGateRangeOfDoppler = 0;//meiyou
		pElevHead[i].FirstGateRangeOfRef =0; //meiyou
		pElevHead[i].NyquistVelocity =pCChead->vsp[i].usMaxV;
		pElevHead[i].ReflectivityGates = pCChead->vsp[i].usBinNumber;
		pElevHead[i].ReflectivityResolution = 0;//meiyou
		pElevHead[i].SNOfElevationData = 0;//meiyou
		pElevHead[i].SpectrumWidthOfVelocityResolution =0;//meiyou
		pElevHead[i].UnambiguousRange = pCChead->vsp[i].usMaxL;
		pElevHead[i].VelocityResolution=0;//meiyou
	}

	GHRESULT gResult;
	if((gResult = pData->WriteHeadAndElevationHead( pDataHead,pElevHead,ElevationNumber)) != GS_OK)
	{
		delete pDataHead;
		delete []pElevHead;
		return gResult;
	}


	for(int i=0;i < pDataHead->ElevationCount ; i++)
	{
		//pData->WriteElevationHead(i,pElevHead);
		ppDataCC = pCCData->GetTArchiveCC_RVWP(i);
		for(int j = 0; j< pElevHead->AzimuthNumber ; j++)
		{
			pRVWhead = pData->GetRadarDataP(i,j);
			pRVWhead->Azimuth =(gUint16)(360.0*j*100/pElevHead->AzimuthNumber);
			pRVWhead->Date = 0;
			pRVWhead->Time =0 ;
			pRVWhead->Elevation = pCChead->vsp[i].usAngle;
			pRVWhead->SNOfRadialData = j+1;
			pDataR = pData->GetRadarDataR(i,j);
			pDataV = pData->GetRadarDataV(i,j);
			pDataW = pData->GetRadarDataW(i,j);
			for(int k =0; k< pElevHead->ReflectivityGates; k++)
			{
				gInt16 m_Temp = (ppDataCC[j]).z[k];
				if( m_Temp != -32768 )
					pDataR[k] = m_Temp*10/*(m_Temp/5+64)*/;
				else 
					pDataR[k] = RADAR_DATA_NOECHO;

				m_Temp =  (ppDataCC[j]).v[k];
				if( m_Temp != -32768 )
					pDataV[k] = m_Temp*10/**1270.0/pElevHead->NyquistVelocity*/;		
				else 
					 pDataV[k]  = RADAR_DATA_NOECHO;
					
				m_Temp = (ppDataCC[j]).w[k];
				if( m_Temp != -32768 )
						pDataW[k] = m_Temp*10/**1270.0/pElevHead->NyquistVelocity */;	
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
int CCToSt::DateToJulian(int year, int mon, int day)
{
	int JulianDate = day-32075 + 1461*(year+4800+(mon-14)/12)/4  \
		+ 367*(mon-2-(mon-14)/12*12)/12 - 3*((year+4900+(mon-14)/12)/100)/4;

	return (JulianDate-2440587) ;
}
int CCToSt::TimeToSeconds(short hour, short minute, short second)
{
	int ulSeconds =(hour*3600+minute*60+second)*1000 ;

	return ulSeconds;
}
GHRESULT CCToSt::UnInitialize()
{
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)
{
	return new CCToSt;
}