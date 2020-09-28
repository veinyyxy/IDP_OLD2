#pragma once

#include "meca.h"
#include "EarthquakeProductionDraw.h"

class On_MECA3_Draw :
	public EQProductionDraw
{
public:
	On_MECA3_Draw(void);
	virtual ~On_MECA3_Draw(void);
	virtual void LoadFile(const QString& filename);
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0);//画产品图片，指定某张图片
	virtual void LonLatToGL(Layer* layer, osg::Vec3& in, osg::Vec3& out);
	virtual void GetColorFromTable(double iValue, osg::Vec4& color);
	EARTHQUAKE_DATA_TYPE GetTypeOfProduct();
private:
	mech_vec pointArray;
	QString m_fileName;
	QVector<MECHANISM> mes;
};

