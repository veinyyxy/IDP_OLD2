//////////////////////////////////////////////////////////////////////////
// RequestMessage.h
// created by ssy 20080729
// for Product Request Message
//
// ˵����
// ÿ��һ��������Ϣ���Ƚ���֯��list<GenerationData*>����ʽ��
// ����ĸ�������ṹ��������RequestMessageData����RequestMessageDataNoParam��֯��GenerationData��
// ��ΪRequestMessageData��RequestMessageDataNoParam��GenerationData�����ࡣ
//
// ���У�list�ĵ�һ��ΪRequestMessageHeader���ɵ�GenerationData��
//
// �ڶ���Ϊ����Ĺ������֣�����RequestMessageHeader�е�RequestMessageType�Ƕ�Ӧ��
// OnceRequestMessagePublic��RoutineRequestMessagePublic��AlarmRequestMessagePublic�е�һ�ֹ��ɵ�GenerationData��
//
// �������n��Ϊ�����б�ĸ������ݣ�����RequestMessageHeader�е�RequestMessageType�Ƕ�Ӧ��
// OnceRequestMessage��RoutineRequestMessage��AlarmRequestMessage�е�һ�ֹ��ɵ�GenerationData��
//
// ��֯�õ�list<GenerationData*>��ͨ��GDataStream.h�еķ����γ������ڴ��GenerationDataStream�ٽ��з��͡�
// ���յ���GenerationDataStream��Ҫ��ԭ��list<GenerationData*>�ٽ��н�����
// �μ�ʾ������
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

//���ֲ��������ƵĶ��壺
#define RPN_MOSAIC_SITE_LIST		"mosaic"	// �Զ�������վ���б�RPT_gChar8
#define RPN_ALARM_ACA_FORMAT		"almACA%d"	// ��ACA����area-condition-attachment��Ӧ��RPT_gChar��%sΪ0��1��2��������Ϊÿ���ӵ�������Ʒ���ACA����map�е�������ͬ��
#define RPN_ALARM_AREA_LIST			"almArea"	// ���������б�RPT_gDouble
//#define RPN_ALARM_AREA_INFO     	"almAreaInfo"	// �������򼶱𼰱�ţ�RPT_gInt16
#define RPN_ALARM_CONDITION_LIST	"almCondi"	// ���������б�RPT_gInt16���б���ÿһ��������������������������ֵ��ɣ���˾��������б������ֵ��Ӧ����˫�����������������������С���������������ôֵ��-1��ʾ��
#define RPN_ALARM_ATTACHMENT_LIST	"almAtta"	// �������Ӳ�Ʒ�б�RPT_gInt16


//��������㷨�������������ڴ˴��������壺
#define RPN_ONCE_LRM_ANGLE	    "lrmAngle"	  //LRM ��λ��,RPT_gDouble
#define RPN_ONCE_LRM_DISTENCE	"lrmDistence" //LRM �����,RPT_gInt16
#define RPN_ONCE_LRM_ROW	    "mlrmRow"	  //MLRM ��,RPT_gInt16
#define RPN_ONCE_LRM_COL	    "mlrmCol"	  //MLRM ��,RPT_gInt16
#define RPN_ONCE_LRM_COUNT	    "lrmCount"	  //LRM/MLRM ����,RPT_gInt16
#define RPN_ONCE_LRM_HEIGHT	    "lrmHeight"	  //LRM/MLRM ���,RPT_gInt16

#define RPN_ONCE_V_BLUR			"vblur"		  				//��ģ������+���ǲ� RPT_gInt16
#define RPN_ONCE_V_PRODUCTDEPENDENT	"vproductdependent"		//��ģ����Ʒ����

//AS by haodemei 20081007
#define RPN_ONCE_ECHOTOPREFTHRESHOLD    "EchoTopRefThreshold"     //�ز���������������ֵ RPT_gInt32
#define RPN_ONCE_BEAMWIDTH              "BeamWidth"               //�״ﲨ����� RPT_gDouble
#define RPN_ONCE_LIMITOFSTORMV          "LimitOfStormV"           //��ط籩����ٶ���ֵ RPT_gInt16
#define RPN_ONCE_MAXTIMESLOT            "MaxTimeSlot"             //����������ɨ����������ʱ���� RPT_gInt32
#define RPN_ONCE_VELOCITYOFSTORM        "VelocityOfStorm"         //�籩���Ĭ���ƶ��ٶ� RPT_gDouble
#define RPN_ONCE_DIRECTIONOFSTORM       "DirectionOfStorm"        //�籩���Ĭ���ƶ����� RPT_gInt16
#define RPN_ONCE_RESLONLAT              "ResLonLat"               //������γ����ֱ��� RPT_gDouble
#define RPN_ONCE_ZR_A                   "ZR_A"                    //ZR ��ϵ�˷�ϵ��   RPT_gInt32
#define RPN_ONCE_ZR_B                   "ZR_b"                    //ZR ��ϵ�˷�ϵ��   RPT_gDouble
#define RPN_ONCE_H_CAPPI                "H_CAPPI"                 //�ȸ��� RPT_gInt32
//AE by haodemei 20081007

//////////////////////////////////////////////////////////////////////////

// ��������
enum RequestMessageType
{
	Product_Request_None = 0,   //���ɲ�Ʒʧ��
	Product_Request_Routine,	// ��������
	Product_Request_Alarm,		// ��������
	Product_Request_Once,		// һ��������	
	Product_Request_AlarmAttatch, //�������󸽼Ӳ�Ʒ
};

#pragma pack(push,1)

// Ӧ�ò���������ͷ
typedef struct tagRequestMessageHeader
{
	RequestMessageType requestType;	// ��������
	gChar to[8];
	gChar from[8];
	gInt32 length;
public:
	inline void InitializeNoParam();
} RequestMessageHeader;


// ��������ṹ��

// һ��������
// һ���������б�Ĺ�������
typedef struct tagOnceRequestMessagePublic
{
	gChar RadarSite[8];				//�״�վվ���Ż��������ı��
	gInt16 NumberOfParameter;		//�����������Զ������������RadarSiteΪX8000����������Զ��������״�վ�б�
	gInt16 HistoryOrCurrent;		//0����ʷ���ݡ�1��ʵʱ���ݡ�
	gInt32 ScanDateOrRepeatTime;	//HistoryOrCurrentΪ0ʱ��ʾ��ɨ���ڣ�-1Ϊ������Ч��ɨ��HistoryOrCurrentΪ1ʱ����ʾ�ظ�������
	gInt32 ScanTimeOrInterval;		//HistoryOrCurrentΪ0ʱ��ʾ��ɨʱ�䣬-1Ϊ������Ч��ɨ��HistoryOrCurrentΪ1ʱ����ʾ���ؼ����
	gInt16 IsQualityControl;		//0�������Զ����������ơ�1������Ĭ���������ơ�
public:
	inline void Initialize();
} OnceRequestMessagePublic;

// һ���������б�ĸ�������
typedef struct tagOnceRequestMessage
{
	gInt16 ProductCode;				//��Ʒ���ţ�����μ���
	gInt16 NumberOfParameter;		//�����������Զ�������������������ж���㷨������
public:
	inline void Initialize();
} OnceRequestMessage;


// ��������
// ���������б�Ĺ�������
typedef struct tagRoutineRequestMessagePublic
{
	gChar RadarSite[8];				//�״�վվ���Ż��������ı��
public:
	inline void InitializeNoParam();
} RoutineRequestMessagePublic;

// ���������б�ĸ�������
typedef struct tagRoutineRequestMessage
{
	gInt16 ProductCode;				//��Ʒ���ţ�����μ���
	gInt16 Interval;				//���ز�Ʒ���
public:
	inline void InitializeNoParam();
} RoutineRequestMessage;


// ��������
enum AlertAreaType     
{
   RectangleArea = 0,   //���ڲ�Ʒѡ��
   DistrictArea,        //������������ѡ��,limei,04.29 
};
// ���������б�Ĺ�������
typedef struct tagAlarmRequestMessagePublic
{
	gChar RadarSite[8];				//�״�վվ���Ż��������ı��
	gInt32 AlarmEndDate;			//������Ч���ڣ�-1��ʾ������
	gInt32 AlarmEndTime;			//������Чʱ�䣬-1��ʾ������
    AlertAreaType AreaType;   //ѡ����������
public:
	inline void InitializeNoParam();
} AlarmRequestMessagePublic;

// ���������б�ĸ�������
typedef struct tagAlarmRequestMessage
{
	gInt16 ProductCode;				//��Ʒ���ţ�����μ���
	gInt16 NumberOfParameter;		//�����������Զ������������ACA��������������-��������-�������Ӳ�Ʒ��Ӧ���������
public:
	inline void Initialize();
} AlarmRequestMessage;

// ���������б���ÿһ��ProductCode�µġ���������-��������-�������Ӳ�Ʒ��Ӧ���е�һ��
typedef struct tagAlarmACA
{
	gInt16 NumberOfParameter;		//�����������Զ���������������������򡢾��������б��������Ӳ�Ʒ�б�
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
	RPT_gChar8,		//means data is gChar[][8],to use pointer as gChar (*)[8] //������ڼ�¼����վ���б�ġ�
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
	gInt32 TotalSize;					//��ǰ���������ܳ���
	gInt32 NameSize;					//�������ֳ��ȣ�����'\0'
	gInt32 ValueSize;					//�������ݵ�size�����ֽڼ�
	RequestParameterType ValueType;		//��������
} RequestParameterHead;

#pragma pack(pop)


template<typename T>
class RequestMessageData;

class RequestParametersMap : public std::map<std::string,std::pair<RequestParameterType,std::vector<char> > >
										//map<������,pair<��������RequestParameterType,���ݴ��>>
{
	template<typename T>
	friend class RequestMessageData;

public:
	inline RequestParametersMap(){};
	inline ~RequestParametersMap(){};

	//һ�������һ�������Ķ��ֵ��ָ�������������������в����ظ������������ͣ��������鳤�ȣ����������׵�ַ��
	inline GHRESULT AddParam(char * ParamName, const RequestParameterType & ParamType, gInt32 ArrayLength, void * pDataArray=NULL);
	//���һ��������һ��ֵ��ָ�������������������в����ظ������������ͣ�������ַ��
	inline GHRESULT AddParamOne(char * ParamName, const RequestParameterType & ParamType, void * pDataArray=NULL);
	//���ݲ�����ɾ�����в���
	inline GHRESULT RemoveParam(const char * ParamName);

	//���ݲ�������ȡ��ǰ�б��в��������׵�ַָ�롣
	inline void * GetParamPointer(const char * ParamName)const;
	//���ݲ�������ȡ��ǰ�б��в������͡�
	inline RequestParameterType GetParamType(const char * ParamName)const;
	//���ݲ�������ȡ��ǰ�б��в���ռ�ô洢�ռ�(���ֽڼ�)��
	inline gInt32 GetParamSize(const char * ParamName)const;
	//���ݲ�������ȡ��ǰ�б��в������鳤�ȡ�
	inline gInt32 GetParamArrayLength(const char * ParamName)const;
	//��ȡ����ռ���ֽ�����
	inline gInt32 GetParamTypeSize(const RequestParameterType & ParamType)const;
	//��ȡ��ǰ��¼��������
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


//�����в�ȷ������������ݶ�ʹ��
//��������ṹ����Initialize()��NumberOfParameter��,������Ϊ��ģ�����T
template<typename T>
class RequestMessageData :public GenerationData
{
	friend class RequestParametersMap;

public:
	inline RequestMessageData();
	inline ~RequestMessageData();

	//��ȡ������Ϣ�ṹ�����ڶ�д��
	inline T * GetRequestMessage();
	//��ȡ�����б�����RequestParametersMap���ͽ�����
	inline RequestParametersMap GetParametersMap();

	//����RequestParametersMap��д�����б�����֮��NumberOfParameter��ΪĿǰ�Ĳ���������
	//��ע�⣺���ù��˺���֮��һ��Ҫ���µ���GetRequestMessageȡ��Tָ�룡
	inline GHRESULT ReplaceParametersFromMap(const RequestParametersMap & ParamMap);

protected:
	inline void * GetParametersPointer(gInt16 Idx);
	inline gInt16 * GetParametersNumber();
	inline void Init();
	inline void ClearParam();

private:
};


//�������в�ȷ������������ݶ�ʹ��
//��������ṹ����InitializeNoParam()��,������Ϊ��ģ�����T
template<typename T>
class RequestMessageDataNoParam : public GenerationData
{
public:
	inline RequestMessageDataNoParam();
	inline ~RequestMessageDataNoParam();
	inline T * GetRequestMessage();//��ȡ������Ϣ�ṹ�����ڶ�д��

private:
	inline void Init();
};

#include "RequestMessage.inl"
#endif //REQUESTMESSAGEDATA_H
