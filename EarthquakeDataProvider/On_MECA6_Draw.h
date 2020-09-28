#pragma once

#include "meca.h"
#include "earthquakeproductiondraw.h"

class On_MECA6_Draw :
	public EQProductionDraw
{
public:
	On_MECA6_Draw(void);
	virtual ~On_MECA6_Draw(void);

	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();

	virtual void LoadFile( const QString& filename );

	virtual bool DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0 );
	void LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out );
private:
	mech_vec pointArray;
	QString m_fileName;
	QVector<MECHANISM> mes;
	osg::Node* m_pArrowNode1;
	osg::Node* m_pArrowNode2;

};

