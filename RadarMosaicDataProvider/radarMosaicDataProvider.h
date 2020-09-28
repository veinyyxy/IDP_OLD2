/** @file 
* @brief     雷达组网数据提供器接口头文件
* @author    陈志学
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/ 
#pragma once
#include "RadarDPConfig.h"
#include "QT_Include.h"
#include "DataProvider.h"

#include "RadReadProduct_XML.h"
#include "RadReadStationFile.h"

#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "MosaicDataProvider.h"


#include "goGeometry.h"
#include "goQuadrilateral.h"
#include "geos/geom/Polygon.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

using namespace goto_gis;

typedef struct _MosaicAttr 
{
} MosaicAttr,*pMosaicAttr;
/** @class   RadarMosaicDataProvider
    @brief   雷达组网数据解析对外接口类
    @author  陈志学
    @note    雷达数据提供器的对外接口
*/ 
class Radar_EXPORT_IMPORT RadarMosaicDataProvider :public DataProvider
{
public:
	
	RadarMosaicDataProvider(void);
	~RadarMosaicDataProvider(void);
	/** @brief 得到实例 */ 
	static RadarMosaicDataProvider *Instance();
	/** @brief 加载文件*/ 
	bool LoadFile(const std::string& filename);
	/** @brief 得到色卡 */ 
	inline  P_COLORTABLE_VECTOR GetColorTable(){return m_ColorTable;}
	/** @brief 画缩略图，给文件路径，在相同路径下生成.png文件 */ 
	QString  DrawThumbs(QString& filepath);
	/** @brief 获得图像四角坐标 */ 
	QRectF *GetRect();
	/** @brief 获得炮点预警范围 */ 
	QRectF *GetRect(float lat,float lon);
	/** @brief 加载BREF文件 */ 
	bool LoadRefFile(const std::string& filename);

	inline cDrawMultiProduct* GetDrawProduct() {return pDrawMulProduct;}
	inline void SetRequsetLayer(int iLayerNO) {m_iLayerNO = iLayerNO;}
	inline int GetRequsetLayer() {return m_iLayerNO;}
	virtual QString GetDataUnit();//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height);//获取鼠标位置的数据数值
    void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
private: 
	/** @brief /文件名解析，按照站号，类型，时间返回一list */ 
	 QStringList  FileNameAnalyes(QString fName);
	 /** @brief 读取数据 */ 
	 void  ProductDataProvider(Layer *pLayer,GenerationData* gData,int requestLayer = -1);
	  /** @brief 初始化颜色列表 */ 
	 void  InitColorTable();
	  /** @brief 初始化文件名表 */ 
	 void  InitProductNameTable();
	  /** @brief 读取属性 */ 
	 void  DumpMosaicAttr(Layer *layer);
	 /** @brief 获得产品中英文名称 */
	 QString  GetProductName(int ncode);
	 /** @brief 获得产品中英文名称 */
	 QString  GetProductNameByShortName(int productcode);
	 /** @brief 获得产品代号 */
	 int  GetProductCode();
	 /** @brief 绘制产品缩略图 */
	 bool DrawPic(QString& filePath,GenerationData* gData,int requestLayer = -1);
	 /** @brief 获得产品四边属性 */
	 QList<STATION_INFO>* GetStationInfoByRect(QRectF *rect,int id);
	 /** @brief 初始化产品投影 */
	 bool setAngle(float lat,float lon ,float s_hight,QList<STATION_INFO>& station_info);
	 /** @brief 获得方位角 */
	double   Get_Angle_Of_Direction(double dx, double dy);
	/** @brief 求距离 */
	double	     Get_Length(double dx, double dy);
	
private:
	GenerationData*        gData;//所有数据
	//cDrawProduct*          pDrawProduct;//分析算法数据
	cDrawMultiProduct*     pDrawMulProduct;
	MosaicDataProvider*    pMosaicGdata;

	CRadReadColor_XML      xml_reader;
	COLORTABLE_HASH        m_hColorTableHash;
	double                  m_MaxValue;
	PRODUCT                 m_ProductNameTable;
	QRectF                  m_Rect;
	QHash <int,float>     MaxvalHash;
	P_COLORTABLE_VECTOR     m_ColorTable;
	QHash<int ,QVector4D> m_pColorValueIndexHash;
	QHash<int,int>     mHighHash;
	QSqlDatabase m_db;
	int m_iLayerNO; //面积统计图层 number
private:
	static RadarMosaicDataProvider * m_pRadarProvider;
};


