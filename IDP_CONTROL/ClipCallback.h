#pragma once
#include "goConfig.h"
#include "ClipBoard.h"
#include "Map.h"
#include "Layer.h"

class ClipCallback : public ClipBoard::ClipBoardMoveCallback
{
public:
	ClipCallback(goto_gis::Map* map);
	boost::any GetAttributeValue(int index ,goto_gis::Layer* layer)
	{
		boost::any _value;
		Layer::P_LAYER_ATTRIBUTE_MAP pLayerAttribteTable = layer->GetLayerAttribteTable();
		Layer::LAYER_ATTRIBUTE_MAP::iterator findResult = pLayerAttribteTable->find(index);
		_value = findResult->second;
		return _value;
	}

	double Get_Length( double dx, double dy )
	{
		return( sqrt(dx*dx + dy*dy) );
	}

	virtual void MoveCompleted( osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode, osg::Image** ppImage);
	void DrawPlotGraph( goto_gis::Map* srcMap, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode );
	void DrawPlotGraph(goto_gis::Map* srcMap, QImage* pImage, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4);
private:
	goto_gis::Map* _map;
};