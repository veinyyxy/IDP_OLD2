#pragma once
#include "idp_control_global.h"
#include "idpdrawgeometrydata.h"
#include <QtGui/QPixmap>
class IDP_CONTROL_EXPORT DrawSymbol :
	public IDPDrawGeometryData
{
public:
	DrawSymbol(void);
	virtual ~DrawSymbol(void);

	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	void SetPixmap(const QImage& pixmap);

protected:
	virtual void CreateSymbol(const osg::Vec3& pos);

	QImage m_DrawSymbolImage;
	osg::ref_ptr<osg::Image> m_OsgImage;
	osg::ref_ptr<osg::Texture2D> m_Textrue2D;

	int m_uWidth;
	int m_uHeight;
};

