//////////////////////////////////////////////////////////////////////////
// SAToSt.cpp
// created by zy 20080415
// SA雷达数据转写成标准格式，只排序，未合并和差分
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "SAToSt.h"
#include <stdio.h>
#include <math.h>

SAToSt::SAToSt()
{
	;
}
SAToSt::~SAToSt()
{
	;
}

GHRESULT SAToSt::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	m_pParameter = NULL;
	//FileName= NULL;
	//SiteCode=NULL;
	return GS_OK;
}

GHRESULT SAToSt::LoadData(void *pValue,ReadParam * ReadParameter)
{
	m_pParameter=ReadParameter;
	m_pInputList=(GXList< GenerationData > *)pValue;
	if (m_pInputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT SAToSt::OutPutData(void * pValue)
{
	m_pOutputList=(GXList< GenerationData > *)pValue;
	if (m_pOutputList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
gInt32 SAToSt::GetEleNumber(SAProductPretreatment * pMySa)
{
	int LoopCount = pMySa->GetSize()/sizeof(TARCHIVE);
	bool bStartTrue=false;
	bool bEndTrue=false;
	int AziStatus=-1;
	int EleNum=-1;
	int tmpAziStatus=-1;
	int tmpEleNum=-1;
	bool bSetTmp=true;
	for (int i=0;i<LoopCount;i++)
	{
		if (pMySa->GetTArchiveHeadP(i)->RadarData!=1)
		{
			continue;
		}
		if (pMySa->GetTArchiveHeadP(i)->AzimuthStatus!=1)
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
			AziStatus=pMySa->GetTArchiveHeadP(i)->AzimuthStatus;
			EleNum=pMySa->GetTArchiveHeadP(i)->ElevationNumber;
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

extern gInt16 IsArchive2;
extern gInt16 total_tilt_nums;
extern gInt16 max_nrays;
extern gInt16 ref_gates;
extern gInt16 dop_gates;
extern gInt32 total_radial_nums;

GHRESULT SAToSt::Execute()
{
	for (int InputIndex = 0;InputIndex < m_pInputList->GetCount();InputIndex++)
	{
		m_pOutputList->AddTail();
		StandardRadarData * pMySt= &(*(m_pOutputList->GetTailAs<StandardRadarData>()));

		char * pTempName = NULL;
		(*m_pParameter)("SiteCode",pTempName);

		if (strstr(pTempName,"9210")) //WSR-88D类型的雷达数据（Z9210区域的）
		{
			gInt16 nResult = 0;
			gInt32 nSADataNum = 0;//计算之前所有仰角中SA数据块数

			IsArchive2 = 0;
			total_radial_nums = 0;
			total_tilt_nums = 0;
			ref_gates = 0;
			dop_gates = 0;
			max_nrays = 0;

			//准备读出写入的类
			SAProductWSR * pMySaWSR = &(*(m_pInputList->GetAtAs<SAProductWSR>(InputIndex)));

			gInt32 iOffSize = pMySaWSR->GetSize()%sizeof(TARCHIVE2);
			/*m_pA2 = pMySaWSR->GetTArchiveHeadP(0);*/
			gInt16 swapon = 1;
			gInt32 total_radial_nums= pMySaWSR->GetSize()/sizeof(TARCHIVE2);
			gInt16 ElevationNumber = 0;
			gInt16 iRays = 0;
			gInt16 iRefGates = 0;
			gInt16 iDopGates = 0;
			TILTINFO * pTiltInfo = new TILTINFO[20];
			DATAINFO_TYPE * pDataInfo = new DATAINFO_TYPE[total_radial_nums];

			memset(pTiltInfo, 0x00, sizeof(TILTINFO)*20);
			memset(pDataInfo, 0x00, sizeof(DATAINFO_TYPE)*total_radial_nums);

			pMySaWSR->RadialStatusCheck(iOffSize,swapon,total_radial_nums,ElevationNumber,iRays,iRefGates,iDopGates,pTiltInfo,pDataInfo);

			if (ElevationNumber==-1|| ElevationNumber==0 || (ElevationNumber!=7 && ElevationNumber!=8 && ElevationNumber!=11 && ElevationNumber!=16))
			{
				delete []pTiltInfo;
				pTiltInfo = NULL;
				delete []pDataInfo;
				pDataInfo = NULL;
				return GE_OBJECTUNUSEFUL;
			}

			GRADARDATAHEADER * pRadTemHead=new GRADARDATAHEADER;
			if (pRadTemHead==NULL)
			{
				delete []pTiltInfo;
				pTiltInfo = NULL;
				delete []pDataInfo;
				pDataInfo = NULL;
				return GE_NOENOUGHMEMORY;
			}

			GELEVATIONDATAHEAD * pEleTemHead = new GELEVATIONDATAHEAD[ElevationNumber];
			if (pEleTemHead==NULL)
			{
				delete []pTiltInfo;
				delete []pDataInfo;
				delete pRadTemHead;
				pTiltInfo = NULL;
				pDataInfo = NULL;
				pRadTemHead = NULL;
				return GE_NOENOUGHMEMORY;
			}

			for (int iElevIndex=0;iElevIndex< ElevationNumber;iElevIndex++)
			{
				gInt16 iStartIndex = pTiltInfo[iElevIndex].start;
				pEleTemHead[iElevIndex].SNOfElevationData = iElevIndex+1;
				nResult = WriteEleHead(&pEleTemHead[iElevIndex],pMySaWSR->GetTArchiveHeadP(iStartIndex+iRays/2),iRays,swapon);
				if (nResult == -1)
				{
					delete []pTiltInfo;
					delete []pDataInfo;
					delete pRadTemHead;
					delete []pEleTemHead;
					pTiltInfo = NULL;
					pDataInfo = NULL;
					pRadTemHead = NULL;
					pEleTemHead = NULL;
					return GE_NOENOUGHMEMORY;
				}			
			}

			gInt16 iStartIndex = pTiltInfo[ElevationNumber/2+1].start;
			WriteStandardDataHead(pRadTemHead,pMySaWSR->GetTArchiveHeadP(iStartIndex),ElevationNumber,swapon);

			gInt16 *** dataz = NULL;
			gInt16 *** datav = NULL;
			gInt16 *** dataw = NULL;

			dataz = new gInt16**[ElevationNumber];
			datav = new gInt16**[ElevationNumber];
			dataw = new gInt16**[ElevationNumber];
			for(int i=0;i<ElevationNumber;i++)
			{
				dataz[i] = new gInt16*[iRays];
				datav[i] = new gInt16*[iRays];
				dataw[i] = new gInt16*[iRays];
				for(int j=0;j<iRays;j++)
				{
					dataz[i][j] = new gInt16[iRefGates];
					datav[i][j] = new gInt16[iDopGates];
					dataw[i][j] = new gInt16[iDopGates];
				}
			}

			pMySaWSR->GetBaseData(pEleTemHead,dataz,datav,dataw,pTiltInfo,pDataInfo);

			GHRESULT gResult;
			if((gResult = pMySt->WriteHeadAndElevationHead( pRadTemHead,pEleTemHead,ElevationNumber)) != GS_OK)
			{
				delete []pTiltInfo;
				delete []pDataInfo;
				delete pRadTemHead;
				delete []pEleTemHead;
				pTiltInfo = NULL;
				pDataInfo = NULL;
				pRadTemHead = NULL;
				pEleTemHead = NULL;
				for(int i=0;i<ElevationNumber;i++)
				{
					for(int j=0;j<iRays;j++)
					{
						delete []dataz[i][j];
						delete []datav[i][j];
						delete []dataw[i][j];
					}
				}
				for(int i=0;i<ElevationNumber;i++)
				{
					delete []dataz[i];
					delete []datav[i];
					delete []dataw[i];
				}
				delete []dataz;
				delete []datav;
				delete []dataw;
				dataz = datav = dataw = NULL;
				return gResult;
			}

			RVWDATAHEAD * pTempRVW = NULL;
			gInt16 * pTempRVWString = NULL;

			for (int iElevIndex=0;iElevIndex< ElevationNumber;iElevIndex++)
			{
				gInt16 iStartIndex = pTiltInfo[iElevIndex].start;
				int nRefGates = pEleTemHead[iElevIndex].ReflectivityGates;
				int nDopGates = pEleTemHead[iElevIndex].DopplerGates;
				for (int iAziIndex=0;iAziIndex<pEleTemHead[iElevIndex].AzimuthNumber;iAziIndex++)
				{
					gInt16 iIndex = iStartIndex+iAziIndex;

					gUint16 tmp = (gUint16)((gUint64)(pDataInfo[iIndex].AzimuthAngle) * 100 * 180 / 8 / 4096);
					pTempRVW =  pMySt->GetRadarDataP(iElevIndex, ((tmp)/100 )%360);//取标准数据该层nRVWNum块指针
					//写该RVW块数据头
					pTempRVW -> SNOfRadialData = iAziIndex+1;
					pTempRVW -> Elevation = (gInt16)((gUint64)(pDataInfo[iIndex].ElevationAngle) * 100 * 180 / 8 / 4096);          //单位0.01度 
					pTempRVW -> Azimuth =  (gUint16)((gUint64)(pDataInfo[iIndex].AzimuthAngle) * 100 * 180 / 8 / 4096);  //单位0.01度 
					pTempRVW -> Date = pDataInfo[iIndex].MessageGenerationDate;
					pTempRVW -> Time = pDataInfo[iIndex].MessageGenerationTime;
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

					//R数据块
					pTempRVWString = pMySt->GetRadarDataR(iElevIndex, ((pTempRVW -> Azimuth)/100 )%360);
					for( gInt32 iRefIndex = 0;iRefIndex < nRefGates;iRefIndex++ )
					{
						pTempRVWString[iRefIndex] = dataz[iElevIndex][((pTempRVW -> Azimuth)/100 )%360][iRefIndex];
					}

					//V数据块
					pTempRVWString = pMySt->GetRadarDataV(iElevIndex, ((pTempRVW -> Azimuth)/100 )%360);
					for( gInt32 iDopIndex = 0;iDopIndex < nDopGates;iDopIndex++ )
					{
						pTempRVWString[iDopIndex] = datav[iElevIndex][((pTempRVW -> Azimuth)/100 )%360][iDopIndex];
					}

					//W数据块
					pTempRVWString = pMySt->GetRadarDataW(iElevIndex,((pTempRVW -> Azimuth)/100 )%360);
					for( gInt32 iDopIndex = 0;iDopIndex < nDopGates;iDopIndex++ )
					{
						pTempRVWString[iDopIndex] = dataw[iElevIndex][((pTempRVW -> Azimuth)/100 )%360][iDopIndex];
					}
				}
			}

			delete []pTiltInfo;
			delete []pDataInfo;
			delete pRadTemHead;
			delete [] pEleTemHead;
			pTiltInfo = NULL;
			pDataInfo = NULL;
			pRadTemHead = NULL;
			pEleTemHead = NULL;
			for(int i=0;i<ElevationNumber;i++)
			{
				for(int j=0;j<iRays;j++)
				{
					delete []dataz[i][j];
					delete []datav[i][j];
					delete []dataw[i][j];
				}
			}
			for(int i=0;i<ElevationNumber;i++)
			{
				delete []dataz[i];
				delete []datav[i];
				delete []dataw[i];
			}
			delete []dataz;
			delete []datav;
			delete []dataw;
			dataz = datav = dataw = NULL;
			//delete []pTempSA2;pTempSA2 = NULL;

			//return GS_OK;
			continue;
		}
		else//正常数据
		{
			gInt32 nResult = 0;
			gInt32 nSADataNum = 0;//计算之前所有仰角中SA数据块数

			//准备读出写入的类
			SAProductPretreatment * pMySa = &(*(m_pInputList->GetAtAs<SAProductPretreatment>(InputIndex)));

			gInt32 ElevationNumber = GetEleNumber(pMySa);/*(pMySa->GetSize()/sizeof(TARCHIVE))/360;*///计算该SA数据包中包含多少个仰角

			if (ElevationNumber<0||ElevationNumber>20)
			{
				return GE_OBJECTUNUSEFUL;
			}

			if (pMySa->GetTArchiveHeadP((pMySa->GetSize()/sizeof(TARCHIVE))-1)->AzimuthStatus!=4)
			{
				return GE_OBJECTUNUSEFUL;
			}

			for (int i=0;i<10;i++)
			{
				if (pMySa->GetTArchiveHeadP(i)->RadarData!=1)
				{
					if (i==9)
					{
						return GE_OBJECTUNUSEFUL;
					}
					nSADataNum++;
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

			nResult = WriteStandardDataHead( temHead , pMySa->GetTArchiveHeadP(nSADataNum), ElevationNumber );   //写临时 GRadarDataHeader

			if (nResult == -1)
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

			gInt16 *nMyTempSaNum1 = NULL;
			nMyTempSaNum1 = new gInt16[ElevationNumber];
			memset(nMyTempSaNum1,0,sizeof(gInt16)*ElevationNumber);

			gInt16 *nSADataNum1 = NULL;
			nSADataNum1= new gInt16[ElevationNumber];
			memset(nSADataNum1,0,sizeof(gInt16)*ElevationNumber);

			nSADataNum1[0] = nSADataNum;

			for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
			{
				gInt32 nMyTempSaNum = 0;

#if defined(Ignore_Radar_Data)
				nMyTempSaNum++;
				bool DataBlockFlag=true;
				while (DataBlockFlag)
				{
					if (pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )->RadarData!=1)
					{
						nMyTempSaNum++;
						continue;
					} 
					else
					{
						if (( (pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus != 0 && (pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus != 4 ))
						{
							nMyTempSaNum++;
						}
						else
						{
							DataBlockFlag=false;
						}
					}
				}
				if ((pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus == 4)
				{
					nMyTempSaNum++;
				}

#else
				do
				{
					nMyTempSaNum++;
				}while( (pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus != 0 && (pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus != 4 );
				//计算SA下一个仰角第一块前有多少个SA数据块
				if ((pMySa->GetTArchiveHeadP( nSADataNum+nMyTempSaNum )) -> AzimuthStatus == 4)
				{
					nMyTempSaNum++;
				}
#endif

				nMyTempSaNum1[nElevationIndex] = nMyTempSaNum;
				pEleTemHead[nElevationIndex].SNOfElevationData=nElevationIndex+1;

				nResult=0;
				nResult = WriteEleHead( &pEleTemHead[nElevationIndex], pMySa->GetTArchiveHeadP( nSADataNum +nMyTempSaNum/2), nMyTempSaNum);    //写临时仰角块头 
				if (nResult==-1)
				{
					delete temHead;
					temHead=NULL;
					delete [] pEleTemHead;
					pEleTemHead = NULL;
					delete []nMyTempSaNum1 ;
					nMyTempSaNum1 = NULL;
					delete []nSADataNum1;
					nSADataNum1 = NULL;
					return GE_OBJECTUNUSEFUL;
				}

				if (pEleTemHead[nElevationIndex].SNOfElevationData<1||pEleTemHead[nElevationIndex].SNOfElevationData>21)
				{
					delete temHead;
					temHead=NULL;
					delete [] pEleTemHead;
					pEleTemHead = NULL;
					delete []nMyTempSaNum1 ;
					nMyTempSaNum1 = NULL;
					delete []nSADataNum1;
					nSADataNum1 = NULL;
					return GE_OBJECTUNUSEFUL;
				}
				nSADataNum+=nMyTempSaNum;

				if( (pMySa->GetTArchiveHeadP( nSADataNum-1 )) -> AzimuthStatus == 4 )
				{
					break;
				}
				if(nElevationIndex < ElevationNumber-1)
					nSADataNum1[nElevationIndex+1] = nSADataNum;

			}
			//申请产品空间并写头、仰角头

			GHRESULT gResult;
			if((gResult = pMySt->WriteHeadAndElevationHead( temHead,pEleTemHead,ElevationNumber)) != GS_OK)
			{
				delete temHead;
				delete []pEleTemHead;
				delete []nMyTempSaNum1 ;
				nMyTempSaNum1 = NULL;
				delete []nSADataNum1;
				nSADataNum1 = NULL;
				return gResult;
			}

			delete temHead;
			delete [] pEleTemHead;
			pEleTemHead = NULL;

			//写数据块
			for(gInt32 nElevationIndex = 0;nElevationIndex < ElevationNumber; nElevationIndex++ )
			{
				gInt16 nRefGatesA = pMySa->GetTArchiveHeadP( nSADataNum1[nElevationIndex] )->ReflectivityGates;
				gInt16 nDopGatesA = pMySa->GetTArchiveHeadP( nSADataNum1[nElevationIndex] )->DopplerGates;

				nResult= 0;
				nResult=WriteRVW( pMySt, pMySa, nSADataNum1[nElevationIndex], nElevationIndex, nMyTempSaNum1[nElevationIndex], nRefGatesA, nDopGatesA );     //写方位角块 
				if (nResult==-1)
				{
					pMySt->ClearAll();
					delete []nMyTempSaNum1 ;
					nMyTempSaNum1 = NULL;
					delete []nSADataNum1;
					nSADataNum1 = NULL;
					return GE_OBJECTUNUSEFUL;
				}
			}
			delete []nMyTempSaNum1 ;
			nMyTempSaNum1 = NULL;
			delete []nSADataNum1;
			nSADataNum1 = NULL;

			cout<<pMySt<<endl;
			cout<<pMySt->GetHeadP()->RadarLat<<endl;
			cout<<pMySt->GetHeadP()->RadarLon<<endl;

			//return GS_OK;
			continue;
		}
	}

	return GS_OK;
}
GHRESULT SAToSt::UnInitialize()
{
	//if(FileName != NULL)
	//{
	//	delete [] FileName;
	//}

	return GS_OK;
}

//对标准数据包头赋值
gInt32 SAToSt::WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE * pSADataHead, gInt32 ElevationNumber ) 
{
	char * pTempName = NULL;
	//pTempName = "9591";
	(*m_pParameter)("TimeZone",pTempName);
	m_SecondTimeZone = atoi(pTempName);
	pGDataHead -> VCPDate = pSADataHead -> MessageGenerationDate;
	pGDataHead -> VCPTime = pSADataHead -> MessageGenerationTime;//毫秒

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

	if (pSADataHead -> VCP == 11)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x01;
		pGDataHead->VCPMode[1]=0x14;
		pGDataHead->VCPMode[2]=0x16;
		pGDataHead->VCPMode[3]=0x05;
		pGDataHead->ElevationCount=16;
	} 
	else if(pSADataHead -> VCP == 21)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x02;
		pGDataHead->VCPMode[1]=0x09;
		pGDataHead->VCPMode[2]=0x11;
		pGDataHead->VCPMode[3]=0x06;
		pGDataHead->ElevationCount=11;
	}
	else if(pSADataHead -> VCP == 31)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x08;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=8;
	}
	else if(pSADataHead -> VCP == 32)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x07;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=7;
	}
	pGDataHead->SectorCode=pSADataHead->SectorNumber;
	pGDataHead->ReflectivityGateSize=pSADataHead->ReflectivityGateSize;
	if (pGDataHead->ReflectivityGateSize!=1000)
	{
		return -1;
	}
	pGDataHead->DopplerGateSize=pSADataHead->DopplerGateSize;
	if (pGDataHead->DopplerGateSize!=250)
	{
		return -1;
	}
	pGDataHead->ConstantOfSystemCorrect=pSADataHead->CalibrationConstant;
	//memset(pGDataHead->IndexOfElevationData,0,sizeof(gInt32)*20);

	return 0;
}


gInt32 SAToSt::WriteStandardDataHead( GRADARDATAHEADER * pGDataHead, TARCHIVE2 * pSAWSRDataHead, gInt16 ElevationNumber,gInt16 iSwapon) 
{
	pGDataHead->ElevationCount = ElevationNumber;
	gInt16 iSectorCode = pSAWSRDataHead->SectorNumber;
	gInt16 iRefGateSize = pSAWSRDataHead->ReflectivityGateSize;
	gInt16 iDopGateSize = pSAWSRDataHead->DopplerGateSize;
	gUint16 iMGDate = pSAWSRDataHead->MessageGenerationDate;
	gUint32 iMGTime = pSAWSRDataHead->MessageGenerationTime;
	gInt16 iVCP = pSAWSRDataHead->VCP;
	if (iSwapon)
	{
		byteswap(iSectorCode);
		byteswap(iRefGateSize);
		byteswap(iDopGateSize);
		byteswap(iMGDate);
		byteswap(iMGTime);
		byteswap(iVCP);
	}	

	char * pTempName = NULL;

	(*m_pParameter)("TimeZone",pTempName);
	m_SecondTimeZone = atoi(pTempName);
	pGDataHead -> VCPDate = iMGDate;
	pGDataHead -> VCPTime = iMGTime;//毫秒

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

	if (iVCP == 11)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x01;
		pGDataHead->VCPMode[1]=0x14;
		pGDataHead->VCPMode[2]=0x16;
		pGDataHead->VCPMode[3]=0x05;
		pGDataHead->ElevationCount=16;
	} 
	else if(iVCP == 21)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=1;
		pGDataHead->VCPMode[0]=0x02;
		pGDataHead->VCPMode[1]=0x09;
		pGDataHead->VCPMode[2]=0x11;
		pGDataHead->VCPMode[3]=0x06;
		pGDataHead->ElevationCount=11;
	}
	else if(iVCP == 31)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x08;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=8;
	}
	else if(iVCP == 32)
	{
		pGDataHead->OperationMode[0]=0;
		pGDataHead->OperationMode[1]=2;
		pGDataHead->VCPMode[0]=0x03;
		pGDataHead->VCPMode[1]=0x05;
		pGDataHead->VCPMode[2]=0x07;
		pGDataHead->VCPMode[3]=0x10;
		pGDataHead->ElevationCount=7;
	}
	pGDataHead->SectorCode = iSectorCode;
	pGDataHead->ReflectivityGateSize = iRefGateSize;
	pGDataHead->DopplerGateSize = iDopGateSize;
	pGDataHead->ConstantOfSystemCorrect = pSAWSRDataHead->CalibrationConstant;

	return 0;
}

//计算纬度，经度，海拔高度。默认配置中经度、纬度写到小数点后3位，不足0补齐。高度可只有整数位，不写小数位。
gInt32 SAToSt::StrToNum( gChar * pStr, gInt32 nPrecision )
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
	for (int k=nPrecision; k>0; k--)
	{
		nNum = nNum *10 + (pStr[i] - '0');
		i++;
	}
	if((pStr[i]- '0')>=5)
		nNum+= 1;
	/*for (j=nPrecision;pStr[i+nPrecision-j] != '\0';j--)
	{
		nNum = nNum *10 + (pStr[i+nPrecision-j] - '0');
	}

	nPrecision = j;

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
gInt32 SAToSt::WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE * pSADataHead, gInt32 nAziNum )
{
	pGElevationHead -> UnambiguousRange = pSADataHead -> UnambiguousRange;
	pGElevationHead -> NyquistVelocity = pSADataHead -> NyquistVelocity;
	pGElevationHead -> ReflectivityResolution =0;//未知数据，置0
	pGElevationHead -> VelocityResolution = (pSADataHead -> VelocityResolution)*25;
	pGElevationHead -> SpectrumWidthOfVelocityResolution = 0;//未知数据，置0
	pGElevationHead -> FirstGateRangeOfRef = pSADataHead -> FirstGateRangeOfRef;
	pGElevationHead -> FirstGateRangeOfDoppler = pSADataHead -> FirstGateRangeOfDoppler;
	pGElevationHead -> ReflectivityGates = pSADataHead->ReflectivityGates;
	pGElevationHead -> DopplerGates = pSADataHead->DopplerGates;
	pGElevationHead -> Elevation = (gInt32)((gUint64)(pSADataHead -> ElevationAngle) * 100 * 180 / 8 / 4096);          //单位0.01度 ;
	if (pGElevationHead->Elevation<0 || pGElevationHead->Elevation>=9000)
	{
		return -1;
	}
	pGElevationHead -> AzimuthNumber = (gInt16)nAziNum;
	if (pGElevationHead->AzimuthNumber<0 || pGElevationHead->AzimuthNumber>400)
	{
		return -1;
	}
	return 0;
}
//对各仰角块头赋值
gInt16 SAToSt::WriteEleHead( GELEVATIONDATAHEAD * pGElevationHead, TARCHIVE2 * pSAWSRDataHead, gInt32 nAziNum,gInt16 iSwapon)
{
	gUint16 iUnambiguousRange = pSAWSRDataHead->UnambiguousRange;
	gInt16 iNyquistVelocity = pSAWSRDataHead->NyquistVelocity;
	gInt16 iVelocityResolution = pSAWSRDataHead->VelocityResolution;
	gInt16 iFirstGateRangeOfRef = pSAWSRDataHead->FirstGateRangeOfRef;
	gInt16 iFirstGateRangeOfDoppler = pSAWSRDataHead->FirstGateRangeOfDoppler;
	gInt16 iReflectivityGates = pSAWSRDataHead->ReflectivityGates;
	gInt16 iDopplerGates = pSAWSRDataHead->DopplerGates;
	gInt16 iElevationAngle = pSAWSRDataHead->ElevationAngle;

	if (iSwapon)
	{
		byteswap(iUnambiguousRange);
		byteswap(iNyquistVelocity);
		byteswap(iVelocityResolution);
		byteswap(iFirstGateRangeOfRef);
		byteswap(iFirstGateRangeOfDoppler);
		byteswap(iReflectivityGates);
		byteswap(iDopplerGates);
		byteswap(iElevationAngle);
	}

	pGElevationHead -> UnambiguousRange = iUnambiguousRange;
	pGElevationHead -> NyquistVelocity = iNyquistVelocity;
	pGElevationHead -> ReflectivityResolution =0;//未知数据，置0
	pGElevationHead -> VelocityResolution = (iVelocityResolution)*25;
	pGElevationHead -> SpectrumWidthOfVelocityResolution = 0;//未知数据，置0
	pGElevationHead -> FirstGateRangeOfRef = iFirstGateRangeOfRef;
	pGElevationHead -> FirstGateRangeOfDoppler = iFirstGateRangeOfDoppler;
	pGElevationHead -> ReflectivityGates =460;
	pGElevationHead -> DopplerGates = 920;
	pGElevationHead -> Elevation = (gInt16)((gUint64)(iElevationAngle)*180.0/8/40.96);          //单位0.01度 ;
	pGElevationHead -> AzimuthNumber = (gInt16)nAziNum;	
	if (pGElevationHead->Elevation<0 || pGElevationHead->Elevation>=9000)
	{
		return -1;
	}
	if (pGElevationHead->AzimuthNumber<0 || pGElevationHead->AzimuthNumber>400)
	{
		return -1;
	}
	return 0;
}

gInt32 SAToSt::WriteRVW( StandardRadarData * pMySt, SAProductPretreatment * pMySa, gInt32 nSADataNum, gInt32 nElevationIndex,gInt32 nSaAziNum, gInt32 nRefGatesA, gInt32 nDopGatesA )
{
	TARCHIVE * pSATemp = NULL;
	TARCHIVE * pSADataHead = NULL;
	pSADataHead = pMySa->GetTArchiveHeadP(nSADataNum);
	RVWDATAHEAD * pTempRVW = NULL;
	gInt16 * pTempRVWString = NULL;
	gUint16 nLastAzi = 0;
	gUint16 nMinnAzi = 36000;
	gInt32 nLastSolveAziNum=0;

	for (int i=0;i<11;i++)
	{
		m_DBZStatistics[i]=0;
	}

	for( gInt32 nRVWNum = 0;nRVWNum < nSaAziNum;nRVWNum++ )
	{
		nMinnAzi = 36000;
		gInt32 nSANumTemp = 0;
		gUint16 nStartAzi = ((gUint64)(pMySa->GetTArchiveHeadP(nSADataNum) ->AzimuthAngle)) * 100 * 180 / 8 / 4096;
		gUint16 nEndAzi = ((gUint64)(pMySa->GetTArchiveHeadP(nSADataNum+nSaAziNum-1) ->AzimuthAngle)) * 100 * 180 / 8 / 4096;

		pSATemp = pMySa->GetTArchiveHeadP(nSADataNum + nLastSolveAziNum);
		gUint16 nLastSolveAzi = ((gUint64)(pSATemp->AzimuthAngle) * 100 * 180 / 8 / 4096);
// 		if ((nStartAzi>nEndAzi && nLastSolveAzi>nEndAzi && nLastSolveAzi<nStartAzi) || (nStartAzi<nEndAzi && (nLastSolveAzi>nEndAzi || nLastSolveAzi<nStartAzi)))
// 		{
// 			nSANumTemp = nLastSolveAziNum + 1;
// 			nLastAzi = nLastSolveAzi;
// 			nLastSolveAziNum++;
// 		} 
// 		else
// 		{
			for (gInt32 nSANum = 0;nSANum < nSaAziNum;nSANum++)//将pSATemp指向下一个方位角所在块
			{
#if defined(Ignore_Radar_Data)
				if (pMySa->GetTArchiveHeadP(nSADataNum + nSANum)->RadarData!=1)
				{
					continue;
				}
#endif

				if (//(nLastAzi==0&&nRVWNum==0)?((gUint64)(pMySa->GetTArchiveHeadP(nSADataNum + nSANum) ->AzimuthAngle) * 100 * 180 / 8 / 4096)>=nLastAzi:
					((gUint64)(pMySa->GetTArchiveHeadP(nSADataNum + nSANum) ->AzimuthAngle) * 100 * 180 / 8 / 4096)>nLastAzi
					&&  ((gUint64)(pMySa->GetTArchiveHeadP(nSADataNum + nSANum) ->AzimuthAngle) * 100 * 180 / 8 / 4096) < nMinnAzi)
				{
					pSATemp = pMySa->GetTArchiveHeadP(nSADataNum + nSANum);
					nSANumTemp = nSANum;
					nMinnAzi = ((gUint64)(pSATemp->AzimuthAngle) * 100 * 180 / 8 / 4096);
					nLastSolveAziNum = nSANum;
// 					if ((nStartAzi>nEndAzi && nMinnAzi>nEndAzi && nMinnAzi<nStartAzi) || (nStartAzi<nEndAzi && (nMinnAzi>nEndAzi || nMinnAzi<nStartAzi)))
// 					{
// 						break;
// 					}
				}
			}
			nLastAzi = nMinnAzi;
//		}

		//printf("  %d  ",nSANumTemp);
		pTempRVW =  pMySt->GetRadarDataP(nElevationIndex, nRVWNum);//取标准数据该层nRVWNum块指针
		//写该RVW块数据头
		pTempRVW -> SNOfRadialData = nRVWNum+1;
		if (pTempRVW->SNOfRadialData<1 || pTempRVW->SNOfRadialData>401)
		{
			return -1;
		}
		pTempRVW -> Elevation = (gInt16)((gUint64)(pSATemp -> ElevationAngle) * 100 * 180 / 8 / 4096);          //单位0.01度 
		if (pTempRVW->Elevation<0 || pTempRVW->Elevation>=9000)
		{
			return -1;
		}
		pTempRVW -> Azimuth =  ((gUint64)(pSATemp->AzimuthAngle) * 100 * 180 / 8 / 4096);  //单位0.01度 
		if (pTempRVW->Azimuth<0||pTempRVW->Azimuth>=36000)
		{
			return -1;
		}
		pTempRVW -> Date = pSATemp -> MessageGenerationDate;
		pTempRVW -> Time = pSATemp -> MessageGenerationTime;
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
		//printf("r %d ",pSATemp->ReflectivityPointer);
		gUint8 * pTempRRead = pMySa->GetTArchiveRP((nSADataNum+nSANumTemp) * sizeof(TARCHIVE) +(gInt32)pSATemp->ReflectivityPointer + 28);
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

			gInt16 tmpDBZ=pTempRVWString[nRefGates];
			if (tmpDBZ==RADAR_DATA_NOECHO)
			{
				m_DBZStatistics[10]++;
			} 
			else
			{
				if (tmpDBZ<0)
				{
					tmpDBZ=0;
				} 
				if (tmpDBZ>4500)
				{
					tmpDBZ=4500;
				}
				
				m_DBZStatistics[(int)ceil((float)tmpDBZ/500)]++;
			}
		}
		/*
		以上for循环将非有效数据部分填写RADAR_DATA_NOECHO
		改写为
		for( gInt32 nRefGates = 0;nRefGates < nRefGatesA;nRefGates++ )
		{
		pTempRVWString[nRefGates] = GetDBZ( pSATemp -> RefData[nRefGates] );
		}
		则是对非有效数据部分不处理

		对VW处理的for循环原理相同
		*/

		//V数据块
		pTempRVWString = pMySt->GetRadarDataV(nElevationIndex,nRVWNum);
		//printf("v %d ",pSATemp->VelocityPointer);
		gUint8 * pTempVRead = pMySa->GetTArchiveVP((nSADataNum+nSANumTemp) * sizeof(TARCHIVE) +(gInt32)pSATemp->VelocityPointer + 28);
		tempCount=pMySt->GetElevationHeadP(nElevationIndex)->DopplerGates;
		for( gInt32 nVelGates = 0;nVelGates < tempCount;nVelGates++ )
		{
			if (nVelGates>=nDopGatesA)
			{
				pTempRVWString[nVelGates]=RADAR_DATA_NOECHO;
			}
			else
			{
				pTempRVWString[nVelGates] = GetVelData( pTempVRead[nVelGates], pSATemp -> VelocityResolution, pSATemp->NyquistVelocity );
				//printf("\nsa %d  st %d  vel %d  nyq %d  ",pSATemp -> VelData[nVelGates],pTempRVWString[nVelGates], pSATemp -> VelocityResolution, pSATemp->NyquistVelocity  );
			}
		}

		//W数据块
		pTempRVWString = pMySt->GetRadarDataW(nElevationIndex,nRVWNum);
		//printf("w %d \n",pSATemp->SpectrumWidthPointer);
		gUint8 * pTempWRead = pMySa->GetTArchiveWP((nSADataNum+nSANumTemp) * sizeof(TARCHIVE) +(gInt32)pSATemp->SpectrumWidthPointer + 28);
		tempCount=pMySt->GetElevationHeadP(nElevationIndex)->DopplerGates;
		for( gInt32 nSWGates = 0;nSWGates < tempCount;nSWGates++ )
		{
			if (nSWGates>=nDopGatesA)
			{
				pTempRVWString[nSWGates]=RADAR_DATA_NOECHO;
			}
			else
			{
				pTempRVWString[nSWGates] = GetSWData( pTempWRead[nSWGates], pSATemp->NyquistVelocity );
			}
		}
	}//end for nRVWNum (0- nSaAziNum)
	//printf("\n\n\n");

	if (nElevationIndex==0||nElevationIndex==2)
	{
		gInt32 DBZCount=0;
		gInt32 DBZCount1=0;
		for (int i=0;i<11;i++)
		{
			DBZCount+=m_DBZStatistics[i];
			if (i!=10)
			{
				DBZCount1+=m_DBZStatistics[i];
			}
		}
		if (m_DBZStatistics[9]*100/DBZCount>=50)
		{
			return -1;
		}
		if (DBZCount1*100/DBZCount>=80)
		{
			gInt32 DBZMore=0;
			gInt32 DBZLess=0;
			gInt32 DBZ2N=0;
			gInt32 DBZ2N1=0;
			for (int k=0;k<5;k++)
			{
				DBZMore+=m_DBZStatistics[k];
				DBZLess+=m_DBZStatistics[k+5];
				DBZ2N+=m_DBZStatistics[2*k];
				DBZ2N1+=m_DBZStatistics[2*k+1];
			}
			if (   abs((gInt32)((DBZMore-DBZLess)*100/DBZCount1))<=20 
				&& abs((gInt32)((DBZ2N  -DBZ2N1 )*100/DBZCount1))<=20 )
			{
				return -1;
			}
		}
	}
	
	return 0;
}

//得到反射率值  当数值为-32768时，表示无回波数据（低于信噪比阀值）当数值为-32767时，表示距离模糊 其他是DBZ值，单位为0.1DBZ
gInt16 SAToSt::GetDBZ( gUint8 nNum )
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
gInt16 SAToSt::GetVelData( gUint8 nNum, gInt16 VelocityResolution, gInt16 /*NyquistVelocity*/  )
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
gInt16 SAToSt::GetSWData( gUint8 nNum, gInt16 /*NyquistVelocity*/  )
{
	return (gInt16)( nNum == 0 ? RADAR_DATA_NOECHO : ( nNum == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)nNum * 50 - 6450) ) );
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建SAToSt接口 as by dqc 20080422
{
	return new SAToSt;
}
