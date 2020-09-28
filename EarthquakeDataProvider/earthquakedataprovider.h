/** @file    earthquakedataprovider.h   earthquakedataprovider.cpp��
* @brief     �״������ṩ������ӿ����ļ�
* @author    ��־ѧ
* @date      2013��8��4
* @version   1.0
  @note      2013��8��14 ������
*/ 
#ifndef EARTHQUAKEDATAPROVIDER_H
#define EARTHQUAKEDATAPROVIDER_H

#include "earthquakedataprovider_global.h"
#include "DataProvider.h"
#include <QStringList>
#include <QtCore/QFile>
#include "Layer.h"
#include "EarthquakeProductionDraw.h"
using namespace goto_gis ;

/** @class  EarthquakeDataProvider
    @brief  ���������ṩ��
    @author  ��־ѧ
	@note    ����DataProvider�����������ṩ�����ⲿ���ýӿ�
	*/ 
class EARTHQUAKEDATAPROVIDER_EXPORT EarthquakeDataProvider :public DataProvider
{

public:
	EarthquakeDataProvider();
	~EarthquakeDataProvider();
	/** @brief �������� */ 
	virtual bool LoadFile(const std::string& filename);
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p) {}
	virtual QString GetDataUnit(){return "";};//������ݼ�����λ
	virtual float GetPositionValue(double lon,double lat,double height){return 0.0;};//��ȡ���λ�õ�������ֵ
	//virtual bool LoadFile(const std::list<std::string>&filenameList);
private: 
	/** @brief �������� */
	void DumpProduct(Layer *pLayer);
	/** @brief ��ȡ�ض��Ĳ�Ʒ��ͼ��ָ�룬����ָ�� */
	EQProductionDraw* GetProductDrawInstance(EARTHQUAKE_DATA_TYPE nProductCode);
	/** @brief �ͷŲ�Ʒ��ͼ��ָ�룬����ָ�� */
	void ReleaseProductDrawInstance(EQProductionDraw* lpInstance);
	/** @brief ����ļ����� */
	EARTHQUAKE_DATA_TYPE GetFileType(QFile &f);
private:
	QStringList    m_fileNameList;//����ļ�
	QString m_fileName;//�����ļ�
	EQProductionDraw * pProductDraw;

};

#endif // EARTHQUAKEDATAPROVIDER_H
