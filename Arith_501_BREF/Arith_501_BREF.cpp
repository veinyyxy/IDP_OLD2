/////////////////////////////////////////////////////////////////////////
// Arith_501_BREF.cpp
// created by ChenXiang 20080424
// 501号产品,组网算法,BREF
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
#define REarth				6371300				//地球半径 (米)
#endif//REarth
#ifndef MaxLong
#define MaxLong				460000				//最大扫描半径长度 (米)
#endif//MaxLong
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif//max
#ifndef SCALE_OF_BASE_DATA		
#define SCALE_OF_BASE_DATA	100					//基数据数据放大倍数
#endif//SCALE_OF_BASE_DATA
#ifndef SCALE_OF_MULTI_DATA		
#define SCALE_OF_MULTI_DATA	100					//产品数据放大倍数
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
	m_pParameter		= NULL;					//参数指针
	m_pGenerateProductList= NULL;				//输入list
	m_pOutputProductList= NULL;					//输出list
	m_pMultifWeight		= NULL;					//组网权重大数组指针
	m_pMultiRW			= NULL;					//组网反射率乘权重累加大数组指针
	m_ppLonLatData		= NULL;					//极坐标转经纬度数据指针
	m_pRadarDataR		= NULL;					//储存反射率数据径向指针的二位数组
					
	m_SiteLatNBorderline= NULL;					//各站北边界数组
	m_SiteLatSBorderline= NULL;					//各站南边界数组
	m_SiteLonWBorderline= NULL;					//各站西边界数组
	m_SiteLonEBorderline= NULL;					//各站东边界数组
	m_pSiteLon			= NULL;					//站点经度数组指针
	m_pSiteLat			= NULL;					//站点纬度数组指针
	m_pSiteHigh			= NULL;					//站点高度数组指针
	m_pPPI_L			= NULL;					//各站仰角数数组
	m_pCap_nrays		= NULL;					//各站方位角数数组
	m_pCap_gates		= NULL;					//各站最大库数数组
	m_pGateLen			= NULL;					//各站库长数组
	m_el_tilts			= NULL;					//各站仰角值二维数组
	m_ErrorData			= NULL;					//无效数据数组
	m_ScaleOfMultiData	= SCALE_OF_MULTI_DATA;	//产品数据放大倍数
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
	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;				//取得输入数据列表

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
	logfile<<"501_BREF：  Started: "<<endl;

	logfile<<"501_BREF：  Get and Check the m_pInputList's Count."<<endl;

	m_SiteNum = gInt16( m_pGenerateProductList->GetCount() );					//取元素数量//站点数量
	if (m_SiteNum <= 0)															//added by chenxiang 20080625 for 站点数量为0判断
	{
		return GS_FALSE;														//返回失败 站点数为零 下面很多都无法操作
	}
	m_ErrorData= new(::std::nothrow) bool[m_SiteNum];
	memset(m_ErrorData,0,sizeof(bool)*m_SiteNum);
	StandardRadarData * pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData*>(0));//取第0元素//第0个统一基数据文件NULL;
	GRADARDATAHEADER*  pRadarHead = pRadar->GetHeadP();							//第0个站点的头
	GELEVATIONDATAHEAD* pEHead = pRadar->GetElevationHeadP(0);					//第0站第0仰角头

	logfile<<"501_BREF：  Send in and Read Parameters."<<endl;
	gInt32 TemVCPDate = 0;														//各站儒略日的和
	gInt32 TemVCPTime = 0;														//各站毫秒时间的和 //用于求均值
	gChar *pTempVCPDateTime = NULL;		
	gChar *pTempResLonLat	= NULL;
	(*m_pParameter)("ResLonLat",pTempResLonLat);
	if (pTempResLonLat == NULL )
	{
		m_ResLonLat = 1000;//return GE_INVALIDARG;
	}
	m_ResLonLat = (gInt32)atoi(pTempResLonLat);			
	//added by chenxiang 20081125 产品时间/////////
	(*m_pParameter)("ScanDateTime",pTempVCPDateTime);
	if (pTempVCPDateTime == NULL )
	{
		return GE_INVALIDARG;
	}
	//示例:20081120140000
	gChar buffer[32] = {0};
	strcpy(buffer,pTempVCPDateTime);
	buffer[8] = 0;
	//处理日期
	TemVCPDate = (gInt32)atoi(buffer);//日期
	gInt32 Year = TemVCPDate / 10000;
	gInt32 Month= ( TemVCPDate - Year * 10000) / 100;
	gInt32 Day  = TemVCPDate % 100;
	TemVCPDate = CJulMsGMT::Greg2Jul(Year,Month,Day);

	//处理时间
	strcpy(buffer,pTempVCPDateTime);
	for(int i=8;i<14;i++)
	{
		buffer[i-8]=pTempVCPDateTime[i];
	}
	buffer[6] = 0;
	TemVCPTime = (gInt32)atoi(buffer);//时间
	gInt32 T_Hour   = TemVCPTime / 10000;
	gInt32 T_Minute = ( TemVCPTime - T_Hour*10000 ) / 100;
	gInt32 T_Second = TemVCPTime % 100;
	TemVCPTime = CJulMsGMT::Time2MilliSecond(T_Hour,T_Minute,T_Second,0);
	//end of added///////////////////////////////

#if defined (WIN32)|| defined(WIN64)
#	pragma warning(pop)
#endif

	m_SiteLatNBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//站点北端边界数组
	m_SiteLatSBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//站点南端边界数组
	m_SiteLonWBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//站点西端边界数组
	m_SiteLonEBorderline =  new(::std::nothrow) gInt32[m_SiteNum];								//站点东端边界数组
	m_pSiteLon	 =  new(::std::nothrow) gInt32[m_SiteNum];										//站点经度数组//东西经180度会出现麻烦
	m_pSiteLat	 =  new(::std::nothrow) gInt32[m_SiteNum];										//站点纬度数组
	m_pSiteHigh  =  new(::std::nothrow) gInt16[m_SiteNum];										//站点高度数组	
	m_pPPI_L	 =  new(::std::nothrow) gUint8[m_SiteNum];										//各站的仰角数
	m_pCap_nrays =  new(::std::nothrow) gInt16[m_SiteNum];										//各站的方位角数
	m_pCap_gates =  new(::std::nothrow) gInt32[m_SiteNum][20];									//各站的库数[站号][仰角数]
	m_pGateLen	 =  new(::std::nothrow) gInt32[m_SiteNum];										//各站的库长
	//内存申请成功判断
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
	
	logfile<<"501_BREF：  Loop: pos=0-->m_SiteNum("<<m_SiteNum<<") to Get All Sites' Information."<<endl;
	//遍历站点取得信息存到数组中//////////////////////////站遍历开始////////////////////////////////////////
	int AvailabilitySiteNum = 0;												//有效站点数量
	for(int pos = 0;pos<m_SiteNum;pos++)										
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));		
		pRadarHead = pRadar->GetHeadP();										//第pos个站点的头
		if (pRadarHead==NULL || m_ErrorData[pos])
		{
			m_ErrorData[pos]=true;
			continue;
		}
		pEHead = pRadar->GetElevationHeadP(0);									//第pos站第0仰角头
		if (pEHead==NULL || m_ErrorData[pos]/* || pRadar->GetHeadP()->maxAziNum<=0*/)
		{
			m_ErrorData[pos]=true;
			continue;
		}
		if(m_ErrorData[pos]||pRadarHead->ElevationCount<=0||pRadarHead->ReflectivityGateSize<150||(pRadarHead->RadarLon == 0&&pRadarHead->RadarLat ==0))//仰角数判断//库长太小或经纬度同时为0(可能是错误数据)
		{
			m_ErrorData[pos]=true;
			continue;
		}
		m_pSiteLon[pos]  = (pRadarHead->RadarLon) * 100;						//站点经度 注意单位 这里把0.001转为0.00001度
		m_pSiteLat[pos]  = (pRadarHead->RadarLat) * 100;						//站点纬度
		m_pSiteHigh[pos] = gInt16((pRadarHead->RadarHeight)/100);				//雷达高度单位有0.01米转为1米//省去了四舍五入
		AvailabilitySiteNum++;
		m_pGateLen[pos]	 = (pRadarHead->ReflectivityGateSize);					//强度库长(米) *m_ResLonLat/1000 是为了不同经纬分辨率下使用相应的精度
		m_pPPI_L[pos]    = gUint8(pRadar->GetHeadP()->ElevationCount);			//获取仰角数
		m_pCap_nrays[pos]= pEHead->AzimuthNumber;								//仰角扫描的方位数//认为不同仰角的方位角数都一样

			
	}////////////////////////////////////////////////站遍历结束////////////////////////////////////////////////////
	logfile<<"501_BREF：  Loop Finished."<<endl;

	if(AvailabilitySiteNum==0)
	{
		return GS_FALSE;
	}
	logfile<<"501_BREF：  Define a two-dimension Array(m_el_tilts) to Save Elevation Information."<<endl;
	logfile<<"501_BREF：  Loop to go through All of the Sites."<<endl;
	//定义二维数组(记录仰角值)
	m_el_tilts =  new(::std::nothrow) gInt16*[m_SiteNum];
	for (int pos = 0;pos<m_SiteNum;pos++)										//站点遍历
	{
		if(m_el_tilts == NULL)													//一层失败
		{
			return GE_NOENOUGHMEMORY;
		}
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		pRadarHead = pRadar->GetHeadP();										//第pos个站点的头
		if(m_ErrorData[pos])
		{
			m_el_tilts[pos] =  new(::std::nothrow) gInt16[18];
		}
		else
		{
			m_el_tilts[pos] =  new(::std::nothrow) gInt16[m_pPPI_L[pos]];						//建立二层
		}
		if (m_el_tilts[pos] == NULL)											//二层失败
		{
			for (int i = 0;i <= pos;i++)
			{
				delete []m_el_tilts[i];
				m_el_tilts[i] = NULL;
			}
			return GE_NOENOUGHMEMORY;
		}
	}
		logfile<<"501_BREF：  Loop Finished."<<endl;
	//定义二维数组结束

	//调整分辨率使其大于最大库长(小于最大库长会取不到数据)
	gInt32 MaxGateLen = Max(m_pGateLen,m_SiteNum);
	logfile<<"501_BREF：  Find the Max Gate Length."<<endl;
	if (m_ResLonLat < MaxGateLen )												//虽单位不统一但可近似
	{
		m_ResLonLat = MaxGateLen;
	}
	//分辨率取值范围限制
	if(m_ResLonLat>100000)
	{
		m_ResLonLat=100000;
	}
	if(m_ResLonLat<100)
	{
		m_ResLonLat=100;
	}

	logfile<<"501_BREF：  Loop to Get Every Sites' Elevations and Gate's Length."<<endl;
	//保存每个站点的 每个仰角大小 和 每个仰角的库数
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
			m_pCap_gates[pos][i]=0;//清零
		}
		for(int i = 0; i<m_pPPI_L[pos]; i++)
		{
			pEHead = pRadar->GetElevationHeadP(i);
			pEl_tilts[i] = gInt16(pEHead->Elevation);							 //0.01度 //取这个站的各仰角大小
			m_pCap_gates[pos][i]= (pEHead->ReflectivityGates) * 1000/m_ResLonLat;//强度库数  *1000/m_ResLonLat 是为了不同经纬分辨率下使用相应的精度
		}
	}
	logfile<<"501_BREF：  Loop Finished."<<endl;
	
	for(int pos = 0;pos<m_SiteNum;pos++)										
	{			
		m_pGateLen[pos]	 = m_pGateLen[pos]   * m_ResLonLat/1000;				// 强度库长(米) *m_ResLonLat/1000 是为了不同经纬分辨率下使用相应的精度
	}
	//32层给定的cappi海拔高度,单位250米
	gInt16 pCAPPI_High[32]={2,3,4,5,6,7,8,9,10,11,12, 14,16,18,20,22,24,26,28,30,32,34,36,38,40,  44,48,52,56,60,64,    72};
	/////////////////////{500米,750米...........3000,3500.................................10000,11000...........16000,18000}
	gInt16 StartHigh = gInt16( Min(m_pSiteHigh,m_SiteNum) );					//取最低站点高度 单位米
	for(int i = 0;i<32;++i)
	{
		if( pCAPPI_High[i] * 250 > StartHigh + 200 )
		{
			StartHigh = pCAPPI_High[i] * 250;									//开始高度等于大于最低雷达站高度200米以上的最低cappi高度
			break;
		}
	}
	gInt16 EndHigh = 18000;														//cappi最高高度 定义为18000m 对流层上界 单位250米

	//更改cappi高度数组 去掉无效层 节省资源//
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
				pCAPPI_High[Tempk++] = 73;										//72是18000米//大于18000米层是的是不处理的
			}
			break;		
		}
	}
	gInt32 DLon = 0;
	gInt32 DLat = 0;

	//1用站点遍历而得到的数据 定义层界限
	for(int pos = 0;pos<m_SiteNum;pos++)
	{
		if (m_ErrorData[pos])
		{
			continue;
		}
		if(CPolC2LonLatReseau::QuickPolC2DLonDLat(0,MaxLong,m_pSiteLon[pos],m_pSiteLat[pos],DLon,DLat,m_pSiteHigh[pos]))//北 南
		{
			m_SiteLatNBorderline[pos] = m_pSiteLat[pos]+DLat;
			m_SiteLatSBorderline[pos] = m_pSiteLat[pos]-DLat;
		}
		else
		{
			CPolC2LonLatReseau::QuickPolC2DLonDLat(0,MaxLong,12000000,5300000,DLon,DLat,0);//为了提高容错性
			m_SiteLatNBorderline[pos] = m_pSiteLat[pos]+DLat;
			m_SiteLatSBorderline[pos] = m_pSiteLat[pos]-DLat;
		}//////////
		if(CPolC2LonLatReseau::QuickPolC2DLonDLat(27000,MaxLong,m_pSiteLon[pos],m_pSiteLat[pos],DLon,DLat,m_pSiteHigh[pos]))//西 东
		{
			m_SiteLonWBorderline[pos] = m_pSiteLon[pos]+DLon;
			m_SiteLonEBorderline[pos] = m_pSiteLon[pos]-DLon;
		}
		else
		{
			CPolC2LonLatReseau::QuickPolC2DLonDLat(27000,MaxLong,12000000,5300000,DLon,DLat,0);//为了提高容错性//取中国的最高纬度这样所产生的经度差最大 使东西边界够用
			m_SiteLonWBorderline[pos] = m_pSiteLon[pos]+DLon;
			m_SiteLonEBorderline[pos] = m_pSiteLon[pos]-DLon;
		}///////////
	}
	//获取东西南北经纬界限
	m_StartLon = Min(m_SiteLonWBorderline,m_SiteNum);							//开始经度 西
	m_EndLon   = Max(m_SiteLonEBorderline,m_SiteNum);							//结束经度 东
	m_StartLat = Max(m_SiteLatNBorderline,m_SiteNum);							//开始纬度 北
	m_EndLat   = Min(m_SiteLatSBorderline,m_SiteNum);							//结束纬度 南

	
	m_M = ((m_StartLat-m_EndLat)/m_ResLonLat+2);							//数组行数
	m_N = ((m_EndLon-m_StartLon)/m_ResLonLat+2);							//数组列数
	logfile<<"501 bref  "<<"m_M="<<m_M<<"  m_N="<<m_N<<"m_StartLat="<<m_StartLat<<"  m_EndLat="<<m_EndLat<<" m_ResLonLat="<<m_ResLonLat<<endl;

	m_pRadarDataR =  new(::std::nothrow) gInt16**[m_SiteNum];
	for (int pos = 0;pos<m_SiteNum;pos++)										//站点遍历
	{
		if(m_pRadarDataR == NULL)												//一层失败
		{
			return GE_NOENOUGHMEMORY;
		}
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(!m_ErrorData[pos])
		{
			m_pRadarDataR[pos] =  new(::std::nothrow) gInt16*[ m_pPPI_L[pos] * m_pCap_nrays[pos] ];//建立二层 (仰角数*方位角数)
		}
		if (m_pRadarDataR[pos] == NULL)											//二层失败
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
	//定义二维数组结束//存指针开始
	gInt32 Temp1;
	for (int pos = 0;pos<m_SiteNum;pos++)										//站点遍历
	{
		pRadar = *(m_pGenerateProductList->GetAtAs<StandardRadarData *>(pos));
		if(m_ErrorData[pos])
		{
			continue;
		}
		for (int j = 0;j<m_pPPI_L[pos];j++)										//仰角遍历
		{
			Temp1 = j * m_pCap_nrays[pos];
			for (int k = 0;k<m_pCap_nrays[pos];k++,Temp1++)						//方位角遍历
			{
				m_pRadarDataR[pos][Temp1] = pRadar->GetRadarDataR(j,k);			//存储指针
			}
		}	
	}//*/
	//获取径向指针存入数组结束//////////////////////////////////////////////////////////////////////////////////////

	//定义权重大数组(单层用)
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

	if(m_pMultifWeight == NULL||m_pMultiRW==NULL)									//判断内存是否申请成功
	{
		return GE_NOENOUGHMEMORY;
	}//权重大数组(单层用)定义完毕////////
	
	//二维数组的 new(::std::nothrow)//m_ppLonLatData是极坐标转经纬度表指针 [站点序号][方位角和库数] to 经度 纬度
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
	//二维数组的 new(::std::nothrow)结束*/

	//计算经纬度表开始
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
					m_ppLonLatData[pos][TempInt_ij] = m_pSiteLon[pos],				//经度  如果处理失败 就把雷达中心的经纬度给它 减小错误
					m_ppLonLatData[pos][TempInt_ij+1] = m_pSiteLat[pos]				//纬度		
				):false;															//算法优化 由if改为?:表达式		
			}
		}
	}//计算经纬度表结束*/

	//添加层和层头信息开始///////////////
	gInt16 EndLayer = 32;															//最高cappi层
	for(gInt16 Layer = 0;Layer<32&&(pCAPPI_High[Layer] * 250 <= EndHigh);Layer++)
	{
		EndLayer = Layer;
	}
	EndLayer++;

	logfile<<"501_BREF：  Loop to Calculate the Array: m_ppLonLatData."<<"EndLayer="<<EndLayer<<endl;
///////////////////////////

	for( int Layer =0; Layer<EndLayer; Layer++)
	{
		m_pOut = NULL;
	    logfile<<"501_BREF：  Create Send-out Product: Product_501_BREF."<<endl;
		m_pOutputProductList->AddTail();											//添加输出列表	
		m_pOut = &(*(m_pOutputProductList->GetAtAs<Product_501_BREF>(Layer)));
		if(m_pOut->Coustruct(m_SiteNum,m_M,m_N,1)!=GS_OK)
		{
			return GE_NOENOUGHMEMORY;
		}
		logfile<<"m_SiteNum ="<<m_SiteNum<<endl;
		logfile<<"m_M ="<<m_M<<endl;
		logfile<<"m_N ="<<m_N<<endl;
		GHRESULT TemRet = m_pOut->CreateP();										//创建
		if(TemRet != GS_OK)
		{
			logfile<<"501_BREF：  Create Product_501_BREF failed and Return."<<endl;
			logfile.close();
			return TemRet;
		}
		//组网信息头写入开始///////////////////////////////////////////////////////////////////////////////
		//m_pOut->GetMyMultiHead()->MessageCode = 249;								//组网产品信息编号

		gUint16 JulDay;
		gInt32  MilliSecond;
		if(!CJulMsGMT::GetLocalJulMs(JulDay,MilliSecond))
		{
			return GS_FALSE;
		}
		m_pOut->GetMyMultiHead()->GenerationDateOfProduct = JulDay;						//拼图处理Julian日期
		m_pOut->GetMyMultiHead()->GenerationTimeOfProduct = MilliSecond;				//拼图处理时间
		//m_pOut->GetMyMultiHead()->ProductCode = 501;									//产品号
		m_ScaleOfMultiData= m_pOut->GetMyMultiHead()->ScaleOfData = SCALE_OF_MULTI_DATA;//产品数据放大倍数
		m_ScaleOfBaseData = SCALE_OF_BASE_DATA;											//基数据放大倍数
		m_pOut->GetMyMultiHead()->IndexOfTabular = -1;
		m_pOut->GetMyMultiHead()->IndexOfGraphic = -1;
		m_pOut->GetMyMultiHead()->NumberOfLayers = EndLayer;
		//东西南北经纬度边界在下面要计算得出//还有数据的行数//数据的列数				

		///m_pOut->GetMyMultiHead()->ScaleOfData = 10;								//数据放大倍数//放大10倍 和上面重复了

		//gInt32	OffsetBytesOfSiteInfo;											//站点信息偏移字节数 //自动生成
		//gInt32	OffsetBytesOfLayerOffset;										//层偏移量信息的偏移字节数 //自动生成
#if defined (WIN32)|| defined(WIN64)
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif
		//读取参数 共四个参数://产品名称//经纬度分辨率//区域识别码//区域名称"ProductCode"//"AreaCode"//"AreaName"
		//gChar *pTempName		= NULL;												//此处可考虑初始化点东西防止读参失败
		gChar *pTempResLonLat	= NULL;
		gChar *pTempRegionID	= NULL;
		//gChar *pTempRegionName;
		//added by chenxiang 20081125
		//(*m_pParameter)("ProductCName",pTempName);
		////产品名称      
		//if (pTempName == NULL )
		//{
		//	return GE_INVALIDARG;
		//}
		//strcpy(m_pOut->GetMyMultiHead()->ProductCName,pTempName);					//产品名称（1-15个汉字）
		//
		//strcpy(m_pOut->GetMyMultiHead()->ProductName,"BREF");						//产品英文缩写

		//m_ResLonLat = 1000;														//初始化1000
		(*m_pParameter)("ResLonLat",pTempResLonLat);
		if (pTempResLonLat == NULL )
		{
			m_ResLonLat = 1000;//return GE_INVALIDARG;
		}
		m_ResLonLat = (gInt32)atoi(pTempResLonLat);									//经纬度分辨率

		(*m_pParameter)("AreaCode",pTempRegionID);	
		if (pTempRegionID == NULL )
		{
			return GE_INVALIDARG;
		}//区域识别码
		strcpy(m_pOut->GetMyAreaInfo()->AreaCode,pTempRegionID);					//区域识别码

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
			pRadar->GetSiteCode(TemSiteInf.SiteCode);											//添加站点

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
		}////////////////////////////////////////////////站遍历结束////////////////////////////////////////////////////

		m_pOut->GetMyMultiHead()->ProductDate = gUint16(TemVCPDate);				//拼图数据的Julian日期	//ms by chenxiang20081125
		m_pOut->GetMyMultiHead()->ProductTime = gInt32(TemVCPTime);					//拼图数据时间			//ms by chenxiang20081125

		//写文件组网数据头 东西南北边界//数组行数,列数////////////////////////////////////////////////////////////////
		m_pOut->GetMyAreaInfo()->LatitudeOfNorth = gInt32(m_StartLat/10 + 1);		//开始纬度 北  //单位转换0.00001->0.0001 下同
		m_pOut->GetMyAreaInfo()->LatitudeOfSouth = gInt32(m_EndLat  /10 - 1);		//结束纬度 南
		m_pOut->GetMyAreaInfo()->LongitudeOfWest = gInt32(m_StartLon/10 - 1);		//开始经度 西
		m_pOut->GetMyAreaInfo()->LongitudeOfEast = gInt32(m_EndLon  /10 + 1);		//结束经度 东	

		//logfile<<"501_BREF：  Loop All Layers and Call Function LayerMultiProduce() to Process the Layer Data."<<endl;
		/*if(m_pOut->AddMyLayer(m_M,m_N,1)!=GS_OK)
		{
			return GE_NOENOUGHMEMORY;
		}
		*/
		//for(gInt16 Layer = 0;Layer<32&&(pCAPPI_High[Layer] * 250 <= EndHigh);++Layer)	//32层
	
			///EndLayer = Layer;
			/*try
			{*/
			//m_pOut->AddMyLayer(m_M,m_N);											//加层
			//m_pOut->GetMyLayer(Layer)->LatitudeOfNorth = gInt32(m_StartLat/10+1);	//开始纬度 北  //单位转换0.00001->0.0001 下同
			//m_pOut->GetMyLayer(Layer)->LatitudeOfSouth = gInt32(m_EndLat/10-1);		//结束纬度 南（0.0001度）
			//m_pOut->GetMyLayer(Layer)->LongitudeOfEast = gInt32(m_EndLon/10+1);		//结束经度 东
			//m_pOut->GetMyLayer(Layer)->LongitudeOfWest = gInt32(m_StartLon/10-1);	//开始经度 西
			m_pOut->GetMyLayer(0)->HeightOfElevation = pCAPPI_High[Layer] * 250/10;//数据层高度值（10米）
			m_pOut->GetMyLayer(0)->SNOfLayer = Layer ;
			//}
			//catch(...)																//这里还不能捕获异常
			//{
			//	return GE_FAIL;
			//}
			m_pOut->GetMyGrid_Head(0)->ResolutionOfRow = gInt16(m_ResLonLat);						//经向间距（0.00001度）(经向分辨率)
			m_pOut->GetMyGrid_Head(0)->ResolutionOfCol = gInt16(m_ResLonLat);						//纬向间距（0.00001度）(纬向分辨率)
			m_pOut->GetMyGrid_Head(0)->NumberOfRows = m_M =gInt16 ((m_StartLat-m_EndLat)/m_ResLonLat+2);//数组行数
			m_pOut->GetMyGrid_Head(0)->NumberOfCols = m_N = gInt16((m_EndLon-m_StartLon)/m_ResLonLat+2);//数组列数
           logfile<<"m_M="<<m_M<<"   m_N="<<m_N<<" m_ResLonLat="<<m_ResLonLat<<" m_ResLonLat="<<m_ResLonLat<<endl;
		///EndLayer++;
		//添加层和层头信息完毕///////////////

			logfile<<"501_BREF：  Loop All Layers and Call Function LayerMultiProduce() to Process the Layer Data11."<<endl;
		//层数据处理开始////////////////////////////////////////////////////////////////////////////////////
			logfile<<"pCAPPI_High[Layer]="<<pCAPPI_High[Layer]<<endl;
			if(LayerMultiProduce(pCAPPI_High[Layer]* 250,0) != GS_OK)		//是否执行成功
				//if(LayerMultiProduce(gInt16(pCAPPI_High[31]) * 250,31)!=GS_OK)			//是否执行成功
				continue;																//如果本层失败则忽略本层
		//层数据处理结束////////////////////////////////////////////////////////////////////////////////////
			logfile<<"501_BREF：  Loop Finished22."<<endl;
	}

	logfile<<"501_BREF：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
gInt32 Arith_501_BREF::Max(gInt32* p,gInt16 Num)							//取32位整型数组最大数函数
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
gInt32 Arith_501_BREF::Min(gInt32* p,gInt16 Num)							//取32位整型数组最小数函数
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
gInt16 Arith_501_BREF::Min(gInt16* p,gInt16 Num)							//取16位整型数组最小数函数
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
 //CalculateCrossRange()求等高面与各圆锥面的水平距离、水平距离起始和终结 存到EI_Distance[仰角数+2]里
GHRESULT Arith_501_BREF::CalculateCrossRange(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance)
{//参数说明:目标点与雷达站的相对高度(实际),仰角数组指针,站点序号,仰角与cappi高度层交点投影到中心点水平距离数组
	gFloat A;																//求根公式2次项系数
	gFloat B;																//求根公式1次项系数
	gFloat C;																//求根公式0次项系数
	gFloat a;																//a仰角(弧度)
	gFloat L;																//斜距(米)
	memset(EI_Distance,0,sizeof( gInt32 ) * ( m_pPPI_L[pos] + 2 ) ) ;		//零值初始化EI_Distance

	gInt32 max_bin = MaxLong;
	//if (SiteCompCappi_H>10000)
	//{
	//	int aa=1;
	//}
	for(int i = 0;i<m_pPPI_L[pos];i++)										//层遍历(应该是仰角数) 同一仰角的极坐标半径是一样的 注意这里的i是仰角遍历
	{	
		// Added for CINRAD/CC~~~~~~~~~~~~~~~~~~~~~~~~
		max_bin = m_pGateLen[pos] * (m_pCap_gates[pos][i]);					//库长*库数   斜距 米
		( max_bin > MaxLong ) ? max_bin = MaxLong : max_bin;				//最大探测距离有待确定 //斜距大于460公里按460公里算,库长上限限制

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		a = el_tilts[i] * m_Escale ;										//a仰角	
		//以下注意单位转换////////
		A = cos(a) * cos(a);
		//A = 1.0f;
		B = 17000 * sin(a);
		C = -17000 * (gFloat(SiteCompCappi_H)/1000);						//此处长度单位暂用千米
		L = ( sqrt( B * B - 4 * A * C ) - B ) / (2 * A) * 1000;				//求根公式求斜距单位 米 //没有判断除数是否为0(稳定与效率的矛盾)
		if(L > max_bin )
		{
			L = (gFloat)max_bin;
		}
		EI_Distance[m_pPPI_L[pos]-i] = (gInt32)( atan(L * cos(a)/(L * sin(a)+REarth+m_pSiteHigh[pos])) * (REarth+m_pSiteHigh[pos]) + 0.5f);//单位米 此处可能可以近似 看误差有多少
		//EI_Distance[m_pPPI_L[pos]-i] = gInt32(cos(a) * L);
		if( EI_Distance[m_pPPI_L[pos]-i] >max_bin )							//数组下标从 m_pPPI_L[pos]到1 共m_pPPI_L[pos]个
		{
			EI_Distance[m_pPPI_L[pos]-i] = max_bin;
		}
	}
	//求出最上和最下仰角波束宽度内的投影水平和中心点之间的距离
	EI_Distance[0] = EI_Distance[1]-(EI_Distance[2]-EI_Distance[1])/2;		//最高的假想上层仰角斜距投影的水平距离(起始水平距离)
	EI_Distance[m_pPPI_L[pos]+1] = EI_Distance[m_pPPI_L[pos]] + (EI_Distance[m_pPPI_L[pos]]-EI_Distance[m_pPPI_L[pos]-1])/4;//最低仰角的假想下层仰角斜距投影的水平距离(终止水平距离)
	if( EI_Distance[m_pPPI_L[pos]+1] > (m_pCap_gates[pos][0]-1)*m_pGateLen[pos] )								//限制水平长度
	{
		EI_Distance[m_pPPI_L[pos]+1] = (m_pCap_gates[pos][0]-1) * m_pGateLen[pos];
	}
	return GS_OK;
}
//计算各个仰角插值系数放到m_pCappiCoefficient[i]里
GHRESULT Arith_501_BREF::CreateCappiTab(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance)
{	//参数说明://相对高度,仰角数组指针,站点序号,仰角与cappi高度层交点投影到中心点水平距离数组
	gInt16 i;																//水平库数
	gUint8 j;																//类似表示仰角的
	gUint8 k;																//仰角序号
	gInt16 TempStartBin;													//开始的水平库数
	gInt16 TempEndBin;														//结束的水平库数
	gFloat EAlpha; 															//上仰角
	gFloat EAlpha2;															//下仰角
	gFloat cosa;
	gFloat cosb;															//上下仰角cos值//简化公式用
	gFloat tR;
	gFloat bR;																//上下仰角竖线交点上的径向库数 实型
	gInt16 tH;
	gInt16 bH;																//上下仰角竖线交点上高度 米
	gFloat Coefficient = m_ResLonLat / 1000.0f;								//分辨率与1000(默认分辨率)的比例

	//gInt32 max_bin = m_pGateLen[pos] * m_pCap_gates[pos];					//库长*库数   斜距 米
	//if (max_bin==0)														//added by chenxiang 20080625 库长或库数为0判断
	//{
	//	return GS_FALSE;
	//}
	//if( max_bin > MaxLong )												//最大探测距离有待确定
	//{
	//	max_bin = MaxLong;													//斜距大于460公里按460公里算,库长上限限制
	//}
	gInt16 MaxGates[20] = {0};
	for (int i=0;i<20;i++)
	{
		MaxGates[i] = gInt16( m_pCap_gates[pos][i] -1 );						//重定最大库数
	}
	
	memset(m_pCappiCoefficient,0,sizeof(BREFCAPSTRUCT) * m_pCap_gates[pos][0]);//零值初始化m_map
    ///////////////////////////////////////////////
	// R(1,0)~R1之间取第一层对应的数据值
	EAlpha = el_tilts[m_pPPI_L[pos]-1] * m_Escale;							//第m_pPPI_L[pos]-1(因为仰角共m_pPPI_L[pos]个并且是从0开始算的)个仰角 也就是最高仰角
	TempStartBin = (gInt16)(gFloat(EI_Distance[0])/m_pGateLen[pos]+0.5f);	//定义水平方向上也以库长乘以库数来分隔点 最高仰角上面那个假想仰角的水平库数  左
	TempEndBin = (gInt16)(gFloat(EI_Distance[1])/m_pGateLen[pos]+0.5f);		//最高仰角的水平库数  右
	if(TempStartBin == TempEndBin&&m_pCappiCoefficient[TempEndBin].n == 0)
	{
		//m_pCappiCoefficient[TempStartBin].r = TempStartBin;				//水平库数
		m_pCappiCoefficient[TempStartBin].n = 1;							//取值标记 1 取下层 也就是在波束宽度范围内的值都一样
		m_pCappiCoefficient[TempStartBin].be = m_pPPI_L[pos]-1;				//比如说有9个仰角那么这个就是8,它是最高仰角的序号 从0开始计数 最高层

		//bR = TempStartBin/cos(EAlpha);//反算径向上的库数用的*/
		bR = REarth*sin(gFloat(TempStartBin * m_pGateLen[pos])/REarth) / cos(EAlpha+TempStartBin * m_pGateLen[pos]/REarth) / m_pGateLen[pos];//已知仰角\水平库数求径向库数
		m_pCappiCoefficient[TempStartBin].bR = (gInt16)bR;					//低仰角径向库数
		m_pCappiCoefficient[TempStartBin].bh2 = (gUint8)(100 * (bR - m_pCappiCoefficient[TempStartBin].bR) + 0.5f);//下层斜距后库插值系数(库数单位*100) 就是位置百分比
		m_pCappiCoefficient[TempStartBin].bh1 = 100-m_pCappiCoefficient[TempStartBin].bh2 ;//下层斜距前库插值系数
		m_pCappiCoefficient[TempStartBin].bR  = (gInt16)(m_pCappiCoefficient[TempStartBin].bR * Coefficient + 0.5f);		//在取数据时用到
		m_pCappiCoefficient[TempStartBin].bf = 100;							//下垂直系数，放大100倍
	}
	for(i = TempStartBin;i<TempEndBin;i++)									//从0到1水平方向上的点n都等于1
	{
		if(m_pCappiCoefficient[i].n == 0)									//如果n没被写过
		{
			//m_pCappiCoefficient[i].r = i;									//CAPPI的距离,像是水平面的分辨率 极坐标方式 类似于库数
			m_pCappiCoefficient[i].n = 1;									//取值标记 1 取下层 也就是在波束宽度范围内的值都一样
			m_pCappiCoefficient[i].be = m_pPPI_L[pos]-1;					//比如说有9个仰角那么这个就是8,它是最高仰角的序号 从0开始计数 最高层

			//bR = i/cos(EAlpha);//反算径向上的库数用的*/
			bR = REarth * sin(gFloat(i * m_pGateLen[pos])/REarth) / cos(EAlpha+i * m_pGateLen[pos]/REarth) / m_pGateLen[pos];//已知仰角\水平库数求径向库数
			if( bR > MaxGates[m_pCappiCoefficient[i].be] )
			{
				bR = MaxGates[m_pCappiCoefficient[i].be];
			}
			m_pCappiCoefficient[i].bR = (gInt16)bR;							//低仰角径向库数
			m_pCappiCoefficient[i].bh2 = (gUint8)(100 * (bR - m_pCappiCoefficient[i].bR) + 0.5f);//下层斜距后库插值系数(库数单位*100) 就是位置百分比
			m_pCappiCoefficient[i].bh1 = 100-m_pCappiCoefficient[i].bh2 ;	//下层斜距前库插值系数
			m_pCappiCoefficient[i].bR  = (gInt16)(m_pCappiCoefficient[i].bR * Coefficient + 0.5f);		//在取数据时用到
			m_pCappiCoefficient[i].bf = 100;								//下垂直系数，放大100倍
		}
	}
	//////////////////////////////////////////////
	EAlpha = el_tilts[0] * m_Escale;										//第0个仰角 也就是最低仰角的值	
	TempEndBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]+1])/m_pGateLen[pos]/*+0.5f*/);//最外的i的大小
	if( TempEndBin >MaxGates[0]) 
	{
		TempEndBin = MaxGates[0] ;												//水平终止库数受限
	}
	/*gFloat IK2 = 0.0;//变量IK2是for里面的i/m_pGateLen[pos]平方 用于简化公式*/
	gInt32 R = REarth;														//用于简化公式
	gFloat B = 0.0;															//用于简化公式//
	gInt32 RH = REarth + m_pSiteHigh[pos] + SiteCompCappi_H;				//用于简化公式
	gFloat RHsinB;
	gFloat R_RHcosB;														//算法优化
	//距离库数的计算///////////////////
	for( i = TempStartBin; i <= TempEndBin; i++ )							//这是从最里的i到最外的i遍历
	{
		/*IK2 = i * i * m_pGateLen[pos] * m_pGateLen[pos] ;
		m_pCappiCoefficient[i].mR = gInt16( sqrt( IK2 * ( 1 + IK2 / 17000.0 / 17000.0 - 2 * SiteCompCappi_H / 17000.0 ) + SiteCompCappi_H * SiteCompCappi_H ) / m_pGateLen[pos] + 0.5 );
		//目标点与雷达中心的距离,根据测高公式和角边关系得出,用来算距离权重*/
		//下面是根据平面库数 高度求斜距上的库数 根据正弦定理推导得出
		B = gFloat(i * m_pGateLen[pos]) / REarth;

		RHsinB = RH * sin(B);												//算法优化
		R_RHcosB = R - RH * cos(B);											//算法优化

		m_pCappiCoefficient[i].mR = gInt16(sqrt( R_RHcosB * R_RHcosB + RHsinB * RHsinB ) / m_pGateLen[pos] +0.5f);
		if (m_pCappiCoefficient[i].mR > MaxGates[ m_pCappiCoefficient[i].be ])//下层仰角库数
		{
			m_pCappiCoefficient[i].mR = MaxGates[ m_pCappiCoefficient[i].be ];
		}	
	}
	//算法优化TempStartBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]])/m_pGateLen[pos]+0.5);//重设开始i
	TempStartBin = (gInt16)((EI_Distance[m_pPPI_L[pos]])/m_pGateLen[pos]);	//重设开始i//最高仰角 内
	gFloat tempEiLR = 0.;

	//这是最小仰角投影到最外的i之间的遍历
	for( i = TempStartBin;i <= TempEndBin;i++)								
	{
		if(m_pCappiCoefficient[i].n == 0)									//如果n没被写过
		{
			//m_pCappiCoefficient[i].r = i;									//距中心的库数
			m_pCappiCoefficient[i].n = 3;									//只算上层
			m_pCappiCoefficient[i].te = 0;									//最低仰角的序号0

			//tR = i/cos(EAlpha);//反算径向库数*/
			tempEiLR = gFloat(i * m_pGateLen[pos])/REarth;					//圆心角(弧度)
			tR  =  REarth * sin(tempEiLR) / cos(EAlpha+tempEiLR) / m_pGateLen[pos];//已知仰角\水平库数求径向库数
			if( tR > gFloat(MaxGates[m_pCappiCoefficient[i].te]) )
			{
				tR = gFloat(MaxGates[m_pCappiCoefficient[i].te]);
			}
			m_pCappiCoefficient[i].tR  = (gInt16)tR;
			m_pCappiCoefficient[i].th2 = (gUint8)(100 * (tR-m_pCappiCoefficient[i].tR)+0.5f) ;
			m_pCappiCoefficient[i].th1 = 100-m_pCappiCoefficient[i].th2 ;
			m_pCappiCoefficient[i].tR  = (gInt16)(m_pCappiCoefficient[i].tR * Coefficient + 0.5f);	//在取数据时用到
			m_pCappiCoefficient[i].tf  = 100;								//上垂直系数，放大100倍
		}
	}

	//计算垂直及斜插值系数////////////
	k = m_pPPI_L[pos]-1;													//el_tilts[m_pPPI_L[pos]-1] k仰角序号,从里往外
	j = 1 ;																	//EI_Distance[1] 水平 从里往外
	EAlpha = el_tilts[k] * m_Escale;										//还是从最高仰角开始
	cosa = cos(EAlpha);
	TempStartBin = (gInt16)(gFloat(EI_Distance[j])/m_pGateLen[pos]+0.5f);	//0和m_pPPI_L[pos]+1都算过了所以j从1到m_pPPI_L[pos]
	i = TempStartBin;														//从内开始

	while( j < m_pPPI_L[pos] && k >= 1)										//仰角遍历EI_Distance[j]由里向外遍历
	{
		EAlpha2 = el_tilts[k-1] * m_Escale;									//下面一个仰角的值
		cosb = cos(EAlpha2);
		TempEndBin = (gInt16)(gFloat(EI_Distance[j+1])/m_pGateLen[pos]/*+0.5f*/);//取整后大于等于原值
		//if( TempEndBin >= MaxGates[0] )
		//{
		//	TempEndBin = MaxGates[0] ;										//如果超过上限则减1
		//}
		while( i <= TempEndBin)												//仰角间的水平库数遍历
		{
			//m_pCappiCoefficient[i].r = i;
			m_pCappiCoefficient[i].te = k;
			m_pCappiCoefficient[i].be = k - 1;
			//tR = i/cosa ;//反算径向库数*/
			tempEiLR = gFloat(i * m_pGateLen[pos])/REarth;					//圆心角(弧度)
			tR = REarth * sin(tempEiLR) / cos(EAlpha+tempEiLR) / m_pGateLen[pos];//斜距上的高仰角库数(实型)//已知仰角,水平库数求 径向库数
			if( tR>gFloat(MaxGates[k]-1) )
			{
				tR = gFloat(MaxGates[k]-1);
				m_pCappiCoefficient[i].n = 1;
				m_pCappiCoefficient[i].bf = 100;
			}
			m_pCappiCoefficient[i].tR  = (gInt16)tR;
			m_pCappiCoefficient[i].th2 = (gUint8)(100 * (tR-m_pCappiCoefficient[i].tR)+0.5f) ;	//上层斜距后库插值系数，放大100倍
			m_pCappiCoefficient[i].th1 = 100-m_pCappiCoefficient[i].th2 ;						//上层斜距前库插值系数，放大100倍
			m_pCappiCoefficient[i].tR  = (gInt16)(m_pCappiCoefficient[i].tR * Coefficient + 0.5f);//在取数据时用到
			tH = gInt16(tR * m_pGateLen[pos] * sin(EAlpha) + tR * m_pGateLen[pos] * cosa * tR * cosa * m_pGateLen[pos] /17000000 + 0.5f);//修正后的测高公式 高度 单位m

			//bR = i/cosb;//
			bR = REarth * sin(tempEiLR) / cos(EAlpha2+tempEiLR) / m_pGateLen[pos];	//斜距上的低仰角库数(实型)//已知仰角,水平库数求 径向库数
			if( bR> gFloat(MaxGates[k-1]-1) )
			{
				bR = gFloat(MaxGates[k-1]-1);
				m_pCappiCoefficient[i].n = 4;	//上下都没有
				m_pCappiCoefficient[i].tf = 100;
			}
			m_pCappiCoefficient[i].bR  = (gInt16)bR;
			m_pCappiCoefficient[i].bh2 = (gUint8)(100 * (bR-m_pCappiCoefficient[i].bR)+0.5f) ;
			m_pCappiCoefficient[i].bh1 = 100-m_pCappiCoefficient[i].bh2 ;
			m_pCappiCoefficient[i].bR  = (gInt16)(m_pCappiCoefficient[i].bR * Coefficient + 0.5f);	//在取数据时用到

			bH = gInt16(bR * m_pGateLen[pos] * sin(EAlpha2) + bR * m_pGateLen[pos] * cosb * cosb * bR* m_pGateLen[pos] /17000000+0.5f);	//测高公式//注意单位
			if(m_pCappiCoefficient[i].n == 0)								//如果n没被记录过
			{
				if( (tH < SiteCompCappi_H)||(i == TempEndBin) )				//1=取下层，2=上下层插值， 3=取上层  ,4=上下都没有
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
					if (tH-bH != 0)																					//added by chenxiang 20080625 for 除数不为0判断
					{	m_pCappiCoefficient[i].n = 2;
						m_pCappiCoefficient[i].tf = gUint8(gFloat(SiteCompCappi_H-bH)/(tH-bH) * 100+0.5f);			//上垂直系数
						m_pCappiCoefficient[i].bf = gUint8(gFloat(tH-SiteCompCappi_H)/(tH-bH) * 100+0.5f);			//下垂直系数
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

 //单站主功能函数 输出极坐标存储的反射率和距离权重数组(每层每站执行一次)
GHRESULT Arith_501_BREF::SingleProduce( gInt16 CAPPI_High,gInt16 *PolCappiLayerR,gUint8 *PolCappiLayerWeight,gInt16 pos) 
{	//参数说明:cappi高度,极坐标方式储存的反射率数组(方位角,水平库数)(一站一层用一次),极坐标存储的权重,站点序号
	gInt32 SiteCompCappi_H = CAPPI_High-m_pSiteHigh[pos];								//CAPPI相对雷达高度, 米
	//const gInt32 iThreshR = 150; 														//门限距离150公里 这个门限的话 460公里的权重为0 单位千米
	gInt32 TemInt = 0;		
	//初始化PolCappiLayer 极坐标存储的 反射率 和 权重
	//wmemset((gWchar*)PolCappiLayerR,((gWchar)(0x8000)),sizeof(gInt16) * m_pCap_gates[pos] * m_pCap_nrays[pos]/2);
	gWmemset((gWchar*)PolCappiLayerR,((gWchar)(0x8000)),sizeof(gInt16) * m_pCap_gates[pos][0] * m_pCap_nrays[pos]/2);
	memset(PolCappiLayerWeight,0,sizeof(gUint8) * m_pCap_gates[pos][0] * m_pCap_nrays[pos]);
	//此处需考虑考虑~~~~~~~~~~~~
	if( m_pPPI_L[pos] < 3 ) 
	{
		return GS_FALSE;																//至少3个仰角
	}//~~~~~~~~~~~~~~~~~~~~~~~~~
	m_Escale  =  gFloat(M_PI) / 180 / 100;												//角度/100再转弧度用的系数(单站用) 角度单位0.01度
	gInt32 EI_Distance[18] = {0};														//m_pPPI_L[pos]+2存放水平距离的一个数组从0到m_pPPI_L[pos]+1(单站用)18够用尽量减少 new(::std::nothrow)的次数
	m_pCappiCoefficient  =   new(::std::nothrow) BREFCAPSTRUCT[m_pCap_gates[pos][0]];					//定义一个结构类型的数组,存放不同库数中的插值系数等内容(单站用)
	if(m_pCappiCoefficient == NULL)
	{
		return GE_NOENOUGHMEMORY;
	}
	///////初始化结束//////////////////////////////////////
	gInt16 *el_tilts=m_el_tilts[pos];
	if( CalculateCrossRange(SiteCompCappi_H,el_tilts, pos,EI_Distance) != GS_OK||		//求等高面与各圆锥面的水平距离、水平距离起始和终结 放到EI_Distance[m_pPPI_L[pos]]里
		CreateCappiTab(SiteCompCappi_H,el_tilts,pos,EI_Distance) != GS_OK )				//计算各个仰角插值系数放到m_pCappiCoefficient[i]里
	{
		delete []m_pCappiCoefficient;
		m_pCappiCoefficient = NULL;
		return GS_FALSE;
	}
	m_StartBin = (gInt16)(gFloat(EI_Distance[0])/m_pGateLen[pos]+0.5f);					//起始=最里的那条的投影
	if ( m_StartBin == 0 )
	{
		m_StartBin = 1;
	}
	m_EndBin = (gInt16)(gFloat(EI_Distance[m_pPPI_L[pos]+1])/m_pGateLen[pos]/*+0.5f*/);		//终止=最外的那条的投影
	if( m_EndBin > m_pCap_gates[pos][0]-1)
	{
		m_EndBin = m_pCap_gates[pos][0]-1 ;												//界限限制
	}
	gInt16 tZ;																			//上层反射率
	gInt16 bZ;																			//下层反射率
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
		2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0};//e指数表 算法优化
	gInt16 **pppDataR = m_pRadarDataR[pos];
	gInt16 TempNRays = m_pCap_nrays[pos];
	gInt32* pCap_gates= m_pCap_gates[pos];													//各站各仰角库数数组的指针
	gFloat ScaleB2M  = (gFloat)m_ScaleOfMultiData / m_ScaleOfBaseData ;						//两个放大倍数的比值
	for(int jj = 0; jj < m_pCap_nrays[pos]; ++jj)											//方位角遍历
	{		
		TemInt = jj * m_pCap_gates[pos][0] + m_StartBin;
		for(int ii = m_StartBin; ii <= m_EndBin; ii++,TemInt++)								//水平库数遍历(作用相当与仰角遍历了)
		{
			//水平计算开始
			if(m_pCappiCoefficient[ii].n == 0 || m_pCappiCoefficient[ii].bR+1 < 0 || m_pCappiCoefficient[ii].be > 20 )
			{
				continue;																	//n=0 不计算//越界 忽略
			}//			
			//pTempte = pRadar->GetRadarDataR(m_pCappiCoefficient[ii].te,jj);
			//pTempbe = pRadar->GetRadarDataR(m_pCappiCoefficient[ii].be,jj);
			pTempte = pppDataR[ m_pCappiCoefficient[ii].te * TempNRays + jj ];
			pTempbe = pppDataR[ m_pCappiCoefficient[ii].be * TempNRays + jj ] ;				//得到R数据指针	
			if (pTempte==NULL || pTempbe==NULL)
			{
				continue;
			}
			PolCappiLayerWeight[TemInt] = expTable[ m_pCappiCoefficient[ii].mR * m_pGateLen[pos] / 1000 ];//取距离权重			
			/*if(	pTempte[m_pCappiCoefficient[ii].tR]	<= RADAR_DATA_OUTOFRANGE&&
				pTempte[m_pCappiCoefficient[ii].tR+1]<= RADAR_DATA_OUTOFRANGE&&
				pTempbe[m_pCappiCoefficient[ii].bR]	<= RADAR_DATA_OUTOFRANGE&&
				pTempbe[m_pCappiCoefficient[ii].bR+1] <= RADAR_DATA_OUTOFRANGE
				)//如果它们都是无回波或模糊或无数据的情况 那么tz bz都等于其中最大值
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
				{																						 //回波值大于-50用权重方法计算
					tZ = gInt16( (pTempte[m_pCappiCoefficient[ii].tR]*m_pCappiCoefficient[ii].th1 + 
							  pTempte[m_pCappiCoefficient[ii].tR+1]*m_pCappiCoefficient[ii].th2+50)/100 );
				}
				else
				{
					tZ = max(pTempte[m_pCappiCoefficient[ii].tR],pTempte[m_pCappiCoefficient[ii].tR+1] );//如果回波值小于-50就取前后库中的大数
					if( tZ<-160 )
					{
						tZ=-160;																		 //反射率下限限制
					}
				}
				//下层反射率计算
				if( pTempbe[m_pCappiCoefficient[ii].bR]>-50 && pTempbe[m_pCappiCoefficient[ii].bR+1]>-50)//回波值大于-50用权重方法计算
				{
					bZ = (pTempbe[m_pCappiCoefficient[ii].bR]*m_pCappiCoefficient[ii].bh1 + 
						pTempbe[m_pCappiCoefficient[ii].bR+1]*m_pCappiCoefficient[ii].bh2+50)/100;
				}
				else
				{
					bZ=max( pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1] );//如果回波值小于-50就取前后库中的大数
					if( bZ<-160 )
					{
						bZ=-160;																		//反射率下限限制
					}
				}
			}//else*/
			//算法优化//将以上if语句改成?:表达式提高效率//为提高可读性多行显示////////////////////////////////////////////////////
			(	pTempte[m_pCappiCoefficient[ii].tR]	 <= RADAR_DATA_OUTOFRANGE&&/*距离模糊-32766*/\
				pTempte[m_pCappiCoefficient[ii].tR+1]<= RADAR_DATA_OUTOFRANGE&&\
				pTempbe[m_pCappiCoefficient[ii].bR]	 <= RADAR_DATA_OUTOFRANGE&&\
				pTempbe[m_pCappiCoefficient[ii].bR+1]<= RADAR_DATA_OUTOFRANGE\
				)?\
			(\
				tZ = bZ = max(\
					max(pTempte[m_pCappiCoefficient[ii].tR],pTempte[m_pCappiCoefficient[ii].tR+1]),\
					max(pTempbe[m_pCappiCoefficient[ii].bR],pTempbe[m_pCappiCoefficient[ii].bR+1])\
						 )/* * ScaleB2M不用乘*/ ,\
				PolCappiLayerWeight[TemInt] = 0 \
			)\
			:\
			(\
				( pTempte[m_pCappiCoefficient[ii].tR]   * ScaleB2M  > -5 * m_ScaleOfMultiData &&\
					   pTempte[m_pCappiCoefficient[ii].tR+1] * ScaleB2M  > -5 * m_ScaleOfMultiData  )?\
				(\
					tZ = gInt16( (pTempte[m_pCappiCoefficient[ii].tR]   * ScaleB2M * m_pCappiCoefficient[ii].th1 + \
							 pTempte[m_pCappiCoefficient[ii].tR+1] * ScaleB2M * m_pCappiCoefficient[ii].th2 + 50)/100 /*此处固定为100*/)\
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
				//下层反射率计算
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

            //垂直计算开始
			/*if(PolCappiLayerWeight[TemInt]!=0)
			{
				if(m_pCappiCoefficient[ii].n==1) 
				{
					PolCappiLayerR[TemInt]=bZ;																		//直接赋下层的值
				}
				else 
				{
					if( m_pCappiCoefficient[ii].n==2)
					{
						PolCappiLayerR[TemInt]=(bZ*m_pCappiCoefficient[ii].bf+tZ*m_pCappiCoefficient[ii].tf+50)/100;//通过权重算出该点的反射率
					}
					else 
					{
						if( m_pCappiCoefficient[ii].n==3)
						{
							PolCappiLayerR[TemInt]=tZ ;																//直接赋上层的反射率值
						}
					}
				}
			}
			else
			{
				PolCappiLayerR[TemInt]=tZ;
			}//*/
			//算法优化 由if语句转为?:表达式来提高速度///////////////////////////////////////////////
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
				if(PolCappiLayerR[TemInt]!=RADAR_DATA_OUTOFRANGE)						//这里要对距离模糊做判断
				{
					PolCappiLayerR[TemInt] = RADAR_DATA_NOECHO;							//很小就赋-32767 无回波
				}
				PolCappiLayerWeight[TemInt] = 0;										//反射率很小时定义距离权重为0 也不参与计算
			}//*/
			//算法优化//////////////////////////////////////////////////////////////////////////////
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
 //组网处理:定层经纬边界,定义权重大数组,文件写入权重乘反射率累加(每层执行一次)
GHRESULT Arith_501_BREF::LayerMultiProduce(gInt16 CAPPI_High,gInt16 Layer)				//参数说明:本层cappi海拔高度,层序号
{									
	//wmemset((gWchar*)(m_pMultifWeight),0x0,sizeof(gUint16) * m_M * m_N/2);			//清空权重大数组
	memset(m_pMultifWeight,0x0,sizeof(gChar) * m_M * m_N*2);					//清空权重大数组
	gInt32 Tempxy = 0;
	gInt32 MxN = m_M * m_N;
	memset32((gInt32*)(m_pMultiRW),(unsigned long)RADAR_DATA_NODATA,sizeof(gInt32) * MxN);
	gInt32 Lon0 = 11600000;																//雷达站点经度(单站用) 这是随意设置的初值 防止无初值产生的错误 下面会去读文件中的数值
	gInt32 Lat0 = 3900000;																//雷达站点纬度(单站用) 同上
	gInt16 RadarHigh = 0;																//雷达海拔高度(单站用) 也是随意设置的初值
	int x = 0;																		//横坐标 对应m_N 第x列 (大数组 单层用)
	int y = 0;																		//纵坐标 对应m_M 第y行 (大数组 单层用)
	gInt16 x0= 0;
	gInt16 y0= 0;																		//雷达站中心对应的行列
	gInt16 * pTem  = NULL;
	gInt32 TemInt  = 0;
	gInt32 TemInt2 = 0;

	gInt16 *PolCappiLayerR = NULL; 														//初始化输出反射率数组指针
	gUint8 *PolCappiLayerWeight = NULL; 												//初始化输出权重数组指针

	gInt32 FillNum[25] = {0,1,2,3,4,5,6,7,9,9 ,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};//{0,1,2,3,3,4,5,6,7,7};//填充长度数组//下标为扫描距离/50公里
	//0度到180度的10000倍的K值 斜率数组
	gInt32 K[]={1,875,1763,2679,3640,4663,5774,7002,8391,10000,11918,14281,17321,21445,27475,37321,56713,114301,428602,\
				-114301,-56713,-37321,-27475,-21445,-17321,-14281,-11918,-10000,-8391,-7002,-5774,-4663,-3640,-2679,-1763,-875,-1};	
	gInt32 TempNum= 0;
	gInt32 Temp1  = 0;
	gInt32 *pTempLonLatData = NULL;														//经纬数据临时指针

	gInt32 TempK = 1;																	//与某方位角径向垂直的那条线的斜率
	gInt16 m = 0;																		//临时替代y的填充变量
	gInt16 n = 0;																		//临时替代x的填充变量
	gInt16 Round_4 = 0;																	//圆中区域区分
	gInt32 TempFillNum = 0;

	for(gInt16 pos = 0;pos<m_SiteNum;pos++)												//开始站遍历
	{
		//list<StandardRadarData>::iterator& pRadar = m_pGenerateProductList->GetAtAs<StandardRadarData>(pos);
		if(m_ErrorData[pos])															//是否有效
		{
			continue;																	//无效就跳过本站
		}
		pTempLonLatData=m_ppLonLatData[pos];
		if ( PolCappiLayerR == NULL )													//MS by chenxiang 20080630
		{
			PolCappiLayerR =  new(::std::nothrow) gInt16[m_pCap_nrays[pos] * m_pCap_gates[pos][0]];			//定义输出数组反射率
		}
		if (PolCappiLayerWeight == NULL)												//MS by chenxiang 20080630
		{
			PolCappiLayerWeight =  new(::std::nothrow) gUint8[m_pCap_nrays[pos] * m_pCap_gates[pos][0]];	//定义输出数组权重
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
				PolCappiLayerWeight = NULL;												//修正内存泄漏
			}																			//End of MS~~~~~~~~~~~~~~~~~~~~
			continue;
		}
		if(SingleProduce( CAPPI_High,PolCappiLayerR,PolCappiLayerWeight,pos) == GS_OK ) //站点内处理 得到单站单层极坐标的反射率和权重
		{
			//开始把极坐标数组上的权重反射率搬到经纬大数组里(单站单层数组 到 单层数组)
			TempNum  = 0;
			TempK = 0;
			gInt16 inrays8 =  m_pCap_nrays[pos]/8;
			gInt16 inrays4 =  m_pCap_nrays[pos]/4;
			gInt16 inrays2 =  m_pCap_nrays[pos]/2;
			for(gInt16 j = 0; j < m_pCap_nrays[pos];j++)/*360; j++)	*/					//方位角遍历
			{
				TemInt = j * m_pCap_gates[pos][0] + m_StartBin;							//极坐标PolCappiLayer中的下标
				TemInt2= TemInt + TemInt;												//经纬数据LonLatData中的经度下标
				TempK = K[((j%inrays2)/5)];			/* K[((j%180)/5)]; */   				//斜率 放大10000倍  
				x0 = gInt16( ( pTempLonLatData[0] - m_StartLon ) / m_ResLonLat);		//站点的格点数坐标
				y0 = gInt16( ( m_StartLat - pTempLonLatData[1] ) / m_ResLonLat);		//算法优化屏蔽四舍五入
				Round_4 = ((j + inrays8) /inrays4) % 4 ;/*((j + 45) / 90) % 4 ;*/
				for(gInt32 i = m_StartBin;i <= m_EndBin; i++,TempNum= i / 50,TemInt++,TemInt2 += 2)//水平库数遍历
				{
					//TempNum=i/50;														//i*1000/50000 即50公里一个单位//FillNum下标		
					//1.计算该点到西端和北端的格子数
					x = gInt16((pTempLonLatData[TemInt2] - m_StartLon) / m_ResLonLat);
					y = gInt16((m_StartLat - pTempLonLatData[TemInt2 + 1]) / m_ResLonLat);//算法优化屏蔽四舍五入
					Temp1 = PolCappiLayerR[TemInt] * PolCappiLayerWeight[TemInt];		//R*W	
					TempFillNum = FillNum[TempNum];
					//填充开始//虽然代码有部分重复但这样效率高////////////////////////////////////////////					
					switch( Round_4 )													//把圆分成4个区域分别操作
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
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//权重 累加  小心!!!!!!类型转换可能会出问题
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
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//权重 累加  小心!!!!!!类型转换可能会出问题
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
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16(PolCappiLayerWeight[TemInt]);//权重 累加  小心!!!!!!类型转换可能会出问题
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
									m_pMultifWeight[Tempxy] = m_pMultifWeight[Tempxy] + gUint16( PolCappiLayerWeight[TemInt] );//权重 累加  小心!!!!!!类型转换可能会出问题
									Tempxy++;
								}
							}
							break;
					}//*/
					//填充结束///////////////////////////////////////////////
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
	}//end of "for" 站遍历结束
	//最终处理//文件中的 反射率乘权重 的和 除以 权重的和
	for(y = 0;y<m_M;y++)														//纵坐标 对应m 第y行
	{
		pTem = m_pOut->GetMyLineData(Layer,y);
		Tempxy = y * m_N;
		for(x = 0; x<m_N; x++,Tempxy++)											//横坐标 对应n 第x列
		{			
			//写数据//msdfjlasf(XY)->rw / m_pMultifWeight->at(y * n+x);//处理"文件"中的 总(反射率 * 权重)比 总权重 单层Data[NumberOfRows * NumberOfCols]
			pTem[x] =(m_pMultifWeight[Tempxy] != 0)?gInt16( m_pMultiRW[Tempxy] / m_pMultifWeight[Tempxy] ): gInt16( m_pMultiRW[Tempxy] );//*//算法优化
		}//for
	}//for
	return GS_OK;
}

extern "C" DLLEXP_GSYS IArith *Create_arith (void)								//用于创建Arith_501_BREF接口
{
	return  new(::std::nothrow) Arith_501_BREF;
}

