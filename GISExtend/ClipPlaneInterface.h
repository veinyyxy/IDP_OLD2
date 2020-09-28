#pragma once
#include <osg/Group>
#include <osg/Vec4>

class ClipBoard;

class ClipPlaneInterface
{
public:
	virtual ClipBoard* AddClipPlane(osg::Node* pClipedNode, osg::Group* pMapRootNode,  const osg::Vec4& rect) = 0;
	virtual void DelClipPlane(ClipBoard*) = 0;
};

