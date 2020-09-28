#include "VectorLayer.h"
#include <osg/MatrixTransform>
#include "RadarVectorLayer.h"
#include "VectorLayer.h"
#include "GraticuleCoordinates.h"
#include "Map.h"
#include <windows.h>
GOTO_NAMESPACE_START
#define PI 3.1415927

Map::Map(osgViewer::View* pAttchViewer)
	: m_pView(pAttchViewer)
	, m_mapScale(1.0)
	, m_pDataProvider(0)
	, m_pCoordTrans(0)
	,m_basisLayerList()
{
	m_pRootNode = new osg::Group;
	//if (b90)
	//{
	//	osg::Matrix viewMatrix;
	//	osg::Matrix ScaleMatrix;
	//	osg::Matrix RotateMatrix;
	//	RotateMatrix.makeRotate(M_PI/180 * 15, 0, 1, 1);
	//	ScaleMatrix.makeScale(1/5000, 1/5000, 1);
	//	viewMatrix.makeRotate(PI/180*90, 1, 0, 0);
	//	m_pRootNode->setMatrix(viewMatrix/* * RotateMatrix * ScaleMatrix*/);
	//}
	m_pCurrentLayer = NULL;
	m_pSwitch = new osg::Switch();
	m_pRootNode->addChild(m_pSwitch);
	//地图开灯
#if 1
	osg::StateSet* pStateSet = m_pRootNode->getOrCreateStateSet();
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	/*osg::ref_ptr<osg::Material> mat = new osg::Material;
	mat->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setSpecular(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setShininess(osg::Material::FRONT,90);
	mat->setAmbient(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	mat->setEmission(osg::Material::FRONT,osg::Vec4(1.0,1.0,1.0,1.0));
	pStateSet->setAttribute(mat.get());*/
	
#endif
}


Map::~Map(void)
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		//std::string name = layerIter->first;
		Layer* layer = layerIter->second;
		delete layer;
		layer = NULL;
		layerIter++;
	}
	
	/*if (m_pCoordTrans)
	{
	delete m_pCoordTrans;
	m_pCoordTrans = NULL;
	}*/

}

Layer* Map::CreateVectorLayer( const std::string& name )
{
	if(m_pDataProvider == 0 || m_pCoordTrans == 0) return 0;
	Layer* layer = new VectorLayer(m_pDataProvider, m_pCoordTrans);
	layer->CreateLayer(name);
	return layer;
}

Layer* Map::GetLayer( const std::string& name )
{
	if(m_mapLayer.find(name) != m_mapLayer.end())
		return m_mapLayer.at(name);
	else
		return 0;
}

Layer* Map::GetLayer( int iID )
{
	size_t sSize = m_mapLayer.size();
	std::map<std::string, Layer*>::iterator ifinded = m_mapLayer.begin();
	for(size_t i = 0; i < sSize; i++, ifinded++)
	{
		if(i == iID)
		{
			return ifinded->second;
		}
	}
	return 0;
}

void Map::AddFishNet( float lonstart,float latstart,float lonend,float latend,float lonCellSize,float LatCellSize ,qreal qrz )
{
	Layer *newLayer = new QGraticuleCoordinates(0,m_pCoordTrans);
	dynamic_cast<QGraticuleCoordinates*>(newLayer)->SetGrid( lonstart, latstart, lonend, latend, lonCellSize, LatCellSize);
	newLayer->LayerZ(qrz);
	newLayer->UseLayerZ(false);
	newLayer->CreateLayer("FishNet");
	newLayer->SetParentMap(this);
	newLayer->CoordTrans(m_pCoordTrans);
	newLayer->CreateLayerData();

	osg::StateSet* pSateSet = newLayer->GetLayerNode()->getOrCreateStateSet();
	pSateSet->setRenderBinDetails(2, "FishNet");
	//pSateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
	//pSateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON);
	AddLayer("地理网格",newLayer);
}
int Map::AddLayer( const std::string& name, Layer* mapLayer )
{
	osg::Node* geode = mapLayer->GetLayerNode();
	bool b = m_pSwitch->addChild(geode, mapLayer->Visible());//m_pRootNode->addChild(geode);
	if (b)
	{
		//mapLayer->Hidden(true);
		mapLayer->SetLayerName(name);
		m_mapLayer.insert(MapLayerPair(name,mapLayer));
	}
	return true;
}

int Map::RemoveLayer( const std::string& name, bool bIsDel /*= true*/ )
{
	std::map<std::string, Layer*>::iterator iFinded = m_mapLayer.find(name);
	if(iFinded != m_mapLayer.end())
	{
		osg::Node* pNode = iFinded->second->GetLayerNode();
		bool b = m_pSwitch->removeChild(pNode);
		if (b)
		{
			m_mapLayer.erase(name);
		}
		return true;
	}
	else
		return false;
}

int Map::RemoveLayer( goto_gis::Layer* pLayer, bool bIsDel /*= true*/ )
{
	std::map<std::string, Layer*>::iterator be = m_mapLayer.begin();
	for(be; be != m_mapLayer.end(); be++)
	{
		ULONGLONG f, s;
		f = (ULONGLONG)(be->second);
		s = (ULONGLONG)pLayer;
		if(f == s)
		{
			osg::Node* pNode = be->second->GetLayerNode();
			bool b = m_pSwitch->removeChild(pNode);
			if (b)
			{
				m_mapLayer.erase(be->first);
			}
			return true;
		}
		else
			return false;
	}
	
}

void Map::TramsformMap()
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		Layer* layer = layerIter->second;
		if(layer)
		{
			layer->CoordTrans(m_pCoordTrans);
			layer->TransformLayer();
		}
		layerIter++;
	}
}

void Map::TramsformMap( bool useLayerZ )
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		Layer* layer = layerIter->second;
		if(layer)
		{
			layer->CoordTrans(m_pCoordTrans);
			layer->UseLayerZ(useLayerZ);
			layer->TransformLayer();
		}
		layerIter++;
	}
}

void Map::CreateMapData()
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		Layer* layer = layerIter->second;
		if(layer)
		{
			layer->CoordTrans(m_pCoordTrans);
			layer->CreateLayerData();
		}
		layerIter++;
	}
}

void Map::SetVisibleLayer( const std::string& name,bool visible )
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		Layer* layer = layerIter->second;
		if(layer)
		{
			if (layerIter->first == name)
			{
				layer->Visible(visible);
				m_pSwitch->setChildValue(layer->GetLayerNode(),visible);
			}

		}
		layerIter++;
	}
}

void Map::operator=( const Map& other )
{
	std::map<std::string, Layer*> mapLayers = 	other.m_mapLayer;
	std::map<std::string, Layer*>::iterator layerIter = mapLayers.begin();
	while(layerIter != mapLayers.end())
	{
		std::string otherName = layerIter->first;
		Layer* otherLayer = layerIter->second;
		if (otherLayer->LayerType() == "VectorLayer")
		{
			VectorLayer* thisLayer = new VectorLayer(other.m_pDataProvider,this->m_pCoordTrans);
			thisLayer->SetParentMap(this);
			*thisLayer = *dynamic_cast<VectorLayer*>(otherLayer);
			thisLayer->Visible(otherLayer->Visible());
			//thisLayer->CreateLayerFont();
			this->AddLayer(otherName,thisLayer);
			//this->m_mapLayer.insert(MapLayerPair(otherName,&thisLayer));
		}
		else
		{
			RadarVectorLayer* thisLayer = new RadarVectorLayer(other.m_pDataProvider,this->m_pCoordTrans);
			thisLayer->SetParentMap(this);
			*thisLayer = *dynamic_cast<RadarVectorLayer*>(otherLayer);
			thisLayer->Visible(otherLayer->Visible());
			//thisLayer->CreateLayerGeometry();
			this->AddLayer(otherName,thisLayer);
		}
		layerIter++;
	}

	//this->m_pRootNode = dynamic_cast<osg::MatrixTransform*>(other.m_pRootNode->clone(GraphCopyOp(osg::CopyOp::DEEP_COPY_NODES)));
	//this->m_pRootSwitch = dynamic_cast<osg::Switch*>(other.m_pRootSwitch->clone(GraphCopyOp(osg::CopyOp::DEEP_COPY_NODES)));
	//this->m_pCoordTrans = new Proj4Transform;
	//this->m_pCoordTrans			=	other.m_pCoordTrans;
	//this->m_pDataProvider	=	other.m_pDataProvider;
	this->m_backgroundColor	=	other.m_backgroundColor;
	this->m_basisLayerList = other.m_basisLayerList;
}

void Map::SetVisibleAllLayer( bool visible )
{
	std::map<std::string, Layer*>::iterator layerIter = m_mapLayer.begin();
	while(layerIter != m_mapLayer.end())
	{
		Layer* layer = layerIter->second;
		if(layer)
		{
			if (layerIter->first == "距离圈")
			{

			}
			else
			{
				layer->Visible(visible);
				m_pSwitch->setChildValue(layer->GetLayerNode(),visible);
			}
		}
		layerIter++;
	}
}

void Map::ClearMap()
{
	m_mapLayer.clear();
	m_pSwitch = new osg::Switch;
}

void Map::ClearMapAndDeleteAllLayer()
{

}

void Map::Switch2DMap()
{

}

void Map::Switch3DMap()
{

}

void Map::GetLayerZOrder(std::map<float, std::string>* LayerName)
{
	std::map<std::string, Layer*>::iterator iterBegin;

	iterBegin = m_mapLayer.begin();

	for(;iterBegin != m_mapLayer.end(); iterBegin++)
	{
		LayerName->insert(std::pair<float, std::string>(iterBegin->second->LayerZ(), iterBegin->first));
	}
}

void Map::MoveUpByZ( std::string& strName )
{
	SwapLayerZ(true, strName);
}

void Map::MoveDownByZ( std::string& strName )
{
	SwapLayerZ(false, strName);
}

void Map::SwapLayerZ( bool bdirection, std::string& strName )
{
	Layer* pLayer = 0;
	if(strName == "")
		pLayer = CurrentLayer();
	else
		pLayer = GetLayer(strName);

	if(!pLayer) return;
	float fZ = pLayer->LayerZ();
	float fPreZ = 0, fNextZ = 0;

	std::map<float, std::string>::iterator iterCurrent, preIter, nextIter; 
	std::map<float, std::string> LayerName;
	LayerName.clear();

	GetLayerZOrder(&LayerName);

	for(iterCurrent = LayerName.begin(); iterCurrent != LayerName.end(); iterCurrent++)
	{
		if(abs(fZ - iterCurrent->first) < 0.00000001)
		{
			if(bdirection)
			{
				if(iterCurrent != LayerName.begin())
				{
					preIter = --iterCurrent;
					++iterCurrent;

					Layer* pPreLayer = GetLayer(preIter->second);

					fPreZ = pPreLayer->LayerZ();
					pPreLayer->LayerZ(fZ);
					pPreLayer->TransformLayer();

					pLayer->LayerZ(fPreZ);
					pLayer->TransformLayer();
				}
			}
			else
			{
				nextIter = ++iterCurrent;

				if(nextIter == LayerName.end()) return;
				--iterCurrent;

				Layer* pNextLayer = GetLayer(nextIter->second);

				fNextZ = pNextLayer->LayerZ();

				pNextLayer->LayerZ(fZ);
				pNextLayer->TransformLayer();

				pLayer->LayerZ(fNextZ);
				pLayer->TransformLayer();
			}
		}
	}
}

GOTO_NAMESPACE_END