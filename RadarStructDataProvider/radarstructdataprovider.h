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
	static RadarStructDataProvider *Instance();//�õ�ʵ��
	bool LoadFile(const std::string& filename);
	QString DrawThumbs(QString& filepath);//������ͼ�����ļ�·��������ͬ·��������.png�ļ�
	 void GetRect(QRectF &rect );
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
	virtual QString GetDataUnit();//������ݼ�����λ
	virtual float GetPositionValue(double lon,double lat,double height);//��ȡ���λ�õ�������ֵ
private:
	QStringList  FileNameAnalyes(QString fName);//�ļ�������������վ�ţ����ͣ�ʱ�䷵��һlist
	void  ProductDataProvider(GenerationData* gData,int requestLayer = -1);//��ȡ����
	void  InitColorTable();//��ʼ����ɫ��
	void  InitProductNameTable();//��ʼ���ļ�����
	RadarStructProductionDraw* GetProductDrawInstance(STRUCT_DATA_TYPE nProductCode);//��ȡ�ض��Ĳ�Ʒ��ͼ��ָ�룬����ָ��
	void ReleaseProductDrawInstance(RadarStructProductionDraw* lpInstance);//�ͷŲ�Ʒ��ͼ��ָ�룬����ָ��
	//���úͻ���״�վ����
	QVector3D  stationPosition();
	bool  setRadarCentral(QVector3D p);
	void AnalysisFile(const std::string& filename);
private:
	GenerationData*     gData;//��������
	cDrawProduct*     pDrawProduct;//�����㷨����
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
