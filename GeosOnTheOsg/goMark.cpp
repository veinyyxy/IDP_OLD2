#include <osg/MatrixTransform>
#include <QDebug>
#include "Layer.h"
#include "Map.h"
#include "goMark.h"

GOTO_NAMESPACE_START

goMark::goMark(void) : m_bUsedSet(true)
{
	m_markText = new osgText::Text();
	m_alignment = osgText::Text::CENTER_TOP;
	m_strFontPath ="";
}

goMark::goMark( std::string markLabel,osg::Vec3 markPostion , Layer* pL): m_bUsedSet(true)
{
	m_markText = new osgText::Text();
	m_markLabel = markLabel;
	m_markPostion = markPostion;
	m_pParentLayer = pL;
	m_alignment = osgText::Text::CENTER_TOP;
	m_strFontPath ="";
}


goMark::~goMark(void)
{
}

void goMark::CreateOsgFromGeos( osg::Node* pNode )
{
	unsigned int  colour = 0xFFFFFFFF;
	if(m_bUsedSet)
	{
		m_markSize = m_pParentLayer->FontSize();
		colour = m_pParentLayer->TextColor();

		unsigned char ia = colour >> 24 & 0xFF;
		unsigned char ir = colour >> 16 & 0xFF;
		unsigned char ig = colour >> 8 & 0xFF;
		unsigned char ib = colour & 0xFF;
		m_markColor.set(ir,ig,ib,ia);
		m_alignment = osgText::Text::CENTER_TOP;
	}
	
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, m_markLabel.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, m_markLabel.c_str(), requiredSize+1); 
	m_markText->setFont("Fonts/simsun.ttc");
	m_markText->setText(wtext); 
	
	delete [] wtext; 
	wtext = NULL; 

	osg::Vec3 destVec3;
	if (m_pCorrdinateTransform == 0)
	{
		m_pCorrdinateTransform =m_pParentLayer->CoordTrans();//GetParentMap()->GetCoordinateTransform();
	}

	if(!m_pCorrdinateTransform)
	{
		std::exception* pE = new std::exception("投影变换未设置");

		throw pE;

		return;
	}

	m_pCorrdinateTransform->Transform(m_markPostion, destVec3);

	double x = destVec3.x();
	//double y = destVec3.y();
	//double z = m_pParentLayer->LayerZ();
	double z = destVec3.z();
	double y = m_pParentLayer->LayerZ();
	destVec3.set(x,y,z);

	m_markText->setPosition( destVec3 );
	m_markText->setAutoRotateToScreen( true );
	m_markText->setCharacterSize(m_markSize);
	m_markText->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	m_markText->setAxisAlignment(osgText::Text::SCREEN);
	m_markText->setDrawMode(osgText::Text::TEXT);
	m_markText->setColor(m_markColor);
	if(m_strFontPath.length() != 0)
	{
		m_markText->setFont(m_strFontPath);
	}
	m_markText->setAlignment(m_alignment);
	osg::StateSet* stateset = m_markText->getOrCreateStateSet();
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	//stateset->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF  );


	osg::Geode* geode = pNode->asGeode();
	if(geode)
		geode->addDrawable(m_markText);
	else
	{
		osg::Group* pGroup = pNode->asGroup();

		if(pGroup)
		{
#if 0
			osg::Vec3 zero0(0, 0, 0);
			osg::MatrixTransform* pMt = new osg::MatrixTransform;
			osg::Geode* pGeode = new osg::Geode;
			osg::Matrix moveo, rot, r, moveb;

			rot.makeRotate(m_fTextDirection, osg::Vec3(0, 0, 1));
			moveb.makeTranslate(destVec3);
			moveo.makeTranslate(zero0 - destVec3);

			r = moveo * rot * moveb;
			pGeode->addDrawable(m_markText);
			//pMt->setMatrix(r);
			pMt->addChild(pGeode);
			pGroup->addChild(pMt);
#else
			osg::Geode* pGeode = new osg::Geode;
			pGeode->addDrawable(m_markText);
			pGroup->addChild(pGeode);

#endif
		}
	}
}

void goMark::SetCoordinateTransform( CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}

geom::GeometryTypeId goMark::GetGeometryType()
{
		return (geom::GeometryTypeId)100;
}

void goMark::TransformGeometry()
{
	osg::Vec3 destVec3;
	if (m_pCorrdinateTransform == 0)
	{
		m_pCorrdinateTransform = m_pParentLayer->CoordTrans();//->GetCoordinateTransform();
	}
	TransormPoint(m_markPostion, destVec3);
	m_markText->setCharacterSize(m_markSize);  
	m_markText->setPosition(destVec3);
}

void goMark::operator=( const goMark& other )
{
	this->m_markLabel = other.m_markLabel;
	this->m_markPostion = other.m_markPostion;//图形颜色
	this->m_markSize = other.m_markSize;
	this->m_markColor= other.m_markColor;
	this->m_alignment = other.m_alignment;
	this->m_bUsedSet = other.m_bUsedSet;
	this->m_fTextDirection = other.m_fTextDirection;
	this->m_strFontPath = other.m_strFontPath;
}

GOTO_NAMESPACE_END