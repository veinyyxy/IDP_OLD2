/*============================================================ 
文件名：DrawLineSymbol.h DrawLineSymbol.cpp 
类 名： CDrawLineSymbol
父 类： ManualDraw
子 类： 无 
功能说明：画矢量符号线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

--------------------------版本更新----------------------------
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年7月27日
============================================================*/
#pragma once
#include <osg/Geometry>
#include "IDPDrawGeometryData.h"
#include "AlgorithmInterface.h"
#include "CardinalSpline.h"
#include "Layer.h"

class CDrawLineSymbol : public IDPDrawGeometryData
{
public:
	CDrawLineSymbol(int nType);
	~CDrawLineSymbol(void);
	int GetType(){return m_nType;};
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );

private:
	osg::Geometry* CreateLine();
	int    m_nType;	
	osg::ref_ptr<osg::Geometry> m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//判断是否结束画线
	osg::ref_ptr<osg::Vec3Array>  m_pArray;
	osg::ref_ptr<osg::Vec3Array>  m_InputData, m_OutputData;
	//osg::Geometry* m_pSymbolLine;
	CCardinalSpline *m_pSpLine;
};

