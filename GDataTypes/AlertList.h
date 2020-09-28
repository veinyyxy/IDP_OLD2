#include "GDef.h"
#include "IBase.h"
#include <list>
//#include "GXList.h"
#include "GDataStream.h"
#include "RequestMessage.h"
#pragma once
using namespace std;

typedef struct tagPoint     //报警生成消息中每个报警区域中报警条件下每层产品每个点的位置，栅格和经纬为坐标，径向产品为径向号和库数
{
	gInt16 x;    
	gInt16 y;
}Point;

typedef struct tagAreaPoint     //区域点
{
	gDouble x;    
	gDouble y;
}AreaPoint;

typedef struct tagTresholdValue     //报警生成消息中每个报警区域中报警条件下每层产品每个点的坐标
{
	gInt16 TopTresholdValue;
	gInt16 BottomTresholdValue;
}Threshold;

typedef struct tagAlertArea      //报警生成消息中每个报警区域矩形坐标
{
	AreaPoint first;
	AreaPoint second;
	AreaPoint thirdth;
	AreaPoint fouth;
}AlertArea;

typedef struct tagEveryLayerAlert       //报警生成消息中每个报警区域中报警条件下每层产品描述
{
	gInt16 LayerNo;                     //层号（单层产品为-1）
	list<Point> layerpointlist;         
}EveryLayerAlert;

typedef struct tagEveryCondAlert       //报警生成消息中每个报警区域中报警条件描述
{
	gInt16 AlertLevelCode;               //报警级别（1-5）
	gInt16 TresholdValue[2];             //该级别报警阈值范围（最小值和最大值）
	list<EveryLayerAlert> conditionlist;
}EveryCondAlert;
typedef struct tagEveryAlarmArea         //报警生成消息中报警区域描述
{
	AlertArea alertarea;               //报警区域，是一个表示报警区域边界四边形
	gInt32 NumberOfAlertCondition;      //同一报警区域的有效报警数
	list<EveryCondAlert> listofeverycond;
}EveryAlarmArea;

typedef struct tagAlertMessageHead      // 报警生成消息格式头文件
{
	char SiteID[8];
	gUint16 AlertDate;                  //警报产品日期，儒略日表示
	gInt16 ProductCode;                 //产品编号
	gInt16 NumberOfLayer;               //单层产品为0，多层产品为层数
	gInt16 NumberOfAlertArea;           //报警区域数量
	gInt16 TypeOfDataPackets;
	gInt16 MaxAlertLevel;		//有报警的阈值最大的级别
	gUint16	PosPara1;			//数据行分辨率（1米）经纬网格则是0.00001度,径向则是库长（米）
	gUint16	PosPara2;			//数据列分辨率（1米）经纬网格则是0.00001度，径向则是库数
	gUint16  PosPara3;			//栅格或经纬是行数，径向则是第1根的方位角单位（0.01度）0-359.99
	gUint16	PosPara4;			//栅格或经纬是列数，径向则是角间距（0.01度）
	gInt32 ALertTime;                  //警报产品时间，毫秒表示
	AreaPoint PosPara5;			//单站是雷达的经纬度，组网是左上角的经纬度
	gFloat PosPara6;			//单站的雷达高度
	AlertAreaType Areatype;
}AlertMessageHead;  

typedef struct  
{
	AlertArea alertarea;               //报警区域，是一个表示报警区域边界四边形
	gUint32 NumberOfAlertCondition;      //同一报警区域的有效报警数
}AlarmArea;

typedef struct  
{
	gInt16 AlertLevelCode;               //报警级别（1-5）
	gInt16 TresholdValue[2];             //该级别报警阈值范围（最小值和最大值）
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
	void SetMsgHead( AlertMessageHead alertmsghead );  //设置报警生成消息头信息
	void AddEverycondTail(list<EveryCondAlert> &listofeverycond,gInt16 AlertLevelCode,gInt16 TresholdValue[2], EveryLayerAlert* layeralert); //向某一个报警区域中某一报警条件下的list添加某一个报警条件下坐标数据（单层或多层）
	void AddEveryareaTail(AlertArea *alertarea,gInt32 NumberOfAlertCondition,list<EveryCondAlert> *conditionlist, gInt16 MaxAlertLevel); //向报警区域列表添加某一报警条件列表
	
     AlertMessageHead * GetAertMessageHead(); //
	 AlertArea * GetAlarea(gInt16 Index);
	 gInt16 GetNumberConditionofarea(gInt16 IndexArea);
	 gInt16 GetNumberOfConditions(gInt16 IndexArea);
	 gInt16 GetAlertLevel(gInt16 IndexArea,gInt16 IndexNumCond);
	 gInt16* GetThresholdVal(gInt16 IndexArea,gInt16 IndexNumCond);
	 EveryLayerAlert* GetEveryLayerAlert(gInt16 IndexArea,gInt16 IndexNumCond,gInt16 IndexLayerNum);//第一个Index ,报警区序号，第二个Index报警条件序号，第三个Index，产品层序号
	 //list<EveryAlarmArea> GetAlarmAreaList();
	list<EveryAlarmArea>& GetAlarmAreaList();
	 GHRESULT GetSiteCode(char * pSiteCode);
	void TransToGData(/*AlertList InList, */GenerationData * OutData);
	void TransFromGData(/*AlertList * pOutList,*/ GenerationData * pInData);
private:
	list<EveryAlarmArea>  listofeveryarea; 
	AlertMessageHead alertmsghead;	
};
