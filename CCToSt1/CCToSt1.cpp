//////////////////////////////////////////////////////////////////////////
// SCToSt1.cpp
// created by zy 20080415
// SC雷达数据转写成标准格式，只排序，未合并和差分
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "CCToSt1.h"
#include <stdio.h>


CCToSt1::CCToSt1()
{
	;
}
CCToSt1::~CCToSt1()
{
	;
}

GHRESULT CCToSt1::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	//FileName= NULL;
	//SiteCode=NULL;
	return GS_OK;
}

GHRESULT CCToSt1::LoadData(void *pValue,ReadParam * ReadParameter)
{
	m_pParameter=ReadParameter;
	m_pInputList=(GXList< GenerationData > *)pValue;
	if (m_pInputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT CCToSt1::OutPutData(void * pValue)
{
	m_pOutputList=(GXList< GenerationData > *)pValue;
	if (m_pOutputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
gInt32 CCToSt1::GetEleNumber(CCProductPretreatment1 * pMySC)
{
	int LoopCount = pMySC->GetSize()/sizeof(TARCHIVE);
	bool bStartTrue=false;
	bool bEndTrue=false;
	int AziStatus=-1;
	int EleNum=-1;
	int tmpAziStatus=-1;
	int tmpEleNum=-1;
	bool bSetTmp=true;
	for (int i=0;i<LoopCount;i++)
	{
		if (pMySC->GetTArchiveHeadP(i)->RadarData!=1)
		{
			continue;
		}
		if (pMySC->GetTArchiveHeadP(i)->AzimuthStatus!=1)
		{
			if (bSetTmp)
			{
				tmpAziStatus=AziStatus;
				tmpEleNum=EleNum;
			}
			else
			{
				bSetTmp=true;
			}
			AziStatus=pMySC->GetTArchiveHeadP(i)->AzimuthStatus;
			EleNum=pMySC->GetTArchiveHeadP(i)->ElevationNumber;
		}
		else
		{
			continue;
		}
		switch(AziStatus)
		{
		case 0:
			if (tmpAziStatus!=2 || tmpEleNum+1!=EleNum)
			{
				return -1;
			}
			break;
		case 2:
			if (!(tmpAziStatus==0 || tmpAziStatus==3) || tmpEleNum!= EleNum)
			{
				return -1;
			}
			break;
		case 3:
			if (tmpAziStatus!=-1 || tmpEleNum!=-1)
			{
				return -1;
			}
			bStartTrue=true;
			break;
		case 4:
			if (tmpAziStatus!=0 || tmpEleNum!=EleNum)
			{
				return -1;
			}
			bEndTrue=true;
			break;
		default:
			bSetTmp=false;
			break;
		}
	}
	if (bStartTrue==false||bEndTrue==false)
	{
		return -1;
	}
	return EleNum;
}

GHRESULT CCToSt1::Execute()
{
	m_pOutputList->AddTail();
	StandardRadarData * pMySt= &(*(m_pOutputList->GetTailAs<StandardRadarData>()));

	char * pTempName = NULL;
	(*m_pParameter)("SiteCode",pTempName);

	gInt32 nResult = 0;
	gInt32 nSCDataNum = 0;//计算之前所有仰角中SC数据块数

	//准备读出写入的类
	CCProductPretreatment1 * pMySC = &(*(m_pInputList->GetHeadAs<CCProductPretreatment1>()));

	gInt32 ElevationNumber = GetEleNumber(pMySC);/*(pMySC->GetSize()/sizeof(TARCHIVE))/360;*///计算该SC数据包中包含多少个仰角

	if (ElevationNumber==-1)
	{
		return GE_OBJECTUNUSEFUL;
	}

	if (pMySC->GetTArchiveHeadP((pMySC->GetSize()/sizeof(TARCHIVE))-1)->AzimuthStatus!=4)
	{
		return GE_OBJECTUNUSEFUL;
	}

	for (int i=0;i<10;i++)
	{
		if (pMySC->GetTArchiveHeadP(i)->RadarData!=1)
		{
			if (i==9)
			{
				return GE_OBJECTUNUSEFUL;
			}
			nSCDataNum++;
		}
		else
		{
			break;
		}
	}

	//写头并分配空间
	GRADARDATAHEADER * temHead=new GRADARDATAHEADER;
	if (temHead==NULL)
	{
		return GE_NOENOUGHMEMORY;
	}

	nResult = WriteStandardDataHead( temHead , pMySC->GetTArchiveHeadP(nSCDataNum), ElevationNumber );   //写临时 GRadarDataHeader

	if (nResult == 1)
	{
		if (temHead != NULL)
		{
			delete temHead;
			temHead=NULL;
		}
		return GE_OBJECTUNUSEFUL;
	}
	GHRESULT TemRet;

	GELEVATIONDATAHEAD * pEleTemHead = new GELEVATIONDATAHEAD[ElevationNumber];
	if (pEleTemHead==NULL)
	{
		if (temHead != NULL)
		{
			delete temHead;
			temHead=NULL;
		}
		return GE_NOENOUGHMEMORY;
	}

	gInt16 *nMyTempSCNum1 = NULL;
	nMyTempSCNum1 = new gInt16[ElevationNumber];
	memset(nMyTempSCNum1,0,sizeof(gInt16)*ElevationNumber);

	gInt16 *nSCDataNum1 = NULL;
	nSCDataNum1= new gInt16[ElevationNumber];
	memset(nSCDataNum1,0,sizeof(gInt16)*ElevationNumber);

	nSCDataNum1[0] = nSCDataNum;

	for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
	{
		gInt32 nMyTempSCNum = 0;

#if defined(Ignore_Radar_Data)
		nMyTempSCNum++;
		bool DataBlockFlag=true;
		while (DataBlockFlag)
		{
			if (pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )->RadarData!=1)
			{
				nMyTempSCNum++;
				continue;
			} 
			else
			{
				if (( (pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus != 0 && (pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus != 4 ))
				{
					nMyTempSCNum++;
				}
				else
				{
					DataBlockFlag=false;
				}
			}
		}
		if ((pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus == 4)
		{
			nMyTempSCNum++;
		}
#else
		do
		{
			nMyTempSCNum++;
		}while( (pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus != 0 && (pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus != 4 );
		if ((pMySC->GetTArchiveHeadP( nSCDataNum+nMyTempSCNum )) -> AzimuthStatus == 4)
		{
			nMyTempSCNum++;
		}
		//计算SC下一个仰角第一块前有多少个SC数据块
#endif

		nMyTempSCNum1[nElevationIndex] = nMyTempSCNum;

		WriteEleHead( &pEleTemHead[nElevationIndex], pMySC->GetTArchiveHeadP( nSCDataNum + nMyTempSCNum/2), nMyTempSCNum);    //写临时仰角块头 
		pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;

		nSCDataNum+=nMyTempSCNum;

		if( (pMySC->GetTArchiveHeadP( nSCDataNum-1 )) -> AzimuthStatus == 4 )
		{
			break;
		}
		if(nElevationIndex < ElevationNumber-1)
			nSCDataNum1[nElevationIndex+1] = nSCDataNum;

	}
	//申请产品空间并写头、仰角头

	temHead->ElevationCount = ElevationNumber;
	GHRESULT gResult;
	if((gResult = pMySt->WriteHeadAndElevationHead(temHead,pEleTemHead,ElevationNumber)) != GS_OK)
	{
		delete temHead;
		delete [] pEleTemHead;
		delete []nMyTempSCNum1 ;
		nMyTempSCNum1 = NULL;
		delete []nSCDataNum1;
		nSCDataNum1 = NULL;
		return gResult;
	}

	delete temHead;
	delete [] pEleTemHead;
	pEleTemHead = NULL;

	//写数据块
	for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
	{
		gInt16 nRefGatesA = pMySC->GetTArchiveHeadP( nSCDataNum1[nElevationIndex] )->ReflectivityGates;
		gInt16 nDopGatesA = pMySC->GetTArchiveHeadP( nSCDataNum1[nElevationIndex] )->DopplerGates;

		WriteRVW( pMySt, pMySC, nSCDataNum1[nElevationIndex], nElevationIndex, nMyTempSCNum1[nElevationIndex], nRefGatesA, nDopGatesA );     //写方位角块 
	}
	delete []nMyTempSCNum1 ;
	nMyTempSCNum1 = NULL;
	delete []nSCDataNum1;
	nSCDataNum1 = NULL;

	return GS_OK;
}
GHRESULT CCToSt1::UnInitialize()
{
	//if(FileName != NULL)
	//{
	//	delete [] FileName;
	//}

	return GS_OK;
}

//对标准数据包头赋值
gInt32 CCToSt1::WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSCDataHead, gInt32 ElevationNumber ) 
{
	char * pTempName = NULL;

	(*m_pParameter)("TimeZone",pTempName);
	m_SecondTimeZone = atoi(pTempName);
	pGDataHead -> VCPDate = pSCDataHead -> MessageGenerationDate;
	pGDataHead -> VCPTime = pSCDataHead -> MessageGenerationTime;//毫秒

	if (m_SecondTimeZone > 28800)
	{
		gInt32 TimeDiff = (m_SecondTimeZone - 28800) * 1000;
		pGDataHead->VCPTime-=TimeDiff;
		if (pGDataHead->VCPTime < 0)
		{
			pGDataHead->VCPDate--;
			pGDataHead->VCPTime+=24*3600*1000;
		}
	} 
	else if ( m_SecondTimeZone < 28800)
	{
		gInt32 TimeDiff = (28800 - m_SecondTimeZone) * 1000;
		pGDataHead->VCPTime+=TimeDiff;
		if (pGDataHead->VCPTime >= 24*3600*1000)
		{
			pGDataHead->VCPDate++;
			pGDataHead->VCPTime-=24*3600*1000;
		}
	} 
	else
	{
		;
	}

	(*m_pParameter)("SiteCode",pTempName);
	pGDataHead->SiteCode[0]=pGDataHead->SiteCode[1]=pGDataHead->SiteCode[4]='0';
	pGDataHead->SiteCode[2]=pTempName[0];
	pGDataHead->SiteCode[3]=pTempName[1];
	pGDataHead->SiteCode[5]=pTempName[2];
	pGDataHead->SiteCode[6]=pTempName[3];
	pGDataHead->SiteCode[7]=pTempName[4];

	/*
	4字节8位数字编码
	第一个字节：表示雷达类型，01：多普勒天气雷达，其它雷达类型（相控阵、风廓线等）编码待定
	第二个字节：高4位表示雷达部署方式（1：表示固定式雷达、2：表示移动式雷达），低4位表示子类型（固定式雷达分1：地基雷达，机动式雷达分1：车载、2：船载、3：机载）
	第三个字节：表示雷达波段，01：K波段，02：S波段，03：C波段，04：X波段
	第四个字节：表示雷达型号，高4位表示雷达厂商编号，低4位表示雷达型号编码
	*/
	(*m_pParameter)("RadarType",pTempName);
	pGDataHead->RadarType[0]=16*(pTempName[0]-'0')+(pTempName[1]-'0');
	pGDataHead->RadarType[1]=16*(pTempName[2]-'0')+(pTempName[3]-'0');
	pGDataHead->RadarType[2]=16*(pTempName[4]-'0')+(pTempName[5]-'0');
	pGDataHead->RadarType[3]=16*(pTempName[6]-'0')+(pTempName[7]-'0');

	(*m_pParameter)("RadarLon",pTempName);
	pGDataHead -> RadarLon = StrToNum(pTempName, 3);//  int32 单位：.001度

	(*m_pParameter)("RadarLat",pTempName);
	pGDataHead -> RadarLat = StrToNum(pTempName, 3);//  int32 单位：.001度

	(*m_pParameter)("RadarHeight",pTempName);
	pGDataHead -> RadarHeight = (gUint32)StrToNum(pTempName, 2);//  uint16 单位：.01米

	(*m_pParameter)("ScanMode",pTempName);
	if (!strcmp(pTempName,"VCP"))
	{
		pGDataHead->ScanMode[0]=0;
		pGDataHead->ScanMode[1]=0x06;
		pGDataHead->SizeOfSector=360;//其他可能不同
	}
	else if (!strcmp(pTempName,"PPI"))
	{
		pGDataHead->ScanMode[0]=0;
		pGDataHead->ScanMode[1]=0x02;
		//pGDataHead->SizeOfSector=0;//其他可能不同
	}
	else if (!strcmp(pTempName,"RHI"))
	{
		pGDataHead->ScanMode[0]=0;
		pGDataHead->ScanMode[1]=0x05;
		//pGDataHead->SizeOfSector=0;//其他可能不同
	}
	else
	{
		;
	}

	if (pSCDataHead -> VCP == 11)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x01;
		pGDataHead->VCPMode[1]=0x14;
		pGDataHead->VCPMode[2]=0x16;
		pGDataHead->VCPMode[3]=0x05;
		pGDataHead->ElevationCount=14;
	} 
	else if(pSCDataHead -> VCP == 21)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x02;
		pGDataHead->VCPMode[1]=0x09;
		pGDataHead->VCPMode[2]=0x11;
		pGDataHead->VCPMode[3]=0x06;
		if ( ElevationNumber == 9)
		{
			pGDataHead->ElevationCount= 9;
		}
		else
		{
			pGDataHead->ElevationCount=11;
		}
	}
	else if(pSCDataHead -> VCP == 31)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x08;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=5;
	}
	else if(pSCDataHead -> VCP == 32)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x07;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=5;
	}
	pGDataHead->SectorCode=pSCDataHead->SectorNumber;
	pGDataHead->ReflectivityGateSize=pSCDataHead->ReflectivityGateSize;
	pGDataHead->DopplerGateSize=pSCDataHead->DopplerGateSize;
	pGDataHead->ConstantOfSystemCorrect=pSCDataHead->CalibrationConstant;
	//memset(pGDataHead->IndexOfElevationData,0,sizeof(gInt32)*20);

	return 0;
}


//计算纬度，经度，海拔高度。默认配置中经度、纬度写到小数点后3位，不足0补齐。高度可只有整数位，不写小数位。
gInt32 CCToSt1::StrToNum( gChar * pStr, gInt32 nPrecision )
{
	gInt32 nNum = 0;
	gInt32 i=0;
	gInt32 nFlag = 0;
	for (i=0;pStr[i] != '\0' && nFlag!=1;i++)
	{
		if (pStr[i]!='.')
		{
			nNum = nNum *10 + (pStr[i] - '0');
		} 
		else
		{
			nFlag = 1;
		}
	}

	gInt32 j =0;
	for (j=nPrecision;pStr[i+nPrecision-j] != '\0';j--)
	{
		nNum = nNum *10 + (pStr[i+nPrecision-j] - '0');
	}

	for (int k=nPrecision; k>0; k--)
	{
		nNum = nNum *10 + (pStr[i] - '0');
		i++;
	}
	if((pStr[i]- '0')>=5)
		nNum+= 1;
	/*nPrecision = j;

	while (nPrecision!=0)
	{
		if (nPrecision>0)
		{
			nNum*=10;
			nPrecision--;
		} 
		else
		{
			nNum = (nNum+5)/10;
			nPrecision++;
		}
	}*/

	return nNum;
}

//对各仰角块头赋值
void CCToSt1::WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSCDataHead, gInt32 nAziNum )
{
	pGElevationHead -> UnambiguousRange = pSCDataHead -> UnambiguousRange;
	pGElevationHead -> NyquistVelocity = pSCDataHead -> NyquistVelocity;
	pGElevationHead -> ReflectivityResolution =0;//未知数据，置0
	pGElevationHead -> VelocityResolution = (pSCDataHead -> VelocityResolution)*25;
	pGElevationHead -> SpectrumWidthOfVelocityResolution = 0;//未知数据，置0
	pGElevationHead -> FirstGateRangeOfRef = pSCDataHead -> FirstGateRangeOfRef;
	pGElevationHead -> FirstGateRangeOfDoppler = pSCDataHead -> FirstGateRangeOfDoppler;
	pGElevationHead -> ReflectivityGates = pSCDataHead->ReflectivityGates;
	pGElevationHead -> DopplerGates = pSCDataHead->DopplerGates;
	pGElevationHead -> Elevation = (gInt32)((gUint64)(pSCDataHead -> ElevationAngle) * 100 * 180 / 8 / 4096);          //单位0.01度 ;
	pGElevationHead -> AzimuthNumber = (gInt16)nAziNum;
}
//对各仰角块头赋值
void CCToSt1::WriteRVW( StandardRadarData * pMySt, CCProductPretreatment1 * pMySc, gInt32 nSCDataNum, gInt32 nElevationIndex,gInt32 nScAziNum, gInt32 nRefGatesA, gInt32 nDopGatesA )
{
	TARCHIVE * pSCTemp = NULL;
	TARCHIVE * pSCDataHead = NULL;
	pSCDataHead = pMySc->GetTArchiveHeadP(nSCDataNum);
	RVWDATAHEAD * pTempRVW = NULL;
	gInt16 * pTempRVWString = NULL;
	gUint16 nLastAzi = 0;
	gUint16 nMinnAzi = 36000;
	gInt32 nLastSolveAziNum=0;


	for( gInt32 nRVWNum = 0;nRVWNum < nScAziNum;nRVWNum++ )
	{
		nMinnAzi = 36000;
		gInt32 nSCNumTemp = 0;
		gUint16 nStartAzi = ((gUint64)(pMySc->GetTArchiveHeadP(nSCDataNum) ->AzimuthAngle)) * 100 * 180 / 8 / 4096;
		gUint16 nEndAzi = ((gUint64)(pMySc->GetTArchiveHeadP(nSCDataNum+nScAziNum-1) ->AzimuthAngle)) * 100 * 180 / 8 / 4096;

		pSCTemp = pMySc->GetTArchiveHeadP(nSCDataNum + nLastSolveAziNum);
		gUint16 nLastSolveAzi = ((gUint64)(pSCTemp->AzimuthAngle) * 100 * 180 / 8 / 4096);
// 		if ((nStartAzi>nEndAzi && nLastSolveAzi>nEndAzi && nLastSolveAzi<nStartAzi) || (nStartAzi<nEndAzi && (nLastSolveAzi>nEndAzi || nLastSolveAzi<nStartAzi)))
// 		{
// 			nSCNumTemp = nLastSolveAziNum + 1;
// 			nLastAzi = nLastSolveAzi;
// 			nLastSolveAziNum++;
// 		} 
// 		else
// 		{
			for (gInt32 nSCNum = 0;nSCNum < nScAziNum;nSCNum++)//将pSCTemp指向下一个方位角所在块
			{
#if defined(Ignore_Radar_Data)
				if (pMySc->GetTArchiveHeadP(nSCDataNum + nSCNum)->RadarData!=1)
				{
					continue;
				}
#endif

				if (// ((nLastAzi==0&&nRVWNum==0)?((gUint64)(pMySc->GetTArchiveHeadP(nSCDataNum + nSCNum) ->AzimuthAngle) * 100 * 180 / 8 / 4096)>=nLastAzi:
					(((gUint64)(pMySc->GetTArchiveHeadP(nSCDataNum + nSCNum) ->AzimuthAngle) * 100 * 180 / 8 / 4096)>nLastAzi)
					&&  ((gUint64)(pMySc->GetTArchiveHeadP(nSCDataNum + nSCNum) ->AzimuthAngle) * 100 * 180 / 8 / 4096) < nMinnAzi)
				{
					pSCTemp = pMySc->GetTArchiveHeadP(nSCDataNum + nSCNum);
					nSCNumTemp = nSCNum;
					nMinnAzi = ((gUint64)(pSCTemp->AzimuthAngle) * 100 * 180 / 8 / 4096);
					nLastSolveAziNum = nSCNum;
// 					if ((nStartAzi>nEndAzi && nMinnAzi>nEndAzi && nMinnAzi<nStartAzi) || (nStartAzi<nEndAzi && (nMinnAzi>nEndAzi || nMinnAzi<nStartAzi)))
// 					{
// 						break;
// 					}
				}
			}
			nLastAzi = nMinnAzi;
//		}

		//printf("  %d  ",nSCNumTemp);
		pTempRVW =  pMySt->GetRadarDataP(nElevationIndex, nRVWNum);//取标准数据该层nRVWNum块指针
		//写该RVW块数据头
		pTempRVW -> SNOfRadialData = nRVWNum+1;
		pTempRVW -> Elevation = (gInt16)((gUint64)(pSCTemp -> ElevationAngle) * 100 * 180 / 8 / 4096);          //单位0.01度 
		pTempRVW -> Azimuth =  ((gUint64)(pSCTemp->AzimuthAngle) * 100 * 180 / 8 / 4096);  //单位0.01度 
		pTempRVW -> Date = pSCTemp -> MessageGenerationDate;
		pTempRVW -> Time = pSCTemp -> MessageGenerationTime;
		if (m_SecondTimeZone > 28800)
		{
			gInt32 TimeDiff = (m_SecondTimeZone - 28800) * 1000;
			pTempRVW -> Time-=TimeDiff;
			if (pTempRVW -> Time < 0)
			{
				pTempRVW -> Date--;
				pTempRVW -> Time+=24*3600*1000;
			}
		} 
		else if ( m_SecondTimeZone < 28800)
		{
			gInt32 TimeDiff = (28800 - m_SecondTimeZone) * 1000;
			pTempRVW -> Time+=TimeDiff;
			if (pTempRVW -> Time >= 24*3600*1000)
			{
				pTempRVW -> Date++;
				pTempRVW -> Time-=24*3600*1000;
			}
		} 
		else
		{
			;
		}

		//R数据块
		pTempRVWString = pMySt->GetRadarDataR(nElevationIndex, nRVWNum);
		//printf("r %d ",pSCTemp->ReflectivityPointer);
		gUint8 * pTempRRead = pMySc->GetTArchiveRP((nSCDataNum+nSCNumTemp) * sizeof(TARCHIVE) +(gInt32)pSCTemp->ReflectivityPointer + 28);
		int tempCount=pMySt->GetElevationHeadP(nElevationIndex)->ReflectivityGates;
		for( gInt32 nRefGates = 0;nRefGates < tempCount;nRefGates++ )
		{
			if (nRefGates>=nRefGatesA)
			{
				pTempRVWString[nRefGates]=RADAR_DATA_NOECHO;
			}
			else
			{
				pTempRVWString[nRefGates] = GetDBZ( pTempRRead[nRefGates] );
			}
		}
		/*
		以上for循环将非有效数据部分填写RADAR_DATA_NOECHO
		改写为
		for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
		{
		pTempRVWString[nRefGates] = GetDBZ( pSCTemp -> RefData[nRefGates] );
		}
		则是对非有效数据部分不处理

		对VW处理的for循环原理相同
		*/

		//V数据块
		pTempRVWString = pMySt->GetRadarDataV(nElevationIndex,nRVWNum);
		//printf("v %d ",pSCTemp->VelocityPointer);
		gUint8 * pTempVRead = pMySc->GetTArchiveVP((nSCDataNum+nSCNumTemp) * sizeof(TARCHIVE) +(gInt32)pSCTemp->VelocityPointer + 28);
		tempCount=pMySt->GetElevationHeadP(nElevationIndex)->DopplerGates;
		for( gInt32 nVelGates = 0;nVelGates < tempCount;nVelGates++ )
		{
			if (nVelGates>=nDopGatesA)
			{
				pTempRVWString[nVelGates]=RADAR_DATA_NOECHO;
			}
			else
			{
				pTempRVWString[nVelGates] = GetVelData( pTempVRead[nVelGates], pSCTemp -> VelocityResolution, pSCTemp->NyquistVelocity );
				//printf("\nsc %d  st %d  vel %d  nyq %d  ",pSCTemp -> VelData[nVelGates],pTempRVWString[nVelGates], pSCTemp -> VelocityResolution, pSCTemp->NyquistVelocity  );
			}
		}

		//W数据块
		pTempRVWString = pMySt->GetRadarDataW(nElevationIndex,nRVWNum);
		//printf("w %d \n",pSCTemp->SpectrumWidthPointer);
		gUint8 * pTempWRead = pMySc->GetTArchiveWP((nSCDataNum+nSCNumTemp) * sizeof(TARCHIVE) +(gInt32)pSCTemp->SpectrumWidthPointer + 28);
		tempCount=pMySt->GetElevationHeadP(nElevationIndex)->DopplerGates;
		for( gInt32 nSWGates = 0;nSWGates < tempCount;nSWGates++ )
		{
			if (nSWGates>=nDopGatesA)
			{
				pTempRVWString[nSWGates]=RADAR_DATA_NOECHO;
			}
			else
			{
				pTempRVWString[nSWGates] = GetSWData( pTempWRead[nSWGates], pSCTemp->NyquistVelocity );
			}
		}
	}//end for nRVWNum (0- nScAziNum)
	//printf("\n\n\n");
}

//得到反射率值  当数值为-32768时，表示无回波数据（低于信噪比阀值）当数值为-32767时，表示距离模糊 其他是DBZ值，单位为0.1DBZ
gInt16 CCToSt1::GetDBZ( gUint8 nNum )
{
	//if( nNum == 0 )
	//{
	//	return -32768;
	//}
	//else if( nNum == 1 )
	//{
	//	return -32767;
	//}
	//else
	//{
	//	return (nNum - 66) * 5;
	//}

	return (gInt16)( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)nNum - 66) * 50 ) );
	//当数值为-32768时，表示无回波数据（低于信噪比阀值）当数值为-32767时，表示距离模糊 其他是DBZ值，单位为0.1DBZ
}

//得到速度值  当数值为-32768 或 -32767时，意义同上  单位 0.1米/秒
gInt16 CCToSt1::GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 /*NyquistVelocity*/  )
{
	//if ( VelocityResolution == 2 )//分辨率0.5
	//{
	//	return (gInt16)( nNum == 0 ? -32768 : ( nNum == 1 ? -32767 : (nNum * 5 - 645) ) );
	//} 
	//else
	//{
	//	return (gInt16)( nNum == 0 ? -32768 : ( nNum == 1 ? -32767 : (nNum * 10 - 1290) ) );
	//}

	gInt16 nVel = 0;
	if (VelocityResolution==2)
	{
		nVel = 5;
	} 
	else
	{
		nVel = 10;
	}
	return (gInt16)( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : (VelocityResolution == 2 ?((gInt32)nNum * 50 - 6450) :((gInt32)nNum * 100 - 12900))));
	//return (gInt16)(VelocityResolution == 2 ? ( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)nNum * 5 - 645) ) ) : ( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)nNum * 10 - 1290) ) ));
}

//得到谱宽值  当数值为-32768 或 -32767时，意义同上  单位 0.1米/秒
gInt16 CCToSt1::GetSWData( gUint8 nNum, gInt16 /*NyquistVelocity*/  )
{
	return (gInt16)( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)nNum * 50 - 6450) ) );
}

GHRESULT DLLEXP_GSYS ExecuteArithCCToSt1( GXList< GenerationData>  *Datas,  ReadParam* pParam,  GXList< GenerationData > * pOutputList)
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
	CCToSt1 *Arith = new CCToSt1();
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
	printf("begin CCToSt1 Execute\n");
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

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建SCToSt1接口 as by dqc 20080422
{
	return new CCToSt1;
}
