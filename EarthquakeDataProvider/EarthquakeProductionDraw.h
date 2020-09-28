/** @file   EarthquakeProductionDraw.h
* @brief     地震数据绘制方法接口文件
* @author    陈志学
* @date      2013年8月10
* @version   v1.0
  @note      detailed description 修改人  修改时间 修改历史
*/ 
#pragma once
#include "earthquakedataprovider_global.h"
#include "layer.h"
#include <QObject>
#include <QtCore/QString>
#include <QtGui/QImage>


typedef enum
{
	UNKONW_TYPE,
	STRUCT_MECA1,
	STRUCT_MECA2,
	STRUCT_MECA3,
	STRUCT_MECA4,
	STRUCT_MECA5,
	STRUCT_MECA6,
	STRUCT_MECA7,
	STRUCT_MECA8,
	STRUCT_MECA9,
	STRUCT_PROFILE,
	STRUCT_RASTER_ASC,
	STRUCT_RASTER_XYZ,
	STRUCT_CUBE_DATA

} EARTHQUAKE_DATA_TYPE, *P_EARTHQUAKE_DATA_TYPE;

typedef struct _ColorNode_Product
{
	double fStart;
	double fEnd;
	int   nRed;
	int   nGreen;
	int   nBlue;
}E_COLORNODE_PRODUCT, *P_E_COLORNODE_PRODUCT;

typedef QVector<P_E_COLORNODE_PRODUCT>  E_COLORTABLE_VECTOR, *P_E_COLORTABLE_VECTOR;//颜色数组

struct _MECHANISM
{
	QString id;
	double lat;
	double lon;
	double hight;
	double magms;
	QImage img;
};
typedef QVector<_MECHANISM> mech_vec;

using namespace goto_gis;
/** @class  EQProductionDraw
    @brief   地震数据绘制接口类（虚基类）
    @author  陈志学
    @note    数据分类型绘制接口的虚基类，绘制方法将在子类中实现
*/ 
class EQProductionDraw :public QObject
{
public:
	/** @brief 判断产品绘图类型 */ 
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct()=0;
	/** @brief 加载文件 */ 
	virtual void LoadFile(const QString& filename) = 0;
	/** @brief 画产品图片，指定某张图片 */ 
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0)=0;
};
