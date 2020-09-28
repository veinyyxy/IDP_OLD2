#include "DrawPolygonalLine.h"
#include "CardinalSpline.h"
#include "ColdFront.h"
#include "WarmFront.h"

class DebugDraw : public osg::Drawable::DrawCallback
{
public:
	virtual void drawImplementation( osg::RenderInfo& renderInfo, const osg::Drawable* drawable) const
	{
		unsigned id = renderInfo.getContextID();
		int i = 0;
		i++;
	}

	virtual osg::Object* clone( const osg::CopyOp& ) const
	{
		return 0;
	}

};

DrawPolygonalLine::DrawPolygonalLine(void) : m_bStart(false), m_pAlgorithmInterface(0)
{
	m_pLayer = 0;
	m_DrawNode = new osg::Geode;
	m_pVertexs = new osg::Vec3Array;
	m_pTempVertexs = new osg::Vec3Array;
	m_Colors = new osg::Vec4Array;
	m_pLine = new osg::Geometry;
	m_DrawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES,0,0);
	osg::Geode* pGeode = m_DrawNode->asGeode();
	pGeode->addDrawable(m_pLine);

	m_Colors->push_back(osg::Vec4(1, 1, 1, 1));
	
	m_pLine->setVertexArray(m_pTempVertexs);
	m_pLine->setColorArray(m_Colors);

	m_pLine->addPrimitiveSet(m_DrawArrays);
	m_pLine->setColorBinding(osg::Geometry::BIND_OVERALL);
}


DrawPolygonalLine::~DrawPolygonalLine(void)
{
}

bool DrawPolygonalLine::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	float fZ = 0;

	if(m_pLayer)
		fZ = m_pLayer->LayerZ();

	osg::Vec3 locPos;
	locPos.set(pos.x(), pos.y(), fZ);
	//locPos.set(ea.getX(), ea.getY(), 0);

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		if(m_bStart)
		{
			if(m_pVertexs->size()>1)
				m_pVertexs->pop_back();
			m_pVertexs->push_back(locPos);
		}
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if(m_bStart)
				{
					m_pVertexs->push_back(locPos);
					//m_pVertexs->push_back(locPos);
				}
				else
				{
					m_pVertexs->clear();
					m_pVertexs->push_back(locPos);
					//m_pVertexs->push_back(locPos);
					m_bStart = true;
				}

			}
		}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			m_bStart = false;
			CreateLineGeometry();
		}
		break;
	default:
		return false;
	}
	if(m_bStart)
	{
		(*m_Colors)[0] = m_color;
		*m_pTempVertexs = *m_pVertexs;
		std::vector<osg::ref_ptr<osg::Vec3Array>> outputLine;
		if(m_pTempVertexs->size() > 2)
		{
			m_CardinalSpline.GetSmoothPoints(*m_pVertexs, *m_pTempVertexs);
			if(m_pAlgorithmInterface)
			{
				m_pAlgorithmInterface->SymbolLineAlgorithm(*m_pTempVertexs, outputLine);
				m_pLine->removePrimitiveSet(0, m_pLine->getNumPrimitiveSets());
				m_pTempVertexs->clear();
				for(int i = 0; i < outputLine.size(); i++)
				{

					unsigned iStartC = 0;
					osg::Vec3Array* pVec3Array = outputLine[i];
					unsigned iS = pVec3Array->size();
					iStartC = m_pTempVertexs->size();
					for(unsigned iC = 0; iC < iS; iC++)
					{
						m_pTempVertexs->push_back(pVec3Array->at(iC));
					}

					if(i == 0)
						m_pLine->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, iStartC, iS));
					else
					{
						m_pLine->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLE_FAN, iStartC, iS));
					}
				}
			}
			else
				m_DrawArrays->set(osg::PrimitiveSet::LINE_STRIP, 0, m_pTempVertexs->size());
		}
		else
			m_DrawArrays->set(osg::PrimitiveSet::LINE_STRIP, 0, m_pTempVertexs->size());
		

#if 0
		for(int i = 0; i < m_pTempVertexs->size(); i++)
		{
			osg::Vec3 vec3P = (*m_pTempVertexs)[i];
			osg::Matrix VPW = m_Camera->getViewMatrix() *
				m_Camera->getProjectionMatrix() *
				m_Camera->getViewport()->computeWindowMatrix();
			osg::Matrix inverseVPW;
			inverseVPW.invert(VPW);
			vec3P = vec3P  * inverseVPW;
			vec3P.set(vec3P.x(), vec3P.z(), fZ);
			(*m_pTempVertexs)[i] = vec3P;
		}
#endif
		m_pLine->dirtyDisplayList();
		m_pLine->dirtyBound();
		return true;
	}
	return true;
}

void DrawPolygonalLine::CreateLineGeometry()
{
	GLsizei si = m_DrawArrays->getCount();
	if(0 == si)
		return;

	osg::ref_ptr<osg::Geometry> lpLine  = (osg::Geometry*)(m_pLine->clone(osg::CopyOp::DEEP_COPY_ALL));
	//osg::ref_ptr<osg::Geometry> lpBoundBox = new osg::Geometry;

	//osg::BoundingBox bx = lpLine->computeBound();
	//CreateBoundBox(bx, lpBoundBox);

	//lpBoundBox->setUserData(lpLine);
	//lpBoundBox->setName("Symbol_Image");

	if(m_pLayer)
	{
		osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
		if(pGroup)
		{
			osg::ref_ptr<osg::Geode> pGeode = new osg::Geode();
			pGeode->setName("Line_Line");
			if(pGeode)
			{
				pGeode->addDrawable(lpLine);
				//pGeode->addDrawable(lpBoundBox);
				pGroup->addChild(pGeode);
			}
		}
	}

	m_pVertexs->clear();
	m_DrawArrays->setCount(0);
	m_DrawArrays->dirty();

	m_pLine->removePrimitiveSet(0, m_pLine->getNumPrimitiveSets());
	m_pLine->addPrimitiveSet(m_DrawArrays);
}

void DrawPolygonalLine::SetColor( osg::Vec4& v4c )
{
	m_color = v4c;
	(*m_Colors)[0] = m_color;
}

void DrawPolygonalLine::SetDrawLayer( goto_gis::Layer* pLayer )
{
	if(!pLayer) return;

	osg::Group* pGroup = dynamic_cast<osg::Group*>(pLayer->GetLayerNode());
	if(!pGroup) return;

	pGroup->addChild(m_DrawNode);
	m_pLayer = pLayer;
}

void DrawPolygonalLine::CreateBoundBox(osg::BoundingBox& bb, osg::Geometry* pGeom)
{
	osg::ref_ptr<osg::Vec3Array> pVertexs = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;

	pColors->push_back(osg::Vec4(1, 1, 1, 0));


	pVertexs->push_back(bb.corner(0));
	pVertexs->push_back(bb.corner(2));
	pVertexs->push_back(bb.corner(3));
	pVertexs->push_back(bb.corner(1));

	/*pVertexs->push_back(bb.corner(4));
	pVertexs->push_back(bb.corner(5));
	pVertexs->push_back(bb.corner(6));
	pVertexs->push_back(bb.corner(7));*/

	pGeom->setVertexArray(pVertexs);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS, 0, pVertexs->size()));

	pGeom->setColorArray(pColors);
	pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
}