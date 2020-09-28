#include <osg/AutoTransform>
#include <QtOpenGL/QGLWidget>
#include "DrawSymbol.h"
#include "goSymbolContainer.h"


DrawSymbol::DrawSymbol(void)
{
}

DrawSymbol::~DrawSymbol( void )
{

}

bool DrawSymbol::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:

		break;
	case osgGA::GUIEventAdapter::PUSH:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{

		}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			CreateSymbol(pos);
		}
		break;
	default:
		return false;
	}
	return true;
}

void DrawSymbol::SetPixmap( const QImage& pixmap )
{
	if(32 != pixmap.depth()) return;
	m_DrawSymbolImage = QGLWidget::convertToGLFormat(pixmap);
	unsigned char* data = new unsigned char[m_DrawSymbolImage.byteCount()];
	for (int i = 0; i< m_DrawSymbolImage.byteCount();i++)
	{
		data[i] = m_DrawSymbolImage.bits()[i];
	}
	m_uWidth = m_DrawSymbolImage.width();
	m_uHeight = m_DrawSymbolImage.height();
	m_OsgImage = new osg::Image;
	m_OsgImage->setImage(m_uWidth, m_uHeight, 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);

	m_Textrue2D = new osg::Texture2D;

	m_Textrue2D->setImage(m_OsgImage);
	m_Textrue2D->setResizeNonPowerOfTwoHint(false);
	m_Textrue2D->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
	m_Textrue2D->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	m_Textrue2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
}

void DrawSymbol::CreateSymbol(const osg::Vec3& pos)
{
	osg::Geometry* pGeom = osg::createTexturedQuadGeometry(
		osg::Vec3(-m_uWidth/2, -m_uHeight/2, 0), osg::Vec3(m_uWidth, 0, 0), osg::Vec3(0, m_uHeight, 0));

	pGeom->setName("Symbol_Image");
	pGeom->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Textrue2D, osg::StateAttribute::ON);
	osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
	if(pGroup)
	{
		osg::AutoTransform* pAutoTransform = new osg::AutoTransform;
		
		osg::Geode* pGeode = new osg::Geode;
		pGeode->addDrawable(pGeom);

		pAutoTransform->addChild(pGeode);
		pAutoTransform->setAutoScaleToScreen(true);
		pAutoTransform->setPosition(pos);
		
		pGroup->addChild(pAutoTransform);
	}
}
