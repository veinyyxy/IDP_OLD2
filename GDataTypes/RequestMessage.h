//////////////////////////////////////////////////////////////////////////
// RequestMessage.h
// created by ssy 20080729
// for Product Request Message
//
// 说明：
// 每种一条请求消息首先将组织成list<GenerationData*>的形式。
// 下面的各种请求结构都可以用RequestMessageData或者RequestMessageDataNoParam组织成GenerationData，
// 因为RequestMessageData和RequestMessageDataNoParam是GenerationData的子类。
//
// 其中，list的第一项为RequestMessageHeader构成的GenerationData。
//
// 第二项为请求的公共部分，根据RequestMessageHeader中的RequestMessageType是对应的
// OnceRequestMessagePublic、RoutineRequestMessagePublic或AlarmRequestMessagePublic中的一种构成的GenerationData。
//
// 第三项到第n项为请求列表的各自内容，根据RequestMessageHeader中的RequestMessageType是对应的
// OnceRequestMessage、RoutineRequestMessage或AlarmRequestMessage中的一种构成的GenerationData。
//
// 组织好的list<GenerationData*>将通过GDataStream.h中的方法形成连续内存段GenerationDataStream再进行发送。
// 接收到的GenerationDataStream需要还原成list<GenerationData*>再进行解析。
// 参见示例程序。
//////////////////////////////////////////////////////////////////////////


#ifndef REQUESTMESSAGEDATA_H
#define REQUESTMESSAGEDATA_H

#pragma once

#include <map>
#include <vector>
#include <string>

#include "GData.h"

#ifndef USER_DEFINE_AREA_CODE
#define USER_DEFINE_AREA_CODE "X8000"
#endif //USER_DEFINE_AREA_CODE

//各种参数项名称的定义：
#define RPN_MOSAIC_SITE_LIST		"mosaic"	// 自定义组网站点列表，RPT_gChar8
#define RPN_ALARM_ACA_FORMAT		"almACA%d"	// “ACA”：area-condition-attachment对应表，RPT_gChar，%s为0、1、2……是因为每个加到警报产品后的ACA项在map中的索引不同。
#define RPN_ALARM_AREA_LIST			"almArea"	// 警报区域列表，RPT_gDouble
//#define RPN_ALARM_AREA_INFO     	"almAreaInfo"	// 行政区域级别及编号，RPT_gInt16
#define RPN_ALARM_CONDITION_LIST	"almCondi"	// 警报条件列表，RPT_gInt16。列表中每一个警报条件都由上下限两个数值组成，因此警报条件列表里的数值都应该是双数个。另外如果下限是无穷小，上限是无穷大，那么值以-1表示。
#define RPN_ALARM_ATTACHMENT_LIST	"almAtta"	// 警报附加产品列表，RPT_gInt16


//此外各种算法参数的名称请在此处继续定义：
#define RPN_ONCE_LRM_ANGLE	    "lrmAngle"	  //LRM 方位角,RPT_gDouble
#define RPN_ONCE_LRM_DISTENCE	"lrmDistence" //LRM 距离库,RPT_gInt16
#define RPN_ONCE_LRM_ROW	    "mlrmRow"	  //MLRM 行,RPT_gInt16
#define RPN_ONCE_LRM_COL	    "mlrmCol"	  //MLRM 列,RPT_gInt16
#define RPN_ONCE_LRM_COUNT	    "lrmCount"	  //LRM/MLRM 层数,RPT_gInt16
#define RPN_ONCE_LRM_HEIGHT	    "lrmHeight"	  //LRM/MLRM 层高,RPT_gInt16

#define RPN_ONCE_V_BLUR			"vblur"		  				//退模糊层数+仰角层 RPT_gInt16
#define RPN_ONCE_V_PRODUCTDEPENDENT	"vproductdependent"		//退模糊产品依赖

//AS by haodemei 20081007
#define RPN_ONCE_ECHOTOPREFTHRESHOLD    "EchoTopRefThreshold"     //回波顶反射率因子阈值 RPT_gInt32
#define RPN_ONCE_BEAMWIDTH              "BeamWidth"               //雷达波束宽度 RPT_gDouble
#define RPN_ONCE_LIMITOFSTORMV          "LimitOfStormV"           //相关风暴最大速度阈值 RPT_gInt16
#define RPN_ONCE_MAXTIMESLOT            "MaxTimeSlot"             //相邻两个体扫所允许的最大时间间隔 RPT_gInt32
#define RPN_ONCE_VELOCITYOFSTORM        "VelocityOfStorm"         //风暴体的默认移动速度 RPT_gDouble
#define RPN_ONCE_DIRECTIONOFSTORM       "DirectionOfStorm"        //风暴体的默认移动方向 RPT_gInt16
#define RPN_ONCE_RESLONLAT              "ResLonLat"               //组网经纬网格分辨率 RPT_gDouble
#define RPN_ONCE_ZR_A                   "ZR_A"                    //ZR 关系乘法系数   RPT_gInt32
#define RPN_ONCE_ZR_B                   "ZR_b"                    //ZR 关系乘法系数   RPT_gDouble
#define RPN_ONCE_H_CAPPI                "H_CAPPI"                 //等高面 RPT_gInt32
//AE by haodemei 20081007

//////////////////////////////////////////////////////////////////////////

// 请求类型
enum RequestMessageType
{
	Product_Request_None = 0,   //生成产品失败
	Product_Request_Routine,	// 常规请求
	Product_Request_Alarm,		// 警报请求
	Product_Request_Once,		// 一次性请求	
	Product_Request_AlarmAttatch, //警报请求附加产品
};

#pragma pack(push,1)

// 应用层请求数据头
typedef struct tagRequestMessageHeader
{
	RequestMessageType requestType;	// 请求类型
	gChar to[8];
	gChar from[8];
	gInt32 length;
public:
	inline void InitializeNoParam();
} RequestMessageHeader;


// 各种请求结构：

// 一次性请求：
// 一次性请求列表的公共部分
typedef struct tagOnceRequestMessagePublic
{
	gChar RadarSite[8];				//雷达站站网号或区域中心编号
	gInt16 NumberOfParameter;		//参数个数，自动计数不用填（若RadarSite为X8000则参数含有自定义组网雷达站列表）
	gInt16 HistoryOrCurrent;		//0：历史数据。1：实时数据。
	gInt32 ScanDateOrRepeatTime;	//HistoryOrCurrent为0时表示体扫日期，-1为最晚有效体扫。HistoryOrCurrent为1时，表示重复次数。
	gInt32 ScanTimeOrInterval;		//HistoryOrCurrent为0时表示体扫时间，-1为最晚有效体扫。HistoryOrCurrent为1时，表示返回间隔。
	gInt16 IsQualityControl;		//0：进行自定义质量控制。1：进行默认质量控制。
public:
	inline void Initialize();
} OnceRequestMessagePublic;

// 一次性请求列表的各自内容
typedef struct tagOnceRequestMessage
{
	gInt16 ProductCode;				//产品代号，具体参见表
	gInt16 NumberOfParameter;		//参数个数，自动计数不用填（参数可能有多个算法参数）
public:
	inline void Initialize();
} OnceRequestMessage;


// 常规请求：
// 常规请求列表的公共部分
typedef struct tagRoutineRequestMessagePublic
{
	gChar RadarSite[8];				//雷达站站网号或区域中心编号
public:
	inline void InitializeNoParam();
} RoutineRequestMessagePublic;

// 常规请求列表的各自内容
typedef struct tagRoutineRequestMessage
{
	gInt16 ProductCode;				//产品代号，具体参见表
	gInt16 Interval;				//返回产品间隔
public:
	inline void InitializeNoParam();
} RoutineRequestMessage;


// 警报请求：
enum AlertAreaType     
{
   RectangleArea = 0,   //基于产品选择
   DistrictArea,        //基于行政区域选择,limei,04.29 
};
// 警报请求列表的公共部分
typedef struct tagAlarmRequestMessagePublic
{
	gChar RadarSite[8];				//雷达站站网号或区域中心编号
	gInt32 AlarmEndDate;			//警报有效日期，-1表示无期限
	gInt32 AlarmEndTime;			//警报有效时间，-1表示无期限
    AlertAreaType AreaType;   //选择区域类型
public:
	inline void InitializeNoParam();
} AlarmRequestMessagePublic;

// 警报请求列表的各自内容
typedef struct tagAlarmRequestMessage
{
	gInt16 ProductCode;				//产品代号，具体参见表
	gInt16 NumberOfParameter;		//参数个数，自动计数不用填（即ACA个数，警报区域-警报条件-警报附加产品对应表的项数）
public:
	inline void Initialize();
} AlarmRequestMessage;

// 警报请求列表中每一个ProductCode下的“警报区域-警报条件-警报附加产品对应表”中的一项
typedef struct tagAlarmACA
{
	gInt16 NumberOfParameter;		//参数个数，自动计数不用填（包括警报区域、警报条件列表、警报附加产品列表）
public:
	inline void Initialize();
} AlarmACA;
typedef struct tagIpAddr    //add by ygf
{
	char IpAddr[24];
public:
	inline void Initialize();
}IpAddr;

typedef struct tagSingleRequestMessage //add by ygf
{
	gInt16 ProductCode;				
	gChar  SiteCode[8];	
	gInt16 VCPDate;		
	gInt32 VCPTime;
public:
	inline void Initialize();
	bool operator == (const tagSingleRequestMessage&srm) //by wp
	{
		if (ProductCode == srm.ProductCode && 
			VCPDate == srm.VCPDate &&
			VCPTime == srm.VCPTime &&
			!strcmp(SiteCode, srm.SiteCode))
		{
			return true;
		}
		return false;
	}
} SingleRequestMessage;

enum RequestParameterType
{
	RPT_TYPE_ERROR,	//means error type
	RPT_gChar,		//means data is gChar[],to use pointer as gChar *
	RPT_gInt16,		//means data is gInt16[],to use pointer as gInt16 *
	RPT_gInt32,		//means data is gInt32[],to use pointer as gInt32 *
	RPT_gFloat,		//means data is gFloat[],to use pointer as gFloat *
	RPT_gDouble,	//means data is gDouble[],to use pointer as gDouble *
	RPT_gChar8,		//means data is gChar[][8],to use pointer as gChar (*)[8] //设计用于记录组网站点列表的。
	//RPT_gInt162,	//means data is gInt16[][2]
};

enum QualityControlID
{
	QC_AziSort=0,
	QC_VelocityDealiasing,
	QC_FirstOfRef,
};

typedef struct tagRequestParameterHead
{
	gInt32 TotalSize;					//当前参数数据总长度
	gInt32 NameSize;					//参数名字长度，包括'\0'
	gInt32 ValueSize;					//参数数据的size。以字节计
	RequestParameterType ValueType;		//参数类型
} RequestParameterHead;

#pragma pack(pop)


template<typename T>
class RequestMessageData;

class RequestParametersMap : public std::map<std::string,std::pair<RequestParameterType,std::vector<char> > >
										//map<参数名,pair<参数类型RequestParameterType,数据存放>>
{
	template<typename T>
	friend class RequestMessageData;

public:
	inline RequestParametersMap(){};
	inline ~RequestParametersMap(){};

	//一次性添加一个参数的多个值。指定参数名（不可与已有参数重复），参数类型，参数数组长度，参数数组首地址。
	inline GHRESULT AddParam(char * ParamName, const RequestParameterType & ParamType, gInt32 ArrayLength, void * pDataArray=NULL);
	//添加一个参数的一个值。指定参数名（可以与已有参数重复），参数类型，参数地址。
	inline GHRESULT AddParamOne(char * ParamName, const RequestParameterType & ParamType, void * pDataArray=NULL);
	//根据参数名删除已有参数
	inline GHRESULT RemoveParam(const char * ParamName);

	//根据参数名获取当前列表中参数数组首地址指针。
	inline void * GetParamPointer(const char * ParamName)const;
	//根据参数名获取当前列表中参数类型。
	inline RequestParameterType GetParamType(const char * ParamName)const;
	//根据参数名获取当前列表中参数占用存储空间(以字节计)。
	inline gInt32 GetParamSize(const char * ParamName)const;
	//根据参数名获取当前列表中参数数组长度。
	inline gInt32 GetParamArrayLength(const char * ParamName)const;
	//获取类型占用字节数。
	inline gInt32 GetParamTypeSize(const RequestParameterType & ParamType)const;
	//获取当前记录参数数量
	inline gInt32 GetParamCount()const;

	typedef std::map<std::string,std::pair<RequestParameterType,std::vector<char> > >::iterator iterator;
	typedef std::map<std::string,std::pair<RequestParameterType, std::vector<char> > >::const_iterator const_iterator;

protected:
	template<typename T>
	inline GHRESULT LoadParameters(RequestMessageData<T> * pRequestMessageData);	//load parameters from pRequestMessageData
	template<typename T>
	inline GHRESULT SaveParameters(RequestMessageData<T> * pRequestMessageData)const;	//save parameters to pRequestMessageData
	inline gInt32 GetParameterDataSize()const;

private:
};


//供含有不确定因素项的数据段使用
//各种请求结构中有Initialize()和NumberOfParameter的,可以作为此模版类的T
template<typename T>
class RequestMessageData :public GenerationData
{
	friend class RequestParametersMap;

public:
	inline RequestMessageData();
	inline ~RequestMessageData();

	//获取请求信息结构。用于读写。
	inline T * GetRequestMessage();
	//获取参数列表，按照RequestParametersMap类型解析。
	inline RequestParametersMap GetParametersMap();

	//根据RequestParametersMap重写参数列表。调用之后NumberOfParameter变为目前的参数个数。
	//！注意：调用过此函数之后，一定要重新调用GetRequestMessage取得T指针！
	inline GHRESULT ReplaceParametersFromMap(const RequestParametersMap & ParamMap);

protected:
	inline void * GetParametersPointer(gInt16 Idx);
	inline gInt16 * GetParametersNumber();
	inline void Init();
	inline void ClearParam();

private:
};


//供不含有不确定因素项的数据段使用
//各种请求结构中有InitializeNoParam()的,可以作为此模版类的T
template<typename T>
class RequestMessageDataNoParam : public GenerationData
{
public:
	inline RequestMessageDataNoParam();
	inline ~RequestMessageDataNoParam();
	inline T * GetRequestMessage();//获取请求信息结构。用于读写。

private:
	inline void Init();
};

#include "RequestMessage.inl"
#endif //REQUESTMESSAGEDATA_H
