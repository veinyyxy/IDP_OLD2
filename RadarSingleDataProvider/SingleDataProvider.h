/**************************************************************************************/
/*��Ŀ���� �˻�����ƽ̨                                                               */
/*�� �ߣ�  ��÷                                                                     */
/*�� λ�� �й�����뼼������ɷ����޹�˾                                              */
/*����޸�ʱ�䣺 2011�ꣿ�£���                                                       */
/*��Ȩ������ ��Ȩ����2010-2018 ����������ҵ����������Ȩ��                             */
/*Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.All rights reserved */
/*��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����        */
/*           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���                */
/**************************************************************************************/
/*�ļ�����SingleDataProvider.h SingleDataProvider.cpp                             */
/*�� ���� SingleDataProvider                                                 */
/*�� �ࣺ ProductBase                                                  */
/*�� �ࣺ ��                                                            */
/*����˵������վ��Ʒ���ݽ�����                                               */
/*����˵����                                                            */
/************************************************************************/
/*V 1.1                                                                 */
/*�� �ߣ���÷                                                        */
/*������ڣ�2012��2��23��                                               */
/*����˵���� �������ӿ�                                               */
/*V 1.0                                                                 */
/*ԭ���� ����÷                                                       */
/*������ڣ�2012�ꣿ�£���                                              */
/************************************************************************/
#pragma once
#include "QT_Include.h"
//#include "DivisionData.h"

#include "radarsingledataprovider_global.h"
#include "../GDataTypes/SingleProductBase.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "RadReadColor_XML.h"
#include "Layer.h"
#include "RadProjTransform.h"
#define M_PI 3.1415926535897932384626433832795

// typedef enum tgProductType
// {
// 	enProductRadial,
// 	enProductGrid,
// 	enProductStruct,
// 	enProductUnkown
// }CProductDrawType;



//��վ��Ʒ���ݽ�����
using namespace goto_gis;
class  SingleDataProvider :public ProductBase
{
	//Q_OBJECT

public:
	SingleDataProvider();
	~SingleDataProvider();
	void ProdDataProvider(int requestLayer,double &maxValue,QHash<int ,QVector4D>colorHash, Layer &pLayer,RadProjTransform *proj);
	CProductDrawType GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	int GetCountOfLayer();  //��Ʒ����

	//void GetRadialData(QMap<QString ,RadialGridDataList*> &RDataMap, int requestLayer,P_COLORTABLE_VECTOR pColorTable,double &maxValue); //�����Ʒ���ݽ���
	void GetRadialData(Layer &pLayer, RadProjTransform *proj,int requestLayer,QHash<int ,QVector4D>colorHash,double &maxValue); //�����Ʒ���ݽ���
	void SinGetGridData(Layer &pLayer, RadProjTransform *proj,int requestLayer,QHash<int ,QVector4D>colorHash,double &maxValue); //դ���Ʒ���ݽ���

	void DrawRadialPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); //�����Ʒ��ͼ
	void DrawGridPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); //դ���Ʒ��ͼ
    double GetValue(double lon,double lat,double height,RadProjTransform *proj);//������λ�õĻز�ֵ
	double GetRadialValue(double lon,double lat,double height,RadProjTransform *proj);//������λ�õĻز�ֵ
	double GetGridValue(double lon,double lat,double height,RadProjTransform *proj);//������λ�õĻز�ֵ

	double GetDataRadius(int ilayer);
	int ProductCode();
	double GetRadiaResolution(int ilayer);
	double GetRadiaElevation(int ilayer);
	void DrawPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor);
	double GetRadialRadius(int ilayer);
	double	GetGridRadius(int ilayer);

	static int GetColorIndexKey(int productcode,double fstart);
};
