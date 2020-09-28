/** @file 
* @brief     组网产品数据解析类   MosaicDataProvider.h MosaicDataProvider.cpp
* @author    陈志学
* @date      2012年6月23日
* @version   version
  @note      2013年6月
*/ 
#pragma once
#include "QT_Include.h"

#include "../GDataTypes/MultiProductBase.h"
#include "../GDataTypes/SingleProductBase.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "RadReadColor_XML.h"
#include "CoordinateTransform.h"
#include "Layer.h"


typedef struct tagSaveEleInfo
{
	int id;
	float Lon;                 //经度 （度）
	float Lat;                 //纬度 （度）
	float Hgt;                 //海拨高度 km 

}SAVEELE_INFO;


typedef struct _staionInfo
{
	int id;                    //作业区id
	int code;                  //炮点编号
	float Lon;                 //经度 （度）
	float Lat;                 //纬度 （度）
	float elevation;           //仰角
	float azimuthal;           //方位角
	int count;                 //炮弹数量
	QDateTime start;           //起始时间
	QDateTime end ;            //结束时间

}STATION_INFO;

typedef QVector<STATION_INFO>Station_Vec;
/** @class   MosaicDataProvider
    @brief   组网产品数据解析类
    @author  陈志学
    @note    基类MultiProductBase 。不可再此类中添加成员变量
*/ 

using namespace goto_gis;
class  MosaicDataProvider :/* public RadarProductDataProvider,*/public MultiProductBase
{
	//Q_OBJECT

public:
	MosaicDataProvider();
	~MosaicDataProvider();
	/** @brief 数据导出接口函数 */ 
	void ProdDataProvider(Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash);
	/** @brief 判断产品绘图类型 */ 
	CProductDrawType GetTypeOfProduct();
	/** @brief 经纬网格数据解析 */ 
    void MosGetGridData(Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash ); //经纬网格产品数据解析
	/** @brief 绘制缩略图 */
	void DrawPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor);
	/** @brief 绘制经纬网格数图片 */
	void DrawGridPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); 
	/** @brief 获得数据代码 */
	int ProductCode();
	/** @brief 获得数据范围 */
	QStringList GetRange();
	/** @brief 获得数据范围 */
	QRectF *GetRect();
	/** @brief 获得当前数据回波值 */
	double GetValue(double lon,double lat,double height,CoordinateTransform *proj);//获得鼠标位置的回波值
	void LoadWeaEleAlgorithm(std::vector<SAVEELE_INFO> & savele_info);
	void LoadSaveEleInfo(gInt32 lon, gInt32 lat,gInt16 * m_pCap,gInt32 *m_pCapHgt,std::vector<SAVEELE_INFO> & savele_info);
};


