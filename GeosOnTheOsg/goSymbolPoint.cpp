#include <osg/Billboard>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/BlendFunc>

#include "Layer.h"
#include "gogeometry.h"
#include "CoordinateTransform.h"
#include "goSymbolPoint.h"

GOTO_NAMESPACE_START

goSymbolPoint::goSymbolPoint(void) : m_v4Color(1.0f,1.0f,1.0f,1.0f), m_fAnagle(0)
{
	m_pPrimitiveSet = new osg::DrawArrays(osg::DrawArrays::QUADS, 0, 4);
	m_pPrimitiveSet2 = new osg::DrawArrays(osg::DrawArrays::POINTS, 0, 4);
	m_pStateSet = new osg::StateSet;
	m_pVertexes = new osg::Vec3Array;
	m_pColors = new osg::Vec4Array;
	m_pTexCoord = new osg::Vec2Array;

	m_pVertexes->push_back(osg::Vec3(0, 30, 0));
	m_pVertexes->push_back(osg::Vec3(0, 0, 0));
	m_pVertexes->push_back(osg::Vec3(30, 0, 0));
	m_pVertexes->push_back(osg::Vec3(30, 30, 0));

	m_pTexCoord->push_back(osg::Vec2(0, 0));
	m_pTexCoord->push_back(osg::Vec2(0, 1));
	m_pTexCoord->push_back(osg::Vec2(1, 1));
	m_pTexCoord->push_back(osg::Vec2(1, 0));

	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);

	osg::Geometry::setTexCoordArray(0, m_pTexCoord);
	osg::Geometry::setColorArray(m_pColors);
	osg::Geometry::setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Geometry::setUseDisplayList(false);
}

goSymbolPoint::goSymbolPoint( int iSize, const osg::Vec3& pos, osg::Image* pImage ) : m_v4Color(0.0f,0.0f,0.0f,1.0f), m_fAnagle(0)
{
	m_iSize = iSize;
	m_v3Position = pos;
	m_pStateSet = new osg::StateSet;
	
	m_pVertexes = new osg::Vec3Array;
	m_pColors = new osg::Vec4Array;
	m_pTexCoord = new osg::Vec2Array;

	m_pVertexes->push_back(osg::Vec3(0, 30, 0));
	m_pVertexes->push_back(osg::Vec3(0, 0, 0));
	m_pVertexes->push_back(osg::Vec3(30, 0, 0));
	m_pVertexes->push_back(osg::Vec3(30, 30, 0));

	m_pTexCoord->push_back(osg::Vec2(0, 0));
	m_pTexCoord->push_back(osg::Vec2(0, 1));
	m_pTexCoord->push_back(osg::Vec2(1, 1));
	m_pTexCoord->push_back(osg::Vec2(1, 0));

	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);
	m_pColors->push_back(m_v4Color);

	osg::Geometry::setTexCoordArray(0, m_pTexCoord);
	osg::Geometry::setColorArray(m_pColors);
	osg::Geometry::setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::Texture2D* pTex2D = new osg::Texture2D(pImage);
	m_pStateSet->setTextureAttributeAndModes(0, pTex2D, osg::StateAttribute::ON);
	m_pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	osg::Geometry::setStateSet(m_pStateSet);

	osg::Geometry::setUseDisplayList(false);
}


goSymbolPoint::~goSymbolPoint(void)
{
}

void goSymbolPoint::CreateOsgFromGeos( osg::Node* pNode )
{
	m_pCorrdinateTransform->Transform(m_v3Position, m_v3WorldPosition);

	SetAnglePrivate(m_fAnagle);
	if(pNode->asGeode())
	{
		pNode->asGeode()->addDrawable(this);
	}
	else
	{
		m_pParentLayer->SetLayerNode(new osg::Geode);
		osg::Group* pGroup = dynamic_cast<osg::Group*>(pNode);
		if(!pGroup) return;
		osg::Geode* pGeode = new osg::Geode;
		pGeode->addDrawable(this);
		pGroup->addChild(pGeode);
	}

	//((osg::Geometry*)this)->setUseDisplayList(false);
}

void goSymbolPoint::SetCoordinateTransform( goto_gis::CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}

geom::GeometryTypeId goSymbolPoint::GetGeometryType()
{
	return (geom::GeometryTypeId)13;
}

void goSymbolPoint::TransformGeometry()
{
	m_pCorrdinateTransform->Transform(m_v3Position, m_v3WorldPosition);
	if(m_pParentLayer)
	{
		if(m_pParentLayer->UseLayerZ())
			m_v3WorldPosition.set(m_v3WorldPosition.x(), m_v3WorldPosition.y(), m_pParentLayer->LayerZ());
	}
}

void goSymbolPoint::drawImplementation( osg::RenderInfo& renderInfo ) const
{
#if 0
	osg::Camera* pCamera = renderInfo.getCurrentCamera();
	if(!pCamera)
	{
		((osg::Geometry*)this)->dirtyDisplayList();
		((osg::Geometry*)this)->dirtyBound();
		return;
	}
	int halfSize = m_iSize/2;

	osg::Vec3 v3ScreenPos, p1, p2, p3, p4, pw1, pw2, pw3, pw4, pwi1, pwi2, pwi3, pwi4, v3ComputPos;
	ComputeWorld(pCamera, m_v3WorldPosition, v3ComputPos);
	WorldToScreen(pCamera, v3ComputPos, v3ScreenPos);
	p1.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() - halfSize, m_pParentLayer->LayerZ());
	p2.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() + halfSize, m_pParentLayer->LayerZ());
	p3.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() + halfSize, m_pParentLayer->LayerZ());
	p4.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() - halfSize, m_pParentLayer->LayerZ());
	ScreenToWorld(pCamera, p1, pw1);
	ScreenToWorld(pCamera, p2, pw2);
	ScreenToWorld(pCamera, p3, pw3);
	ScreenToWorld(pCamera, p4, pw4);
	ComputeNotVPWorld(pCamera, pw1, pwi1);
	ComputeNotVPWorld(pCamera, pw2, pwi2);
	ComputeNotVPWorld(pCamera, pw3, pwi3);
	ComputeNotVPWorld(pCamera, pw4, pwi4);

	pwi1.set(pwi1.x(), pwi1.y(), m_pParentLayer->LayerZ());
	pwi2.set(pwi2.x(), pwi2.y(), m_pParentLayer->LayerZ());
	pwi3.set(pwi3.x(), pwi3.y(), m_pParentLayer->LayerZ());
	pwi4.set(pwi4.x(), pwi4.y(), m_pParentLayer->LayerZ());

	m_pVertexes->clear();

	m_pVertexes->push_back(pwi1);
	m_pVertexes->push_back(pwi2);
	m_pVertexes->push_back(pwi3);
	m_pVertexes->push_back(pwi4);

	osg::Geometry::drawImplementation(renderInfo);

	((osg::Geometry*)this)->dirtyDisplayList();
	((osg::Geometry*)this)->dirtyBound();
#else
	((osg::Geometry*)this)->setUseDisplayList(false);
	osg::Camera* pCamera = renderInfo.getCurrentCamera();
	if(!pCamera)
	{
		((osg::Geometry*)this)->dirtyDisplayList();
		((osg::Geometry*)this)->dirtyBound();
		return;
	}
	int halfSize = m_iSize/2;

	osg::Vec3 v3ScreenPos, p1, p2, p3, p4, pw1, pw2, pw3, pw4, pwi1, pwi2, pwi3, pwi4, v3ComputPos;
	WorldToScreen(pCamera, m_v3WorldPosition, v3ScreenPos);
	
	/*p1.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() - halfSize, 0);
	p2.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() + halfSize, 0);
	p3.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() + halfSize, 0);
	p4.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() - halfSize, 0);*/

	p1.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() + halfSize, 0);
	p2.set(v3ScreenPos.x() - halfSize, v3ScreenPos.y() - halfSize, 0);
	p3.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() - halfSize, 0);
	p4.set(v3ScreenPos.x() + halfSize, v3ScreenPos.y() + halfSize, 0);

	ScreenToWorld(pCamera, p1, pw1, m_pParentLayer->LayerZ());
	ScreenToWorld(pCamera, p2, pw2, m_pParentLayer->LayerZ());
	ScreenToWorld(pCamera, p3, pw3, m_pParentLayer->LayerZ());
	ScreenToWorld(pCamera, p4, pw4, m_pParentLayer->LayerZ());

	m_pVertexes->at(0).set(pw1);
	m_pVertexes->at(1).set(pw2);
	m_pVertexes->at(2).set(pw3);
	m_pVertexes->at(3).set(pw4);

	((osg::Geometry*)this)->setVertexArray(m_pVertexes);
	((osg::Geometry*)this)->addPrimitiveSet(m_pPrimitiveSet);
	//((osg::Geometry*)this)->addPrimitiveSet(m_pPrimitiveSet2);
	osg::Geometry::drawImplementation(renderInfo);

	((osg::Geometry*)this)->setUseDisplayList(false);
#endif
}

void goSymbolPoint::SetTexture( osg::Image* pI )
{
	osg::Texture2D* pTex2D = new osg::Texture2D(pI);
	m_pStateSet->setTextureAttributeAndModes(0, pTex2D, osg::StateAttribute::ON);
	m_pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	osg::Geometry::setStateSet(m_pStateSet);
}

void goSymbolPoint::ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out, float fZ) const
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;
	v3Out.set(v3Out.x(), fZ, v3Out.z());

	v3Out = v3Out * m_m4Rotate;
}

void goSymbolPoint::WorldToScreen( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out ) const
{
	/*osg::Matrix viewMatrix;
	viewMatrix.makeRotate(M_PI/180*90, 1, 0, 0);*/
	osg::Vec3 v3localIn;
	v3localIn.set(v3In.x(), v3In.y(), v3In.z());
	osg::Matrix VPW = pCamera->getViewMatrix() * 
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();

	v3Out = v3localIn * VPW;
	//v3Out.set(v3Out.x(), v3Out.y(), 0);
}

void goSymbolPoint::ComputeWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out ) const
{
	osg::Matrix VP = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix();
	v3Out = v3In * VP;
}

void goSymbolPoint::ComputeNotVPWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out ) const
{
	osg::Matrix VP = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix();
	osg::Matrix InVP = osg::Matrix::inverse(VP);
	v3Out = v3In * VP;
}

void goSymbolPoint::SetAnglePrivate( float fA )
{
	osg::Vec3 v3Zero(0, 0, 0), v3MoveTo, v3MoveBack;
	osg::Matrix m4Rot, m4To, m4Back;

	v3MoveTo = v3Zero - m_v3WorldPosition;
	v3MoveBack = m_v3WorldPosition;

	m4To.makeTranslate(v3MoveTo);
	m4Back.makeTranslate(v3MoveBack);

	m4Rot.makeRotate(m_fAnagle, osg::Vec3(0, 0, 1));

	m_m4Rotate = m4To * m4Rot * m4Back;
}


GOTO_NAMESPACE_END