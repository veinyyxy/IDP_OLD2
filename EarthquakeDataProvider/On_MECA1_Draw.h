#pragma once
#include "earthquakeproductiondraw.h"
#include <QStringList>
#include <QVector>
#include <QtGui/QImage>
#include "helper.h"
using namespace goto_gis ;

class On_MECA1_Draw :public EQProductionDraw
{
public:
	On_MECA1_Draw(void);
	~On_MECA1_Draw(void);
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual void LoadFile(const QString& filename);
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0);//画产品图片，指定某张图片
	virtual void LonLatToGL(Layer* layer, osg::Vec3& in, osg::Vec3& out);
private:
	mech_vec pointArray;
	Helper imgHelp;
	QString m_fileName;
	QVector<MECHANISM> mes;
};

