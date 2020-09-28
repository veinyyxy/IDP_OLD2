#ifndef RADARSTRUCTDATAPROVIDER_H
#define RADARSTRUCTDATAPROVIDER_H

#include "radarstructdataprovider_global.h"
#include "DataProvider.h"
#include "RadarStructProductionDraw.h"

using namespace goto_gis;

class RADARSTRUCTDATAPROVIDER_EXPORT RadarStructDataProvider:public DataProvider
{
public:
	RadarStructDataProvider();
	~RadarStructDataProvider();
	static RadarStructDataProvider *Instance();//得到实例
	bool LoadFile(const std::string& filename);
	QString DrawThumbs(QString& filepath);//画缩略图，给文件路径，在相同路径下生成.png文件
	 void GetRect(QRectF &rect );
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
	virtual QString GetDataUnit();//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height);//获取鼠标位置的数据数值
private:
	QStringList  FileNameAnalyes(QString fName);//文件名解析，按照站号，类型，时间返回一list
	void  ProductDataProvider(GenerationData* gData,int requestLayer = -1);//读取数据
	void  InitColorTable();//初始化颜色表
	void  InitProductNameTable();//初始化文件名表
	RadarStructProductionDraw* GetProductDrawInstance(STRUCT_DATA_TYPE nProductCode);//获取特定的产品绘图类指针，基类指针
	void ReleaseProductDrawInstance(RadarStructProductionDraw* lpInstance);//释放产品绘图类指针，基类指针
	//设置和获得雷达站中心
	QVector3D  stationPosition();
	bool  setRadarCentral(QVector3D p);
	void AnalysisFile(const std::string& filename);
private:
	GenerationData*     gData;//所有数据
	cDrawProduct*     pDrawProduct;//分析算法数据
	cDrawMultiProduct*     pDrawMulProduct;
	P_Radar_Struct_Data  m_pStructData;
	int  m_requestLayer;
	P_COLORTABLE_VECTOR     m_ColorTable;
	CRadReadColor_XML     xml_reader;
	COLORTABLE_HASH     m_hColorTableHash;
	PRODUCT     m_ProductNameTable;
	QHash<QString,QVector4D>     m_pColorValueHash;
	RadarStructProductionDraw*	 m_pDraw;
	QVector3D m_central;
	//RadarStructDataProvider*     pStructGdata;
	RadarStructProductionDraw* pDraw;
private:
	static RadarStructDataProvider * m_pStructProvider;
};

RadarStructDataProvider* g_pRadarProvider = 0;



#endif // RADARSTRUCTDATAPROVIDER_H
