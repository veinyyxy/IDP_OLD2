/////////////////////////////////////////////////////////////////////////
// Arith_501_BREF.cpp
// created by ChenXiang 20080424
// 501�Ų�Ʒ,�����㷨,BREF
//////////////////////////////////////////////////////////////////////////
#include "Arith_501_BREF.h"
#include "CJulMsGMT.h"
#include "CPolC2LonLatReseau.h"
#include "memset32.h"

#include "GDef.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include <fstream>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif//_USE_MATH_DEFINES
#include <math.h>

#ifndef REarth
#define REarth				6371300				//����뾶 (��)
#endif//REarth
#ifndef MaxLong
#define MaxLong				460000				//���ɨ��뾶���� (��)
#endif//MaxLong
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif//max
#ifndef SCALE_OF_BASE_DATA		
#define SCALE_OF_BASE_DATA	100					//���������ݷŴ���
#endif//SCALE_OF_BASE_DATA
#ifndef SCALE_OF_MULTI_DATA		
#define SCALE_OF_MULTI_DATA	100					//��Ʒ���ݷŴ���
#endif//SCALE_OF_MULTI_DATA

Arith_501_BREF::Arith_501_BREF()
{
	;
}
Arith_501_BREF::~Arith_501_BREF()
{
	;
}
GHRESULT Arith_501_BREF::Initialize()
{
	m_pParameter		= NULL;					//����ָ��
	m_pGenerateProductList= NULL;				//����list
	m_pOutputProductList= NULL;					//���list
	m_pMultifWeight		= NULL;					//����Ȩ�ش�����ָ��
	m_pMultiRW			= NULL;					//���������ʳ�Ȩ���ۼӴ�����ָ��
	m_ppLonLatData		= NULL;					//������ת��γ������ָ��
	m_pRadarDataR		= NULL;					//���淴�������ݾ���ָ��Ķ�λ����
					
	m_SiteLatNBorderline= NULL;					//��վ���߽�����
	m_SiteLatSBorderline= NULL;					//��վ�ϱ߽�����
	m_SiteLonWBorderline= NULL;					//��վ���߽�����
	m_SiteLonEBorderline= NULL;					//��վ���߽�����
	m_pSiteLon			= NULL;					//վ�㾭������ָ��
	m_pSiteLat			= NULL;					//վ��γ������ָ��
	m_pSiteHigh			= NULL;					//վ��߶�����ָ��
	m_pPPI_L			= NULL;					//��վ����������
	m_pCap_nrays		= NULL;					//��վ��λ��������
	m_pCap_gates		= NULL;					//��վ����������
	m_pGateLen			= NULL;					//��վ�ⳤ����
	m_el_tilts			= NULL;					//��վ����ֵ��ά����
	m_ErrorData			= NULL;					//��Ч��������
	m_ScaleOfMultiData	= SCALE_OF_MULTI_DATA;	//��Ʒ���ݷŴ���
	m_ResLonLat=0;
	
	return GS_OK;
}
GHRESULT Arith_501_BREF::UnInitialize()
{
	if(m_SiteLatNBorderline!=NULL){delete []m_SiteLatNBorderline;	m_SiteLatNBorderline=NULL;}
	if(m_SiteLatSBorderline!=NULL){delete []m_SiteLatSBorderline;	m_SiteLatNBorderline=NULL;}
	if(m_SiteLonWBorderline!=NULL){delete []m_SiteLonWBorderline;	m_SiteLatNBorderline=NULL;}
	if(m_SiteLonEBorderline!=NULL){delete []m_SiteLonEBorderline;	m_SiteLatNBorderline=NULL;}
	if(m_pSiteLon!=NULL)		  {delete []m_pSiteLon;	m_SiteLatNBorderline=NULL;}
	if(m_pSiteLat!=NULL)		  {delete []m_pSiteLat;	m_SiteLatNBorderline=NULL;}
	if(m_pSiteHigh!=NULL)		  {delete []m_pSiteHigh;	m_SiteLatNBorderline=NULL;}
	if(m_pPPI_L!=NULL)			  {delete []m_pPPI_L;		m_SiteLatNBorderline=NULL;}
	if(m_pCap_nrays!=NULL)		  {delete []m_pCap_nrays;	m_SiteLatNBorderline=NULL;}
	if(m_pCap_gates!=NULL)		  {delete []m_pCap_gates;	m_SiteLatNBorderline=NULL;}
	if(m_pGateLen!=NULL)		  {delete []m_pGateLen;	m_SiteLatNBorderline=NULL;}

	if(m_pRadarDataR!=NULL)
	{
		for (int pos=0;pos<m_SiteNum;pos++)
		{		
			if (m_pRadarDataR[pos]!=NULL&&!m_ErrorData[pos])
			{
				delete []m_pRadarDataR[pos];
				m_pRadarDataR[pos]=NULL;
			}					
		}
		delete []m_pRadarDataR;
		m_pRadarDataR=NULL;
	}
	if(m_ppLonLatData!=NULL)
	{
		for (int pos=0;pos<m_SiteNum;pos++)
		{
			if (m_ppLonLatData[pos]!=NULL&&!m_ErrorData[pos])	
			{
				delete []m_ppLonLatData[pos];	
				m_ppLonLatData[pos]=NULL;
			}
		}
		delete []m_ppLonLatData;
		m_ppLonLatData=NULL;
	}
	if(m_el_tilts!=NULL)
	{
		for (int pos=0;pos<m_SiteNum;pos++)
		{
			if (m_el_tilts[pos]!=NULL)	{delete []m_el_tilts[pos];	m_el_tilts[pos]=NULL;}
		}
		delete []m_el_tilts;
		m_el_tilts=NULL;
	}
	if (m_pMultifWeight!=NULL)	
	{
		delete []m_pMultifWeight;	
		m_pMultifWeight=NULL;
	}
	if (m_pMultiRW != NULL)		
	{
		delete []m_pMultiRW;
		m_pMultiRW = NULL;
	}
	if (m_ErrorData != NULL)		
	{
		delete []m_ErrorData;
		m_ErrorData = NULL;
	}

	return GS_OK;
}
GHRESULT Arith_501_BREF::LoadData(void *pValue,ReadParam * ReadParameter)
{	
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pParameter=ReadParameter;
	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;				//ȡ�����������б�

	return GS_OK;
}
GHRESULT Arith_501_BREF::OutPutData(void * pValue)
{
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	if (m_pOutputProductList==NULL)
	{
		return GS_FALSE;
	}
	return GS_OK;
}
GHRESULT Arith_501_BREF::Execute()
{
	ofstream logfile;
	logfile.open("Arith_log.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"		"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"501_BREF��  Started: "<<endl;

	logfile<<"501_BREF��  Get and Check the m_pInputList's Count."<<endl;

	m_SiteNum = gInt16( m_pGenerateProductList->GetCount() );					//ȡԪ������//վ������
	if (m_SiteNum <= 0)															//added by chenxiang 20080625 for վ������Ϊ0�ж�
	{
		return GS_FALSE;														//����ʧ�� վ����Ϊ�� ����ܶ඼�޷�����
	}
	m_ErrorData= new(::std::nothrow) bool[m_SiteNum];
	memset(m_ErrorData,0,sizeof(bool)*m_SiteNum);
	StandardRadarData * pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData*>(0));//ȡ��0Ԫ��//��0��ͳһ�������ļ�NULL;
	GRADARDATAHEADER*  pRadarHead = pRadar->GetHeadP();							//��0��վ���ͷ
	GELEVATIONDATAHEAD* pEHead = pRadar->GetElevationHeadP(0);					//��0վ��0����ͷ

	logfile<<"501_BREF��  Send in and Read Parameters."<<endl;
	gInt32 TemVCPDate = 0;														//��վ�����յĺ�
	gInt32 TemVCPTime = 0;														//��վ����ʱ��ĺ� //�������ֵ
	gChar *pTempVCPDateTime = NULL;		
	gChar *pTempResLonLat	= NULL;
	(*m_pParameter)("ResLonLat",pTempResLonLat);
	if (pTempResLonLat == NULL )
	{
		m_ResLonLat = 1000;//return GE_INVALIDARG;
	}
	m_ResLonLat = (gInt32)atoi(pTempResLonLat);			
	//added by chenxiang 20081125 ��Ʒʱ��/////////
	(*m_pParameter)("ScanDateTime",pTempVCPDateTime);
	if (pTempVCPDateTime == NULL )
	{
		return GE_INVALIDARG;
	}
	//ʾ��:20081120140000
	gChar buffer[32] = {0};
	strcpy(buffer,pTempVCPDateTime);
	buffer[8] = 0;
	//��������
	TemVCPDate = (gInt32)atoi(buffer);//����
	gInt32 Year = TemVCPDate / 10000;
	gInt32 Month= ( TemVCPDate - Year * 10000) / 100;
	gInt32 Day  = TemVCPDate % 100;
	TemVCPDate = CJulMsGMT::Greg2Jul(Year,Month,Day);

	//����ʱ��
	strcpy(buffer,pTempVCPDateTime);
	for(int i=8;i<14;i++)
	{
		buffer[i-8]=pTempVCPDateTime[i];
	}
	buffer[6] = 0;
	TemVCPTime = (gInt32)atoi(buffer);//ʱ��
	gInt32 T_Hour   = TemVCPTime / 10000;
	gInt32 T_Minute = ( TemVCPTime - T_Hour*10000 ) / 100;
	gInt32 T_Second = TemVCPTime % 100;
	TemVCPTime = CJulMsGMT::Time2MilliSecond(T_Hour,T_Minute,T_Second,0);
	//end of added///////////////////////////////

#if defined (WIN32)|| defined(WIN64)
#	pragma warning(pop)
#endif

	m_SiteLatNBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//վ�㱱�˱߽�����
	m_SiteLatSBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//վ���϶˱߽�����
	m_SiteLonWBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//վ�����˱߽�����
	m_SiteLonEBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//վ�㶫�˱߽�����
	m_pSiteLon	 =  new(::std::nothrow) gInt32[m_SiteNum];										//վ�㾭������//������180�Ȼ�����鷳
	m_pSiteLat	 =  new(::std::nothrow) gInt32[m_SiteNum];										//վ��γ������
	m_pSiteHigh  =  new(::std::nothrow) gInt16[m_SiteNum];										//վ��߶�����	
	m_pPPI_L	 =  new(::std::nothrow) gUint8[m_SiteNum];										//��վ��������
	m_pCap_nrays =  new(::std::nothrow) gInt16[m_SiteNum];										//��վ�ķ�λ����
	m_pCap_gates =  new(::std::nothrow) gInt32[m_SiteNum][20];									//��վ�Ŀ���[վ��][������]
	m_pGateLen	 =  new(::std::nothrow) gInt32[m_SiteNum];										//��վ�Ŀⳤ
	//�ڴ�����ɹ��ж�
	if(m_SiteLatNBorderline == NULL||m_SiteLatSBorderline == NULL||
	   m_SiteLonWBorderline == NULL||m_SiteLonEBorderline == NULL||
	   m_pSiteLon			== NULL||m_pSiteLat			  == NULL||
	   m_pSiteHigh			== NULL||m_pPPI_L			  == NULL||
	   m_pCap_nrays			== NULL||m_pCap_gates		  == NULL||
	   m_pGateLen			== NULL )   
	{	
		return GE_NOENOUGHMEMORY;
	}
	//memset(m_pGateLen,0,sizeof(gInt32)*m_SiteNum);
	
	logfile<<"501_BREF��  Loop: pos=0-->m_SiteNum("<<m_SiteNum<<") to Get All Sites' Information."<<endl;
	//����վ��ȡ����Ϣ�浽������//////////////////////////վ������ʼ////////////////////////////////////////
	int AvailabilitySiteNum = 0;												//��Чվ������
	for(int pos = 0;pos<m_SiteNum;pos++)										
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));		
		pRadarHead = pRadar->GetHeadP();										//��pos��վ���ͷ
		if (pRadarHead==NULL || m_ErrorData[pos])
		{
			m_ErrorData[pos]=true;
			continue;
		}
		pEHead = pRadar->GetElevationHeadP(0);									//��posվ��0����ͷ
		if (pEHead==NULL || m_ErrorData[pos]/* || pRadar->GetHeadP()->maxAziNum<=0*/)
		{
			m_ErrorData[pos]=true;
			continue;
		}
		if(m_ErrorData[pos]||pRadarHead->ElevationCount<=0||pRadarHead->ReflectivityGateSize<150||(pRadarHead->RadarLon == 0&&pRadarHead->RadarLat ==0))//�������ж�//�ⳤ̫С��γ��ͬʱΪ0(�����Ǵ�������)
		{
			m_ErrorData[pos]=true;
			continue;
		}
		m_pSiteLon[pos]  = (pRadarHead->RadarLon) * 100;						//վ�㾭�� ע�ⵥλ �����0.001תΪ0.00001��
		m_pSiteLat[pos]  = (pRadarHead->RadarLat) * 100;						//վ��γ��
		m_pSiteHigh[pos] = gInt16((pRadarHead->RadarHeight)/100);				//�״�߶ȵ�λ��0.01��תΪ1��//ʡȥ����������
		AvailabilitySiteNum++;
		m_pGateLen[pos]	 = (pRadarHead->ReflectivityGateSize);					//ǿ�ȿⳤ(��) *m_ResLonLat/1000 ��Ϊ�˲�ͬ��γ�ֱ�����ʹ����Ӧ�ľ���
		m_pPPI_L[pos]    = gUint8(pRadar->GetHeadP()->ElevationCount);			//��ȡ������
		m_pCap_nrays[pos]= pEHead->AzimuthNumber;								//����ɨ��ķ�λ��//��Ϊ��ͬ���ǵķ�λ������һ��

			
	}////////////////////////////////////////////////վ��������////////////////////////////////////////////////////
	logfile<<"501_BREF��  Loop Finished."<<endl;

	if(AvailabilitySiteNum==0)
	{
		return GS_FALSE;
	}
	logfile<<"501_BREF��  Define a two-dimension Array(m_el_tilts) to Save Elevation Information."<<endl;
	logfile<<"501_BREF��  Loop to go through All of the Sites."<<endl;
	//�����ά����(��¼����ֵ)
	m_el_tilts =  new(::std::nothrow) gInt16*[m_SiteNum];
	for (int pos = 0;pos<m_SiteNum;pos++)										//վ�����
	{
		if(m_el_tilts == NULL)													//һ��ʧ��
		{
			return GE_NOENOUGHMEMORY;
		}
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		pRadarHead = pRadar->GetHeadP();										//��pos��վ���ͷ
		if(m_ErrorData[pos])
		{
			m_el_tilts[pos] =  new(::std::nothrow) gInt16[18];
		}
		else
		{
			m_el_tilts[pos] =  new(::std::nothrow) gInt16[m_pPPI_L[pos]];						//��������
		}
		if (m_el_tilts[pos] == NULL)											//����ʧ��
		{
			for (int i = 0;i <= pos;i++)
			{
				delete []m_el_tilts[i];
				m_el_tilts[i] = NULL;
			}
			return GE_NOENOUGHMEMORY;
		}
	}
		logfile<<"501_BREF��  Loop Finished."<<endl;
	//�����ά�������

	//�����ֱ���ʹ��������ⳤ(С�����ⳤ��ȡ��������)
	gInt32 MaxGateLen = Max(m_pGateLen,m_SiteNum);
	logfile<<"501_BREF��  Find the Max Gate Length."<<endl;
	if (m_ResLonLat < MaxGateLen )												//�䵥λ��ͳһ���ɽ���
	{
		m_ResLonLat = MaxGateLen;
	}
	//�ֱ���ȡֵ��Χ����
	if(m_ResLonLat>100000)
	{
		m_ResLonLat=100000;
	}
	if(m_ResLonLat<100)
	{
		m_ResLonLat=100;
	}

	logfile<<"501_BREF��  Loop to Get Every Sites' Elevations and Gate's Length."<<endl;
	//����ÿ��վ��� ÿ�����Ǵ�С �� ÿ�����ǵĿ���
	for(int pos = 0; pos<m_SiteNum; pos++)										
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		gInt16 *pEl_tilts=m_el_tilts[pos];
		if(m_ErrorData[pos])										
		{
			continue;
		}
		for(int i = 0; i<20; i++)
		{
			m_pCap_gates[pos][i]=0;//����
		}
		for(int i = 0; i<m_pPPI_L[pos]; i++)
		{
			pEHead = pRadar->GetElevationHeadP(i);
			pEl_tilts[i] = gInt16(pEHead->Elevation);							 //0.01�� //ȡ���վ�ĸ����Ǵ�С
			m_pCap_gates[pos][i]= (pEHead->ReflectivityGates) * 1000/m_ResLonLat;//ǿ�ȿ���  *1000/m_ResLonLat ��Ϊ�˲�ͬ��γ�ֱ�����ʹ����Ӧ�ľ���
		}
	}
	logfile<<"501_BREF��  Loop Finished."<<endl;
	
	for(int pos = 0;pos<m_SiteNum;pos++)										
	{			
		m_pGateLen[pos]	 = m_pGateLen[pos]   * m_ResLonLat/1000;				// ǿ�ȿⳤ(��) *m_ResLonLat/1000 ��Ϊ�˲�ͬ��γ�ֱ�����ʹ����Ӧ�ľ���
	}
	//32�������cappi���θ߶�,��λ250��
	gInt16 pCAPPI_High[32]={2,3,4,5,6,7,8,9,10,11,12, 14,16,18,20,22,24,26,28,30,32,34,36,38,40,  44,48,52,56,60,64,    72};
	/////////////////////{500��,750��...........3000,3500.................................10000,11000...........16000,18000}
	gInt16 StartHigh = gInt16( Min(m_pSiteHigh,m_SiteNum) );					//ȡ���վ��߶� ��λ��
	for(int i = 0;i<32;++i)
	{
		if( pCAPPI_High[i] * 250 > StartHigh + 200 )
		{
			StartHigh = pCAPPI_High[i] * 250;									//��ʼ�߶ȵ��ڴ�������״�վ�߶�200�����ϵ����cappi�߶�
			break;
		}
	}
	gInt16 EndHigh = 18000;														//cappi��߸߶� ����Ϊ18000m �������Ͻ� ��λ250��

	//����cappi�߶����� ȥ����Ч�� ��ʡ��Դ//
	gInt32 Tempi = 0;
	gInt32 Tempk = 0;
	for (Tempi = 0;Tempi<32;Tempi++)
	{
		if (pCAPPI_High[Tempi] * 250 >= StartHigh)
		{
			if(pCAPPI_High[Tempk] == pCAPPI_High[Tempi])
			{
				break;
			}
			for (Tempk=0;Tempk<32&&Tempi<32;Tempk++)
			{
				pCAPPI_High[Tempk] = pCAPPI_High[Tempi++];
			}
			while (Tempk<32)
			{
				pCAPPI_High[Tempk++] = 73;										//72��18000��//����18000�ײ��ǵ��ǲ������
			}
			break;		
		}
	}
	gInt32 DLon = 0;
	gInt32 DLat = 0;

	//1��վ��������õ������� ��������
	for(int pos = 0;pos<m_SiteNum;pos++)
	{
		if (m_ErrorData[pos])
		{
			continue;
		}
		if(CPolC2LonLatReseau::QuickPolC2DLonDLat(0,MaxLong,m_pSiteLon[pos],m_pSiteLat[pos],DLon,DLat,m_pSiteHigh[pos]))//�� ��
		{
			m_SiteLatNBorderline[pos] = m_pSiteLat[pos]+DLat;
			m_SiteLatSBorderline[pos] = m_pSiteLat[pos]-DLat;
		}
		else
		{
			CPolC2LonLatReseau::QuickPolC2DLonDLat(0,MaxLong,12000000,5300000,DLon,DLat,0);//Ϊ������ݴ���
			m_SiteLatNBorderline[pos] = m_pSiteLat[pos]+DLat;
			m_SiteLatSBorderline[pos] = m_pSiteLat[pos]-DLat;
		}//////////
		if(CPolC2LonLatReseau::QuickPolC2DLonDLat(27000,MaxLong,m_pSiteLon[pos],m_pSiteLat[pos],DLon,DLat,m_pSiteHigh[pos]))//�� ��
		{
			m_SiteLonWBorderline[pos] = m_pSiteLon[pos]+DLon;
			m_SiteLonEBorderline[pos] = m_pSiteLon[pos]-DLon;
		}
		else
		{
			CPolC2LonLatReseau::QuickPolC2DLonDLat(27000,MaxLong,12000000,5300000,DLon,DLat,0);//Ϊ������ݴ���//ȡ�й������γ�������������ľ��Ȳ���� ʹ�����߽繻��
			m_SiteLonWBorderline[pos] = m_pSiteLon[pos]+DLon;
			m_SiteLonEBorderline[pos] = m_pSiteLon[pos]-DLon;
		}///////////
	}
	//��ȡ�����ϱ���γ����
	m_StartLon = Min(m_SiteLonWBorderline,m_SiteNum);							//��ʼ���� ��
	m_EndLon   = Max(m_SiteLonEBorderline,m_SiteNum);							//�������� ��
	m_StartLat = Max(m_SiteLatNBorderline,m_SiteNum);							//��ʼγ�� ��
	m_EndLat   = Min(m_SiteLatSBorderline,m_SiteNum);							//����γ�� ��

	
	m_M = ((m_StartLat-m_EndLat)/m_ResLonLat+2);							//��������
	m_N = ((m_EndLon-m_StartLon)/m_ResLonLat+2);							//��������
	logfile<<"501 bref  "<<"m_M="<<m_M<<"  m_N="<<m_N<<"m_StartLat="<<m_StartLat<<"  m_EndLat="<<m_EndLat<<" m_ResLonLat="<<m_ResLonLat<<endl;

	m_pRadarDataR =  new(::std::nothrow) gInt16**[m_SiteNum];
	for (int pos = 0;pos<m_SiteNum;pos++)										//վ�����
	{
		if(m_pRadarDataR == NULL)												//һ��ʧ��
		{
			return GE_NOENOUGHMEMORY;
		}
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(!m_ErrorData[pos])
		{
			m_pRadarDataR[pos] =  new(::std::nothrow) gInt16*[ m_pPPI_L[pos] * m_pCap_nrays[pos] ];//�������� (������*��λ����)
		}
		if (m_pRadarDataR[pos] == NULL)											//����ʧ��
		{
			for (int i = 0;i <= pos;i++)
			{
				if(!m_ErrorData[pos])
				{
					delete []m_pRadarDataR[i];
					m_pRadarDataR[i] = NULL;
				}
			}
			return GE_NOENOUGHMEMORY;
		}
	}
	//�����ά�������//��ָ�뿪ʼ
	gInt32 Temp1;
	for (int pos = 0;pos<m_SiteNum;pos++)										//վ�����
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(m_ErrorData[pos])
		{
			continue;
		}
		for (int j = 0;j<m_pPPI_L[pos];j++)										//���Ǳ���
		{
			Temp1 = j * m_pCap_nrays[pos];
			for (int k = 0;k<m_pCap_nrays[pos];k++,Temp1++)						//��λ�Ǳ���
			{
				m_pRadarDataR[pos][Temp1] = pRadar->GetRadarDataR(j,k);			//�洢ָ��
			}
		}	
	}//*/
	//��ȡ����ָ������������//////////////////////////////////////////////////////////////////////////////////////

	//����Ȩ�ش�����(������)
	if(m_M <= 0||m_N <= 0)
	{
		return GS_FALSE;
	}
	if (m_pMultifWeight != NULL)
	{
		m_pMultifWeight = NULL;
	}
	if (m_pMultiRW != NULL)
	{
		m_pMultiRW =NULL;
	}
	m_pMultifWeight =  new(::std::nothrow) gUint16[m_M * m_N];
	m_pMultiRW =   new(::std::nothrow) gInt32[m_M * m_N];

	if(m_pMultifWeight == NULL||m_pMultiRW==NULL)									//�ж��ڴ��Ƿ�����ɹ�
	{
		return GE_NOENOUGHMEMORY;
	}//Ȩ�ش�����(������)�������////////
	
	//��ά����� new(::std::nothrow)//m_ppLonLatData�Ǽ�����ת��γ�ȱ�ָ�� [վ�����][��λ�ǺͿ���] to ���� γ��
	if (m_ppLonLatData != NULL)
	{
		m_ppLonLatData = NULL;	
	}
	m_ppLonLatData =  new(::std::nothrow) gInt32*[m_SiteNum];
	if (m_ppLonLatData == NULL)
	{
		return GE_NOENOUGHMEMORY;
	}
	for(int pos = 0;pos<m_SiteNum;pos++)
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(m_ErrorData[pos]||m_pCap_nrays[pos] * m_pCap_gates[pos][0]<=0)
		{
			m_ErrorData[pos]=true;
			continue;
		}
		m_ppLonLatData[pos] =  new(::std::nothrow) gInt32[m_pCap_nrays[pos] * m_pCap_gates[pos][0] * 2];
		if(m_ppLonLatData[pos] == NULL)
		{
			for (int i = 0;i <= pos;i++)
			{
				if (!m_ErrorData[pos])
				{
					delete []m_ppLonLatData[i];
					m_ppLonLatData[i] = NULL;
				}
				
			}
			return GE_NOENOUGHMEMORY;
		}
	}
	//��ά����� new(::std::nothrow)����*/

	//���㾭γ�ȱ�ʼ
	gInt32 TempInt_ij = 0;
	for (int pos = 0;pos<m_SiteNum;pos++)//158
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(m_ErrorData[pos])
		{		
			continue;
		}
		gInt16 iAzimuth = 36000/m_pCap_nrays[pos];
		for (int i = 0;i<m_pCap_nrays[pos];i++)//360
		{
			TempInt_ij = i * m_pCap_gates[pos][0] * 2;
			for (int j = 0;j<m_pCap_gates[pos][0];j++,TempInt_ij += 2)//460
			{
				(!CPolC2LonLatReseau::QuickPolC2LonLat(i * iAzimuth,gInt32(j * m_pGateLen[pos]),m_pSiteLon[pos],m_pSiteLat[pos],\
					m_ppLonLatData[pos][TempInt_ij],m_ppLonLatData[pos][TempInt_ij+1],m_pSiteHigh[pos] ) )?
					(
					m_ppLonLatData[pos][TempInt_ij] = m_pSiteLon[pos],				//����  �������ʧ�� �Ͱ��״����ĵľ�γ�ȸ��� ��С����
					m_ppLonLatData[pos][TempInt_ij+1] = m_pSiteLat[pos]				//γ��		
				):false;															//�㷨�Ż� ��if��Ϊ?:���ʽ		
			}
		}
	}//���㾭γ�ȱ����*/

	//��Ӳ�Ͳ�ͷ��Ϣ��ʼ///////////////
	gInt16 EndLayer = 32;															//���cappi��
	for(gInt16 Layer = 0;Layer<32&&(pCAPPI_High[Layer] * 250 <= EndHigh);Layer++)
	{
		EndLayer = Layer;
	}
	EndLayer++;

	logfile<<"501_BREF��  Loop to Calculate the Array: m_ppLonLatData."<<"EndLayer="<<EndLayer<<endl;
///////////////////////////

	for( int Layer =0; Layer<EndLayer; Layer++)
	{
		m_pOut = NULL;
	    logfile<<"501_BREF��  Create Send-out Product: Product_501_BREF."<<endl;
		m_pOutputProductList->AddTail();											//�������б�	
		m_pOut = &(*(m_pOutputProductList->GetAtAs<Product_501_BREF>(Layer)));
		if(m_pOut->Coustruct(m_SiteNum,m_M,m_N,1)!=GS_OK)
		{
			return GE_NOENOUGHMEMORY;
		}
		logfile<<"m_SiteNum ="<<m_SiteNum<<endl;
		logfile<<"m_M ="<<m_M<<endl;
		logfile<<"m_N ="<<m_N<<endl;
		GHRESULT TemRet = m_pOut->CreateP();										//����
		if(TemRet != GS_OK)
		{
			logfile<<"501_BREF��  Create Product_501_BREF failed and Return."<<endl;
			logfile.close();
			return TemRet;
		}
		//������Ϣͷд�뿪ʼ///////////////////////////////////////////////////////////////////////////////
		//m_pOut->GetMyMultiHead()->MessageCode = 249;								//������Ʒ��Ϣ���

		gUint16 JulDay;
		gInt32  MilliSecond;
		if(!CJulMsGMT::GetLocalJulMs(JulDay,MilliSecond))
		{
			return GS_FALSE;
		}
		m_pOut->GetMyMultiHead()->GenerationDateOfProduct = JulDay;						//ƴͼ����Julian����
		m_pOut->GetMyMultiHead()->GenerationTimeOfProduct = MilliSecond;				//ƴͼ����ʱ��
		//m_pOut->GetMyMultiHead()->ProductCode = 501;									//��Ʒ��
		m_ScaleOfMultiData= m_pOut->GetMyMultiHead()->ScaleOfData = SCALE_OF_MULTI_DATA;//��Ʒ���ݷŴ���
		m_ScaleOfBaseData = SCALE_OF_BASE_DATA;											//�����ݷŴ���
		m_pOut->GetMyMultiHead()->IndexOfTabular = -1;
		m_pOut->GetMyMultiHead()->IndexOfGraphic = -1;
		m_pOut->GetMyMultiHead()->NumberOfLayers = EndLayer;
		//�����ϱ���γ�ȱ߽�������Ҫ����ó�//�������ݵ�����//���ݵ�����				

		///m_pOut->GetMyMultiHead()->ScaleOfData = 10;								//���ݷŴ���//�Ŵ�10�� �������ظ���

		//gInt32	OffsetBytesOfSiteInfo;											//վ����Ϣƫ���ֽ��� //�Զ�����
		//gInt32	OffsetBytesOfLayerOffset;										//��ƫ������Ϣ��ƫ���ֽ��� //�Զ�����
#if defined (WIN32)|| defined(WIN64)
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif
		//��ȡ���� ���ĸ�����://��Ʒ����//��γ�ȷֱ���//����ʶ����//��������"ProductCode"//"AreaCode"//"AreaName"
		//gChar *pTempName		= NULL;												//�˴��ɿ��ǳ�ʼ���㶫����ֹ����ʧ��
		gChar *pTempResLonLat	= NULL;
		gChar *pTempRegionID	= NULL;
		//gChar *pTempRegionName;
		//added by chenxiang 20081125
		//(*m_pParameter)("ProductCName",pTempName);
		////��Ʒ����      
		//if (pTempName == NULL )
		//{
		//	return GE_INVALIDARG;
		//}
		//strcpy(m_pOut->GetMyMultiHead()->ProductCName,pTempName);					//��Ʒ���ƣ�1-15�����֣�
		//
		//strcpy(m_pOut->GetMyMultiHead()->ProductName,"BREF");						//��ƷӢ����д

		//m_ResLonLat = 1000;														//��ʼ��1000
		(*m_pParameter)("ResLonLat",pTempResLonLat);
		if (pTempResLonLat == NULL )
		{
			m_ResLonLat = 1000;//return GE_INVALIDARG;
		}
		m_ResLonLat = (gInt32)atoi(pTempResLonLat);									//��γ�ȷֱ���

		(*m_pParameter)("AreaCode",pTempRegionID);	
		if (pTempRegionID == NULL )
		{
			return GE_INVALIDARG;
		}//����ʶ����
		strcpy(m_pOut->GetMyAreaInfo()->AreaCode,pTempRegionID);					//����ʶ����

		for(int pos = 0;pos<m_SiteNum;pos++)										
		{
			pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));		
			SiteInf TemSiteInf;
			memset(&TemSiteInf, 0,sizeof(TemSiteInf));
#if defined (WIN32)|| defined(WIN64)
#		pragma warning(push)
#		pragma warning(disable:4996)
#endif
			/*		strcpy(TemSiteInf.SiteCode,(gChar*)(pRadarHead->SiteCode));	*/			
			pRadar->GetSiteCode(TemSiteInf.SiteCode);											//���վ��

#if defined (WIN32)|| defined(WIN64)
#		pragma warning(pop)
#endif		
			memcpy(TemSiteInf.RadarType, pRadar->GetHeadP()->RadarType, sizeof(gChar)* 4);
			TemSiteInf.Longitude = pRadar->GetHeadP()->RadarLon;
			TemSiteInf.Latitude = pRadar->GetHeadP()->RadarLat;
			TemSiteInf.Altitude = pRadar->GetHeadP()->RadarHeight;
			memcpy(TemSiteInf.OperationalMode, pRadar->GetHeadP()->OperationMode, sizeof(gInt8)* 2);
			memcpy(TemSiteInf.VCPMode, pRadar->GetHeadP()->VCPMode, sizeof(gInt8)* 4);
			TemSiteInf.BaseDataDate = pRadar->GetHeadP()->VCPDate;
			TemSiteInf.BaseDataTime = pRadar->GetHeadP()->VCPTime;
			m_pOut->AddMySite(TemSiteInf);				
		}////////////////////////////////////////////////վ��������////////////////////////////////////////////////////

		m_pOut->GetMyMultiHead()->ProductDate = gUint16(TemVCPDate);				//ƴͼ���ݵ�Julian����	//ms by chenxiang20081125
		m_pOut->GetMyMultiHead()->ProductTime = gInt32(TemVCPTime);					//ƴͼ����ʱ��			//ms by chenxiang20081125

		//д�ļ���������ͷ �����ϱ��߽�//��������,����////////////////////////////////////////////////////////////////
		m_pOut->GetMyAreaInfo()->LatitudeOfNorth = gInt32(m_StartLat/10 + 1);		//��ʼγ�� ��  //��λת��0.00001->0.0001 ��ͬ
		m_pOut->GetMyAreaInfo()->LatitudeOfSouth = gInt32(m_EndLat  /10 - 1);		//����γ�� ��
		m_pOut->GetMyAreaInfo()->LongitudeOfWest = gInt32(m_StartLon/10 - 1);		//��ʼ���� ��
		m_pOut->GetMyAreaInfo()->LongitudeOfEast = gInt32(m_EndLon  /10 + 1);		//�������� ��	

		//logfile<<"501_BREF��  Loop All Layers and Call Function LayerMultiProduce() to Process the Layer Data."<<endl;
		/*if(m_pOut->AddMyLayer(m_M,m_N,1)!=GS_OK)
		{
			return GE_NOENOUGHMEMORY;
		}
		*/
		//for(gInt16 Layer = 0;Layer<32&&(pCAPPI_High[Layer] * 250 <= EndHigh);++Layer)	//32��
	
			///EndLayer = Layer;
			/*try
			{*/
			//m_pOut->AddMyLayer(m_M,m_N);											//�Ӳ�
			//m_pOut->GetMyLayer(Layer)->LatitudeOfNorth = gInt32(m_StartLat/10+1);	//��ʼγ�� ��  //��λת��0.00001->0.0001 ��ͬ
			//m_pOut->GetMyLayer(Layer)->LatitudeOfSouth = gInt32(m_EndLat/10-1);		//����γ�� �ϣ�0.0001�ȣ�
			//m_pOut->GetMyLayer(Layer)->LongitudeOfEast = gInt32(m_EndLon/10+1);		//�������� ��
			//m_pOut->GetMyLayer(Layer)->LongitudeOfWest = gInt32(m_StartLon/10-1);	//��ʼ���� ��
			m_pOut->GetMyLayer(0)->HeightOfElevation = pCAPPI_High[Layer] * 250/10;//���ݲ�߶�ֵ��10�ף�
			m_pOut->GetMyLayer(0)->SNOfLayer = Layer ;
			//}
			//catch(...)																//���ﻹ���ܲ����쳣
			//{
			//	return GE_FAIL;
			//}
			m_pOut->GetMyGrid_Head(0)->ResolutionOfRow = gInt16(m_ResLonLat);						//�����ࣨ0.00001�ȣ�(����ֱ���)
			m_pOut->GetMyGrid_Head(0)->ResolutionOfCol = gInt16(m_ResLonLat);						//γ���ࣨ0.00001�ȣ�(γ��ֱ���)
			m_pOut->GetMyGrid_Head(0)->NumberOfRows = m_M =gInt16 ((m_StartLat-m_EndLat)/m_ResLonLat+2);//��������
			m_pOut->GetMyGrid_Head(0)->NumberOfCols = m_N = gInt16((m_EndLon-m_StartLon)/m_ResLonLat+2);//��������
           logfile<<"m_M="<<m_M<<"   m_N="<<m_N<<" m_ResLonLat="<<m_ResLonLat<<" m_ResLonLat="<<m_ResLonLat<<endl;
		///EndLayer++;
		//��Ӳ�Ͳ�ͷ��Ϣ���///////////////

			logfile<<"501_BREF��  Loop All Layers and Call Function LayerMultiProduce() to Process the Layer Data11."<<endl;
		//�����ݴ���ʼ////////////////////////////////////////////////////////////////////////////////////
			logfile<<"pCAPPI_High[Layer]="<<pCAPPI_High[Layer]<<endl;
			if(LayerMultiProduce(pCAPPI_High[Layer]* 250,0) != GS_OK)		//�Ƿ�ִ�гɹ�
				//if(LayerMultiProduce(gInt16(pCAPPI_High[31]) * 250,31)!=GS_OK)			//�Ƿ�ִ�гɹ�
				continue;																//�������ʧ������Ա���
		//�����ݴ������////////////////////////////////////////////////////////////////////////////////////
			logfile<<"501_BREF��  Loop Finished22."<<endl;
	}

	logfile<<"501_BREF��  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
gInt32 Arith_501_BREF::Max(gInt32* p,gInt16 Num)							//ȡ32λ�����������������
{
	gInt16 i = 0;
	gInt32 MaxNum = p[0];
	bool HaveAvailability=false;
	if(Num>0)
	{
		for(i = 0;i<Num;i++)
		{
			if(p[i]>=MaxNum&&!m_ErrorData[i])
			{
				MaxNum = p[i];
				HaveAvailability=true;
			}
			else
			{
				if ( i+1 < Num && !HaveAvailability)
				{
					MaxNum = p[i+1];
				}
			}
		}
	}
	return MaxNum;
}
gInt32 Arith_501_BREF::Min(gInt32* p,gInt16 Num)							//ȡ32λ����������С������
{
	gInt16 i = 0;
	gInt32 MinNum = p[0];
	bool HaveAvailability=false;
	if(Num>0)
	{
		for(i = 0;i<Num;i++)
		{
			if(p[i]<=MinNum && !m_ErrorData[i])
			{
				MinNum = p[i];
				HaveAvailability=true;
			}
			else
			{
				if ( i+1 < Num && !HaveAvailability)
				{
					MinNum = p[i+1];
				}
			}
		}
	}
	return MinNum;
}
gInt16 Arith_501_BREF::Min(gInt16* p,gInt16 Num)							//ȡ16λ����������С������
{
	gInt16 i = 0;
	gInt16 MinNum = p[0];
	bool HaveAvailability=false;
	if(Num>0)
	{
		for(i = 0;i<Num;i++)
		{
			if(p[i]<=MinNum && !m_ErrorData[i])
			{
				MinNum = p[i];
				HaveAvailability=true;
			}
			else
			{
				if ( i+1 < Num && !HaveAvailability)
				{
					MinNum = p[i+1];
				}
			}
		}
	}
	return MinNum;
}
 //CalculateCrossRange()��ȸ������Բ׶���ˮƽ���롢ˮƽ������ʼ���ս� �浽EI_Distance[������+2]��
GHRESULT Arith_501_BREF::CalculateCrossRange(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance)
{//����˵��:Ŀ������״�վ����Ը߶�(ʵ��),��������ָ��,վ�����,������cappi�߶Ȳ㽻��ͶӰ�����ĵ�ˮƽ��������
	gFloat A;																//�����ʽ2����ϵ��
	gFloat B;																//�����ʽ1����ϵ��
	gFloat C;																//�����ʽ0����ϵ��
	gFloat a;																//a����(����)
	gFloat L;																//б��(��)
	memset(EI_Distance,0,sizeof( gInt32 ) * ( m_pPPI_L[pos] + 2 ) ) ;		//��ֵ��ʼ��EI_Distance

	gInt32 max_bin = MaxLong;
	//if (SiteCompCappi_H>10000)
	//{
	//	int aa=1;
	//}
	for(int i = 0;i<m_pPPI_L[pos];i++)										//�����(Ӧ����������) ͬһ���ǵļ�����뾶��һ���� ע�������i�����Ǳ���
	{	
		// Added for CINRAD/CC~~~~~~~~~~~~~~~~~~~~~~~~
		max_bin = m_pGateLen[pos] * (m_pCap_gates[pos][i]);					//�ⳤ*����   б�� ��
		( max_bin > MaxLong ) ? max_bin = MaxLong : max_bin;				//���̽������д�ȷ�� //б�����460���ﰴ460������,�ⳤ��������

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		a = el_tilts[i] * m_Escale ;										//a����	
		//����ע�ⵥλת��////////
		A = cos(a) * cos(a);
		//A = 1.0f;
		B = 17000 * sin(a);
		C = -17000 * (gFloat(SiteCompCappi_H)/1000);						//�˴����ȵ�λ����ǧ��
		L = ( sqrt( B * B - 4 * A * C ) - B ) / (2 * A) * 1000;				//�����ʽ��б�൥λ �� //û���жϳ����Ƿ�Ϊ0(�ȶ���Ч�ʵ�ì��)
		if(L > max_bin )
		{
			L = (gFloat)max_bin;
		}
		EI_Distance[m_pPPI_L[pos]-i] = (gInt32)( atan(L * cos(a)/(L * sin(a)+REarth+m_pSiteHigh[pos])) * (REarth+m_pSiteHigh[pos]) + 0.5f);//��λ�� �˴����ܿ��Խ��� ������ж���
		//EI_Distance[m_pPPI_L[pos]-i] = gInt32(cos(a) * L);
		if( EI_Distance[m_pPPI_L[pos]-i] >max_bin )							//�����±�� m_pPPI_L[pos]��1 ��m_pPPI_L[pos]��
		{
			EI_Distance[m_pPPI_L[pos]-i] = max_bin;
		}
	}
	//������Ϻ��������ǲ�������ڵ�ͶӰˮƽ�����ĵ�֮��ľ���
	EI_Distance[0] = EI_Distance[1]-(EI_Distance[2]-EI_Distance[1])/2;		//��ߵļ����ϲ�����б��ͶӰ��ˮƽ����(��ʼˮƽ����)
	EI_Distance[m_pPPI_L[pos]+1] = EI_Distance[m_pPPI_L[pos]] + (EI_Distance[m_pPPI_L[pos]]-EI_Distance[m_pPPI_L[pos]-1])/4;//������ǵļ����²�����б��ͶӰ��ˮƽ����(��ֹˮƽ����)
	if( EI_Distance[m_pPPI_L[pos]+1] > (m_pCap_gates[pos][0]-1)*m_pGateLen[pos] )								//����ˮƽ����
	{
		EI_Distance[m_pPPI_L[pos]+1] = (m_pCap_gates[pos][0]-1) * m_pGateLen[pos];
	}
	return GS_OK;
}
//����������ǲ�ֵϵ���ŵ�m_pCappiCoefficient[i]��
GHRESULT Arith_501_BREF::CreateCappiTab(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance)
{	//����˵��://��Ը߶�,��������ָ��,վ�����,������cappi�߶Ȳ㽻��ͶӰ�����ĵ�ˮƽ��������
	gInt16 i;																//ˮƽ����
	gUint8 j;																//���Ʊ�ʾ���ǵ�
	gUint8 k;																//�������
	gInt16 TempStartBin;													//��ʼ��ˮƽ����
	gInt16 TempEndBin;														//������ˮƽ����
	gFloat EAlpha; 															//������
	gFloat EAlpha2;															//������
	gFloat cosa;
	gFloat cosb;															//��������cosֵ//�򻯹�ʽ��
	gFloat tR;
	gFloat bR;																//�����������߽����ϵľ������ ʵ��
	gInt16 tH;
	gInt16 bH;																//�����������߽����ϸ߶� ��
	gFloat Coefficient = m_ResLonLat / 1000.0f;								//�ֱ�����1000(Ĭ�Ϸֱ���)�ı���

	//gInt32 max_bin = m_pGateLen[pos] * m_pCap_gates[pos];					//�ⳤ*����   б�� ��
	//if (max_bin==0)														//added by chenxiang 20080625 �ⳤ�����Ϊ0�ж�
	//{
	//	return GS_FALSE;
	//}
	//if( max_bin > MaxLong )												//���̽������д�ȷ��
	//{
	//	max_bin = MaxLong;													//б�����460���ﰴ460������,�ⳤ��������
	//}
	gInt16 MaxGates[20] = {0};
	for (int i=0;i<20;i++)
	{
		MaxGates[i] = gInt16( m_pCap_gates[pos][i] -1 );						//�ض�������
	}
	
	memset(m_pCappiCoefficient,0,sizeof(BREFCAPSTRUCT) * m_pCap_gates[pos][0]);//��ֵ��ʼ��m_map
    ///////////////////////////////////////////////
	// R(1,0)~R1֮��ȡ��һ���Ӧ������ֵ
	EAlpha = el_tilts[m_pPPI_L[pos]-1] * m_Escale;							//��m_pPPI_L[pos]-1(��Ϊ���ǹ�m_pPPI_L[pos]�������Ǵ�0��ʼ���)������ Ҳ�����������
	TempStartBin = (gInt16)(gFloat(EI_Distance[0])/m_pGateLen[pos]+0.5f);	//����ˮƽ������Ҳ�Կⳤ���Կ������ָ��� ������������Ǹ��������ǵ�ˮƽ����  ��
	TempEndBin = (gInt16)(gFloat(EI_Distance[1])/m_pGateLen[pos]+0.5f);		//������ǵ�ˮƽ����  ��
	if(TempStartBin == TempEndBin&&m_pCappiCoefficient[TempEndBin].n == 0)
	{
		//m_pCappiCoefficient[TempStartBin].r = TempStartBin;				//ˮƽ����
		m_pCappiCoefficient[TempStartBin].n = 1;							//ȡֵ��� 1 ȡ�²� Ҳ�����ڲ�����ȷ�Χ�ڵ�ֵ��һ��
		m_pCappiCoefficient[TempStartBin].be = m_pPPI_L[pos]-1;				//����˵��9��������ô�������8,����������ǵ���� ��0��ʼ���� ��߲�

		//bR = TempStartBin/cos(EAlpha);//���㾶���ϵĿ����õ�*/
		bR = REarth*sin(gFloat(TempStartBin * m_pGateLen[pos])/REarth) / cos(EAlpha+TempStartBin * m_pGateLen[pos]/REarth) / m_pGateLen[pos];//��֪����\ˮƽ�����������
		m_pCappiCoefficient[TempStartBin].bR = (gInt16)bR;					//�����Ǿ������
		m_pCappiCoefficient[TempStartBin].bh2 = (gUint8)(100 * (bR - m_pCappiCoefficient[TempStartBin].bR) + 0.5f);//�²�б�����ֵϵ��(������λ*100) ����λ�ðٷֱ�
		m_pCappiCoefficient[TempStartBin].bh1 = 100-m_pCappiCoefficient[TempStartBin].bh2 ;//�²�б��ǰ���ֵϵ��
		m_pCappiCoefficient[TempStartBin].bR  = (gInt16)(m_pCappiCoefficient[TempStartBin].bR * Coefficient + 0.5f);		//��ȡ����ʱ�õ�
		m_pCappiCoefficient[TempStartBin].bf = 100;							//�´�ֱϵ�����Ŵ�100��
	}
	for(i = TempStartBin;i<TempEndBin;i++)									//��0��1ˮƽ�����ϵĵ�n������1
	{
		if(m_pCappiCoefficient[i].n == 0)									//���nû��д��
		{
			//m_pCappiCoefficient[i].r = i;									//CAPPI�ľ���,����ˮƽ��ķֱ��� �����귽ʽ �����ڿ���
			m_pCappiCoefficient[i].n = 1;									//ȡֵ��� 1 ȡ�²� Ҳ�����ڲ�����ȷ�Χ�ڵ�ֵ��һ��
			m_pCappiCoefficient[i].be = m_pPPI_L[pos]-1;					//����˵��9��������ô�������8,����������ǵ���� ��0��ʼ���� ��߲�

			//bR = i/cos(EAlpha);//���㾶���ϵĿ����õ�*/
			bR = REarth * sin(gFloat(i * m_pGateLen[pos])/REarth) / cos(EAlpha+i * m_pGateLen[pos]/REarth) / m_pGateLen[pos];//��֪����\ˮƽ�����������
			if( bR > MaxGates[m_pCappiCoefficient[i].be] )
			{
				bR = MaxGates[m_pCappiCoefficient[i].be];
			}
			m_pCappiCoefficient[i].bR = (gInt16)bR;							//�����Ǿ������
			m_pCappiCoefficient[i].bh2 = (gUint8)(100 * (bR - m_pCappiCoefficient[i].bR) + 0.5f);//�²�б�����ֵϵ��(������λ*100) ����λ�ðٷֱ�
			m_pCappiCoefficient[i].bh1 = 100-m_pCappiCoefficient[i].bh2 ;	//�²�б��ǰ���ֵϵ��
			m_pCappiCoefficient[i].bR  = (gInt16)(m_pCappiCoefficient[i].bR * Coefficient + 0.5f);		//��ȡ����ʱ�õ�
			m_pCappiCoefficient[i].bf = 100;								//�´�ֱϵ�����Ŵ�100��
		}
	}
	//////////////////////////////////////////////
	EAlpha = el_tilts[0] * m_Escale;										//��0������ Ҳ����������ǵ�ֵ	
	TempEndBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]+1])/m_pGateLen[pos]/*+0.5f*/);//�����i�Ĵ�С
	if( TempEndBin >MaxGates[0]) 
	{
		TempEndBin = MaxGates[0] ;												//ˮƽ��ֹ��������
	}
	/*gFloat IK2 = 0.0;//����IK2��for�����i/m_pGateLen[pos]ƽ�� ���ڼ򻯹�ʽ*/
	gInt32 R = REarth;														//���ڼ򻯹�ʽ
	gFloat B = 0.0;															//���ڼ򻯹�ʽ//
	gInt32 RH = REarth + m_pSiteHigh[pos] + SiteCompCappi_H;				//���ڼ򻯹�ʽ
	gFloat RHsinB;
	gFloat R_RHcosB;														//�㷨�Ż�
	//��������ļ���///////////////////
	for( i = TempStartBin; i <= TempEndBin; i++ )							//���Ǵ������i�������i����
	{
		/*IK2 = i * i * m_pGateLen[pos] * m_pGateLen[pos] ;
		m_pCappiCoefficient[i].mR = gInt16( sqrt( IK2 * ( 1 + IK2 / 17000.0 / 17000.0 - 2 * SiteCompCappi_H / 17000.0 ) + SiteCompCappi_H * SiteCompCappi_H ) / m_pGateLen[pos] + 0.5 );
		//Ŀ������״����ĵľ���,���ݲ�߹�ʽ�ͽǱ߹�ϵ�ó�,���������Ȩ��*/
		//�����Ǹ���ƽ����� �߶���б���ϵĿ��� �������Ҷ����Ƶ��ó�
		B = gFloat(i * m_pGateLen[pos]) / REarth;

		RHsinB = RH * sin(B);												//�㷨�Ż�
		R_RHcosB = R - RH * cos(B);											//�㷨�Ż�

		m_pCappiCoefficient[i].mR = gInt16(sqrt( R_RHcosB * R_RHcosB + RHsinB * RHsinB ) / m_pGateLen[pos] +0.5f);
		if (m_pCappiCoefficient[i].mR > MaxGates[ m_pCappiCoefficient[i].be ])//�²����ǿ���
		{
			m_pCappiCoefficient[i].mR = MaxGates[ m_pCappiCoefficient[i].be ];
		}	
	}
	//�㷨�Ż�TempStartBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]])/m_pGateLen[pos]+0.5);//���迪ʼi
	TempStartBin = (gInt16)((EI_Distance[m_pPPI_L[pos]])/m_pGateLen[pos]);	//���迪ʼi//������� ��
	gFloat tempEiLR = 0.;

	//������С����ͶӰ�������i֮��ı���
	for( i = TempStartBin;i <= TempEndBin;i++)								
	{
		if(m_pCappiCoefficient[i].n == 0)									//���nû��д��
		{
			//m_pCappiCoefficient[i].r = i;									//�����ĵĿ���
			m_pCappiCoefficient[i].n = 3;									//ֻ���ϲ�
			m_pCappiCoefficient[i].te = 0;									//������ǵ����0

			//tR = i/cos(EAlpha);//���㾶�����*/
			tempEiLR = gFloat(i * m_pGateLen[pos])/REarth;					//Բ�Ľ�(����)
			tR  =  REarth * sin(tempEiLR) / cos(EAlpha+tempEiLR) / m_pGateLen[pos];//��֪����\ˮƽ�����������
			if( tR > gFloat(MaxGates[m_pCappiCoefficient[i].te]) )
			{
				tR = gFloat(MaxGates[m_pCappiCoefficient[i].te]);
			}
			m_pCappiCoefficient[i].tR  = (gInt16)tR;
			m_pCappiCoefficient[i].th2 = (gUint8)(100 * (tR-m_pCappiCoefficient[i].tR)+0.5f) ;
			m_pCappiCoefficient[i].th1 = 100-m_pCappiCoefficient[i].th2 ;
			m_pCappiCoefficient[i].tR  = (gInt16)(m_pCappiCoefficient[i].tR * Coefficient + 0.5f);	//��ȡ����ʱ�õ�
			m_pCappiCoefficient[i].tf  = 100;								//�ϴ�ֱϵ�����Ŵ�100��
		}
	}

	//���㴹ֱ��б��ֵϵ��////////////
	k = m_pPPI_L[pos]-1;													//el_tilts[m_pPPI_L[pos]-1] k�������,��������
	j = 1 ;																	//EI_Distance[1] ˮƽ ��������
	EAlpha = el_tilts[k] * m_Escale;										//���Ǵ�������ǿ�ʼ
	cosa = cos(EAlpha);
	TempStartBin = (gInt16)(gFloat(EI_Distance[j])/m_pGateLen[pos]+0.5f);	//0��m_pPPI_L[pos]+1�����������j��1��m_pPPI_L[pos]
	i = TempStartBin;														//���ڿ�ʼ

	while( j < m_pPPI_L[pos] && k >= 1)										//���Ǳ���EI_Distance[j]�����������
	{
		EAlpha2 = el_tilts[k-1] * m_Escale;									//����һ�����ǵ�ֵ
		cosb = cos(EAlpha2);
		TempEndBin = (gInt16)(gFloat(EI_Distance[j+1])/m_pGateLen[pos]/*+0.5f*/);//ȡ������ڵ���ԭֵ
		//if( TempEndBin >= MaxGates[0] )
		//{
		//	TempEndBin = MaxGates[0] ;										//��������������1
		//}
		while( i <= TempEndBin)												//���Ǽ��ˮƽ��������
		{
			//m_pCappiCoefficient[i].r = i;
			m_pCappiCoefficient[i].te = k;
			m_pCappiCoefficient[i].be = k - 1;
			//tR = i/cosa ;//���㾶�����*/
			tempEiLR = gFloat(i * m_pGateLen[pos])/REarth;					//Բ�Ľ�(����)
			tR = REarth * sin(tempEiLR) / cos(EAlpha+tempEiLR) / m_pGateLen[pos];//б���ϵĸ����ǿ���(ʵ��)//��֪����,ˮƽ������ �������
			if( tR>gFloat(MaxGates[k]-1) )
			{
				tR = gFloat(MaxGates[k]-1);
				m_pCappiCoefficient[i].n = 1;
				m_pCappiCoefficient[i].bf = 100;
			}
			m_pCappiCoefficient[i].tR  = (gInt16)tR;
			m_pCappiCoefficient[i].th2 = (gUint8)(100 * (tR-m_pCappiCoefficient[i].tR)+0.5f) ;	//�ϲ�б�����ֵϵ�����Ŵ�100��
			m_pCappiCoefficient[i].th1 = 100-m_pCappiCoefficient[i].th2 ;						//�ϲ�б��ǰ���ֵϵ�����Ŵ�100��
			m_pCappiCoefficient[i].tR  = (gInt16)(m_pCappiCoefficient[i].tR * Coefficient + 0.5f);//��ȡ����ʱ�õ�
			tH = gInt16(tR * m_pGateLen[pos] * sin(EAlpha) + tR * m_pGateLen[pos] * cosa * tR * cosa * m_pGateLen[pos] /17000000 + 0.5f);//������Ĳ�߹�ʽ �߶� ��λm

			//bR = i/cosb;//
			bR = REarth * sin(tempEiLR) / cos(EAlpha2+tempEiLR) / m_pGateLen[pos];	//б���ϵĵ����ǿ���(ʵ��)//��֪����,ˮƽ������ �������
			if( bR> gFloat(MaxGates[k-1]-1) )
			{
				bR = gFloat(MaxGates[k-1]-1);
				m_pCappiCoefficient[i].n = 4;	//���¶�û��
				m_pCappiCoefficient[i].tf = 100;
			}
			m_pCappiCoefficient[i].bR  = (gInt16)bR;
			m_pCappiCoefficient[i].bh2 = (gUint8)(100 * (bR-m_pCappiCoefficient[i].bR)+0.5f) ;
			m_pCappiCoefficient[i].bh1 = 100-m_pCappiCoefficient[i].bh2 ;
			m_pCappiCoefficient[i].bR  = (gInt16)(m_pCappiCoefficient[i].bR * Coefficient + 0.5f);	//��ȡ����ʱ�õ�

			bH = gInt16(bR * m_pGateLen[pos] * sin(EAlpha2) + bR * m_pGateLen[pos] * cosb * cosb * bR* m_pGateLen[pos] /17000000+0.5f);	//��߹�ʽ//ע�ⵥλ
			if(m_pCappiCoefficient[i].n == 0)								//���nû����¼��
			{
				if( (tH < SiteCompCappi_H)||(i == TempEndBin) )				//1=ȡ�²㣬2=���²��ֵ�� 3=ȡ�ϲ�  ,4=���¶�û��
				{
					m_pCappiCoefficient[i].n = 1;	
					m_pCappiCoefficient[i].bf = 100;
				}
				else if( bH > SiteCompCappi_H )
				{
					m_pCappiCoefficient[i].n = 3;	
					m_pCappiCoefficient[i].tf = 100;
				}
				else
				{				
					if (tH-bH != 0)																					//added by chenxiang 20080625 for ������Ϊ0�ж�
					{	m_pCappiCoefficient[i].n = 2;
						m_pCappiCoefficient[i].tf = gUint8(gFloat(SiteCompCappi_H-bH)/(tH-bH) * 100+0.5f);			//�ϴ�ֱϵ��
						m_pCappiCoefficient[i].bf = gUint8(gFloat(tH-SiteCompCappi_H)/(tH-bH) * 100+0.5f);			//�´�ֱϵ��
					}
					else
					{
						m_pCappiCoefficient[i].n = 1;	
						m_pCappiCoefficient[i].bf = 100;
					}
				}
			}//*/
			i++ ;
		}
		j++;
		k--;
		EAlpha = EAlpha2;
		cosa = cosb;
	}
	return GS_OK;
}

 //��վ�����ܺ��� ���������洢�ķ����ʺ;���Ȩ������(ÿ��ÿվִ��һ��)
GHRESULT Arith_501_BREF::SingleProduce( gInt16 CAPPI_High,gInt16 *PolCappiLayerR,gUint8 *PolCappiLayerWeight,gInt16 pos) 
{	//����˵��:cappi�߶�,�����귽ʽ����ķ���������(��λ��,ˮƽ����)(һվһ����һ��),������洢��Ȩ��,վ�����
	gInt32 SiteCompCappi_H = CAPPI_High-m_pSiteHigh[pos];								//CAPPI����״�߶�, ��
	//const gInt32 iThreshR = 150; 														//���޾���150���� ������޵Ļ� 460�����Ȩ��Ϊ0 ��λǧ��
	gInt32 TemInt = 0;		
	//��ʼ��PolCappiLayer ������洢�� ������ �� Ȩ��
	//wmemset((gWchar*)PolCappiLayerR,((gWchar)(0x8000)),sizeof(gInt16) * m_pCap_gates[pos] * m_pCap_nrays[pos]/2);
	gWmemset((gWchar*)PolCappiLayerR,((gWchar)(0x8000)),sizeof(gInt16) * m_pCap_gates[pos][0] * m_pCap_nrays[pos]/2);
	memset(PolCappiLayerWeight,0,sizeof(gUint8) * m_pCap_gates[pos][0] * m_pCap_nrays[pos]);
	//�˴��迼�ǿ���~~~~~~~~~~~~
	if( m_pPPI_L[pos] < 3 ) 
	{
		return GS_FALSE;																//����3������
	}//~~~~~~~~~~~~~~~~~~~~~~~~~
	m_Escale  =  gFloat(M_PI) / 180 / 100;												//�Ƕ�/100��ת�����õ�ϵ��(��վ��) �Ƕȵ�λ0.01��
	gInt32 EI_Distance[18] = {0};														//m_pPPI_L[pos]+2���ˮƽ�����һ�������0��m_pPPI_L[pos]+1(��վ��)18���þ������� new(::std::nothrow)�Ĵ���
	m_pCappiCoefficient  =   new(::std::nothrow) BREFCAPSTRUCT[m_pCap_gates[pos][0]];					//����һ���ṹ���͵�����,��Ų�ͬ�����еĲ�ֵϵ��������(��վ��)
	if(m_pCappiCoefficient == NULL)
	{
		return GE_NOENOUGHMEMORY;
	}
	///////��ʼ������//////////////////////////////////////
	gInt16 *el_tilts=m_el_tilts[pos];
	if( CalculateCrossRange(SiteCompCappi_H,el_tilts, pos,EI_Distance) != GS_OK||		//��ȸ������Բ׶���ˮƽ���롢ˮƽ������ʼ���ս� �ŵ�EI_Distance[m_pPPI_L[pos]]��
		CreateCappiTab(SiteCompCappi_H,el_tilts,pos,EI_Distance) != GS_OK )				//����������ǲ�ֵϵ���ŵ�m_pCappiCoefficient[i]��
	{
		delete []m_pCappiCoefficient;
		m_pCappiCoefficient = NULL;
		return GS_FALSE;
	}
	m_StartBin = (gInt16)(gFloat(EI_Distance[0])/m_pGateLen[pos]+0.5f);					//��ʼ=�����������ͶӰ
	if ( m_StartBin == 0 )
	{
		m_StartBin = 1;
	}
	m_EndBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]+1])/m_pGateLen[pos]/*+0.5f*/);		//��ֹ=�����������ͶӰ
	if( m_EndBin > m_pCap_gates[pos][0]-1)
	{
		m_EndBin = m_pCap_gates[pos][0]-1 ;												//��������
	}
	gInt16 tZ;																			//�ϲ㷴����
	gInt16 bZ;																			//�²㷴����
	gInt16 *pTempte = NULL;
	gInt16 *pTempbe = NULL;
	gUint8 expTable[462] = {
		200,200,200,200,200,200,200,200,199,199,199,199,199,199,198,198,198,197,197,197,196,196,196,195,195,195,
		194,194,193,193,192,192,191,191,190,189,189,188,188,187,186,186,185,184,184,183,182,181,181,180,179,178,
		177,177,176,175,174,173,172,171,170,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,
		153,152,150,149,148,147,146,145,144,143,142,141,140,138,137,136,135,134,133,132,131,129,128,127,126,125,
		124,123,121,120,119,118,117,116,115,113,112,111,110,109,108,107,105,104,103,102,101,100,99,98,97,95,94,
		93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,61,60,
		59,58,57,56,55,55,54,53,52,51,50,50,49,48,47,47,46,45,44,44,43,42,42,41,40,40,39,38,38,37,36,36,35,34,34,
		33,33,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,22,21,21,20,20,19,19,19,18,18,18,17,
		17,16,16,16,15,15,15,14,14,14,14,13,13,13,12,12,12,12,11,11,11,11,10,10,10,10,9,9,9,9,9,8,8,8,8,8,7,7,7,
		7,7,7,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0};//eָ���� �㷨�Ż�
	gInt16 **pppDataR = m_pRadarDataR[pos];
	gInt16 TempNRays = m_pCap_nrays[pos];
	gInt32* pCap_gates= m_pCap_gates[pos];													//��վ�����ǿ��������ָ��
	gFloat ScaleB2M  = (gFloat)m_ScaleOfMultiData / m_ScaleOfBaseData ;						//�����Ŵ����ı�ֵ
	for(int jj = 0; jj < m_pCap_nrays[pos]; ++jj)											//��λ�Ǳ���
	{		
		TemInt = jj * m_pCap_gates[pos][0] + m_StartBin;
		for(int ii = m_StartBin; ii <= m_EndBin; ii++,TemInt++)								//ˮƽ��������(�����൱�����Ǳ�����)
		{
			//ˮƽ���㿪ʼ
			if(m_pCappiCoefficient[ii].n == 0 || m_pCappiCoefficient[ii].bR+1 < 0 || m_pCappiCoefficient[ii].be > 20 )
			{
				continue;																	//n=0 ������//Խ�� ����
			}//			
			//pTempte = pRadar->GetRadarDataR(m_pCappiCoefficient[ii].te,jj);
			//pTempbe = pRadar->GetRadarDataR(m_pCappiCoefficient[ii].be,jj);
			pTempte = pppDataR[ m_pCappiCoefficient[ii].te * TempNRays + jj ];
			pTempbe = pppDataR[ m_pCappiCoefficient[ii].be * TempNRays + jj ] ;				//�õ�R����ָ��	
			if (pTempte==NULL || pTempbe==NULL)
			{
				continue;
			}
			PolCappiLayerWeight[TemInt] = expTable[ m_pCappiCoefficient[ii].mR * m_pGateLen[pos] / 1000 ];//ȡ����Ȩ��			
			/*if(	pTempte[m_pCappiCoefficient[ii].tR]	<= RADAR_DATA_OUTOFRANGE&&
				pTempte[m_pCappiCoefficient[ii].tR+1]<= RADAR_DATA_OUTOFRANGE&&
				pTempbe[m_pCappiCoefficient[ii].bR]	<= RADAR_DATA_OUTOFRANGE&&
				pTempbe[m_pCappiCoefficient[ii].bR+1] <= RADAR_DATA_OUTOFRANGE
				)//������Ƕ����޻ز���ģ���������ݵ���� ��ôtz bz�������������ֵ
			{
				tZ=bZ=max(
					max(pTempte[m_pCappiCoefficient[ii].tR],pTempte[m_pCappiCoefficient[ii].tR+1]),
					max(pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1])
						 );
				PolCappiLayerWeight[TemInt]=0;
			}
			else 
			{
				if( pTempte[m_pCappiCoefficient[ii].tR] > -50 && pTempte[m_pCappiCoefficient[ii].tR+1] >-50  )
				{																						 //�ز�ֵ����-50��Ȩ�ط�������
					tZ = gInt16( (pTempte[m_pCappiCoefficient[ii].tR]*m_pCappiCoefficient[ii].th1 + 
							  pTempte[m_pCappiCoefficient[ii].tR+1]*m_pCappiCoefficient[ii].th2+50)/100 );
				}
				else
				{
					tZ = max(pTempte[m_pCappiCoefficient[ii].tR],pTempte[m_pCappiCoefficient[ii].tR+1] );//����ز�ֵС��-50��ȡǰ����еĴ���
					if( tZ<-160 )
					{
						tZ=-160;																		 //��������������
					}
				}
				//�²㷴���ʼ���
				if( pTempbe[m_pCappiCoefficient[ii].bR]>-50 && pTempbe[m_pCappiCoefficient[ii].bR+1]>-50)//�ز�ֵ����-50��Ȩ�ط�������
				{
					bZ = (pTempbe[m_pCappiCoefficient[ii].bR]*m_pCappiCoefficient[ii].bh1 + 
						pTempbe[m_pCappiCoefficient[ii].bR+1]*m_pCappiCoefficient[ii].bh2+50)/100;
				}
				else
				{
					bZ=max( pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1] );//����ز�ֵС��-50��ȡǰ����еĴ���
					if( bZ<-160 )
					{
						bZ=-160;																		//��������������
					}
				}
			}//else*/
			//�㷨�Ż�//������if���ĳ�?:���ʽ���Ч��//Ϊ��߿ɶ��Զ�����ʾ////////////////////////////////////////////////////
			(	pTempte[m_pCappiCoefficient[ii].tR]	 <= RADAR_DATA_OUTOFRANGE&&/*����ģ��-32766*/\
				pTempte[m_pCappiCoefficient[ii].tR+1]<= RADAR_DATA_OUTOFRANGE&&\
				pTempbe[m_pCappiCoefficient[ii].bR]	 <= RADAR_DATA_OUTOFRANGE&&\
				pTempbe[m_pCappiCoefficient[ii].bR+1]<= RADAR_DATA_OUTOFRANGE\
				)?\
			(\
				tZ = bZ = max(\
					max(pTempte[m_pCappiCoefficient[ii].tR],pTempte[m_pCappiCoefficient[ii].tR+1]),\
					max(pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1])\
						 )/* * ScaleB2M���ó�*/ ,\
				PolCappiLayerWeight[TemInt] = 0 \
			)\
			:\
			(\
				( pTempte[m_pCappiCoefficient[ii].tR]   * ScaleB2M  > -5 * m_ScaleOfMultiData &&\
					   pTempte[m_pCappiCoefficient[ii].tR+1] * ScaleB2M  > -5 * m_ScaleOfMultiData  )?\
				(\
					tZ = gInt16( (pTempte[m_pCappiCoefficient[ii].tR]   * ScaleB2M * m_pCappiCoefficient[ii].th1 + \
							 pTempte[m_pCappiCoefficient[ii].tR+1] * ScaleB2M * m_pCappiCoefficient[ii].th2 + 50)/100 /*�˴��̶�Ϊ100*/)\
				)\
				:\
				(\
					tZ = (gInt16)(max(pTempte[m_pCappiCoefficient[ii].tR], pTempte[m_pCappiCoefficient[ii].tR+1] ) * ScaleB2M) ,\
					( tZ < -16 * m_ScaleOfMultiData )?\
					(\
						tZ = -16 * m_ScaleOfMultiData\
					)\
					:\
					(\
						tZ\
					)\
				),\
				//�²㷴���ʼ���
				( pTempbe[m_pCappiCoefficient[ii].bR]   * ScaleB2M > -5 * m_ScaleOfMultiData && \
				  pTempbe[m_pCappiCoefficient[ii].bR+1] * ScaleB2M > -5 * m_ScaleOfMultiData )?\
				(\
					 bZ  = ( (gInt16)(pTempbe[m_pCappiCoefficient[ii].bR]   * ScaleB2M) * m_pCappiCoefficient[ii].bh1 + \
							 (gInt16)(pTempbe[m_pCappiCoefficient[ii].bR+1] * ScaleB2M) * m_pCappiCoefficient[ii].bh2 + 50)/100 \
				)\
				:\
				(\
					bZ = (gInt16)(max( pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1] ) * ScaleB2M),\
					( bZ < -16 * m_ScaleOfMultiData )?\
					(\
						bZ = -16 * m_ScaleOfMultiData\
					)\
					:\
					(\
						bZ\
					)\
				)\
			);//*/

            //��ֱ���㿪ʼ
			/*if(PolCappiLayerWeight[TemInt]!=0)
			{
				if(m_pCappiCoefficient[ii].n==1) 
				{
					PolCappiLayerR[TemInt]=bZ;																		//ֱ�Ӹ��²��ֵ
				}
				else 
				{
					if( m_pCappiCoefficient[ii].n==2)
					{
						PolCappiLayerR[TemInt]=(bZ*m_pCappiCoefficient[ii].bf+tZ*m_pCappiCoefficient[ii].tf+50)/100;//ͨ��Ȩ������õ�ķ�����
					}
					else 
					{
						if( m_pCappiCoefficient[ii].n==3)
						{
							PolCappiLayerR[TemInt]=tZ ;																//ֱ�Ӹ��ϲ�ķ�����ֵ
						}
					}
				}
			}
			else
			{
				PolCappiLayerR[TemInt]=tZ;
			}//*/
			//�㷨�Ż� ��if���תΪ?:���ʽ������ٶ�///////////////////////////////////////////////
			PolCappiLayerR[TemInt] = \
				(PolCappiLayerWeight[TemInt] != 0)? \
				( \
					(m_pCappiCoefficient[ii].n == 1)? \
					( \
						bZ \
					) \
					: \
					( \
						(m_pCappiCoefficient[ii].n == 2)? \
						( \
							(bZ * m_pCappiCoefficient[ii].bf+tZ * m_pCappiCoefficient[ii].tf+50)/100 \
						) \
						: \
						( \
							(m_pCappiCoefficient[ii].n == 3)?\
							( \
								tZ \
							) \
							: \
							( \
								PolCappiLayerR[TemInt] \
							) \
						) \
					 ) \
				) \
				: \
				( \
					PolCappiLayerR[TemInt]/*tZ*/ \
				);//*/

			/*if( PolCappiLayerR[TemInt]<-70 )
			{
				if(PolCappiLayerR[TemInt]!=RADAR_DATA_OUTOFRANGE)						//����Ҫ�Ծ���ģ�����ж�
				{
					PolCappiLayerR[TemInt] = RADAR_DATA_NOECHO;							//��С�͸�-32767 �޻ز�
				}
				PolCappiLayerWeight[TemInt] = 0;										//�����ʺ�Сʱ�������Ȩ��Ϊ0 Ҳ���������
			}//*/
			//�㷨�Ż�//////////////////////////////////////////////////////////////////////////////
			(PolCappiLayerR[TemInt]< -7 * ScaleB2M ) ? \
			((PolCappiLayerR[TemInt] != RADAR_DATA_OUTOFRANGE) ? ((PolCappiLayerR[TemInt] = RADAR_DATA_NOECHO),(PolCappiLayerWeight[TemInt] = 0)) : (0)) \
			:(0);//*/
		}
	}
	if(m_pCappiCoefficient!=NULL)
	{
		delete []m_pCappiCoefficient;
		m_pCappiCoefficient=NULL;
	}
    return GS_OK;
}
 //��������:���㾭γ�߽�,����Ȩ�ش�����,�ļ�д��Ȩ�س˷������ۼ�(ÿ��ִ��һ��)
GHRESULT Arith_501_BREF::LayerMultiProduce(gInt16 CAPPI_High,gInt16 Layer)				//����˵��:����cappi���θ߶�,�����
{									
	//wmemset((gWchar*)(m_pMultifWeight),0x0,sizeof(gUint16) * m_M * m_N/2);			//���Ȩ�ش�����
	memset(m_pMultifWeight,0x0,sizeof(gChar) * m_M * m_N*2);					//���Ȩ�ش�����
	gInt32 Tempxy = 0;
	gInt32 MxN = m_M * m_N;
	memset32((gInt32*)(m_pMultiRW),(unsigned long)RADAR_DATA_NODATA,sizeof(gInt32) * MxN);
	gInt32 Lon0 = 11600000;																//�״�վ�㾭��(��վ��) �����������õĳ�ֵ ��ֹ�޳�ֵ�����Ĵ��� �����ȥ���ļ��е���ֵ
	gInt32 Lat0 = 3900000;																//�״�վ��γ��(��վ��) ͬ��
	gInt16 RadarHigh = 0;																//�״ﺣ�θ߶�(��վ��) Ҳ���������õĳ�ֵ
	int x = 0;																		//������ ��Ӧm_N ��x�� (������ ������)
	int y = 0;																		//������ ��Ӧm_M ��y�� (������ ������)
	gInt16 x0= 0;
	gInt16 y0= 0;																		//�״�վ���Ķ�Ӧ������
	gInt16 * pTem  = NULL;
	gInt32 TemInt  = 0;
	gInt32 TemInt2 = 0;

	gInt16 *PolCappiLayerR = NULL; 														//��ʼ���������������ָ��
	gUint8 *PolCappiLayerWeight = NULL; 												//��ʼ�����Ȩ������ָ��

	gInt32 FillNum[25] = {0,1,2,3,4,5,6,7,9,9 ,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};//{0,1,2,3,3,4,5,6,7,7};//��䳤������//�±�Ϊɨ�����/50����
	//0�ȵ�180�ȵ�10000����Kֵ б������
	gInt32 K[]={1,875,1763,2679,3640,4663,5774,7002,8391,10000,11918,14281,17321,21445,27475,37321,56713,114301,428602,\
				-114301,-56713,-37321,-27475,-21445,-17321,-14281,-11918,-10000,-8391,-7002,-5774,-4663,-3640,-2679,-1763,-875,-1};	
	gInt32 TempNum= 0;
	gInt32 Temp1  = 0;
	gInt32 *pTempLonLatData = NULL;														//��γ������ʱָ��

	gInt32 TempK = 1;																	//��ĳ��λ�Ǿ���ֱ�������ߵ�б��
	gInt16 m = 0;																		//��ʱ���y��������
	gInt16 n = 0;																		//��ʱ���x��������
	gInt16 Round_4 = 0;																	//Բ����������
	gInt32 TempFillNum = 0;

	for(gInt16 pos = 0;pos<m_SiteNum;pos++)												//��ʼվ����
	{
		//list<StandardRadarData>::iterator& pRadar = m_pGenerateProductList->GetAtAs<StandardRadarData>(pos);
		if(m_ErrorData[pos])															//�Ƿ���Ч
		{
			continue;																	//��Ч��������վ
		}
		pTempLonLatData=m_ppLonLatData[pos];
		if ( PolCappiLayerR == NULL )													//MS by chenxiang 20080630
		{
			PolCappiLayerR =  new(::std::nothrow) gInt16[m_pCap_nrays[pos] * m_pCap_gates[pos][0]];			//����������鷴����
		}
		if (PolCappiLayerWeight == NULL)												//MS by chenxiang 20080630
		{
			PolCappiLayerWeight =  new(::std::nothrow) gUint8[m_pCap_nrays[pos] * m_pCap_gates[pos][0]];	//�����������Ȩ��
		}		
		if(PolCappiLayerR == NULL||PolCappiLayerWeight == NULL)
		{
			delete []PolCappiLayerR;
			PolCappiLayerR = NULL;
			delete []PolCappiLayerWeight;
			PolCappiLayerWeight = NULL;
			continue;
		}
		Lon0 = m_pSiteLon[pos];
		Lat0 = m_pSiteLat[pos];
		RadarHigh = m_pSiteHigh[pos];
		if (CAPPI_High < (RadarHigh + 200))
		{
			if (PolCappiLayerR != NULL)													//MS by chenxiang 20080630 ~~~~
			{
				delete []PolCappiLayerR;
				PolCappiLayerR = NULL;
			}
			if (PolCappiLayerWeight != NULL)
			{
				delete []PolCappiLayerWeight;
				PolCappiLayerWeight = NULL;												//�����ڴ�й©
			}																			//End of MS~~~~~~~~~~~~~~~~~~~~
			continue;
		}
		if(SingleProduce( CAPPI_High,PolCappiLayerR,PolCappiLayerWeight,pos) == GS_OK ) //վ���ڴ��� �õ���վ���㼫����ķ����ʺ�Ȩ��
		{
			//��ʼ�Ѽ����������ϵ�Ȩ�ط����ʰᵽ��γ��������(��վ�������� �� ��������)
			TempNum  = 0;
			TempK = 0;
			gInt16 inrays8 =  m_pCap_nrays[pos]/8;
			gInt16 inrays4 =  m_pCap_nrays[pos]/4;
			gInt16 inrays2 =  m_pCap_nrays[pos]/2;
			for(gInt16 j = 0; j < m_pCap_nrays[pos];j++)/*360; j++)	*/					//��λ�Ǳ���
			{
				TemInt = j * m_pCap_gates[pos][0] + m_StartBin;							//������PolCappiLayer�е��±�
				TemInt2= TemInt + TemInt;												//��γ����LonLatData�еľ����±�
				TempK = K[((j%inrays2)/5)];			/* K[((j%180)/5)]; */   				//б�� �Ŵ�10000��  
				x0 = gInt16( ( pTempLonLatData[0] - m_StartLon ) / m_ResLonLat);		//վ��ĸ��������
				y0 = gInt16( ( m_StartLat - pTempLonLatData[1] ) / m_ResLonLat);		//�㷨�Ż�������������
				Round_4 = ((j + inrays8) /inrays4) % 4 ;/*((j + 45) / 90) % 4 ;*/
				for(gInt32 i = m_StartBin;i <= m_EndBin; i++,TempNum= i / 50,TemInt++,TemInt2 += 2)//ˮƽ��������
				{
					//TempNum=i/50;														//i*1000/50000 ��50����һ����λ//FillNum�±�		
					//1.����õ㵽���˺ͱ��˵ĸ�����
					x = gInt16((pTempLonLatData[TemInt2] - m_StartLon) / m_ResLonLat);
					y = gInt16((m_StartLat - pTempLonLatData[TemInt2 + 1]) / m_ResLonLat);//�㷨�Ż�������������
					Temp1 = PolCappiLayerR[TemInt] * PolCappiLayerWeight[TemInt];		//R*W	
					TempFillNum = FillNum[TempNum];
					//��俪ʼ//��Ȼ�����в����ظ�������Ч�ʸ�////////////////////////////////////////////					
					switch( Round_4 )													//��Բ�ֳ�4������ֱ����
					{
					case 0:	n = x;
							for(gInt16 l = 0; l <= TempFillNum; l++,n++)
							{
								m=gInt16( TempK * (n - x) / 10000 + y);
								Tempxy = m * m_N + n;
								Tempxy = (Tempxy >= MxN - 2) ? MxN - 2 : Tempxy;				
								for( int o = 0; o<2; o++ )
								{	
									m_pMultiRW[Tempxy] =(m_pMultiRW[Tempxy] != RADAR_DATA_NODATA&&
														m_pMultiRW[Tempxy] != RADAR_DATA_NOECHO&&
														m_pMultiRW[Tempxy] != RADAR_DATA_OUTOFRANGE)?
														(m_pMultiRW[Tempxy] + Temp1):
														((PolCappiLayerR[TemInt] < RADAR_DATA_OUTOFRANGE + 1) ? max(PolCappiLayerR[TemInt],gInt16(m_pMultiRW[Tempxy])) :Temp1);
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//Ȩ�� �ۼ�  С��!!!!!!����ת�����ܻ������
									//Tempxy = (Tempxy >= m_N) ? Tempxy - m_N : 0,o = 3;
									(Tempxy >= m_N) ? Tempxy -= m_N :o = 3;										
								}
							}
							break;
					case 1:	m = y;
							for(gInt16 l = 0;l <= TempFillNum ; l++,m++)
							{
								n=gInt16( (m - y) * 10000 / TempK + x );
								Tempxy = m * m_N + n;
								for(int o = 0; o < 2; o++)
								{
									(Tempxy >= MxN-2)? Tempxy = MxN-2 : false;
									m_pMultiRW[Tempxy] =(m_pMultiRW[Tempxy] != RADAR_DATA_NODATA&&
														m_pMultiRW[Tempxy] != RADAR_DATA_NOECHO&&
														m_pMultiRW[Tempxy] != RADAR_DATA_OUTOFRANGE)?
														(m_pMultiRW[Tempxy] + Temp1):
														((PolCappiLayerR[TemInt]<RADAR_DATA_OUTOFRANGE+1) ? max(PolCappiLayerR[TemInt],gInt16(m_pMultiRW[Tempxy])) : Temp1);
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//Ȩ�� �ۼ�  С��!!!!!!����ת�����ܻ������
									Tempxy--;
								}
							}
							break;
					case 2:n = x;
							for(gInt16 l = 0;l <= TempFillNum; l++,n--)
							{
								m = gInt16( TempK * (n - x) / 10000 + y);
								Tempxy = m * m_N + n;						
								for(int o = 0; o<2; o++)
								{
									(Tempxy >= MxN-2) ? Tempxy = MxN-2 : false;
									m_pMultiRW[Tempxy] =(m_pMultiRW[Tempxy] != RADAR_DATA_NODATA&&
														m_pMultiRW[Tempxy] != RADAR_DATA_NOECHO&&
														m_pMultiRW[Tempxy] != RADAR_DATA_OUTOFRANGE)?
														(m_pMultiRW[Tempxy] + Temp1):
														((PolCappiLayerR[TemInt]<RADAR_DATA_OUTOFRANGE+1) ? max(PolCappiLayerR[TemInt],gInt16(m_pMultiRW[Tempxy])): Temp1);
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//Ȩ�� �ۼ�  С��!!!!!!����ת�����ܻ������
									Tempxy += m_N;
								}
							}
							break;
					case 3:m = y;
							for(gInt16 l = 0;l <= TempFillNum; l++,m--)
							{
								n = gInt16( ( m - y ) * 10000 / TempK + x );
								Tempxy = m * m_N + n;
								for(int o = 0; o < 2; o++)
								{
									(Tempxy >= MxN-2) ? Tempxy = MxN-2 : false;
									m_pMultiRW[Tempxy] =(m_pMultiRW[Tempxy] != RADAR_DATA_NODATA&&
														m_pMultiRW[Tempxy] != RADAR_DATA_NOECHO&&
														m_pMultiRW[Tempxy] != RADAR_DATA_OUTOFRANGE)?
														(m_pMultiRW[Tempxy] + Temp1):
														((PolCappiLayerR[TemInt]<RADAR_DATA_OUTOFRANGE+1) ? max(PolCappiLayerR[TemInt],gInt16(m_pMultiRW[Tempxy])) : Temp1);
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16( PolCappiLayerWeight[TemInt] );//Ȩ�� �ۼ�  С��!!!!!!����ת�����ܻ������
									Tempxy++;
								}
							}
							break;
					}//*/
					//������///////////////////////////////////////////////
				}//end of for(gInt32 i=m_StartBin;i<=m_EndBin;i++,TemInt++,TemInt2+=2)
			}//end of for(gInt16 j=0;j<m_pCap_nrays[pos];j++)
		}//end of if(SingleProduce(...)==..)
		
		if (PolCappiLayerR != NULL)
		{
			delete []PolCappiLayerR;
			PolCappiLayerR = NULL;
		}
		if (PolCappiLayerWeight != NULL)
		{
			delete []PolCappiLayerWeight;
			PolCappiLayerWeight = NULL;
		}		
	}//end of "for" վ��������
	//���մ���//�ļ��е� �����ʳ�Ȩ�� �ĺ� ���� Ȩ�صĺ�
	for(y = 0;y<m_M;y++)														//������ ��Ӧm ��y��
	{
		pTem = m_pOut->GetMyLineData(Layer,y);
		Tempxy = y * m_N;
		for(x = 0; x<m_N; x++,Tempxy++)											//������ ��Ӧn ��x��
		{			
			//д����//msdfjlasf(XY)->rw / m_pMultifWeight->at(y * n+x);//����"�ļ�"�е� ��(������ * Ȩ��)�� ��Ȩ�� ����Data[NumberOfRows * NumberOfCols]
			pTem[x] =(m_pMultifWeight[Tempxy] != 0)?gInt16( m_pMultiRW[Tempxy] / m_pMultifWeight[Tempxy] ): gInt16( m_pMultiRW[Tempxy] );//*//�㷨�Ż�
		}//for
	}//for
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)								//���ڴ���Arith_501_BREF�ӿ�
{
	return  new(::std::nothrow) Arith_501_BREF;
}

