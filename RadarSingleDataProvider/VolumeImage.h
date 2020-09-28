#pragma once
#include <QtGui/QIMage>
#include "RadReadColor_XML.h"
#include "osg/Vec3"
#include "osg/Node"
#include "CoordinateTransform.h"

class VolumeImage
{
public:
	virtual void GetImages(QVector<QImage>* imageList) = 0;
	virtual void Cutting(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode,goto_gis::CoordinateTransform* proj) = 0;
};