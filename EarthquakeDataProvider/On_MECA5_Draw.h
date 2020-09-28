#pragma once
#include "meca.h"
#include "EarthquakeProductionDraw.h"

class On_MECA5_Draw :
	public EQProductionDraw
{
public:
	On_MECA5_Draw(void);
	virtual ~On_MECA5_Draw(void);
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();
	virtual void LoadFile( const QString& filename );
	virtual bool DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0 );
	virtual void LonLatToGL(Layer* layer, osg::Vec3& in, osg::Vec3& out);
	virtual void GetColorFromTable(double iValue, osg::Vec4& color);

private:
	mech_vec pointArray;
	QString m_fileName;
	QVector<MECHANISM> mes;
};

