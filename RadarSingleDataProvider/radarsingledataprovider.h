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
	static RadarSingleDataProvider *Instance();//�õ�ʵ��
	bool LoadFile(const std::string& filename);
	P_COLORTABLE_VECTOR GetColorTable();
	QStringList  DrawThumbs(QString& filepath);//������ͼ�����ļ�·��������ͬ·��������.png�ļ�
	void GetRect(QRectF &rect );
	osg::Vec3  stationPosition();//���úͻ���״�վ����
	RadProjTransform* Coords();
	bool  QualityControl_fuzzy(bool fuzzy);//�ٶ���ģ���㷨

	/////////////////////////////���ͳ��/////////////////////////////////////////////
	inline cDrawProduct*   GetDrawProduct(){return  pDrawProduct;}
	inline void SetRequsetLayer(int iLayerNO) {m_iLayerNO = iLayerNO;}
	inline int GetRequsetLayer() {return m_iLayerNO;}
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
	virtual QString GetDataUnit();//������ݼ�����λ
	virtual float GetPositionValue(double lon,double lat,double height);//��ȡ���λ�õ�������ֵ
private:
	//QStringList  FileNameAnalyes(QString fName);//�ļ�������������վ�ţ����ͣ�ʱ�䷵��һlist
	void  ProductDataProvider(GenerationData* gData,Layer* layer,RadProjTransform *proj,int requestLayer = -1);//��ȡ����
	bool  InitColorTable();//��ʼ����ɫ��
	void  InitProductNameTable();//��ʼ���ļ�����
	int   GetCountOfLayer();  //��Ʒ����
	int  GetSingleProductCode();
	double  GetSingleProductRadius(int ilayer);//��õ�վ��Ʒ��ɨ��뾶
	//osg::Vec3  stationPosition();//���úͻ���״�վ����
	void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QStringList FileNameAnalyes( QString fName );
	void DumpSingleAttr(Layer *layer,int requestlayer);
	//void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QString  GetProductNameByShortName(int productcode);
	QString GetCircleRange(int ncode);
	bool DrawCircle(Layer * pLayer,double X,double Y,double h,double R,double Res,RadProjTransform *proj);
	bool DrawPic(QString& filePath,GenerationData* gData,int requestLayer = -1);

private:
	GenerationData*        gData;//��������
	cDrawProduct*          pDrawProduct;//�����㷨����
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
	int m_iLayerNO; //���ͳ��ͼ�� number
private:
	static RadarSingleDataProvider * m_pSingleProvider;
	SymbolLibraryInterface* m_pSymbolLibraryInterface;
};





#endif // RADARSINGLEDATAPROVIDER_H
