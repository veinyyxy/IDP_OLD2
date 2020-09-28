/** @file   EarthquakeProductionDraw.h
* @brief     �������ݻ��Ʒ����ӿ��ļ�
* @author    ��־ѧ
* @date      2013��8��10
* @version   v1.0
  @note      detailed description �޸���  �޸�ʱ�� �޸���ʷ
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

typedef QVector<P_E_COLORNODE_PRODUCT>  E_COLORTABLE_VECTOR, *P_E_COLORTABLE_VECTOR;//��ɫ����

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
    @brief   �������ݻ��ƽӿ��ࣨ����ࣩ
    @author  ��־ѧ
    @note    ���ݷ����ͻ��ƽӿڵ�����࣬���Ʒ�������������ʵ��
*/ 
class EQProductionDraw :public QObject
{
public:
	/** @brief �жϲ�Ʒ��ͼ���� */ 
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct()=0;
	/** @brief �����ļ� */ 
	virtual void LoadFile(const QString& filename) = 0;
	/** @brief ����ƷͼƬ��ָ��ĳ��ͼƬ */ 
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0)=0;
};
