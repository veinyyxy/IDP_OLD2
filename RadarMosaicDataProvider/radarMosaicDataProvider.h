/** @file 
* @brief     �״����������ṩ���ӿ�ͷ�ļ�
* @author    ��־ѧ
* @date      2013��3��
* @version   version
  @note      2013�� 6��
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
    @brief   �״��������ݽ�������ӿ���
    @author  ��־ѧ
    @note    �״������ṩ���Ķ���ӿ�
*/ 
class Radar_EXPORT_IMPORT RadarMosaicDataProvider :public DataProvider
{
public:
	
	RadarMosaicDataProvider(void);
	~RadarMosaicDataProvider(void);
	/** @brief �õ�ʵ�� */ 
	static RadarMosaicDataProvider *Instance();
	/** @brief �����ļ�*/ 
	bool LoadFile(const std::string& filename);
	/** @brief �õ�ɫ�� */ 
	inline  P_COLORTABLE_VECTOR GetColorTable(){return m_ColorTable;}
	/** @brief ������ͼ�����ļ�·��������ͬ·��������.png�ļ� */ 
	QString  DrawThumbs(QString& filepath);
	/** @brief ���ͼ���Ľ����� */ 
	QRectF *GetRect();
	/** @brief ����ڵ�Ԥ����Χ */ 
	QRectF *GetRect(float lat,float lon);
	/** @brief ����BREF�ļ� */ 
	bool LoadRefFile(const std::string& filename);

	inline cDrawMultiProduct* GetDrawProduct() {return pDrawMulProduct;}
	inline void SetRequsetLayer(int iLayerNO) {m_iLayerNO = iLayerNO;}
	inline int GetRequsetLayer() {return m_iLayerNO;}
	virtual QString GetDataUnit();//������ݼ�����λ
	virtual float GetPositionValue(double lon,double lat,double height);//��ȡ���λ�õ�������ֵ
    void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p);
private: 
	/** @brief /�ļ�������������վ�ţ����ͣ�ʱ�䷵��һlist */ 
	 QStringList  FileNameAnalyes(QString fName);
	 /** @brief ��ȡ���� */ 
	 void  ProductDataProvider(Layer *pLayer,GenerationData* gData,int requestLayer = -1);
	  /** @brief ��ʼ����ɫ�б� */ 
	 void  InitColorTable();
	  /** @brief ��ʼ���ļ����� */ 
	 void  InitProductNameTable();
	  /** @brief ��ȡ���� */ 
	 void  DumpMosaicAttr(Layer *layer);
	 /** @brief ��ò�Ʒ��Ӣ������ */
	 QString  GetProductName(int ncode);
	 /** @brief ��ò�Ʒ��Ӣ������ */
	 QString  GetProductNameByShortName(int productcode);
	 /** @brief ��ò�Ʒ���� */
	 int  GetProductCode();
	 /** @brief ���Ʋ�Ʒ����ͼ */
	 bool DrawPic(QString& filePath,GenerationData* gData,int requestLayer = -1);
	 /** @brief ��ò�Ʒ�ı����� */
	 QList<STATION_INFO>* GetStationInfoByRect(QRectF *rect,int id);
	 /** @brief ��ʼ����ƷͶӰ */
	 bool setAngle(float lat,float lon ,float s_hight,QList<STATION_INFO>& station_info);
	 /** @brief ��÷�λ�� */
	double   Get_Angle_Of_Direction(double dx, double dy);
	/** @brief ����� */
	double	     Get_Length(double dx, double dy);
	
private:
	GenerationData*        gData;//��������
	//cDrawProduct*          pDrawProduct;//�����㷨����
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
	int m_iLayerNO; //���ͳ��ͼ�� number
private:
	static RadarMosaicDataProvider * m_pRadarProvider;
};


