/*============================================================ 
文件名： Wind.h 
类  名： CWind
父  类： CAlgorithmInterface
子  类： 无 
功能说明： 风标算法
调用说明： 针对地面填图、高空填图的绘制，而非人工手绘的风标
---------------------------版本更新---------------------------
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ： renxue
完成日期：2011年08月23日
============================================================*/

#pragma once
#include "AlgorithmInterface.h"

class IDP_CONTROL_EXPORT CWind :public CAlgorithmInterface
{
public:
	CWind(int iWidth);
	//CWind(int nType);
	~CWind(void);
	virtual void SetLineAlgorithmWidth(float width);
	virtual bool SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);
	virtual bool WindAlgorithm(float fSpeed, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);//P是风标的位置，fAngle是风向，fSpeed是风速

private:
	int  WindScaleBasedSpeed(float fSpeed);                  //根据风速返回风的级别
	bool CaculateBarb(osg::Vec3Array &array, int nType, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);//根据风级nType和风杆（array）计算风羽上的点；
	int   m_nRatio;
};

