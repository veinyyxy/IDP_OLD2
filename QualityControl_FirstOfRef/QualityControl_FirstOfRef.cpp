/////////////////////////////////////////
//QualityControl_FirstOfRef.h 
//modifyed by zsc 
////////////////////////////////////////
//ʵ����QualityControl_FirstOfRef
//���������ʵĵ�һ�������ز�ȥ��
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include <iostream>
#include <fstream>

#include "../GDataTypes/CJulMsGMT.h"

#include "QualityControl_FirstOfRef.h"

QualityControl_FirstOfRef::QualityControl_FirstOfRef()
{
	;/*Initialize();*/
}

QualityControl_FirstOfRef::~QualityControl_FirstOfRef()
{
	;
}

GHRESULT QualityControl_FirstOfRef :: Initialize()
{
	m_pGenerateProductList = NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;

	return GS_OK;
}
GHRESULT QualityControl_FirstOfRef :: UnInitialize()
{
	return GS_OK;
}

GHRESULT QualityControl_FirstOfRef::LoadData (void *pValue,ReadParam * ReadParameter)
{
	if (pValue == NULL )
	{
		return GE_INVALIDARG;
	}

	m_pGenerateProductList = (GXList< GenerationData > *)pValue;//ȡ�����������б�
	m_pParameter = ReadParameter;	//ȡ�ò�������ָ��

	return GS_OK;
}

GHRESULT QualityControl_FirstOfRef ::Execute()
{
	ofstream logfile;
	logfile.open("./Log/QualityControl_FirstOfRef.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"QC_FirstOfRef��  Started: "<<endl;

	logfile<<"QC_FirstOfRef��  Send in and Read Parameter: EchoPointRateThreshold."<<endl;
	StandardRadarData * pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData>()));
	char *pTempName=NULL;
	(* m_pParameter)("EchoPointRateThreshold",pTempName);
	if (pTempName == NULL)
	{
		return GE_INVALIDARG;
	}
	int EchoPointRateThreshold = atoi(pTempName);

	logfile<<"QC_FirstOfRef��  Get Input Product and Create Output Product: StandardRadarData."<<endl;
	m_pOutputProductList->AddTail();
	StandardRadarData * pOutData = &(*(m_pOutputProductList->GetHeadAs<StandardRadarData>()));

	logfile<<"QC_FirstOfRef��  Get Head Information And Write Into Output Product."<<endl;
	GRADARDATAHEADER * pGDataHead = new GRADARDATAHEADER;
	pGDataHead->ElevationCount = pGData->GetHeadP()->ElevationCount;//gInt16(1);
	gInt16 ElevationNumber = pGDataHead->ElevationCount;
	//memcpy(pGDataHead->IndexOfElevationData, pGData->GetHeadP()->IndexOfElevationData, 20*sizeof(gInt32));
	pGDataHead->RadarHeight = pGData->GetHeadP()->RadarHeight;
	pGDataHead->RadarLat = pGData->GetHeadP()->RadarLat;
	pGDataHead->RadarLon = pGData->GetHeadP()->RadarLon;
	memcpy(pGDataHead->ScanMode, pGData->GetHeadP()->ScanMode, 2*sizeof(gInt8));
	memcpy(pGDataHead->SiteCode, pGData->GetHeadP()->SiteCode, 8*sizeof(gInt8));
	memcpy(pGDataHead->RadarType, pGData->GetHeadP()->RadarType, 4*sizeof(gInt8));
	pGDataHead->VCPTime = pGData->GetHeadP()->VCPTime;
	pGDataHead->VCPDate = pGData->GetHeadP()->VCPDate;
	memcpy(pGDataHead->VCPMode, pGData->GetHeadP()->VCPMode, 4*sizeof(gInt8));

	/*pOutData->WriteHead(pGDataHead);*/
	GELEVATIONDATAHEAD * pGelDataHead = new GELEVATIONDATAHEAD[ElevationNumber];
	logfile<<"QC_FirstOfRef��  Get and Set Elevation Head Information."<<endl;
	for (gInt16 i=0;i<pGData->GetHeadP()->ElevationCount;i++)
	{ 
		pGelDataHead[i].SNOfElevationData = i+1;
		pGelDataHead[i].UnambiguousRange = pGData->GetElevationHeadP(i) -> UnambiguousRange;
		pGelDataHead[i].NyquistVelocity = pGData->GetElevationHeadP(i) -> NyquistVelocity;
		pGelDataHead[i].ReflectivityResolution =pGData->GetElevationHeadP(i)->ReflectivityResolution;//δ֪���ݣ���0
		pGelDataHead[i].VelocityResolution = pGData->GetElevationHeadP(i) -> VelocityResolution;
		pGelDataHead[i].SpectrumWidthOfVelocityResolution = pGData->GetElevationHeadP(i)->SpectrumWidthOfVelocityResolution;//δ֪���ݣ���0
		pGelDataHead[i].FirstGateRangeOfRef = pGData->GetElevationHeadP(i) -> FirstGateRangeOfRef;
		pGelDataHead[i].FirstGateRangeOfDoppler = pGData->GetElevationHeadP(i) -> FirstGateRangeOfDoppler;
		pGelDataHead[i].ReflectivityGates = pGData->GetElevationHeadP(i)->ReflectivityGates;
		pGelDataHead[i].DopplerGates = pGData->GetElevationHeadP(i)->DopplerGates;
		pGelDataHead[i].Elevation = pGData->GetElevationHeadP(i)->Elevation;//��λ0.01��;
		pGelDataHead[i].AzimuthNumber = pGData->GetElevationHeadP(i)->AzimuthNumber;
		//pOutData->WriteElevationHead(i, pGelDataHead[i]);
	}
	GHRESULT gResult;
	if((gResult = pOutData->WriteHeadAndElevationHead( pGDataHead,pGelDataHead,ElevationNumber)) != GS_OK)
	{
		delete pGDataHead;
		pGDataHead = NULL;
		delete []pGelDataHead;
		pGelDataHead = NULL;
		return gResult;
	}
	/*GELEVATIONDATAHEAD * pGelDataHead = pGData->GetElevationHeadP(0);
	pOutData->WriteElevationHead(0, pGelDataHead);*/

	delete pGDataHead;
	pGDataHead = NULL;
	delete []pGelDataHead;
	pGelDataHead = NULL;
	//int MaxAzimuth=pGData->GetHeadP()->maxAziNum;
	//int MaxGates=pGData-> GetHeadP()->maxRefgatenum;	
	gInt16 MaxAzimuth = 0;	
	gInt16 MaxGates = 0;

	logfile<<"QC_FirstOfRef��  Loop to Find the MaxGates and MaxAzimuth."<<endl;
	for(gInt16 i = 0; i< pGData->GetHeadP()->ElevationCount; i++)
	{
		gInt16 TempNumOfGates = pGData->GetElevationHeadP(i)->ReflectivityGates;
		if(MaxGates < TempNumOfGates)
		{		
			MaxGates = TempNumOfGates;
		}
		if (MaxAzimuth < pGData->GetElevationHeadP(i)->AzimuthNumber)
		{
			MaxAzimuth = pGData->GetElevationHeadP(i)->AzimuthNumber;
		}
	}

	float *Onefeature=NULL;
	Onefeature=new float [6];
	for (int i=0;i<=5 ;i++)
	{
		Onefeature[i]=0;
	}

	gInt16 **Mask=NULL;
	Mask=new gInt16*[MaxAzimuth];
	for (int i=0;i<MaxAzimuth;i++)
	{
		Mask[i]=new gInt16[MaxGates]; 
		for(int j = 0;j < MaxGates ; j++ )
		{
			Mask[i][j] = 0; 
		}
	}

	logfile<<"QC_FirstOfRef��  Loop to Save Input Product's Data into the Array: RData."<<endl;
	gInt16 ***RData=NULL;
	RData=new gInt16**[2];
	for (int i=0;i<2;i++)
	{
		RData[i]=new gInt16*[MaxAzimuth];
		for (int j=0;j<MaxAzimuth;j++)
		{
			RData[i][j]=pGData->GetRadarDataR(i,j);
		}
	}
	logfile<<"QC_FirstOfRef��  Loop Finished."<<endl;

	int CycleMaxAzimuth=MaxAzimuth-1;
	int CycleMaxGates=MaxGates-3;

	logfile<<"QC_FirstOfRef��  Loop All Azimuths and Gates to Calculate Array: Mask[][]."<<endl;
	logfile<<"QC_FirstOfRef��   Call Function CalcFeature()."<<endl;
	for(int a=0;a<=CycleMaxAzimuth;a++)         //�Ƕ�ѭ��
	{
		for(int p=2;p<=CycleMaxGates;p++)  //����ѭ��
		{
			for(int k = 0;k<=5;k++) 
			{
				Onefeature[k]=RADAR_DATA_NOECHO;    //��ȫ�������Ч�������Ч����Ȼ����������ֵ���档
			}
			if (RData[0][a][p]>RADAR_DATA_OUTOFRANGE)
			{
				if(CalcFeature( a ,p ,Onefeature, MaxAzimuth,RData)) //���㵱ǰ�������
				{	
					if (Onefeature[0]>=1200)   //������Ľ�ˮ����
					{
						if( Onefeature[3]>=3000)  //Ԥ��������ε����Ǿֲ���ֵ����30����ֱ�ӱ��������μ������ļ���
						{
							Mask[a][p]=1;
						}
						else
						{
							if (Onefeature[4]<=100)
							{
								if ((Onefeature[3]>820)||((Onefeature[3]==0) && (Onefeature[0]>2*Onefeature[4]+900) && (Onefeature[1]<200000)))
								{
									Mask[a][p]=1;
								}
								else
								{
									if (Onefeature[1]<=50000)
									{
										Mask[a][p]=2;  //��ջز�
									}
									else 
										Mask[a][p]=3;  //������
								}
							}
							else
							{
								if ((Onefeature[3]>=13.33*Onefeature[4]-483.5) && (Onefeature[0]>=1500))
								{
									Mask[a][p]=1;
								}
								else
								{
									Mask[a][p]=3;  //������
								}
							}
						}
					}
					else
					{
						Mask[a][p]=2;    //��ջز�
					}
				}
				else
				{
					Mask[a][p]=0;      //����
				}
			}
		}
	}
	logfile<<"QC_FirstOfRef��  Loop Finished."<<endl;

	for(int a=0;a<=CycleMaxAzimuth;a++)
	{
		for(int p=1;p>=0;p--)
		{
			if (RData[0][a][p]>RADAR_DATA_OUTOFRANGE)
			{
				Mask[a][p]=Mask[a][p+1];
			}
		}
		for(int p=MaxGates-2;p<= MaxGates-1;p++)
		{
			if (RData[0][a][p]>RADAR_DATA_OUTOFRANGE)
			{
				Mask[a][p]=Mask[a][p-1];
			}
		}
	}


	logfile<<"QC_FirstOfRef��  Call Function Finalproc() to Delete Isolated Points and Calculate Output Product."<<endl;
	//ȥ�������Ĺ�����,�����
	Finalproc(pGData,pOutData,Mask,EchoPointRateThreshold, MaxAzimuth, MaxGates);	
	logfile<<"QC_FirstOfRef��  Function Finished."<<endl;
	//int y=0;
	//for(int m=0;m<=0;m++){for(int a=0;a<359;a++){for (int p=0;p<=459;p++){
	//	if(pGData->GetRadarDataR(m,a)[p]!=-32767&&pGData->GetRadarDataR(m,a)[p]!=-32768)y++;
	//	/*printf("pGData->GetRadarDataR(%d,%d)[%d] %d \n",m,a,p,pGData->GetRadarDataR(m,a)[p])*/;}}}printf("%d\n",y);

	for (int i=0;i<MaxAzimuth;i++)
	{
		delete[] Mask[i];
	}
	delete[] Mask;
	Mask=NULL;

	for (int i=0;i<2;i++)
	{
		delete[] RData[i];
	}
	delete[] RData;
	RData=NULL;

	delete[] Onefeature;
	Onefeature=NULL;

	logfile<<"QC_FirstOfRef��  Finished."<<endl;
	logfile.close();

	return GS_OK;
}


//////�㷨������Ҫ�ĺ���������ÿһ���ز�����������������������ֵ
//////�Է����ڶ�����࣬�����ĵ������������Ǵ������ֵ������
//////ע��:��������ʱ�õ��˵ڶ�������(�ε����ǲ�)
bool  QualityControl_FirstOfRef::CalcFeature(int a,int p,float * Afeature,int MaxAzimuth,gInt16 ***RData)
{
	int *D=NULL;
	D=new int [4];

	gInt32 Sum=0;                 //��
	int Num=0;                  //����
	gInt32 Temp=0;
	gInt32 Temp2=0;
	int AzIndex=0;
	int  RoundA=2; //�������С
	int  RadialP=2; //�������С
	int CycleRoundA=a+RoundA;
	int CycleRadialP=p+RadialP;
	for(int i=a-RoundA ;i<=CycleRoundA;i++)     //�Ƕ�ѭ��
	{
		AzIndex=(i+MaxAzimuth)%MaxAzimuth;                 //����Ƕ�Խ��
		for( int j=p-RadialP;j<=CycleRadialP;j++)   //������ѭ��
		{
			if (RData[0][AzIndex][j]>RADAR_DATA_OUTOFRANGE)
			{
				Num++;
				Sum+=RData[0][AzIndex][j];
			} 
		}
	}
	Afeature[0]=(float)Sum/Num;     //feature 7: local mean

	Sum=0;                   //Sum���¶���
	for( int i=a-RoundA ;i<=CycleRoundA;i++)
	{
		AzIndex=(i+MaxAzimuth)%MaxAzimuth;                 //����Ƕ�Խ��
		for(int  j=p-RadialP;j<=CycleRadialP;j++)
		{
			if (RData[0][AzIndex][j]>RADAR_DATA_OUTOFRANGE)
			{
				Temp=(RData[0][AzIndex][j]-Afeature[0])*(RData[0][AzIndex][j]-Afeature[0]);
				Sum+=Temp;
			}
		}
	}
	Afeature[1]=(float)Sum/Num;    //feature 8: local variance

	Sum=0;                   //Sum���¶���
	int  FE=3;
	int  Xmax=75;
	for(int i=a-RoundA ;i<=CycleRoundA;i++)
	{
		AzIndex=(i+MaxAzimuth)%MaxAzimuth;                 //����Ƕ�Խ��
		for(int j=p-RadialP;j<= CycleRadialP;j++)
		{	
			//��ȡ��ǰ���ڽ����ĸ���
			D[0]=RData[0][AzIndex][j]; 
			D[1]=RData[0][AzIndex][j-1];
			if( AzIndex+1>MaxAzimuth-1)
			{
				D[2]=RData[0][AzIndex+1-MaxAzimuth][j];
				D[3]=RData[0][AzIndex+1-MaxAzimuth][j-1];
			}
			else
			{
				D[2]=RData[0][AzIndex+1][j] ;
				D[3]=RData[0][AzIndex+1][j-1] ;
			}
			Temp=-32768;  //����ֲ����ֵ
			Temp2=32767;  //����ֲ���Сֵ
			for (int k = 0;k<= 3;k++)
			{
				if (D[k]<=RADAR_DATA_OUTOFRANGE)
				{
					D[k]=0;
				}
				if(D[k]>RADAR_DATA_OUTOFRANGE)
				{
					Temp =(D[k]>Temp)?D[k]:Temp;
					Temp2=(D[k]<Temp2)?D[k]:Temp2;
				}
			}
			if((Temp!=-32768)&&(Temp2!=32767))
			{
				Temp=gInt32(exp(-FE*log(1+(Xmax-Temp/100.0)/Xmax))*100);
				Temp2=gInt32(exp(-FE*log(1+(Xmax-Temp2/100.0)/Xmax))*100);
				gInt32 Temp1=Temp-Temp2;
				Sum+=fabs(Temp1);
			}
		}
	}
	Afeature[4]=Sum;    //feature mzy: STֵ
	//����ε����ǵ�����

	Sum=0;
	Num=0;
	for(int  i=a-RoundA;i<=CycleRoundA;i++)
	{
		AzIndex=(i+MaxAzimuth)%MaxAzimuth;                 //����Ƕ�Խ��
		for( int j=p-RadialP;j<=CycleRadialP;j++)
		{
			if (RData[1][AzIndex][j]>RADAR_DATA_OUTOFRANGE)
			{
				Num++;
				Sum+=RData[1][AzIndex][j];
			}
		}
	}
	if(Num==0 )  //���������һ����Ч��ֵ��û�еĻ����ͼ�Ϊ��
	{
		Afeature[3]=0;
	}
	else
	{
		Afeature[3]=(float)Sum/Num; 
	}

	delete[] D;
	D=NULL;

	return true;
}

////ȥ�������Ĺ�����
void  QualityControl_FirstOfRef ::Finalproc(StandardRadarData *pGData,StandardRadarData *pOutData, gInt16 **Mask,int EchoPointRateThreshold, gInt16 MaxAzimuth, gInt16 MaxGates)
{
	//int MaxAzimuth=pGData->GetHeadP()->maxAziNum;
	//int MaxGates=pGData-> GetHeadP()->maxRefgatenum;
	gInt32 Temp=0;
	gInt32 EsNum=0;
	gInt16 **tempMask=NULL;
	int  RoundA=2; //�������С
	int  RadialP=2; //�������С
	gInt16 ElevationCount = pGData->GetHeadP()->ElevationCount;

	tempMask=new gInt16*[MaxAzimuth];
	for (int i=0;i<MaxAzimuth;i++)
	{		
		tempMask[i]=new gInt16[MaxGates]; 
		for(int j=0;j<MaxGates;j++)
		{
			tempMask[i][j]=Mask[i][j];
		}
	}

	int Range=(2*RadialP+1)*(2*RoundA+1);

	for( int a=0;a<=MaxAzimuth-1;a++)
	{
		for( int p=0;p<=MaxGates-1;p++)
		{
			if (tempMask[a][p]==1)
			{
				EsNum=0;
				int CycleRoundA=a+RoundA;
				int CycleRadialP=p+RadialP;
				for( int i=a-RoundA;i<=CycleRoundA;i++)     //�Ƕ�ѭ��
				{
					Temp=(i+MaxAzimuth)%MaxAzimuth;                 //����Ƕ�Խ��

					for(int  j=p-RadialP;j<=CycleRadialP;j++)   //������ѭ��
					{ 
						if( tempMask[Temp][j]==1)
						{
							EsNum++;
						}
					}
				}
				if((EsNum)*100<EchoPointRateThreshold*Range)      
				{
					Mask[a][p]=0;
				}
			}
		}
	}
	
	for (int i=0;i<MaxAzimuth;i++)
	{
		delete[] tempMask[i];
	}
	delete[] tempMask;
	tempMask=NULL;

	for ( int i=0;i<ElevationCount;i++)
	{
		for( int a=0;a<pGData->GetElevationHeadP(i)->AzimuthNumber;a++)         //�Ƕ�ѭ��
		{
			RVWDATAHEAD *pOutRVWHead = pOutData->GetRadarDataP(i, a);
			RVWDATAHEAD *pInRVWHead = pGData->GetRadarDataP(i, a);
			pOutRVWHead->Azimuth = pInRVWHead->Azimuth;
			pOutRVWHead->Date = pInRVWHead->Date;
			pOutRVWHead->Elevation = pInRVWHead->Elevation;
			pOutRVWHead->Time = pInRVWHead->Time;
			gInt16 *pOutRData = pOutData->GetRadarDataR(i, a);
			gInt16 *pOutVData = pOutData->GetRadarDataV(i, a);
			gInt16 *pOutWData = pOutData->GetRadarDataW(i, a);
			gInt16 *pInRData = pGData->GetRadarDataR(i, a);
			gInt16 *pInVData = pGData->GetRadarDataV(i, a);
			gInt16 *pInWData = pGData->GetRadarDataW(i, a);
			if (i==0)
			{
				for( int p=0 ;p<=MaxGates-1;p++)  //����ѭ��
				{
					//////��������������
					if (Mask[a][p]!=1)      //Mask[a][p]=2��ջز�  ��Mask[a][p]=3������
					{
						pOutRData[p]=RADAR_DATA_NOECHO;
					}
					else
					{
						pOutRData[p]=pInRData[p];
					}
				}
			}
			else
			{
				for(int p =0; p< pOutData->GetElevationHeadP(i)->ReflectivityGates; p++)
					pOutRData[p] = pInRData[p];
			}
			for(int p =0; p< pOutData->GetElevationHeadP(i)->DopplerGates; p++)
			{
				pOutVData[p] = pInVData[p];
				pOutWData[p] = pInWData[p];
			}
		}
	}
	
	/*for( int a=0;a<=MaxAzimuth-1;a++)         //�Ƕ�ѭ��
	{
		RVWDATAHEAD *pOutRVWHead = pOutData->GetRadarDataP(0, a);
		RVWDATAHEAD *pInRVWHead = pGData->GetRadarDataP(0, a);
		pOutRVWHead->Azimuth = pInRVWHead->Azimuth;
		pOutRVWHead->Date = pInRVWHead->Date;
		pOutRVWHead->Elevation = pInRVWHead->Elevation;
		pOutRVWHead->Time = pInRVWHead->Time;
		gInt16 *pOutRData = pOutData->GetRadarDataR(0, a);
		gInt16 *pOutVData = pOutData->GetRadarDataV(0, a);
		gInt16 *pOutWData = pOutData->GetRadarDataW(0, a);
		gInt16 *pInRData = pGData->GetRadarDataR(0, a);
		gInt16 *pInVData = pGData->GetRadarDataV(0, a);
		gInt16 *pInWData = pGData->GetRadarDataW(0, a);
		for( int p=0 ;p<=MaxGates-1;p++)  //����ѭ��
		{
			//////��������������
			if (Mask[a][p]!=1)      //Mask[a][p]=2��ջز�  ��Mask[a][p]=3������
			{
				pOutRData[p]=RADAR_DATA_NOECHO;
			}
			else
			{
				pOutRData[p]=pInRData[p];
			}
		}
		for(int p =0; p< pOutData->GetElevationHeadP(0)->DopplerGates; p++)
		{
			pOutVData[p] = pInVData[p];
			pOutWData[p] = pInWData[p];
		}
	}*/
}

GHRESULT QualityControl_FirstOfRef ::OutPutData(void *pValue)
{
	m_pOutputProductList=(GXList< GenerationData > *)pValue;
	if (m_pOutputProductList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//���ڴ����ӿ� as by dqc 20080519
{
	return new QualityControl_FirstOfRef;
}