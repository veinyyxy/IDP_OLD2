#ifndef RADARSINGLEDATAPROVIDER_H
#define RADARSINGLEDATAPROVIDER_H

#include "radarsingledataprovider_global.h"
#include "QT_Include.h"
#include "DataProvider.h"

#include "RadReadProduct_XML.h"
#include "RadReadStationFile.h"

#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "../XMLProvider/QT_Include.h"
#include "SingleDataProvider.h"

#include "goGeometry.h"
#include "goQuadrilateral.h"
#include "geos/geom/Polygon.h"
#include "RadarVectorLayer.h"
#include "RadProjTransform.h"


using namespace goto_gis;
class InterpolationRadarDataInterface;
class RadarMarchingCubes;

class RADARSINGLEDATAPROVIDER_EXPORT RadarSingleDataProvider:public DataProvider
{
public:
	RadarSingleDataProvider();
	virtual ~RadarSingleDataProvider();
	static RadarSingleDataProvider *Instance();//得到实例
	bool LoadFile(const std::string& filename);
	P_COLORTABLE_VECTOR GetColorTable();
	QStringList  DrawThumbs(QString& filepath);//画缩略图，给文件路径，在相同路径下生成.png文件
	void GetRect(QRectF &rect );
	osg::Vec3  stationPosition();//设置和获得雷达站中心
	RadProjTransform* Coords();
	bool  QualityControl_fuzzy(bool fuzzy);//速度退模糊算法

	/////////////////////////////面积统计/////////////////////////////////////////////
	inline cDrawProduct*   GetDrawProduct(){return  pDrawProduct;}
	inline void SetRequsetLayer(int iLayerNO) {m_iLayerNO = iLayerNO;}
	inline int GetRequsetLayer() {return m_iLayerNO;}
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
	virtual QString GetDataUnit();//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height);//获取鼠标位置的数据数值
private:
	//QStringList  FileNameAnalyes(QString fName);//文件名解析，按照站号，类型，时间返回一list
	void  ProductDataProvider(GenerationData* gData,Layer* layer,RadProjTransform *proj,int requestLayer = -1);//读取数据
	bool  InitColorTable();//初始化颜色表
	void  InitProductNameTable();//初始化文件名表
	int   GetCountOfLayer();  //产品层数
	int  GetSingleProductCode();
	double  GetSingleProductRadius(int ilayer);//获得但站产品的扫描半径
	//osg::Vec3  stationPosition();//设置和获得雷达站中心
	void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QStringList FileNameAnalyes( QString fName );
	void DumpSingleAttr(Layer *layer,int requestlayer);
	//void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QString  GetProductNameByShortName(int productcode);
	QString GetCircleRange(int ncode);
	bool DrawCircle(Layer * pLayer,double X,double Y,double h,double R,double Res,RadProjTransform *proj);
	bool DrawPic(QString& filePath,GenerationData* gData,int requestLayer = -1);

private:
	GenerationData*        gData;//所有数据
	cDrawProduct*          pDrawProduct;//分析算法数据
	SingleDataProvider*    pSingleGdata;
	RadProjTransform *proj;
	CRadReadColor_XML      xml_reader;
	int                     m_nParam;
	int                     m_layers;
	int                     m_requestLayer;
	double                  m_MaxValue;
	float                   m_radius;
	PRODUCT                 m_ProductNameTable;
	bool                    m_btransform;
	QMap<QString ,osg::Vec3> m_box;
	QHash <int,float>     MaxvalHash;
	QHash<int ,QVector4D> m_pColorValueIndexHash;
	InterpolationRadarDataInterface* m_ExtendInterface;
	
	RadarMarchingCubes* m_MarchinCubesExtend;
	int m_iLayerNO; //面积统计图层 number
private:
	static RadarSingleDataProvider * m_pSingleProvider;
	SymbolLibraryInterface* m_pSymbolLibraryInterface;
};





#endif // RADARSINGLEDATAPROVIDER_H
