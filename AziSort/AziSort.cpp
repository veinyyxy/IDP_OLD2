//////////////////////////////////////////////////////////////////////////
// AziSort.cpp
// created by zy 20080415
// 将方位角已排序的标准格式数据  低仰角（VCP31为3个低仰角，其他为2个）2层合为一层，并将每个仰角方位角数目差分为360
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include <stdio.h>
#include "AziSort.h"



AziSort::AziSort()
{
	;
}
AziSort::~AziSort()
{
	;
}

GHRESULT AziSort::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	//FileName= NULL;
	//SiteCode=NULL;
	return GS_OK;
}

GHRESULT AziSort::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if (ReadParameter==NULL)
	{
		return GE_INVALIDARG;
	}
	m_pParameter = ReadParameter;
	m_pInputList=(GXList< GenerationData > *)pValue;
	if (m_pInputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}

GHRESULT AziSort::OutPutData(void * pValue)
{
	m_pOutputList=(GXList< GenerationData > *)pValue;
	if (m_pOutputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT AziSort::Execute()
{
	GHRESULT gResult;
	//准备读出写入的类
    StandardRadarData* pMyStIn = &(*(m_pInputList->GetHeadAs<StandardRadarData>()));
	m_pOutputList->AddTail();
    StandardRadarData * pMySt = &(*(m_pOutputList->GetHeadAs<StandardRadarData>()));

	gInt32 ElevationNumber = 0;
	if (NULL == pMyStIn->GetHeadP()) return GS_FALSE;
	if ((pMyStIn->GetHeadP())->VCPMode[2]==0x08)
	{
		ElevationNumber = (pMyStIn->GetHeadP())->ElevationCount-3;
	}
	else
	{
		ElevationNumber = (pMyStIn->GetHeadP())->ElevationCount-2;
	}//VCP31模式需要合并3个仰角，其他需要合并2个

	//写头并分配空间
	GRADARDATAHEADER * temHead=new GRADARDATAHEADER;
	if (temHead==NULL)
	{
		return GE_NOENOUGHMEMORY;
	}

	WriteStandardDataHead( temHead , pMyStIn->GetHeadP(), ElevationNumber );   //写 GRadarDataHeader
	GELEVATIONDATAHEAD * pEleTemHead = new GELEVATIONDATAHEAD[ElevationNumber];
	if (pEleTemHead==NULL)
	{
		delete temHead;
		temHead = NULL;
		return GE_NOENOUGHMEMORY;
	}

	gInt32 nInEleNum=0;//使用的原始数据第几个仰角

	if ((pMyStIn->GetHeadP())->VCPMode[3]!=0x08)//非VCP31模式
	{
		for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
		{
			if (nElevationIndex==0 || nElevationIndex==1)
			{
				pEleTemHead[nElevationIndex].UnambiguousRange=(pMyStIn->GetElevationHeadP(nInEleNum))->UnambiguousRange;
				pEleTemHead[nElevationIndex].NyquistVelocity=(pMyStIn->GetElevationHeadP(nInEleNum+1))->NyquistVelocity;
				pEleTemHead[nElevationIndex].ReflectivityResolution=(pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityResolution;
				pEleTemHead[nElevationIndex].VelocityResolution=(pMyStIn->GetElevationHeadP(nInEleNum+1))->VelocityResolution;
				pEleTemHead[nElevationIndex].SpectrumWidthOfVelocityResolution=(pMyStIn->GetElevationHeadP(nInEleNum+1))->SpectrumWidthOfVelocityResolution;
				pEleTemHead[nElevationIndex].FirstGateRangeOfRef=(pMyStIn->GetElevationHeadP(nInEleNum))->FirstGateRangeOfRef;
				pEleTemHead[nElevationIndex].ReflectivityGates=(pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				pEleTemHead[nElevationIndex].FirstGateRangeOfDoppler=(pMyStIn->GetElevationHeadP(nInEleNum+1))->FirstGateRangeOfDoppler;
				pEleTemHead[nElevationIndex].DopplerGates=(pMyStIn->GetElevationHeadP(nInEleNum+1))->DopplerGates;
				pEleTemHead[nElevationIndex].Elevation=(pMyStIn->GetElevationHeadP(nInEleNum))->Elevation;
				pEleTemHead[nElevationIndex].AzimuthNumber=360;
				pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;
				
				nInEleNum+=2;
			}
			else
			{
				GELEVATIONDATAHEAD * ele = pMyStIn->GetElevationHeadP(nInEleNum);
				WriteEleHead( &pEleTemHead[nElevationIndex], (pMyStIn->GetElevationHeadP(nInEleNum)));    //写仰角块头 
				pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;

				nInEleNum++;
			}
		}// 0 - ElevationNumber
		/*delete pEleTemHead;
		pEleTemHead = NULL;*/

		if((gResult = pMySt->WriteHeadAndElevationHead( temHead,pEleTemHead,ElevationNumber)) != GS_OK)
		{
			delete temHead;
			temHead = NULL;
			delete [] pEleTemHead;
			pEleTemHead = NULL;
			return gResult;
		}

		delete temHead;
		temHead = NULL;
		delete [] pEleTemHead;
		pEleTemHead = NULL;

		nInEleNum = 0;

		for(int i=0; i< 360; i++)
		{
			for(int j=0; j< pMySt->GetElevationHeadP(0)->ReflectivityGates;j++)
				gInt16 data = pMySt->GetRadarDataR(0,i)[j];
		}
		for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
		{
			if (nElevationIndex==0 || nElevationIndex==1)
			{
				gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 0, nRefGatesA, nDopGatesA, (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 R

				nInEleNum++;

				nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 1, nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 VW
				
				nInEleNum++;
			}
			else
			{
				gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 2 , nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 RVW 

				nInEleNum++;
			}
		}
	} 
	else//VCP31模式
	{
		//GELEVATIONDATAHEAD * pEleTemHead = new GELEVATIONDATAHEAD;
		/*if (pEleTemHead==NULL)
		{
			return GE_NOENOUGHMEMORY;
		}*/
		for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
		{
			if (nElevationIndex==0 || nElevationIndex==1 || nElevationIndex==2)
			{
				pEleTemHead[nElevationIndex].UnambiguousRange=(pMyStIn->GetElevationHeadP(nInEleNum))->UnambiguousRange;
				pEleTemHead[nElevationIndex].NyquistVelocity=(pMyStIn->GetElevationHeadP(nInEleNum+1))->NyquistVelocity;
				pEleTemHead[nElevationIndex].ReflectivityResolution=(pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityResolution;
				pEleTemHead[nElevationIndex].VelocityResolution=(pMyStIn->GetElevationHeadP(nInEleNum+1))->VelocityResolution;
				pEleTemHead[nElevationIndex].SpectrumWidthOfVelocityResolution=(pMyStIn->GetElevationHeadP(nInEleNum+1))->SpectrumWidthOfVelocityResolution;
				pEleTemHead[nElevationIndex].FirstGateRangeOfRef=(pMyStIn->GetElevationHeadP(nInEleNum))->FirstGateRangeOfRef;
				pEleTemHead[nElevationIndex].ReflectivityGates=(pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				pEleTemHead[nElevationIndex].FirstGateRangeOfDoppler=(pMyStIn->GetElevationHeadP(nInEleNum+1))->FirstGateRangeOfDoppler;
				pEleTemHead[nElevationIndex].DopplerGates=(pMyStIn->GetElevationHeadP(nInEleNum+1))->DopplerGates;
				pEleTemHead[nElevationIndex].Elevation=(pMyStIn->GetElevationHeadP(nInEleNum))->Elevation;
				pEleTemHead[nElevationIndex].AzimuthNumber=360;
				pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;
				/*if ((TemRet = pMySt->WriteElevationHead(nElevationIndex, pEleTemHead))!=GS_OK)
				{
					delete pEleTemHead;
					pEleTemHead = NULL;
					return TemRet;
				}*/

				//gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				//gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				//WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 0, nRefGatesA, nDopGatesA, (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 R

				//nInEleNum++;

				//nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				//WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 1, nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 VW

				//nInEleNum++;
			}
			else
			{
				WriteEleHead( &pEleTemHead[nElevationIndex], (pMyStIn->GetElevationHeadP(nInEleNum)));    //写仰角块头 
				pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;
				//if ((TemRet = pMySt->WriteElevationHead(nElevationIndex, pEleTemHead))!=GS_OK)
				//{
				//	delete pEleTemHead;
				//	pEleTemHead = NULL;
				//	return TemRet;
				//}

				//gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				//gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				//WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 2 , nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 RVW 

				//nInEleNum++;
			}
		}// 0 - ElevationNumber
		/*delete pEleTemHead;
		pEleTemHead = NULL;*/

		if((gResult = pMySt->WriteHeadAndElevationHead( temHead,pEleTemHead,ElevationNumber)) != GS_OK)
		{
			delete temHead;
			temHead = NULL;
			delete [] pEleTemHead;
			pEleTemHead = NULL;
			return gResult;
		}

		delete temHead;
		temHead = NULL;
		delete [] pEleTemHead;
		pEleTemHead = NULL;

		nInEleNum = 0;
		for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
		{
			if (nElevationIndex==0 || nElevationIndex==1 || nElevationIndex==2)
			{
				gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 0, nRefGatesA, nDopGatesA, (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 R

				nInEleNum++;

				nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 1, nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 VW

				nInEleNum++;
			}
			else
			{
				gInt16 nRefGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->ReflectivityGates;
				gInt16 nDopGatesA = (pMyStIn->GetElevationHeadP(nInEleNum))->DopplerGates;
				WriteRVW( pMySt, pMyStIn, nElevationIndex, nInEleNum, 2 , nRefGatesA, nDopGatesA , (pMyStIn->GetElevationHeadP(nInEleNum))->AzimuthNumber);     //写360个方位角块 RVW 

				nInEleNum++;
			}
		}
	}

	return GS_OK;
}
GHRESULT AziSort::UnInitialize()
{
	//if(FileName != NULL)
	//{
	//	delete [] FileName;
	//}

	return GS_OK;
}

//对标准数据包头赋值
void AziSort::WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, GRADARDATAHEADER * pStInHead, gInt32 ElevationNumber ) 
{
	//gInt8	SiteCode[8];
	memcpy(&(pGDataHead->SiteCode[0]), &(pStInHead->SiteCode[0]), 8*sizeof(gInt8));
	//gInt8	RadarType[4];
	memcpy(&(pGDataHead->RadarType[0]), &(pStInHead->RadarType[0]), 4*sizeof(gInt8));
	pGDataHead->RadarLon=pStInHead->RadarLon;
	pGDataHead->RadarLat=pStInHead->RadarLat;
	pGDataHead->RadarHeight=pStInHead->RadarHeight;
	pGDataHead->VCPDate=pStInHead->VCPDate;
	pGDataHead->VCPTime=pStInHead->VCPTime;
	//gInt8   OperationMode[2];
	memcpy(&(pGDataHead->OperationMode[0]), &(pStInHead->OperationMode[0]), 2*sizeof(gInt8));
	//gInt8   ScanMode[2];
	memcpy(&(pGDataHead->ScanMode[0]), &(pStInHead->ScanMode[0]), 2*sizeof(gInt8));
	//gInt8	VCPMode[4];
	memcpy(&(pGDataHead->VCPMode[0]), &(pStInHead->VCPMode[0]), 4*sizeof(gInt8));
	pGDataHead->SectorCode=pStInHead->SectorCode;
	pGDataHead->SizeOfSector=pStInHead->SizeOfSector;
	pGDataHead->ReflectivityGateSize=pStInHead->ReflectivityGateSize;
	pGDataHead->DopplerGateSize=pStInHead->DopplerGateSize;
	pGDataHead->ConstantOfSystemCorrect=pStInHead->ConstantOfSystemCorrect;
	pGDataHead->ElevationCount =ElevationNumber;
}


//对各仰角块头赋值
void AziSort::WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, GELEVATIONDATAHEAD * pStInEleHead )
{
	pGElevationHead->UnambiguousRange=pStInEleHead->UnambiguousRange;
	pGElevationHead->NyquistVelocity=pStInEleHead->NyquistVelocity;
	pGElevationHead->ReflectivityResolution=pStInEleHead->ReflectivityResolution;
	pGElevationHead->VelocityResolution=pStInEleHead->VelocityResolution;
	pGElevationHead->SpectrumWidthOfVelocityResolution=pStInEleHead->SpectrumWidthOfVelocityResolution;
	pGElevationHead->FirstGateRangeOfRef=pStInEleHead->FirstGateRangeOfRef;
	pGElevationHead->ReflectivityGates=pStInEleHead->ReflectivityGates;
	pGElevationHead->FirstGateRangeOfDoppler=pStInEleHead->FirstGateRangeOfDoppler;
	pGElevationHead->DopplerGates=pStInEleHead->DopplerGates;
	pGElevationHead->Elevation=pStInEleHead->Elevation;
	pGElevationHead->AzimuthNumber=360;
}

void AziSort::WriteRVW( StandardRadarData * pMySt, StandardRadarData * pMyStIn, gInt32 nElevationIndex, gInt32 nInEleNum, gInt32 nFlag, gInt32 nRefGatesA, gInt32 nDopGatesA, gInt16 nInAziNum )
{
	RVWDATAHEAD * pStTemp = NULL;
	RVWDATAHEAD * pStTemp1 = NULL;
	gInt32 nBegin = -1;
	gInt32 nEnd = -1;

	//以下为肖艳娇差分算法
	for( gInt32 nRVWNum = 0;nRVWNum < 360;nRVWNum++ )
	{
		gInt32 nAZ = 100 * nRVWNum;
		gInt32 nMinn = 1000000;
		gInt32 nMaxx = -1000000;
		nBegin = -1;
		nEnd = -1;

		for( gInt32 nAlNum = 0;nAlNum < nInAziNum;nAlNum++ )
		{
			pStTemp = pMyStIn->GetRadarDataP(nInEleNum, nAlNum);
			gInt32 nDlt = nAZ - (pStTemp -> Azimuth);
			gInt32 nTempDlt = 0;
			if( nRVWNum == 0 )
			{
				nTempDlt = 36000 - (pStTemp -> Azimuth);
				if( fabs(nDlt) > fabs(nTempDlt) )
				{
					nDlt = nTempDlt;
				}
			}
			else if( nRVWNum == 359 )
			{
				nTempDlt = 0-100 - (pStTemp -> Azimuth);
				if( fabs(nDlt) > fabs(nTempDlt) )
				{
					nDlt = nTempDlt;
				}
			}
			if( fabs(nDlt) < 10 )
			{
				nBegin = nEnd = nAlNum;
				break;
			}
			else if( nDlt > 0 && nMinn > nDlt )
			{
				nMinn = nDlt;
				nBegin = nAlNum;
			}
			else if( nDlt < 0 && nMaxx < nDlt )
			{
				nMaxx = nDlt;
				nEnd = nAlNum;
			}
		}//end for nAlNum (0- nAlNumber)



		if( nBegin < 0 || nEnd <0 ) 
		{
			pStTemp = pMyStIn->GetRadarDataP(nInEleNum,1);//取第nBegin块
			RVWDATAHEAD * pTempRVW =  pMySt->GetRadarDataP(nElevationIndex, nRVWNum);//取标准数据该层nRVWNum块指针
			gInt16 * pTempRVWString = NULL;

			//写该RVW块数据头
			pTempRVW->SNOfRadialData=nRVWNum+1;
			pTempRVW -> Elevation = pStTemp -> Elevation;          //单位0.01度 
			pTempRVW -> Azimuth =  (gInt16)nRVWNum*100;  //单位0.01度 
			pTempRVW -> Date = pStTemp -> Date;
			pTempRVW -> Time = pStTemp -> Time;
			if (nFlag==0 || nFlag==2)
			{
				pTempRVWString = pMySt->GetRadarDataR(nElevationIndex, nRVWNum);
				for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
				{
					pTempRVWString[nRefGates]=RADAR_DATA_NOECHO;
				}
			}
			if (nFlag==1 || nFlag==2)
			{
				//V数据块
				pTempRVWString = pMySt->GetRadarDataV(nElevationIndex,nRVWNum);
				for( gInt32 nVelGates = 0;nVelGates < nDopGatesA;nVelGates++ )
				{
					pTempRVWString[nVelGates]=RADAR_DATA_NOECHO;
				}
				//W数据块
				pTempRVWString = pMySt->GetRadarDataW(nElevationIndex,nRVWNum);
				for( gInt32 nSWGates = 0;nSWGates < nDopGatesA;nSWGates++ )
				{
					pTempRVWString[nSWGates]=RADAR_DATA_NOECHO;
				}
			}
			continue;
		}

		//以下为方位线性内插法
		if( nBegin == nEnd )
		{
			pStTemp = pMyStIn->GetRadarDataP(nInEleNum,nBegin);//取第nBegin块
			RVWDATAHEAD * pTempRVW =  pMySt->GetRadarDataP(nElevationIndex, nRVWNum);//取标准数据该层nRVWNum块指针
			gInt16 * pTempRVWString = NULL;
			gInt16 * pTempInRVWString = NULL;

			//写该RVW块数据头
			pTempRVW->SNOfRadialData=nRVWNum+1;
			pTempRVW -> Elevation = pStTemp -> Elevation;          //单位0.01度 
			pTempRVW -> Azimuth =  (gInt16)nRVWNum*100;  //单位0.01度 
			pTempRVW -> Date = pStTemp -> Date;
			pTempRVW -> Time = pStTemp -> Time;

			if (nFlag==0 || nFlag==2)
			{
				//R数据块
				pTempRVWString = pMySt->GetRadarDataR(nElevationIndex, nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataR(nInEleNum, nBegin);
				for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
				{
					if (nRefGates>=nRefGatesA)
					{
						pTempRVWString[nRefGates]=RADAR_DATA_NOECHO;
					}
					else
					{
						pTempRVWString[nRefGates] = pTempInRVWString[nRefGates];
					}
					/*
					以上for循环将非有效数据部分填写-32768
					改写为
					for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
					{
					pTempRVWString[nRefGates] = GetDBZ( pSATemp -> RefData[nRefGates] );
					}
					则是对非有效数据部分不处理

					对VW处理的for循环原理相同
					*/
				}
			}

			if (nFlag==1 || nFlag==2)
			{
				//V数据块
				pTempRVWString = pMySt->GetRadarDataV(nElevationIndex,nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataV(nInEleNum, nBegin);
				for( gInt32 nVelGates = 0;nVelGates < nDopGatesA;nVelGates++ )
				{
					if (nVelGates>=nDopGatesA)
					{
						pTempRVWString[nVelGates]=RADAR_DATA_NOECHO;
					}
					else
					{
						pTempRVWString[nVelGates] = pTempInRVWString[nVelGates];
					}

					//printf("%d  ",pTempRVWString[nVelGates]);
					//if ((nVelGates+1)%230==0)
					//{
					//	printf("\n\n");
					//}
				}
				//printf("\n\n\n\n\n");
				//W数据块
				pTempRVWString = pMySt->GetRadarDataW(nElevationIndex,nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataW(nInEleNum, nBegin);
				for( gInt32 nSWGates = 0;nSWGates < nDopGatesA;nSWGates++ )
				{
					if (nSWGates>=nDopGatesA)
					{
						pTempRVWString[nSWGates]=RADAR_DATA_NOECHO;
					}
					else
					{
						pTempRVWString[nSWGates] = pTempInRVWString[nSWGates];
					}
				}
			}
		}
		else
		{
			gInt32 nV_Temp1 = 100*nMinn/(nMinn-nMaxx);
			pStTemp = pMyStIn->GetRadarDataP(nInEleNum,nBegin);//取SA该层第nBegin块
			pStTemp1 = pMyStIn->GetRadarDataP(nInEleNum,nEnd);//取SA该层第nEnd块
			gInt16 * pTempRVWString = NULL;
			gInt16 * pTempInRVWString = NULL;
			gInt16 * pTempInRVWString1 = NULL;

			RVWDATAHEAD * pTempRVW = pMySt->GetRadarDataP(nElevationIndex, nRVWNum);//取标准数据该层nRVWNum块指针

			//写该RVW块数据头
			pTempRVW->SNOfRadialData=nRVWNum+1;
			pTempRVW -> Elevation = pStTemp -> Elevation;          //单位0.01度 
			pTempRVW -> Azimuth =  (gInt16)nRVWNum*100;  //单位0.01度 
			pTempRVW -> Date = pStTemp -> Date;
			pTempRVW -> Time = pStTemp -> Time;

			if (nFlag==0 || nFlag==2)
			{
				//R数据块
				pTempRVWString = pMySt->GetRadarDataR(nElevationIndex, nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataR(nInEleNum, nBegin);
				pTempInRVWString1 = pMyStIn->GetRadarDataR(nInEleNum, nEnd);
				for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
				{
					if (nRefGates>=nRefGatesA)
					{
						pTempRVWString[nRefGates]=RADAR_DATA_NOECHO;
					}
					else if( pTempInRVWString1[nRefGates] <= 1 && pTempInRVWString[nRefGates] > 1 )
					{
						pTempRVWString[nRefGates] = pTempInRVWString[nRefGates];
					}
					else if( pTempInRVWString[nRefGates] <= 1 && pTempInRVWString1[nRefGates] > 1 )
					{
						pTempRVWString[nRefGates] = pTempInRVWString[nRefGates];
					}
					else if( pTempInRVWString1[nRefGates] >= pTempInRVWString[nRefGates] ? ( pTempInRVWString1[nRefGates] - pTempInRVWString[nRefGates])  > 10 : ( pTempInRVWString[nRefGates] - pTempInRVWString1[nRefGates])  > 10 )
					{
						if( pTempInRVWString1[nRefGates] > pTempInRVWString[nRefGates] )
						{
							pTempRVWString[nRefGates] = pTempInRVWString1[nRefGates];
						}
						else
						{
							pTempRVWString[nRefGates] = pTempInRVWString[nRefGates];
						}
					}
					else
					{
						gInt32 nV_Temp2 = ( pTempInRVWString1[nRefGates] - pTempInRVWString[nRefGates] ) * nV_Temp1 + 100*pTempInRVWString[nRefGates];
						pTempRVWString[nRefGates] = (gInt16)(nV_Temp2/100);
					}
				}
			}

			if (nFlag==1||nFlag==2)
			{
				//V数据块
				pTempRVWString = pMySt->GetRadarDataV(nElevationIndex,nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataV(nInEleNum, nBegin);
				pTempInRVWString1 = pMyStIn->GetRadarDataV(nInEleNum, nEnd);
				for( gInt32 nVelGates = 0;nVelGates < nDopGatesA;nVelGates++ )
				{
					if (nVelGates>=nDopGatesA)
					{
						pTempRVWString[nVelGates]=RADAR_DATA_NOECHO;
					}
					else if( pTempInRVWString1[nVelGates] <= 1 && pTempInRVWString[nVelGates] > 1 )
					{
						pTempRVWString[nVelGates] = pTempInRVWString[nVelGates];
					}
					else if( pTempInRVWString[nVelGates] <= 1 && pTempInRVWString1[nVelGates] > 1 )
					{
						pTempRVWString[nVelGates] = pTempInRVWString1[nVelGates];
					}
					else if( pTempInRVWString1[nVelGates] >= pTempInRVWString[nVelGates] ? ( pTempInRVWString1[nVelGates] - pTempInRVWString[nVelGates])  > 10 : ( pTempInRVWString[nVelGates] - pTempInRVWString1[nVelGates])  > 10 )
					{
						if( pTempInRVWString1[nVelGates] > pTempInRVWString[nVelGates] )
						{
							pTempRVWString[nVelGates] = pTempInRVWString1[nVelGates];
						}
						else
						{
							pTempRVWString[nVelGates] = pTempInRVWString[nVelGates];
						}
					}
					else
					{
						gInt32 nV_Temp2 = ( pTempInRVWString1[nVelGates] - pTempInRVWString[nVelGates] ) * nV_Temp1 + 100 * pTempInRVWString[nVelGates];
						pTempRVWString[nVelGates] = (gInt16)(nV_Temp2 / 100);
					}

					//printf("%d  ",pTempRVWString[nVelGates]);
					//if ((nVelGates+1)%230==0)
					//{
					//	printf("\n\n");
					//}
				}
				//printf("\n\n\n\n\n");
				//W数据块
				pTempRVWString  = pMySt->GetRadarDataW(nElevationIndex,nRVWNum);
				pTempInRVWString = pMyStIn->GetRadarDataW(nInEleNum, nBegin);
				pTempInRVWString1 = pMyStIn->GetRadarDataW(nInEleNum, nEnd);
				for( gInt32 nSWGates = 0;nSWGates < nDopGatesA;nSWGates++ )
				{
					if (nSWGates>=nDopGatesA)
					{
						pTempRVWString[nSWGates]=RADAR_DATA_NOECHO;
					}
					else if( pTempInRVWString1[nSWGates] <= 1 && pTempInRVWString[nSWGates] > 1 )
					{
						pTempRVWString[nSWGates] = pTempInRVWString[nSWGates];
					}
					else if( pTempInRVWString[nSWGates] <= 1 && pTempInRVWString1[nSWGates] > 1 )
					{
						pTempRVWString[nSWGates] = pTempInRVWString1[nSWGates];
					}
					else if( pTempInRVWString1[nSWGates] >= pTempInRVWString[nSWGates] ? ( pTempInRVWString1[nSWGates] - pTempInRVWString[nSWGates])  > 10 : ( pTempInRVWString[nSWGates] - pTempInRVWString1[nSWGates])  > 10 )
					{
						if( pTempInRVWString1[nSWGates] > pTempInRVWString[nSWGates] )
						{
							pTempRVWString[nSWGates] = pTempInRVWString1[nSWGates];
						}
						else
						{
							pTempRVWString[nSWGates] = pTempInRVWString[nSWGates];
						}
					}
					else
					{
						gInt32 nV_Temp2 = ( pTempInRVWString1[nSWGates] - pTempInRVWString[nSWGates] ) * nV_Temp1 + 100 * pTempInRVWString[nSWGates];
						pTempRVWString[nSWGates] = (gInt16)(nV_Temp2 / 100);
					}
				}
			}
		}
	}//end for nRVWNum (0- 360)
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建SAToSt接口 as by dqc 20080423
{
	return new AziSort;
}


