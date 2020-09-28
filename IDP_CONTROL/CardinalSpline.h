/*============================================================ 
项目名： 人机交互平台
作 者： lugsh
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CardinalSpline.h CardinalSpline.cpp 
类 名： CCardinalSpline
父 类： NULL
子 类： 无 
功能说明： 基数样条插值算法
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：lugsh
完成日期：2011年？月？日
============================================================*/
#pragma once
#include "idp_control_global.h"
#include <osg/Array>

class IDP_CONTROL_EXPORT CCardinalSpline
{
public:
	CCardinalSpline(void);
	CCardinalSpline(const CCardinalSpline&);
	~CCardinalSpline(void);
	void GetSmoothPoints(osg::Vec3Array &inputData, osg::Vec3Array &OutputData);
	void MergeSmoothLines(osg::Vec3Array &OutPutData, osg::Vec3Array &OutputData2,osg::Vec3Array &OutPutDataSum);

private :
	static osg::Vec3 Eq(float t, const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4);
	osg::Vec3 GetInterpolatedSplinePoint(float t);      //取出平滑后曲线点
	int GetNumPoints();									//得到指定曲线控制点的数量
	osg::Vec3& GetNthPoint(int n);						//返回指定曲线对应的控制点
	void ClearSplinePoint();							//清除曲线的所有控制点
	void AddSplinePoint(const osg::Vec3& v);			//向里面指定2D\3D曲线控制点数据
	osg::ref_ptr<osg::Vec3Array> m_ControlPoints;
	float m_delta;
};

