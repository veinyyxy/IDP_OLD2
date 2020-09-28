#pragma once
#define _NMC_TITAN_FLAG_ "NMCTITAN"

struct SYrMonDayHrMin
{
	unsigned short m_nYear;
	unsigned short m_nMonth;
	unsigned char  m_nDay;
	unsigned char  m_nHour;
	unsigned char  m_nMinute;  //����ʱ
	unsigned char  m_nSecond;  //����ʱ
};

struct SLonLat
{
	float lon;   //����
	float lat;   //γ��
};

struct STITAN_StormBoundary
{
	//	int  m_nStormBoundaryPointNum;//�籩�߽�����
	SLonLat lonlat[73];//[nStormBoundaryPointNum];  // �籩�߽��
};
struct STITANStormProps
{
	double proj_area_centroid_x; //�籩ͶӰ���ľ���
	double proj_area_centroid_y; //�籩ͶӰ����γ��
	double vol_centroid_z;      //�籩���ĸ߶�
	double refl_centroid_z;     //�籩�������������ĸ߶�
	double top;			//�籩���߶�
	double dbz_max;		//�����������
	double volume;		//�籩���
	double mass;       //�籩����
	double proj_area;		//�籩���
	double smoothed_speed;//�籩�ƶ��ٶ�
	double smoothed_direction;//�籩�ƶ���������Ϊ0
	double vil;   //�籩VILֵ
}; //�籩����


////��Ʒ���ݹ����Ŀ��������


///��Ʒ�е�һ�����ݵĽṹ�壬��ʾ��ÿ����ɨʶ����ķ籩����Ϣ
typedef struct STITAN_OutputStorm
{
	SYrMonDayHrMin m_DateTime;             //����ʱ
	int            m_nStormCount;          // 4���ֽڣ�����������ʾ�籩�ĸ�����
	STITAN_StormBoundary m_storm[47];//[nStormCount];//�籩��Ϣ
	STITANStormProps    m_props[47];//[nStormCount];//�籩����
};

struct STITAN_TrackPoint
{
	SLonLat        m_LonLat;
	SYrMonDayHrMin m_DateTime;
};
///��Ʒ�еڶ������ݵĽṹ�壬��ʾ�籩��׷�ٹ켣��������صķ籩���ӳ���
struct STITAN_Track
{
	int               m_nPointNum;
	STITAN_TrackPoint m_point[50];
};


//////��Ʒ�е��������ݵĽṹ�壬��ʾԤ���һСʱ�ķ籩����Ϣ
struct STITAN_StormForecast
{  
	unsigned short m_nForecastMinute;  ///Ԥ��ʱ�䣬��λ����
	int            m_nStormCount;      // 4���ֽڣ�����������ʾ�籩�ĸ�����
	STITAN_StormBoundary m_storm[50];      //[nStormCount];//�籩��Ϣ
};

struct STITANTrackPointForecast
{
	SLonLat m_LonLat;
	int     m_nForecastMinute; //Ԥ��ʱ�䣬��λ����
	int     m_nReserve;        //for 8-byte alignment
};

//////��Ʒ�е��Ŀ����ݵĽṹ�壬��ʾԤ���һСʱ�ķ籩��켣��
struct STITAN_TrackForecast
{
	int                       m_nPointNum;
	STITANTrackPointForecast m_point[50];
};

////////// ͼƬ��С��Ϣ���þ�γ�ȱ�ʾ��
struct LonLatSPCE
{
	float m_Cat_Lat_Spacing ;
	float m_Cat_Lon_Spacing ;///��϶
	float m_Cat_East  ;
	float m_Cat_North;
	float m_Cat_South;
	float m_Cat_West ;
};





/////////////////////////////////////////////////////////////////////////////
struct STITANStormBoundary
{
	int  m_nStormBoundaryPointNum;//�籩�߽�����
	SLonLat* lonlat;//[nStormBoundaryPointNum];  // �籩�߽��
};





struct STITANOutputStorm
{
	SYrMonDayHrMin m_DateTime;             //����ʱ
	int            m_nStormCount;          // 4���ֽڣ�����������ʾ�籩�ĸ�����
	STITANStormBoundary* m_storm;//[nStormCount];//�籩��Ϣ
	STITANStormProps   * m_props;//[nStormCount];//�籩����
};




struct STITANStormForecast
{  
	unsigned short m_nForecastMinute;  ///Ԥ��ʱ�䣬��λ����
	int            m_nStormCount;      // 4���ֽڣ�����������ʾ�籩�ĸ�����
	STITANStormBoundary* m_storm;      //[nStormCount];//�籩��Ϣ
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