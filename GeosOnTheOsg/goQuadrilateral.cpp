#include "goQuadrilateral.h"
#include "CoordinateTransform.h"
#include <osgUtil/Tessellator>
#include <osgUtil/SmoothingVisitor>
#include <osg/Geometry>
#include "Proj4Transform.h"

#include "Layer.h"

GOTO_NAMESPACE_START
goQuadrilateral::goQuadrilateral(void)
{
	m_coodArray = new osg::Vec3Array;
	m_colorArray = new osg::Vec4Array;
	m_mode = osg::PrimitiveSet::QUADS;
	b_tess = false;
}


goQuadrilateral::~goQuadrilateral(void)
{
}

void goQuadrilateral::SetOsgArray( osg::Vec3Array* coodinateArray )
{
	m_coodArray = coodinateArray;
}

void goQuadrilateral::SetOsgColorArray( osg::Vec4Array &colorArray )
{
	*m_colorArray = colorArray;
}

void goQuadrilateral::TransformGeometry()
{
	osg::Vec3 destVec3, normalVec3, zeroVec3(0, 0, 0);
	osg::ref_ptr<osg::Vec3Array> pyramidVertices  = dynamic_cast<osg::Vec3Array*>(m_pOsgGeometry->getVertexArray());
	osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array;
	if (pyramidVertices && !m_coodArray->empty())
	{
		pyramidVertices->clear();
		for (size_t i = 0; i< m_coodArray->size() ;i++)
		{
			if (GetBTransform())
			{
				m_pCorrdinateTransform->Transform(m_coodArray->at(i),destVec3);
			}
			else
			{
				destVec3 = m_coodArray->at(i);
			}

			if (m_pParentLayer->UseLayerZ())
			{
				double x = destVec3.x();
				double y = m_pParentLayer->LayerZ();
				double z = destVec3.z();
				destVec3.set(x,y,z);
			}
			pyramidVertices->push_back(destVec3);

			normalVec3 = /*zeroVec3 - */destVec3;
			normalVec3.normalize();
			normalArray->push_back(normalVec3);
		}
#if 0
		goto_gis::Proj4Transform* pProj4 = dynamic_cast<goto_gis::Proj4Transform*>(m_pCorrdinateTransform);
		if(pProj4)
			osgUtil::SmoothingVisitor::smooth(*m_pOsgGeometry, osg::PI);
		else
		{
			m_pOsgGeometry->setNormalArray(normalArray);
			m_pOsgGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		}
#endif//去掉平滑，法线计算，当前三维不加载天空，没有光照

		m_pOsgGeometry->dirtyBound();
		m_pOsgGeometry->dirtyDisplayList();
		
	}
	
}

void goQuadrilateral::operator=( const goQuadrilateral& other )
{
	this->m_coodArray = other.m_coodArray;
	this->m_colorArray = other.m_colorArray;
	this->m_mode = other.m_mode;
}

void goto_gis::goQuadrilateral::CreateOsgFromGeos(osg::Node* pNode)
{
	osg::Vec3 srcVec3, destVec3;
	if(m_coodArray->empty()) return;
	osg::Geode* geode = pNode->asGeode();
	if(geode == 0) return;

	std::size_t csSize = m_coodArray->size();
	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	osg::Vec3Array* pNormalArray = new osg::Vec3Array;

	for(std::size_t scs = 0; scs < csSize; scs++)
	{
		srcVec3.set(m_coodArray->at(scs).x(),m_coodArray->at(scs).y(),m_coodArray->at(scs).z());

		if (GetBTransform())
		{
			m_pCorrdinateTransform->Transform(srcVec3, destVec3);
		}
		else
		{
			destVec3 = srcVec3;
		}
		if (m_pParentLayer->UseLayerZ())
		{
			double x = destVec3.x();
			double y = m_pParentLayer->LayerZ();
			double z = destVec3.z();
			destVec3.set(x,y,z);
		}
		
		pNormalArray->push_back(osg::Vec3(0, 1, 0));

		pyramidVertices->push_back(destVec3);
	}
	m_pOsgGeometry->setVertexArray(pyramidVertices);
	m_pOsgGeometry->setColorArray(m_colorArray);

	m_pOsgGeometry->setNormalArray(pNormalArray);
	m_pOsgGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	if (b_tess)
	{
		m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,14));
		//m_pOsgGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
		m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,14,22));
		m_pOsgGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
		osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;  
		m_pOsgGeometry->setNormalArray(normals.get());  
		m_pOsgGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);  
		osg::Vec3 normal = osg::Z_AXIS;  
		normals->push_back(normal);

		osg::ref_ptr<osgUtil::Tessellator> tes = new osgUtil::Tessellator();  
		tes->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY); 
		tes->setBoundaryOnly(false);  	
		tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_POSITIVE);
		tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD); 
		//tes->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);  
		tes->retessellatePolygons(*m_pOsgGeometry);
	
	}
	else
	{
		m_pOsgGeometry->addPrimitiveSet(new osg::DrawArrays(m_mode/*osg::PrimitiveSet::QUADS*/,0,csSize)); 
		m_pOsgGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	}

	//osgUtil::SmoothingVisitor::smooth(*m_pOsgGeometry, osg::PI);//不计算法线，但是不能加光照

	geode->addDrawable(m_pOsgGeometry);

}

void goto_gis::goQuadrilateral::SetCoordinateTransform( CoordinateTransform* pCT )
{
	m_pCorrdinateTransform = pCT;
}
GOTO_NAMESPACE_END