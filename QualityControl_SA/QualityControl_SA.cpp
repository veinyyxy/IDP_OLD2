/////////////////////////////////////////
//QualityControl_SA.h
//modifyed by cmm
////////////////////////////////////////
//实现类QualityControl_SA
//质量控制
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "../GDataTypes/CJulMsGMT.h"
#include "QualityControl_SA.h"

QualityControl_SA::QualityControl_SA()
{
	;
}

QualityControl_SA::~QualityControl_SA()
{
	;
}

GHRESULT QualityControl_SA :: Initialize()
{
	m_pGenerateProductList = NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;
	memset(RadarType,0,sizeof(RadarType));
	tempscale =10.0;
//  径向速度的区域平均值
	mem_para_ap.mdve[0]=-23;
	mem_para_ap.mdve[1]=0;
	mem_para_ap.mdve[2]=23;

	//速度谱宽的区域平均值
	mem_para_ap.mdsw[0]=-1000;
	mem_para_ap.mdsw[1]=0;
	mem_para_ap.mdsw[2]=32;
	//方差
	mem_para_ap.sdve[0]=-1000;
	mem_para_ap.sdve[1]=0;
	mem_para_ap.sdve[2]=10;
	//回波强度的纹理
	mem_para_ap.tdbz[0]=0;
	mem_para_ap.tdbz[1]=45;
	mem_para_ap.tdbz[2]=10000;
	//沿径向的库间变化程度!
	mem_para_ap.spin[0]=0;
	mem_para_ap.spin[1]=50;
	mem_para_ap.spin[2]=100;
	//沿径向方向的变号!
	mem_para_ap.sign[0]=-60;
	mem_para_ap.sign[1]=0;
	mem_para_ap.sign[2]=60;
	// 垂直变化
	mem_para_ap.gdz[0]=-9000;
	mem_para_ap.gdz[1]=-150;
	mem_para_ap.gdz[2]=0;
	

	return GS_OK;
}
GHRESULT QualityControl_SA :: UnInitialize()
{
	return GS_OK;
}

GHRESULT QualityControl_SA::LoadData (void *pValue,ReadParam * ReadParameter)
{
	if (pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	if ( ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pGenerateProductList = (GXList< GenerationData > *)pValue;//取得输入数据列表
	m_pParameter = ReadParameter;	//取得参数函数指针

	return GS_OK;
}

GHRESULT QualityControl_SA ::OutPutData(void *pValue)
{
	if (pValue == NULL)
	{
		return GE_INVALIDARG;
	}
	m_pOutputProductList = (GXList<GenerationData>*)pValue;
	return GS_OK;
}


GHRESULT QualityControl_SA ::Execute()
{
	/*ofstream logfile;*/
	logfile.open("./Log/QualityControl_SA.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	//------------------------------------------------------------------------------------------
	gInt16 iAzimuthNumber=0,iReflectGates[3],iDopplerGates[3];

	StandardRadarData * pInData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData>()));

	m_pOutputProductList->AddTail();
	StandardRadarData * pOutData = &(*(m_pOutputProductList->GetTailAs<StandardRadarData>()));

	GHRESULT gResult;
	////////////申请处理后基数据的空间并写入头结构体信息
	if ((gResult =writeHead(pInData,pOutData)) != GS_OK)
	{
		return gResult;
	}

	/////////下面是对所有层进行去噪声和条形回波处理
	float NOISE_FILTER_THRESHOLD =0.3; //可调参数，0.35~0.45

	///////////////////////////////////////
	gInt16 **REF = NULL;
	gInt16 **VEL = NULL;
	gInt16 ** SPW = NULL;

	if(pInData->GetElevationHeadP(0)->AzimuthNumber ==512)
		strcpy(RadarType,"cc");

	else 
		strcpy(RadarType,"SA");

	logfile<<"sa：  before noise_remove and sun_beam_remove: "<<endl;

	iAzimuthNumber = pInData->GetElevationHeadP(0)->AzimuthNumber;
	REF = new gInt16*[pInData->GetElevationHeadP(0)->AzimuthNumber];
	VEL = new gInt16*[pInData->GetElevationHeadP(0)->AzimuthNumber];
	SPW = new gInt16*[pInData->GetElevationHeadP(0)->AzimuthNumber];
	for(int i=0; i< pInData->GetElevationHeadP(0)->AzimuthNumber; i++)
	{
		REF[i] = new gInt16[pInData->GetElevationHeadP(0)->ReflectivityGates];
		VEL[i] = new gInt16[pInData->GetElevationHeadP(0)->DopplerGates];
		SPW[i] = new gInt16[pInData->GetElevationHeadP(0)->DopplerGates];

	}

	for( int k =0; k< pInData->GetHeadP()->ElevationCount;k++)
	{
		logfile<<"sa：  come into for 1: "<<k<<endl;
		for(int i=0; i< pInData->GetElevationHeadP(0)->AzimuthNumber; i++)
		{
			for(int j =0;j<pInData->GetElevationHeadP(k)->ReflectivityGates;j++)
				REF[i][j] = pInData->GetRadarDataR(k,i)[j];

			for(int j =0;j<pInData->GetElevationHeadP(k)->DopplerGates;j++)
			{
				VEL[i][j] = pInData->GetRadarDataV(k,i)[j];
				SPW[i][j] = pInData->GetRadarDataW(k,i)[j];
			}
		}
		logfile<<"sa：  before noise_remove: "<<k<<endl;
		noise_remove(REF,VEL, SPW, iAzimuthNumber,pInData->GetElevationHeadP(k)->ReflectivityGates, 0, pInData->GetElevationHeadP(k)->ReflectivityGates, NOISE_FILTER_THRESHOLD,pInData->GetHeadP()->ReflectivityGateSize,100);

		logfile<<"sa：  before sun_beam_remove: "<<k<<endl;
		sun_beam_remove(REF,VEL, SPW,iAzimuthNumber, pInData->GetElevationHeadP(k)->ReflectivityGates,pInData->GetElevationHeadP(k)->DopplerGates);
		
		logfile<<"sa：  after sun_beam_remove: "<<k<<endl;
		writedata(pInData,pOutData,k,REF,VEL,SPW);
		
	}
	logfile<<"sa：  finished noise_remove and sun_beam_remove : "<<endl;
	///////////////////////下面是去地物杂波处理部分
	gInt16 ** ref[3];
	gInt16 ** vel[3];
	gInt16 ** spw[3];
	iAzimuthNumber = pOutData->GetElevationHeadP(0)->AzimuthNumber;
	for(int k=0;k<3;k++)
	{
		iReflectGates[k] = pOutData->GetElevationHeadP(k)->ReflectivityGates;
		iDopplerGates[k] = pOutData->GetElevationHeadP(k)->DopplerGates;

		ref[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		vel[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		spw[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		for (int i=0;i<iAzimuthNumber;i++)
		{
			ref[k][i]=new(::std::nothrow) gInt16[iReflectGates[k]];
			vel[k][i]=new(::std::nothrow) gInt16[iDopplerGates[k]];
			spw[k][i]=new(::std::nothrow) gInt16[iDopplerGates[k]];
		}
		for (int i=0;i<iAzimuthNumber;i++)
		{
			for (int j=0;j<iReflectGates[k];j++)
			{
				if(pOutData->GetRadarDataR(k,i)[j]> RADAR_DATA_OUTOFRANGE )
				
					ref[k][i][j]=pOutData->GetRadarDataR(k,i)[j]/tempscale;
				else 
					ref[k][i][j] = RADAR_DATA_NOECHO;
			}
			for (int j=0;j<iDopplerGates[k];j++)
			{
				if(pOutData->GetRadarDataV(k,i)[j] > RADAR_DATA_OUTOFRANGE )
					vel[k][i][j]=pOutData->GetRadarDataV(k,i)[j]/tempscale;
				else 
					vel[k][i][j] = RADAR_DATA_NOECHO;

				if(pOutData->GetRadarDataW(k,i)[j]> RADAR_DATA_OUTOFRANGE )
				
					spw[k][i][j]=pOutData->GetRadarDataW(k,i)[j]/tempscale;
				else 
					spw[k][i][j] = RADAR_DATA_NOECHO;;
			
			}
		}
	}
	logfile<<"sa：  start ap()  : "<<endl;
	for(int k=0;k<2;k++)
	{
		//if(isa.eq.1) then call ap(STRCT_REF(i),STRCT_REF(i+1),STRCT_VEL(i),STRCT_SPW(i))
		logfile<<"sa：  before ap()  : "<< k<<endl;
		ap(pOutData,k,ref[k],ref[k+1],vel[k],spw[k]);
		logfile<<"sa：  after  ap()  : "<< k<<endl;
	}
	logfile<<"sa：  finished ap()  : "<<endl;
	//!!!!!!!!!!!!!!!!填补回波！！！！
	//open(file='a.dat')
	//read a1,a2,a3,b1,b2,b3,c1,c2,c3,d1,d2,d3,e1,e2,e3,f1,f2,f3,g1,g2,g3,h1,h2,h3,i1,j1,j2,a4;

	//for(int k=1;k>=0;k--)
	//{
	//	for(int i=0;i<iAzimuthNumber;i++)
	//	{
	//		for(int j=0;j<iReflectGates[k];j++)
	//		{
	//			if(j< iReflectGates[k+1])
	//			{
	//				if (ref[k+1][i][j]>200 && ref[k][i][j]==RADAR_DATA_NOECHO)//radar_noehco?)
	//				{
	//					ref[k][i][j]=pInData->GetRadarDataR(k,i)[j]/tempscale;
	//				}
	//			}
	//			else
	//				ref[k][i][j]=pInData->GetRadarDataR(k,i)[j]/tempscale;
	//		}
	//	}
	//}

	gResult=writedata(pInData,pOutData,ref,vel,spw);

////////////////////再一次对数据进行去噪声处理（所有层）
	for( int k =0; k< pOutData->GetHeadP()->ElevationCount;k++)
	{
		for(int i=0; i< pOutData->GetElevationHeadP(0)->AzimuthNumber; i++)
		{
			for(int j =0;j<pOutData->GetElevationHeadP(k)->ReflectivityGates;j++)
				REF[i][j] = pOutData->GetRadarDataR(k,i)[j];

			for(int j =0;j<pOutData->GetElevationHeadP(k)->DopplerGates;j++)
			{
				VEL[i][j] = pOutData->GetRadarDataV(k,i)[j];
				SPW[i][j] = pOutData->GetRadarDataW(k,i)[j];
			}
		}

		logfile<<"sa：  between noise_remove: "<<k<<endl;

		noise_remove(REF,VEL, SPW, iAzimuthNumber,pInData->GetElevationHeadP(k)->ReflectivityGates, 0, pInData->GetElevationHeadP(k)->ReflectivityGates, NOISE_FILTER_THRESHOLD,pInData->GetHeadP()->ReflectivityGateSize,100);
		writedata(pInData,pOutData,k,REF,VEL,SPW);

	}

	//noise_filter(pOutData);//RADAR_DATA_OUTOFRANGE);

	for(int i =0; i <pOutData->GetElevationHeadP(0)->AzimuthNumber;i++ )
	{
		delete []REF[i];
		delete []VEL[i];
		delete []SPW[i];
	}
	delete []REF;
	delete []VEL;
	delete []SPW;


	for (int k=0;k<3;k++)
	{
		for (int i=0;i<iAzimuthNumber;i++)
		{
			delete [] ref[k][i];
			delete [] vel[k][i];
			delete [] spw[k][i];
		}
		delete [] ref[k];
		delete [] vel[k];
		delete [] spw[k];
	}

	return gResult;
}
GHRESULT  QualityControl_SA ::writeHead(StandardRadarData* pInData,StandardRadarData* pOutData)
{
	GRADARDATAHEADER * pOutHead = new GRADARDATAHEADER;
	GRADARDATAHEADER * pInHead = pInData->GetHeadP();

	pOutHead->ElevationCount =pInHead->ElevationCount;
	//memcpy(pOutHead->IndexOfElevationData, pInHead->IndexOfElevationData, 20*sizeof(gInt32));
	pOutHead->RadarHeight = pInHead->RadarHeight;
	pOutHead->RadarLat = pInHead->RadarLat;
	pOutHead->RadarLon = pInHead->RadarLon;
	memcpy(pOutHead->ScanMode, pInHead->ScanMode, 2*sizeof(gInt8));
	memcpy(pOutHead->SiteCode, pInHead->SiteCode, 8*sizeof(gInt8));
	memcpy(pOutHead->RadarType, pInHead->RadarType, 4*sizeof(gInt8));
	memcpy(pOutHead->OperationMode, pInHead->OperationMode, 2*sizeof(gInt8));
	pOutHead->VCPTime = pInHead->VCPTime;
	pOutHead->VCPDate = pInHead->VCPDate;
	memcpy(pOutHead->VCPMode, pInHead->VCPMode, 4*sizeof(gInt8));
	pOutHead->SectorCode = pInHead->SectorCode;
	pOutHead->SizeOfSector = pInHead->SizeOfSector;
	pOutHead->ReflectivityGateSize = pInHead->ReflectivityGateSize;
	pOutHead->DopplerGateSize = pInHead->DopplerGateSize;
	pOutHead->ConstantOfSystemCorrect = pInHead->ConstantOfSystemCorrect;

	GHRESULT gResult;
	gInt16 iElevationCount = pInData->GetHeadP()->ElevationCount;
	GELEVATIONDATAHEAD * pOutEleHead = new GELEVATIONDATAHEAD[iElevationCount];
	if (pOutEleHead==NULL)
	{
		delete pOutHead;
		return GS_FALSE;
	}
	for(gInt16 iEleIndex = 0;iEleIndex < iElevationCount; iEleIndex++ )
	{
		GELEVATIONDATAHEAD * pInEleHead=pInData->GetElevationHeadP(iEleIndex);

		pOutEleHead[iEleIndex].SNOfElevationData =iEleIndex+1;
		pOutEleHead[iEleIndex].UnambiguousRange = pInEleHead->UnambiguousRange;
		pOutEleHead[iEleIndex].NyquistVelocity = pInEleHead->NyquistVelocity;
		pOutEleHead[iEleIndex].ReflectivityResolution = pInEleHead->ReflectivityResolution;
		pOutEleHead[iEleIndex].VelocityResolution = pInEleHead->VelocityResolution;
		pOutEleHead[iEleIndex].SpectrumWidthOfVelocityResolution = pInEleHead->SpectrumWidthOfVelocityResolution;
		pOutEleHead[iEleIndex].FirstGateRangeOfRef = pInEleHead->FirstGateRangeOfRef;
		pOutEleHead[iEleIndex].ReflectivityGates = pInEleHead->ReflectivityGates;
		pOutEleHead[iEleIndex].FirstGateRangeOfDoppler = pInEleHead->FirstGateRangeOfDoppler;
		pOutEleHead[iEleIndex].DopplerGates = pInEleHead->DopplerGates;
		pOutEleHead[iEleIndex].Elevation = pInEleHead ->Elevation;
		pOutEleHead[iEleIndex].AzimuthNumber = pInEleHead->AzimuthNumber;
	}
	if ((gResult = pOutData-> WriteHeadAndElevationHead( pOutHead, pOutEleHead,iElevationCount))!=GS_OK)
	{
		delete pOutHead;
		delete []pOutEleHead;
		pOutEleHead = NULL;
		pOutHead = NULL;
		return gResult;
	}

	delete pOutHead;
	delete []pOutEleHead;
	pOutEleHead = NULL;
	pOutHead = NULL;

	return GS_OK;
}
GHRESULT  QualityControl_SA ::writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 layerIndex,gInt16 ** ref,gInt16**  vel, gInt16** spw)
{
	gInt16 iAziCount = pOutData->GetElevationHeadP(layerIndex)->AzimuthNumber;
	gInt16 iRefGates =  pOutData->GetElevationHeadP(layerIndex)->ReflectivityGates;
	gInt16 iDopGates =  pOutData->GetElevationHeadP(layerIndex)->DopplerGates;
	for(gInt16 iAziIndex=0;iAziIndex<iAziCount;iAziIndex++)
	{
		RVWDATAHEAD * pInRVWHead = pInData->GetRadarDataP(layerIndex,iAziIndex);
		RVWDATAHEAD * pOutRVWHead = pOutData->GetRadarDataP(layerIndex,iAziIndex);
		//写该RVW块数据头
		pOutRVWHead->SNOfRadialData = iAziIndex+1;
		pOutRVWHead->Elevation = pInRVWHead->Elevation;          //单位0.01度
		pOutRVWHead->Azimuth =  (gInt16)iAziIndex*100;  //单位0.01度
		pOutRVWHead->Date = pInRVWHead->Date;
		pOutRVWHead->Time = pInRVWHead->Time;

		gInt16 * pOutRData = pOutData->GetRadarDataR(layerIndex,iAziIndex);
		gInt16 * pOutVData = pOutData->GetRadarDataV(layerIndex,iAziIndex);
		gInt16 * pOutWData = pOutData->GetRadarDataW(layerIndex,iAziIndex);

		for(gInt16 i=0;i<iRefGates;i++)
	
			pOutRData[i]=ref[iAziIndex][i];

		for(gInt16 i=0;i<iDopGates;i++)
		{
			pOutVData[i]=vel[iAziIndex][i];

			pOutWData[i]=spw[iAziIndex][i];
		}	
	}
	return GS_OK;
}
GHRESULT  QualityControl_SA ::writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 ** ref[],gInt16**  vel[], gInt16** spw[])
{
	GRADARDATAHEADER * pOutHead = new GRADARDATAHEADER;
	GRADARDATAHEADER * pInHead = pInData->GetHeadP();
	for(gInt16 iEleIndex = 0;iEleIndex < 3; iEleIndex++ )
	{
		gInt16 iAziCount = pOutData->GetElevationHeadP(iEleIndex)->AzimuthNumber;
		gInt16 iRefGates =  pOutData->GetElevationHeadP(iEleIndex)->ReflectivityGates;
		gInt16 iDopGates =  pOutData->GetElevationHeadP(iEleIndex)->DopplerGates;
		for(gInt16 iAziIndex=0;iAziIndex<iAziCount;iAziIndex++)
		{
			RVWDATAHEAD * pInRVWHead = pInData->GetRadarDataP(iEleIndex,iAziIndex);
			RVWDATAHEAD * pOutRVWHead = pOutData->GetRadarDataP(iEleIndex,iAziIndex);
			//写该RVW块数据头
			pOutRVWHead->SNOfRadialData = iAziIndex+1;
			pOutRVWHead->Elevation = pInRVWHead->Elevation;          //单位0.01度
			pOutRVWHead->Azimuth =  (gInt16)iAziIndex*100;  //单位0.01度
			pOutRVWHead->Date = pInRVWHead->Date;
			pOutRVWHead->Time = pInRVWHead->Time;

			gInt16 * pOutRData = pOutData->GetRadarDataR(iEleIndex,iAziIndex);
			gInt16 * pInRData = pInData->GetRadarDataR(iEleIndex,iAziIndex);
			gInt16 * pOutVData = pOutData->GetRadarDataV(iEleIndex,iAziIndex);
			gInt16 * pInVData = pInData->GetRadarDataV(iEleIndex,iAziIndex);
			gInt16 * pOutWData = pOutData->GetRadarDataW(iEleIndex,iAziIndex);
			gInt16 * pInWData = pInData->GetRadarDataW(iEleIndex,iAziIndex);

		
				for(gInt16 i=0;i<iRefGates;i++)
				{
					if(ref[iEleIndex][iAziIndex][i]>RADAR_DATA_OUTOFRANGE)
					
						pOutRData[i]=ref[iEleIndex][iAziIndex][i]*tempscale;
					else 
						pOutRData[i] = RADAR_DATA_NOECHO;


				}
				for(gInt16 i=0;i<iDopGates;i++)
				{
					if(vel[iEleIndex][iAziIndex][i]>RADAR_DATA_OUTOFRANGE)
	
						pOutVData[i]=vel[iEleIndex][iAziIndex][i]*tempscale;
					else 
						pOutVData[i] = RADAR_DATA_NOECHO;

					if(spw[iEleIndex][iAziIndex][i]>RADAR_DATA_OUTOFRANGE)
					
						pOutWData[i]=spw[iEleIndex][iAziIndex][i]*tempscale;
					else 
						pOutWData[i] = RADAR_DATA_NOECHO;
				}	
		}
	}
	delete pOutHead;
	return GS_OK;
}

void QualityControl_SA ::median(gInt16** pData,gInt16 nr,gInt16 np,gInt16** med_fld)
{	//Purpose: Median filter along a radar beam
	//median(ref  ,iRefGates,iAziCount,0     ,iRefGates,mdz1);
	gInt16 l_half = 4 ,ic;

	gInt16* a = new(std::nothrow) gInt16[9];

	for (gInt16 j=0;j<np;j++)
	{
		for (gInt16 i=0;i<nr;i++)
		{
			med_fld[j][i]=RADAR_DATA_NOECHO;
		}
	}

	for (gInt16 j=0;j<np;j++)
	{
		for (gInt16 i=l_half;i<nr-l_half;i++)//gInt16 i=iminrg+l_half;i<nrang-l_half;i++
		{
			if(pData[j][i]>RADAR_DATA_OUTOFRANGE)
			{
				ic=0;
				for(gInt16 m=i-l_half;m<=i+l_half;m++)
				{
					a[ic]=pData[j][m];
					ic++;
				}

				for (gInt16 jj=1;jj<9;jj++)
				{
					for (gInt16 ii=jj-1;ii>=0;ii--)
					{
						if(a[ii]<= a[jj])
						{
							a[ii+1]=a[jj];
							break;
						}
						else
						{
							a[ii+1]=a[ii];
						}
					}
				}

				med_fld[j][i]=a[4];
			}
		}
	}

	delete []a;
	a=NULL;

	return ;
}


void QualityControl_SA ::sdt_dev_ve(gInt16** pVData,gInt16** fld_mdv,gInt16 nr,gInt16 np,gInt16 iminrg,gInt16 mgate,gInt16 mazim,gInt16 numthresh,gInt16** sdevv)
{	// calculate the standard deviation of velocity and SW for AP detecting 求速度标准偏差值/平均方差
	//sdt_dev_ve(vel,mdve,iDopGates,iAziCount,iminrgve=mgateve=9,nrang=iDopGates,mgate=9,mazim=5,numthresh=(mgateve*mazimve)/3.0=15,sdevv);
	gInt16 mgate_hf,mazim_hf;
	gInt16 i,j,m,ma,mr;
	gFloat ssv,avg;
	gInt16 numv;
	//!c-------------------------------------------------------
	mgate_hf=(mgate-1)/2;//4
	mazim_hf=(mazim-1)/2;//2
	if(iminrg-mgate_hf<0||iminrg+mgate_hf>=np)
	{
		return ;
	}

	for (j=0;j<np;j++)
	{
		for (i=iminrg;i<nr;i++)
		{
			sdevv[j][i]=RADAR_DATA_NOECHO;
		}
	}

	for (j=0;j<np;j++)
	{
		for (i=iminrg;i<nr;i++)//10-iDopGates
		{
			
			if (fld_mdv[j][i]>RADAR_DATA_OUTOFRANGE)
			{
				ssv=0;
				avg=0;
				numv=0;
				for (ma=j-mazim_hf;ma<=j+mazim_hf;ma++)//<
				{
					m=ma;
					if(ma<0)
						m=np+ma;
					if(ma>=np)
						m=ma-np;
					for (mr=i-mgate_hf;mr<=i+mgate_hf;mr++)//<
					{
						if(mr>=nr)
							break;
						if(pVData[m][mr]>RADAR_DATA_OUTOFRANGE)
						{
							numv++;
							ssv=ssv+gFloat(pVData[m][mr]-fld_mdv[j][i])*gFloat(pVData[m][mr]-fld_mdv[j][i]);
						}
					}
				}
				if(numv>numthresh)
				{
					sdevv[j][i]=gInt16(sqrt(ssv/(numv-1)) +0.5);
				}
			}
		}
	}

	return ;
}
void QualityControl_SA ::dbzpara(gInt16** pData,gInt16 nr,gInt16 np,gInt16 iminrg,gInt32 mgate,gInt32 mazim,gInt32 dbzthresh,gInt32 numthresh,gFloat** tdbz,gInt16** sign,gInt16** spin)
{	//Purpose: calculate the TDBZ,SIGN,SIGN used in AP detecting
	//dbzpara(ref,iDopGates,iAziCount,iminrgref=1+mgateref=6,iRefGates,mgateref=5,mazimref=5,dbzthresh=460,numthreshref=numthreshref=(mgateref*mazimref)/3.0=8,tdbz,sign,spin);
	//Input:iminrg, nrang : Calculating Domain of range
	//Input: mgate,mazim  : Averaging grid number in radial and azm direction
	//output: Tdbz(nr,np),spin(nr,np),signn(nr,np): the value at echo grid
	gInt16 mgate_hf,mazim_hf;
	gInt16 sspin;
	gFloat ttdbz,diff;
	gInt16 i,j,m,ma,mr,num;
	gFloat nsign;

	mgate_hf=(mgate-1)/2;//2
	mazim_hf=(mazim-1)/2;//2
	if(iminrg-mgate_hf<0||iminrg+mgate_hf>=np)
	{
		return ;
	}

	for (j=0;j<np;j++)
	{
		for (i=iminrg;i<nr;i++)//do i=iminrg,nrang
		{
			nsign=0;
			ttdbz=0;
			sspin=0;
			num=0;
			tdbz[j][i]=RADAR_DATA_NOECHO;
			sign[j][i]=RADAR_DATA_NOECHO;
			spin[j][i]=RADAR_DATA_NOECHO;
			if (pData[j][i]>RADAR_DATA_OUTOFRANGE)
			{
				for (ma=j-mazim_hf;ma<=j+mazim_hf;ma++)//<=
				{
					m=ma;
					if (ma<0)
					{
						m=np+ma;
					}
					if (ma>=np)
					{
						m=ma-np;
					}
					for(mr=i-mgate_hf;mr<=i+mgate_hf;mr++)//gate  <=
					{
						if (mr>=nr-1)
							break;
						if (pData[m][mr+1]>RADAR_DATA_OUTOFRANGE && pData[m][mr]>RADAR_DATA_OUTOFRANGE)
						{
							num++;
							diff=pData[m][mr+1]-pData[m][mr];
							ttdbz+=gFloat(diff*diff);
							if (fabs(diff)>dbzthresh)
							{
								sspin++;
							}
							if (diff>0)
							{
								nsign++;
							}
							if (diff<0)
							{
								nsign--;
							}
						}
					}
				}
				if (num>numthresh)
				{
					sign[j][i]=100*nsign/num;
					tdbz[j][i]=ttdbz/num/100;
					spin[j][i]=100*sspin/num;
				}
			}
		}
	}

	return ;
}
void QualityControl_SA ::ttve(gInt16** pData,gFloat** tve,gInt16 nr,gInt16 np,gInt16 iminrg,gInt16 mgate,gInt16 mazim,gInt16 numthresh)
{	//Purpose: calculate the Tve used in AP detecting
	
	gInt16 mgate_hf,mazim_hf;
	gInt16 i,j,ma,mr,num,m;
	gFloat nsign,ttdbz,sspin,diff;

	mgate_hf=(mgate-1)/2;//4
	mazim_hf=(mazim-1)/2;//2
	if(iminrg-mgate_hf<0 || iminrg+mgate_hf>=np)
	{
		return ;
	}
	for (j=0;j<np;j++)
	{
		for (i=0;i<nr;i++)//10-iDopGates
		{
			tve[j][i]=RADAR_DATA_NOECHO;
		}
	}

	for (j=0;j<np;j++)
	{
		for (i=iminrg;i<nr;i++)//10-iDopGates
		{
			nsign=0;
			ttdbz=0;
			sspin=0;
			num=0;
			if (pData[j][i]>RADAR_DATA_OUTOFRANGE)
			{
				for (ma=j-mazim_hf;ma<=j+mazim_hf;ma++)//<=?
				{
					m=ma;
					if (ma<0)
					{
						m=np+ma;
					}
					if (ma>=np)
					{
						m=ma-np;
					}
					for(mr=i-mgate_hf;mr<=i+mgate_hf;mr++)//<=
					{
						if (mr>=nr-1)
							break;
						if (pData[m][mr+1]>RADAR_DATA_OUTOFRANGE && pData[m][mr]>RADAR_DATA_OUTOFRANGE)
						{
							num++;
							diff=pData[m][mr+1]-pData[m][mr];
							ttdbz=ttdbz+(diff*diff);
						}
					}
				}
				if (num>numthresh)
				{
					tve[j][i]=ttdbz/num;
				}
			}
		}
	}

	return ;
}



gInt32 QualityControl_SA ::mfun(gInt32 para[],gInt16 x)
{
	gInt32 y;
	if(x<=para[0]|| x>=para[2])
	{
		y=0;
	}
	else if(x>para[0] && x<=para[1])
	{
		y=100*(x-para[0])/(para[1]-para[0]);
	}
	else
	{
		y=100*(para[2]-x)/(para[2]-para[1]);
	}

	return y;
}


gInt32 QualityControl_SA ::mfun_r(gInt32 para[],gFloat x)
{
	//mfun_r(mem_para_ap.tdbz,tdbz[i][j],mbsh_s);

	//!input para: member function parameter
	//!input X   : input value
	//!output y  : value of member function

	gInt32 y;
	if(x<=para[0] || x >= para[2])
	{
		y=0;
	}
	else if(x>para[0]&& x<=para[1])
	{
		y=100*(x-para[0])/(para[1]-para[0]);
	}
	else
	{
		y=100*(para[2]-x)/(para[2]-para[1]);
	}

	return y;
}

void QualityControl_SA ::ap(StandardRadarData* pInData,gInt16 iElevIndex, gInt16 ** ref,gInt16 ** ref2,gInt16 ** vel, gInt16 ** spw)
{
	gFloat dazim,thresh_ap;
	gInt32 i2,ir,iv,jj,mgateref,mazimref,numthreshref;
	gInt32 m0,n0,mm,nn;
	gInt16 mgateve,mazimve,iminrgve,numthreshve,iminrgref;

	gInt16 iRefGates = pInData->GetElevationHeadP(iElevIndex)->ReflectivityGates;
	gFloat a1 = 45,a2 = 55 ,a3 = iRefGates;
	//b1=-23,b2=0,b3=23,c1=-1000,c2=0,c3=32,d1=-1000,d2=0,d3=10,e1=0,e2=45,e3=10000,f1=0,f2=50,f3=100,g1=-60,g2=0,g3=60,h1=-90000,h2=-150,h3=0
	//nptve=9;
	mgateref=5;
	mazimref=5;
	mgateve=9;
	mazimve=5;

	numthreshref=(mgateref*mazimref)/3.0;//8.33333
	numthreshve=(mgateve*mazimve)/3.0;//15
	iminrgref=mgateref;//1+mgateref//9
	//	nrangref=230;
	iminrgve=1+mgateve;//1+mgateve;
	//	nrangve=ng-mgateve;

	gInt32 dbzthresh=460,ddbz,ddmax,mbsh_s;

	//spval=  VALUE_INVALID;
	//clutter= -9992;
	//clearair=-9993;
	//seaclutter=-9994;


	gInt16 id,jd;

	gFloat r21,ifact;

	//!********parameter for AP******************
	//nptve: gate number in  the mediam smooth
	//mgateref and mazimref,mgateve,mazimve: the average number in range and azmith for calculating the TDBZ, SPIN, SIGN and SDEV.
	//dbzthresh : the thresh of ref for spin
	dazim=1.0;
	gInt16 iAziCount = pInData->GetElevationHeadP(iElevIndex)->AzimuthNumber;
	/*gInt16 iRefGates = pInData->GetElevationHeadP(iElevIndex)->ReflectivityGates;*/
	gInt16 iRefGates2 = pInData->GetElevationHeadP(iElevIndex+1)->ReflectivityGates;
	gInt16 iDopGates = pInData->GetElevationHeadP(iElevIndex)->DopplerGates;

	gInt16 ** mdsw= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** mdve= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** sdevv= new(::std::nothrow) gInt16*[iAziCount];
	gFloat ** tve= new(::std::nothrow) gFloat*[iAziCount];

	gInt16 ** mdz1= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** mdz2= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** spin= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** sign= new(::std::nothrow) gInt16*[iAziCount];
	gFloat ** tdbz= new(::std::nothrow) gFloat*[iAziCount];

	gInt16 ** mbsh_num_ve= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** gdzz= new(::std::nothrow) gInt16*[iAziCount];
	gInt16 ** mbsh_out= new(::std::nothrow) gInt16*[iAziCount];//

	for (int i=0;i<iAziCount;i++)
	{
		mdsw[i]=new(::std::nothrow) gInt16[iDopGates];//!
		mdve[i]=new(::std::nothrow) gInt16[iDopGates];//!
		sdevv[i]=new(::std::nothrow) gInt16[iDopGates];//!
		tve[i]=new(::std::nothrow) gFloat[iDopGates];//!
		memset(mdsw[i],0,2*iDopGates);
		memset(mdve[i],0,2*iDopGates);
		memset(sdevv[i],0,2*iDopGates);
		memset(tve[i],0,2*iDopGates);
	}

	for (int i=0;i<iAziCount;i++)
	{
		mdz1[i]=new(::std::nothrow) gInt16[iRefGates];//!
		spin[i]=new(::std::nothrow) gInt16[iRefGates];//!
		sign[i]=new(::std::nothrow) gInt16[iRefGates];//!
		tdbz[i]=new(::std::nothrow) gFloat[iRefGates];//!
		mbsh_num_ve[i]=new(::std::nothrow) gInt16[iRefGates];//!
		gdzz[i]=new(::std::nothrow) gInt16[iRefGates];
		memset(mdz1[i],0,2*iRefGates);
		memset(spin[i],0,2*iRefGates);
		memset(sign[i],0,2*iRefGates);
		memset(tdbz[i],0,2*iRefGates);
		memset(mbsh_num_ve[i],0,2*iRefGates);
		memset(gdzz[i],0,2*iRefGates);
	}
	for (int i=0;i<iAziCount;i++)
	{
		mdz2[i]=new(::std::nothrow) gInt16[iRefGates2];//!
		memset(mdz2[i],0,2*iRefGates2);
	}

	for (int i=0;i<iAziCount;i++)
	{
		mbsh_out[i]=new(::std::nothrow) gInt16[iRefGates];
		memset(mbsh_out[i],0,2*iRefGates);
	}

	//! calculate of feature fields
	//! median of spw,vel and ref   mdsw
	logfile<<"sa：  before median()  : "<< iElevIndex<<endl;
	median(spw,iDopGates,iAziCount,mdsw);//iDopGates  median2

	median(vel,iDopGates,iAziCount,mdve);//iDopGates median2

	median(ref,iRefGates,iAziCount,mdz1);

	median(ref2,iRefGates2,iAziCount,mdz2);
	logfile<<"sa：  before sdt_dev_ve()  : "<< iElevIndex<<endl;

	sdt_dev_ve(vel,mdve,iDopGates,iAziCount,iminrgve,mgateve,mazimve,numthreshve,sdevv);

	logfile<<"sa：  before dbzpara()  : "<< iElevIndex<<endl;
	dbzpara(ref,iRefGates,iAziCount,iminrgref,mgateref,mazimref,dbzthresh,numthreshref,tdbz,sign,spin);
	logfile<<"sa：  before ttve()  : "<< iElevIndex<<endl;
	ttve(vel,tve,iDopGates,iAziCount,iminrgve,mgateve,mazimve,numthreshve);

	//! calculate of gdz
	r21=cos(0.5*3.14/180.0)/cos(1.5*3.14/180.0);
	logfile<<"sa：  before  caculate  ---------gdzz()  : "<< iElevIndex<<endl;
	for(int j=0;j<iAziCount;j++)
	{
		for(int i=1;i<a3;i++)//! do i=2,iRefGates  //do i=2,a3
		{
			i2=int(i*r21+0.5);
			gdzz[j][i]=RADAR_DATA_NOECHO;
			if(i2 >=iRefGates2)
				continue;

			if(ref[j][i]>RADAR_DATA_OUTOFRANGE)
			{
				if(ref2[j][i2]>RADAR_DATA_OUTOFRANGE)
				{
					ddmax=-1000 ;
					for( m0=-4;m0<=4;m0++) //m0=-4,4
					{
						for( n0=-2;n0<=2;n0++) //n0=-2,2
						{
							mm=i2+m0;
							nn=n0+j;
							if(mm>=0 && mm<iRefGates && mm<iRefGates2 && nn >= 0 && nn < iAziCount) //if(mm>=1&&mm<ngnum_ref && nn>=1&&nn<=npnum)
							{
								ddbz=mdz2[nn][mm]-mdz1[j][i];
								if(ddbz > ddmax)
									ddmax=ddbz;
							}
						}
					}
					ddbz=ddmax;
				}
				else
				{
					ddbz=10-ref[j][i];
				}
				if (i <= 160)
				{
					ifact=1;
				}
				else
				{
					ifact=(300.0-i/4.0)/(300.0-40.0);
				}
				ddbz=ddbz*ifact;
				gdzz[j][i]=ddbz;
			}
		}
	}
	logfile<<"sa：  before  caculate  ---------mbsh_out()  : "<< iElevIndex<<endl;
	//!ground clutter distinguish
	for(int j=0;j<iAziCount;j++)// j=1,iAziCount
	{
		for(int i=1;i<a3-1;i++)//do i=2,a3
		{
			iv=i*4;
			mbsh_num_ve[j][i]=0;
			mbsh_out[j][i]=0.0;
			if(ref[j][i]>-50)
			{
				if(iv< iDopGates)
				{
					if(sdevv[j][iv] != RADAR_DATA_NOECHO)//
					{
						mbsh_s=mfun(mem_para_ap.sdve,sdevv[j][iv]);
						mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
						mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
					}
					if(mdve[j][iv] != RADAR_DATA_NOECHO)//
					{
						mbsh_s=mfun(mem_para_ap.mdve,mdve[j][iv]);
						mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
						mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
					}
					if(mdsw[j][iv]  != RADAR_DATA_NOECHO)//
					{
						mbsh_s=mfun(mem_para_ap.mdsw,mdsw[j][iv]);
						mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
						mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
					}
				}

				if(gdzz[j][i] != RADAR_DATA_NOECHO)//!=
				{
					mbsh_s=mfun(mem_para_ap.gdz,gdzz[j][i]);
					mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
					mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
				}
				if(tdbz[j][i] != RADAR_DATA_NOECHO)//!=
				{
					mbsh_s=mfun_r(mem_para_ap.tdbz,tdbz[j][i]);
					mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
					mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
				}
				if(spin[j][i] != RADAR_DATA_NOECHO)//!=
				{
					mbsh_s=mfun(mem_para_ap.spin,spin[j][i]);
					mbsh_out[j][i]=mbsh_out[j][i]+mbsh_s;
					mbsh_num_ve[j][i]=mbsh_num_ve[j][i]+1;
				}

				if(mbsh_num_ve[j][i] >=1)
				{
					mbsh_out[j][i]=mbsh_out[j][i]/mbsh_num_ve[j][i];
				}
				else
				{
					mbsh_out[j][i]=0;
				}
			}
			//printf("mbsh_out[j][i] =%d\n",mbsh_out[j][i]);
		}
	}
	logfile<<"sa：  before  clear  ap air : "<< iElevIndex<<endl;
//	char *pTemp1=NULL;
	
//	(*m_pParameter)("THRESH_AP",pTemp1);
//	if(pTemp1 == NULL)
//		return ;
//	 int thresh_app = 0;
//	 thresh_app = atoi(pTemp1);
    int thresh_app = 45 ; 
	for(int j=0;j<iAziCount;j++)//do j=1,iAziCount
	{
		for(int i=1;i<a3-2;i++) //do i=2,a3
		{
			iv=i*4;
			thresh_ap=thresh_app;
			
		
			if(iv < iDopGates -2)
			{
				if(abs(vel[j][iv])> 15 && abs(vel[j][iv])<200 )
				thresh_ap=55; //!!!!!thresh_ap=55  !!!!
			}
			
			if(mbsh_out[j][i]>thresh_ap)
			{
				for(jd=-1;jd<1;jd++)//<=
				{
					jj=j+jd;
					if(jj>=iAziCount)
						jj=0;
					if(jj<0)
						jj=iAziCount-1;
					for(id=-1;id<1;id++)//<=
					{
						if (ref[jj][i+id]>RADAR_DATA_OUTOFRANGE)
						{
							ref[jj][i+id]=RADAR_DATA_NOECHO;//-9992
						}
					}
				}
				if(iv<=iDopGates-5)
				{//do jd=-1,1
					for(jd=-1;jd<=1;jd++)
						jj=j+jd;
					for(id=-4;id<=4;id++)
					{
						if(jj>=iAziCount)
							jj=0;
						if(jj<0)
							jj=iAziCount-1;
						if (vel[jj][iv+id]>RADAR_DATA_OUTOFRANGE)
						{
							vel[jj][iv+id]=RADAR_DATA_NOECHO;
						}
						if (spw[jj][iv+id]>RADAR_DATA_OUTOFRANGE)
						{
							spw[jj][iv+id]=RADAR_DATA_NOECHO;
						}
					}
				}
			}
			
		}
	}
		logfile<<"sa：  after clear ap air : "<< iElevIndex<<endl;
	for (int i=0;i<iAziCount;i++)
	{
		delete [] mdsw[i];
		delete [] mdve[i];
		delete [] sdevv[i];
		delete [] tve[i];
		mdsw[i] = NULL;
		mdve[i] = NULL;
		sdevv[i] = NULL;
		tve[i] = NULL;
	}
	for (int i=0;i<iAziCount;i++)
	{
		delete [] mdz1[i];
		delete [] spin[i];
		delete [] sign[i];
		delete [] tdbz[i];
		delete [] mbsh_num_ve[i];
		delete [] gdzz[i];
		mdz1[i] = NULL;
		spin[i] = NULL;
		sign[i] = NULL;
		tdbz[i] = NULL;
		mbsh_num_ve[i] = NULL;
		gdzz[i] = NULL;
	}

	for (int i=0;i<iAziCount;i++)
	{
		delete [] mdz2[i];
		mdz2[i] = NULL;
	}
	for (int i=0;i<iAziCount;i++)
	{
		delete [] mbsh_out[i];
		mbsh_out[i] = NULL;
	}

	delete [] mdsw;
	delete [] mdve;
	delete [] mdz1;
	delete [] mdz2;
	delete [] sdevv;
	delete [] gdzz;
	delete [] spin;
	delete [] sign;
	delete [] mbsh_num_ve;
	delete [] mbsh_out;
	delete [] tve;
	delete [] tdbz;

	mdsw = NULL;
	mdve = NULL;
	mdz1 = NULL;
	mdz2 = NULL;
	sdevv = NULL;
	gdzz = NULL;
	spin = NULL;
	sign = NULL;
	mbsh_num_ve = NULL;
	mbsh_out = NULL;
	tve = NULL;
	tdbz = NULL;

	return ;
}

void QualityControl_SA::sun_beam_remove(short **bref,short **v,short **w,	int nrays,	int ngates,int nVgates )
{
	int jp,jn,num;
	int i,j,k;

	short **echo_num = new short * [nrays];
	for(  i = 0; i < nrays; i++)
		echo_num[i] = new short [ngates];

	for(  j = 0; j < nrays; j++)
		for(  k = 0; k < ngates; k++)
		{
			echo_num[j][k] =  0;
			if( bref[j][k] < -1600)
				continue;

			jn = j - 4;      jp = j + 4;
			if( jn < 0 )     jn = jn + nrays;
			if( jp >= nrays ) jp = jp - nrays;

			if( bref[jp][k] > 0 || bref[jn][k] > 0 )
				continue;

			num = 1;
			for( i = 1; i < 4; i++)
			{
				jn = j - i;
				if( jn < 0 )  jn = jn + nrays;
				if( bref[jn][k] >  0 )
					num++;
				else break;
			}

			for( i = 1; i < 4; i++)
			{
				jp = j + i;
				if( jp >= nrays  ) jp = jp - nrays;
				if( bref[jp][k] >  0 )
					num++;
				else break;
			}
			echo_num[j][k] =  num;
		} //end of (k)

	
		for( j = 0; j < nrays; j++)
		{
				num = 0;
		
			for( k = 0; k < ngates-2; k++)
			{
				if( (echo_num[j][k] > 0 && echo_num[j][k+1] > 0) &&
					abs(echo_num[j][k] - echo_num[j][k+1] )<= 2)
				{
					if( num > 4)
					{

						bref[j][k] = RADAR_DATA_NOECHO;
						if(!strcmp(RadarType,"SA"))
						{
							if(k*4 + 3< nVgates)
							{
								v[j][k*4+3] = RADAR_DATA_NOECHO;
								w[j][k*4+3] = RADAR_DATA_NOECHO;
							}

						}
						if(!strcmp(RadarType,"cc"))
						{
							if(k< nVgates)
							{
								v[j][k] = RADAR_DATA_NOECHO;
								w[j][k] = RADAR_DATA_NOECHO;
							}

						}
						
						
					}
					else if( num == 4)
					{
						for(int i = 1; i <= 4; i++)
						{
							if(k - i < ngates && k-i>0)
								bref[j][k - i] = RADAR_DATA_NOECHO;
							bref[j][k] = RADAR_DATA_NOECHO;
							if(!strcmp(RadarType,"SA"))
							{
								if((k - i)*4 +3< nVgates)
								{
									v[j][(k - i)*4 +3] = RADAR_DATA_NOECHO;
									w[j][(k - i)*4 +3] = RADAR_DATA_NOECHO;
								}

							}
							if(!strcmp(RadarType,"cc"))
							{
								if(k-i< nVgates)
								{
									v[j][k-i] = RADAR_DATA_NOECHO;
									w[j][k-i] = RADAR_DATA_NOECHO;
								}

							}
						

						}
					}

					num++;
				}
			
				else num = 0;
			}
			
		}//end of (k)

			for( i = 0; i < nrays; i++)
				delete [] echo_num[i];

			delete [] echo_num;
}
void QualityControl_SA::noise_remove(short **bref, short **v,short **w,int nrays,   int ngates, int start, int end,  float threshold ,int gatelen,int REF_SCALE )
{
	float NOISE_FILTER_THRESHOLD = threshold; //  可调参数，0.35~0.45
	/*int REF_SCALE   = 100; */   //bref的放大倍数
	// gatelen 距离库的长度(米)

	short fthd = threshold * REF_SCALE;
	int j,k,m,jp1,jp2,jn1,jn2,idigit;

	float Stotal, Sref;

	float S0 = (float)(gatelen*0.001*gatelen*0.001);

	float *SGate = new float[ngates];
	SGate[0] = S0;
	for(k=1;k<ngates;++k)
		SGate[k] = k*k*S0-SGate[k-1];

	// store of average value of 5x5 box
	short  **tmpbuf = new short *[nrays];
	for(j=0;j<nrays;++j)
	{
		tmpbuf[j] = new short [ngates];
		memcpy(tmpbuf[j], bref[j], ngates*2);
	}

	//Loop through the 3-D array by radial/gate position.
	//accumulate total points and total effective points in
	//5 x 5 domain.

	threshold *= REF_SCALE;

	if(end > ngates) end = ngates;
	for( j = 0; j < nrays; j++)
		for( k = start; k < end; k++)
		{
			if( tmpbuf[j][k] < -1600) continue;

			Sref = 0;    Stotal = 0;

			jp1 = j + 1;      jp2 = j + 2;
			jn1 = j - 1;      jn2 = j - 2;
			if( jp1 >= nrays ) jp1 = jp1 - nrays;
			if( jp2 >= nrays ) jp2 = jp2 - nrays;
			if( jn1 < 0 )     jn1 = jn1 + nrays;
			if( jn2 < 0 )     jn2 = jn2 + nrays;

			for( m = max(k-2,0); m <= min(k+2,ngates-1); m++ )
			{
				idigit = 0;
				if(tmpbuf[jp1][m] > threshold )  ++idigit;
				if(tmpbuf[jp2][m] > threshold )  ++idigit;
				if(tmpbuf[j][m]   > threshold )  ++idigit;
				if(tmpbuf[jn1][m] > threshold )  ++idigit;
				if(tmpbuf[jn2][m] > threshold )  ++idigit;
				Stotal += SGate[m]*5;
				Sref += idigit * SGate[m];
			}

			//remove isolated dot, line echo,abnormal dot
			if(Sref/Stotal < NOISE_FILTER_THRESHOLD )
			{
				bref[j][k] = RADAR_DATA_NOECHO;
			/*	if(k*4 + 3< ngates*2)
				{
					v[j][k*4+3] = RADAR_DATA_NOECHO;
					w[j][k*4+3] = RADAR_DATA_NOECHO;
				}*/
			}
		} //end of numcols loop

		for(j=0;j<nrays;++j) delete [] tmpbuf[j];
		delete [] tmpbuf;

		delete []SGate;
		return;
}
GHRESULT DLLEXP_GSYS ExecuteArithQuality_SA( GXList< GenerationData>  *Datas, GXList< GenerationData > * pOutputList, ReadParam* pParam)
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
	QualityControl_SA *Arith = new QualityControl_SA();
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
	printf("begin QualityControl_SA Execute\n");
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
	return new QualityControl_SA;
}
