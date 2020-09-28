#pragma once
#include "radarstructdataprovider_global.h"
#include "QT_Include.h"

#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"

#include "RadReadProduct_XML.h"
#include "RadReadStationFile.h"
#include "RadReadColor_XML.h"
#include "layer.h"
typedef enum
{
	UNKONW_TYPE, 
	STRUCT_VAD,
	STRUCT_VWP,
	STRUCT_SCT, 
	STRUCT_SCI,
	STRUCT_LRM, 
	STRUCT_SBT,  
	STRUCT_EVP,
	STRUCT_SWP,
	STRUCT_HI, 
	STRUCT_M, 
	STRUCT_CL,
	STRUCT_TITAN,
	STRUCT_TVS	,
	STRUCT_MSBT
} STRUCT_DATA_TYPE, *P_STRUCT_DATA_TYPE;
typedef struct 
{
	STRUCT_DATA_TYPE type;
	CProductRead * pData;
} RadarStructData , *P_Radar_Struct_Data;

using namespace goto_gis;
class  RadarStructProductionDraw :public QObject
{
public:
	virtual STRUCT_DATA_TYPE GetTypeOfProduct()=0;//�жϲ�Ʒ��ͼ����
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer = 0)=0;//����ƷͼƬ��ָ��ĳ��ͼƬ
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float) = 0;
	virtual inline float GetRadius(CProductRead*){return 0.0;}//��Ҫ�����й���ͼ��ʱ�����������
	virtual  void GetRect(QRectF &rect ){};
};