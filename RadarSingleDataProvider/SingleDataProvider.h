/**************************************************************************************/
/*项目名： 人机交互平台                                                               */
/*作 者：  李梅                                                                     */
/*单 位： 中国软件与技术服务股份有限公司                                              */
/*最后修改时间： 2011年？月？日                                                       */
/*版权声明： 版权所有2010-2018 中软气象事业部保留所有权利                             */
/*Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.All rights reserved */
/*重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。        */
/*           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。                */
/**************************************************************************************/
/*文件名：SingleDataProvider.h SingleDataProvider.cpp                             */
/*类 名： SingleDataProvider                                                 */
/*父 类： ProductBase                                                  */
/*子 类： 无                                                            */
/*功能说明：单站产品数据解析类                                               */
/*调用说明：                                                            */
/************************************************************************/
/*V 1.1                                                                 */
/*作 者：李梅                                                        */
/*完成日期：2012年2月23日                                               */
/*更新说明： 添加所需接口                                               */
/*V 1.0                                                                 */
/*原作者 ：李梅                                                       */
/*完成日期：2012年？月？日                                              */
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



//单站产品数据解析类
using namespace goto_gis;
class  SingleDataProvider :public ProductBase
{
	//Q_OBJECT

public:
	SingleDataProvider();
	~SingleDataProvider();
	void ProdDataProvider(int requestLayer,double &maxValue,QHash<int ,QVector4D>colorHash, Layer &pLayer,RadProjTransform *proj);
	CProductDrawType GetTypeOfProduct();//判断产品绘图类型
	int GetCountOfLayer();  //产品层数

	//void GetRadialData(QMap<QString ,RadialGridDataList*> &RDataMap, int requestLayer,P_COLORTABLE_VECTOR pColorTable,double &maxValue); //径向产品数据解析
	void GetRadialData(Layer &pLayer, RadProjTransform *proj,int requestLayer,QHash<int ,QVector4D>colorHash,double &maxValue); //径向产品数据解析
	void SinGetGridData(Layer &pLayer, RadProjTransform *proj,int requestLayer,QHash<int ,QVector4D>colorHash,double &maxValue); //栅格产品数据解析

	void DrawRadialPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); //径向产品绘图
	void DrawGridPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); //栅格产品绘图
    double GetValue(double lon,double lat,double height,RadProjTransform *proj);//获得鼠标位置的回波值
	double GetRadialValue(double lon,double lat,double height,RadProjTransform *proj);//获得鼠标位置的回波值
	double GetGridValue(double lon,double lat,double height,RadProjTransform *proj);//获得鼠标位置的回波值

	double GetDataRadius(int ilayer);
	int ProductCode();
	double GetRadiaResolution(int ilayer);
	double GetRadiaElevation(int ilayer);
	void DrawPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor);
	double GetRadialRadius(int ilayer);
	double	GetGridRadius(int ilayer);

	static int GetColorIndexKey(int productcode,double fstart);
};
