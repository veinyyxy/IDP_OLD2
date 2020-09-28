#pragma once
#include "earthquakeproductiondraw.h"
#include <QStringList>
#include <QVector>
#include <QtGui/QImage>
#include "helper.h"
#include "EarthquakeCubeData.h"

using namespace goto_gis ;

class OpenCubeData :public EQProductionDraw
{
public:
	OpenCubeData(void);
	~OpenCubeData(void);
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	virtual void LoadFile(const QString& filename);
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	virtual void LonLatToGL(Layer* layer, osg::Vec3& in, osg::Vec3& out);
	osg::Geometry* GetIsosurface(float fV);
private:
	QVector<float> tempData;
	QVector<float>* VecData;
	QVector<v3Point>* VectTemp;
	int iX, iY, iZ;
	float fJingDu, fWeiDu, fStepJingDu, fStepWeiDu;
	CUBE_DATA_VECTOR CubeCoreData;
};

