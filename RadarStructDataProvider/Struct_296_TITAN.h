#pragma once
#define _NMC_TITAN_FLAG_ "NMCTITAN"

struct SYrMonDayHrMin
{
	unsigned short m_nYear;
	unsigned short m_nMonth;
	unsigned char  m_nDay;
	unsigned char  m_nHour;
	unsigned char  m_nMinute;  //世界时
	unsigned char  m_nSecond;  //世界时
};

struct SLonLat
{
	float lon;   //经度
	float lat;   //纬度
};

struct STITAN_StormBoundary
{
	//	int  m_nStormBoundaryPointNum;//风暴边界点个数
	SLonLat lonlat[73];//[nStormBoundaryPointNum];  // 风暴边界点
};
struct STITANStormProps
{
	double proj_area_centroid_x; //风暴投影中心经度
	double proj_area_centroid_y; //风暴投影中心纬度
	double vol_centroid_z;      //风暴中心高度
	double refl_centroid_z;     //风暴反射率因子中心高度
	double top;			//风暴顶高度
	double dbz_max;		//最大反射率因子
	double volume;		//风暴体积
	double mass;       //风暴质量
	double proj_area;		//风暴面积
	double smoothed_speed;//风暴移动速度
	double smoothed_direction;//风暴移动方向，正北为0
	double vil;   //风暴VIL值
}; //风暴属性


////产品数据共有四块数据组成


///产品中第一块数据的结构体，表示从每个体扫识别出的风暴体信息
typedef struct STITAN_OutputStorm
{
	SYrMonDayHrMin m_DateTime;             //世界时
	int            m_nStormCount;          // 4个字节，整型数，表示风暴的个数；
	STITAN_StormBoundary m_storm[47];//[nStormCount];//风暴信息
	STITANStormProps    m_props[47];//[nStormCount];//风暴属性
};

struct STITAN_TrackPoint
{
	SLonLat        m_LonLat;
	SYrMonDayHrMin m_DateTime;
};
///产品中第二块数据的结构体，表示风暴体追踪轨迹，即将相关的风暴链接成线
struct STITAN_Track
{
	int               m_nPointNum;
	STITAN_TrackPoint m_point[50];
};


//////产品中第三块数据的结构体，表示预测的一小时的风暴体信息
struct STITAN_StormForecast
{  
	unsigned short m_nForecastMinute;  ///预报时间，单位分钟
	int            m_nStormCount;      // 4个字节，整型数，表示风暴的个数；
	STITAN_StormBoundary m_storm[50];      //[nStormCount];//风暴信息
};

struct STITANTrackPointForecast
{
	SLonLat m_LonLat;
	int     m_nForecastMinute; //预报时间，单位分钟
	int     m_nReserve;        //for 8-byte alignment
};

//////产品中第四块数据的结构体，表示预测的一小时的风暴体轨迹线
struct STITAN_TrackForecast
{
	int                       m_nPointNum;
	STITANTrackPointForecast m_point[50];
};

////////// 图片大小信息，用经纬度表示，
struct LonLatSPCE
{
	float m_Cat_Lat_Spacing ;
	float m_Cat_Lon_Spacing ;///间隙
	float m_Cat_East  ;
	float m_Cat_North;
	float m_Cat_South;
	float m_Cat_West ;
};





/////////////////////////////////////////////////////////////////////////////
struct STITANStormBoundary
{
	int  m_nStormBoundaryPointNum;//风暴边界点个数
	SLonLat* lonlat;//[nStormBoundaryPointNum];  // 风暴边界点
};





struct STITANOutputStorm
{
	SYrMonDayHrMin m_DateTime;             //世界时
	int            m_nStormCount;          // 4个字节，整型数，表示风暴的个数；
	STITANStormBoundary* m_storm;//[nStormCount];//风暴信息
	STITANStormProps   * m_props;//[nStormCount];//风暴属性
};




struct STITANStormForecast
{  
	unsigned short m_nForecastMinute;  ///预报时间，单位分钟
	int            m_nStormCount;      // 4个字节，整型数，表示风暴的个数；
	STITANStormBoundary* m_storm;      //[nStormCount];//风暴信息
};


struct STITANTrackPoint
{
	SLonLat        m_LonLat;
	SYrMonDayHrMin m_DateTime;
};
struct STITANTrack
{
	int               m_nPointNum;
	STITANTrackPoint* m_point;
};


struct STITANTrackForecast
{
	int                       m_nPointNum;
	STITANTrackPointForecast* m_point;
};

struct STITANOutputFile
{
	char m_strFlag[8]; //NMCTITAN
	int  m_nVersion  ; //1

	//2008-09-23 modified
	//char m_strReserve[16]; //spare
	int  m_nThreshold; //Storm Threshold
	char m_strReserve[12]; //spare

	SYrMonDayHrMin m_DateTime;

	//storms
	int  m_nStormTimeCount;
	STITANOutputStorm* m_Storm;

	int  m_nForecastCount;
	STITANStormForecast* m_StormForecast;

	//tracks
	int m_nTrackCount;
	STITANTrack* m_TitanTrack;

	int m_nForecastTrackCount;
	STITANTrackForecast* m_TitanTrackForecast;
};