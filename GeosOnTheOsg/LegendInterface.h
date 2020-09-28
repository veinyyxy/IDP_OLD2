#pragma once
#include <osg/Vec4>
#include <vector>
#include <map>
#include <QtGui/QPaintDevice>

#include "goConfig.h"

GOTO_NAMESPACE_START

class LegendInterface
{
public:
	typedef std::vector< std::pair<double, double> > VALUE_SCOPE, *P_VALUE_SCOPE;
	virtual int GetValueScopeList(LegendInterface::P_VALUE_SCOPE valueList) = 0;
	/**以下两个函数获得图例的颜色和大小的参数*/
	virtual int GetLegendColor(double dValue, osg::Vec4* v4Color) = 0;
	virtual int GetLegendSize(double dValue, int* size) = 0;
	/**以下三个函数用来获取图例绘制对象*/
	virtual int GetValueScope(double dValue, QPaintDevice* pPaintDev) = 0;
	virtual int GetValueScope(double dValue, osg::Drawable* pDrawable) = 0;
	virtual int GetValueScope(double dValue, osg::Node* pNode) = 0;
	virtual int GetVauleScope(double dValue, void* pAny) = 0;

};

GOTO_NAMESPACE_END