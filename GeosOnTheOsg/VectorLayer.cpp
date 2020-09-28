#include "Field.h"
#include "goGeometry.h"
#include "goPoint.h"
#include "goLinearRing.h"
#include "goLineString.h"
#include "goPolygon.h"
#include "goMultiPoint.h"
#include "goMark.h"
#include "goMultiLineString.h"
#include "goMultiPolygon.h"
#include "VectorLayer.h"
#include "DataProvider.h"

GOTO_NAMESPACE_START

VectorLayer::VectorLayer(DataProvider* data, CoordinateTransform* ct, int iNT) : Layer(data, ct, iNT)
{
	LayerType("VectorLayer");
	//m_coordTrans = new Proj4Transform;
}


VectorLayer::~VectorLayer(void)
{
}

int VectorLayer::CreateLayer( const std::string& filename )
{
	if(!m_dataProvider) 
	{
		return false;
	}
	m_dataProvider->LoadFile(filename);
	return true;
}

void VectorLayer::TransformLayer()
{
	std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = m_pgoGeometry->begin();
	while(goGeomIter != m_pgoGeometry->end())
	{
		goGeometry* goGeom = goGeomIter->second.get();
		if(goGeom)
		{
			goGeom->SetCoordinateTransform(m_coordTrans);
			goGeom->TransformGeometry();
		}
		goGeomIter++;
	}
}

void VectorLayer::CreateLayerData()
{
	std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = m_pgoGeometry->begin();
	while(goGeomIter != m_pgoGeometry->end())
	{
		goGeometry* goGeom = goGeomIter->second.get();
		if(goGeom)
		{
			goGeom->SetCoordinateTransform(m_coordTrans);
			goGeom->SetParentLayer(this);
			goGeom->CreateOsgFromGeos(m_pNode);
		}
		goGeomIter++;
	}
}

void VectorLayer::operator=( const VectorLayer& other )
{
	//this->m_pGeode = dynamic_cast<osg::Geode*>(other.m_pGeode->clone(osg::CopyOp::DEEP_COPY_NODES));
	//this->m_pParentMap = other.m_pParentMap;
	this->m_layerZ = other.m_layerZ;
	this->m_v4LayerColor = other.m_v4LayerColor;//Í¼ÐÎÑÕÉ«
	this->m_fontSize = other.m_fontSize;
	this->m_textColor = other.m_textColor;
	this->m_bVisible = other.m_bVisible;
	this->m_fSize = other.m_fSize;

	//this->m_pGeodeFont = dynamic_cast<osg::Geode*>(other.m_pGeodeFont->clone(osg::CopyOp::DEEP_COPY_OBJECTS));
	this->m_PendingFieldMap = other.m_PendingFieldMap;
	this->m_LayerAttributeDescripe = other.m_LayerAttributeDescripe;
	this->m_pLayerAttribteTable = other.m_pLayerAttribteTable;
	this->m_coordTrans = other.m_coordTrans;

	//this->m_pKSMapGeometry.reset(other.m_pKSMapGeometry.get());

	int count = 0;
	std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = other.m_pgoGeometry->begin();
	while(goGeomIter != other.m_pgoGeometry->end())
	{
		goGeometry* goGeom = goGeomIter->second.get();
		if (100  == goGeom->GetGeometryType())
		{
			goMark* ss = dynamic_cast<goMark*>(goGeom);
			goMark* thisGeometry = new goMark();
			thisGeometry->SetParentLayer(this);
			*thisGeometry = *dynamic_cast<goMark*>(goGeom);
			//thisGeometry->CreateOsgFromGeos(this->m_pNode);
			this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
		}
		else
		{
			std::string GeometryType = goGeom->GetGeomGeometry()->getGeometryType();
			if (GeometryType == "MultiLineString")
			{
				goMultiLineString* thisGeometry = new goMultiLineString(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goMultiLineString*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
			else if (GeometryType == "MultiPoint")
			{
				goMultiPoint* thisGeometry = new  goMultiPoint(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goMultiPoint*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
			else if (GeometryType == "MultiPolygon")
			{
				goMultiPolygon* thisGeometry = new goMultiPolygon(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goMultiPolygon*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
			else if (GeometryType == "Point")
			{
				goPoint* thisGeometry = new goPoint(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goPoint*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
			else if (GeometryType == "LineString")
			{
				goLineString* thisGeometry = new goLineString(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goLineString*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
			else if (GeometryType == "Polygon")
			{
				goPolygon* thisGeometry = new goPolygon(goGeom->GetGeomGeometry()->clone(), this->m_coordTrans, this);
				*thisGeometry = *dynamic_cast<goPolygon*>(goGeom);
				thisGeometry->SetParentLayer(this);
				//thisGeometry->CreateOsgFromGeos(this->m_pNode);
				this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
			}
		}
		count++;
		goGeomIter++;
	}
}
GOTO_NAMESPACE_END