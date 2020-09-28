/** @file 
* @brief     ������Ʒ���ݽ�����   MosaicDataProvider.h MosaicDataProvider.cpp
* @author    ��־ѧ
* @date      2012��6��23��
* @version   version
  @note      2013��6��
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
	float Lon;                 //���� ���ȣ�
	float Lat;                 //γ�� ���ȣ�
	float Hgt;                 //�����߶� km 

}SAVEELE_INFO;


typedef struct _staionInfo
{
	int id;                    //��ҵ��id
	int code;                  //�ڵ���
	float Lon;                 //���� ���ȣ�
	float Lat;                 //γ�� ���ȣ�
	float elevation;           //����
	float azimuthal;           //��λ��
	int count;                 //�ڵ�����
	QDateTime start;           //��ʼʱ��
	QDateTime end ;            //����ʱ��

}STATION_INFO;

typedef QVector<STATION_INFO>Station_Vec;
/** @class   MosaicDataProvider
    @brief   ������Ʒ���ݽ�����
    @author  ��־ѧ
    @note    ����MultiProductBase �������ٴ�������ӳ�Ա����
*/ 

using namespace goto_gis;
class  MosaicDataProvider :/* public RadarProductDataProvider,*/public MultiProductBase
{
	//Q_OBJECT

public:
	MosaicDataProvider();
	~MosaicDataProvider();
	/** @brief ���ݵ����ӿں��� */ 
	void ProdDataProvider(Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash);
	/** @brief �жϲ�Ʒ��ͼ���� */ 
	CProductDrawType GetTypeOfProduct();
	/** @brief ��γ�������ݽ��� */ 
    void MosGetGridData(Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash ); //��γ�����Ʒ���ݽ���
	/** @brief ��������ͼ */
	void DrawPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor);
	/** @brief ���ƾ�γ������ͼƬ */
	void DrawGridPic(QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable); 
	/** @brief ������ݴ��� */
	int ProductCode();
	/** @brief ������ݷ�Χ */
	QStringList GetRange();
	/** @brief ������ݷ�Χ */
	QRectF *GetRect();
	/** @brief ��õ�ǰ���ݻز�ֵ */
	double GetValue(double lon,double lat,double height,CoordinateTransform *proj);//������λ�õĻز�ֵ
	void LoadWeaEleAlgorithm(std::vector<SAVEELE_INFO> & savele_info);
	void LoadSaveEleInfo(gInt32 lon, gInt32 lat,gInt16 * m_pCap,gInt32 *m_pCapHgt,std::vector<SAVEELE_INFO> & savele_info);
};


