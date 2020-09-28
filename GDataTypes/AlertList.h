#include "GDef.h"
#include "IBase.h"
#include <list>
//#include "GXList.h"
#include "GDataStream.h"
#include "RequestMessage.h"
#pragma once
using namespace std;

typedef struct tagPoint     //����������Ϣ��ÿ�����������б���������ÿ���Ʒÿ�����λ�ã�դ��;�γΪ���꣬�����ƷΪ����źͿ���
{
	gInt16 x;    
	gInt16 y;
}Point;

typedef struct tagAreaPoint     //�����
{
	gDouble x;    
	gDouble y;
}AreaPoint;

typedef struct tagTresholdValue     //����������Ϣ��ÿ�����������б���������ÿ���Ʒÿ���������
{
	gInt16 TopTresholdValue;
	gInt16 BottomTresholdValue;
}Threshold;

typedef struct tagAlertArea      //����������Ϣ��ÿ�����������������
{
	AreaPoint first;
	AreaPoint second;
	AreaPoint thirdth;
	AreaPoint fouth;
}AlertArea;

typedef struct tagEveryLayerAlert       //����������Ϣ��ÿ�����������б���������ÿ���Ʒ����
{
	gInt16 LayerNo;                     //��ţ������ƷΪ-1��
	list<Point> layerpointlist;         
}EveryLayerAlert;

typedef struct tagEveryCondAlert       //����������Ϣ��ÿ�����������б�����������
{
	gInt16 AlertLevelCode;               //��������1-5��
	gInt16 TresholdValue[2];             //�ü��𱨾���ֵ��Χ����Сֵ�����ֵ��
	list<EveryLayerAlert> conditionlist;
}EveryCondAlert;
typedef struct tagEveryAlarmArea         //����������Ϣ�б�����������
{
	AlertArea alertarea;               //����������һ����ʾ��������߽��ı���
	gInt32 NumberOfAlertCondition;      //ͬһ�����������Ч������
	list<EveryCondAlert> listofeverycond;
}EveryAlarmArea;

typedef struct tagAlertMessageHead      // ����������Ϣ��ʽͷ�ļ�
{
	char SiteID[8];
	gUint16 AlertDate;                  //������Ʒ���ڣ������ձ�ʾ
	gInt16 ProductCode;                 //��Ʒ���
	gInt16 NumberOfLayer;               //�����ƷΪ0������ƷΪ����
	gInt16 NumberOfAlertArea;           //������������
	gInt16 TypeOfDataPackets;
	gInt16 MaxAlertLevel;		//�б�������ֵ���ļ���
	gUint16	PosPara1;			//�����зֱ��ʣ�1�ף���γ��������0.00001��,�������ǿⳤ���ף�
	gUint16	PosPara2;			//�����зֱ��ʣ�1�ף���γ��������0.00001�ȣ��������ǿ���
	gUint16  PosPara3;			//դ���γ���������������ǵ�1���ķ�λ�ǵ�λ��0.01�ȣ�0-359.99
	gUint16	PosPara4;			//դ���γ���������������ǽǼ�ࣨ0.01�ȣ�
	gInt32 ALertTime;                  //������Ʒʱ�䣬�����ʾ
	AreaPoint PosPara5;			//��վ���״�ľ�γ�ȣ����������Ͻǵľ�γ��
	gFloat PosPara6;			//��վ���״�߶�
	AlertAreaType Areatype;
}AlertMessageHead;  

typedef struct  
{
	AlertArea alertarea;               //����������һ����ʾ��������߽��ı���
	gUint32 NumberOfAlertCondition;      //ͬһ�����������Ч������
}AlarmArea;

typedef struct  
{
	gInt16 AlertLevelCode;               //��������1-5��
	gInt16 TresholdValue[2];             //�ü��𱨾���ֵ��Χ����Сֵ�����ֵ��
	gInt32 NumOfAlerts;
}AlarmCondition;

typedef struct  
{
	gInt16 LayerNo;
}AlarmLayer;

class DLLEXP_GSYS AlertList
{
public:
	AlertList(void);
	AlertList(AlertList & alertlist);
	~AlertList(void);
	void SetMsgHead( AlertMessageHead alertmsghead );  //���ñ���������Ϣͷ��Ϣ
	void AddEverycondTail(list<EveryCondAlert> &listofeverycond,gInt16 AlertLevelCode,gInt16 TresholdValue[2], EveryLayerAlert* layeralert); //��ĳһ������������ĳһ���������µ�list���ĳһ�������������������ݣ�������㣩
	void AddEveryareaTail(AlertArea *alertarea,gInt32 NumberOfAlertCondition,list<EveryCondAlert> *conditionlist, gInt16 MaxAlertLevel); //�򱨾������б����ĳһ���������б�
	
     AlertMessageHead * GetAertMessageHead(); //
	 AlertArea * GetAlarea(gInt16 Index);
	 gInt16 GetNumberConditionofarea(gInt16 IndexArea);
	 gInt16 GetNumberOfConditions(gInt16 IndexArea);
	 gInt16 GetAlertLevel(gInt16 IndexArea,gInt16 IndexNumCond);
	 gInt16* GetThresholdVal(gInt16 IndexArea,gInt16 IndexNumCond);
	 EveryLayerAlert* GetEveryLayerAlert(gInt16 IndexArea,gInt16 IndexNumCond,gInt16 IndexLayerNum);//��һ��Index ,��������ţ��ڶ���Index����������ţ�������Index����Ʒ�����
	 //list<EveryAlarmArea> GetAlarmAreaList();
	list<EveryAlarmArea>& GetAlarmAreaList();
	 GHRESULT GetSiteCode(char * pSiteCode);
	void TransToGData(/*AlertList InList, */GenerationData * OutData);
	void TransFromGData(/*AlertList * pOutList,*/ GenerationData * pInData);
private:
	list<EveryAlarmArea>  listofeveryarea; 
	AlertMessageHead alertmsghead;	
};
