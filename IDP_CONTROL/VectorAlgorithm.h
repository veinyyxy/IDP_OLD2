/*============================================================ 
项目名： 人机交互平台
作 者： 任雪
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：VectorAlgorithm.h VectorAlgorithm.cpp 
类 名： CVectorAlgorithm 
父 类： 无
子 类： 无 
功能说明： 实现对双线、箭头、四种锋线、霜冻线算法的基础函数
调用说明： osg::Vec3里的数都是采用float型

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
原作者 ：任雪
完成日期：2011年04月15日
============================================================*/ 

#pragma once
#include <osg/Vec3>
#include <osg/Array>

class CVectorAlgorithm
{
public:
	static void getLineFormula(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &formula);//根据两点求直线方程(型如ax+by+c=0),b只有两个取值0或-1；
	static void LineFormula(osg::Vec3 &P, float fDirection, osg::Vec3 &formula);//根据点和夹角求直线方程；
	static double distancePointFToPointF(osg::Vec3 &p1, osg::Vec3 &p2);//求两点之间的距离
	static bool getEquidistanceLines(osg::Vec3 &line, float width, osg::Vec3Array &lines);//根据已知的直线方程，求距其width的等距离的两条直线方程
	static bool getTwoAngularBisectrixFormula(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3Array &angularlines);//获得两条线的两条角平分线
	static bool isLinesIntersected(osg::Vec3 &line1, osg::Vec3 &line2);//判断line1和line2是否相交，是返回true，否则返回false
	static void getPointofIntersection(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3 &P);//求两条线的交点
	static double distancePointToLine(osg::Vec3 &p, osg::Vec3 &line);//求点到直线的距离
	static bool getNormalFormula(osg::Vec3 &p, osg::Vec3 &line, osg::Vec3 &normaline);//求过p点的line的法线
	static void getMidpoint(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &M);//求线段p1p2的中点

	static bool getPointDistantToP(osg::Vec3 &line, osg::Vec3 &p, float width, osg::Vec3Array &Qs);//求解在直线line上计算到p点的距离为width的两点的坐标
	static bool isABCDSameDirection(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &C, osg::Vec3 &D);//点A、B、C、D在一条直线上，向量AB和向量CD方向相同，则返回true，否则返回false
	static bool getPointInVectorABToP(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &p, float width, osg::Vec3 &Q);//在向量AB的正方向上，求解距离p点为width的点Q；
	static bool rightHandRule(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P);//判断p与矢量AB是否满足右手定理，满足返回true，否则返回false//未测试
	//static bool getPoint(osg::Vec3 &A, osg::Vec3 &B, float width, osg::Vec3 &Q);//获取线段AB中垂线上距离AB的中点为width的点Q，该点与向量AB满足右手定理；
	static bool getPointToPOnRightHand(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P, float width, int flag, osg::Vec3 &Q);//P是线段AB上的任意点，获取QP，QP垂直AB，且QP的长度为width，如果flag=1，Q点与向量AB满足右手定理；如果flag=0，Q点与向量AB不满足右手定理
	static float getAngleOfVectorAB(osg::Vec3 &A, osg::Vec3 &B);//以原点O为起点，求向量OA和OB之间的夹角,用弧度标示；
	static float getAngleToX(float angle, osg::Vec3 &P);//在右方向为x轴正向，向上为y轴正方向的坐标系下，已知P的坐标，和OP与x轴正方向的夹角，求OP的角

	static int   direction(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &p);//判断p与ab的相对位置
	static bool  isPointInTriangle(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &c, osg::Vec3 &p);//判断p是否在abc三个点组成的三角形区域内
	static void  getPolyLineFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula);//根据一条折线点集求各个线段的直线方程(型如ax+by+c=0),b只有两个取值0或-1；
	static void  getAngularBisectrixFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula, osg::Vec3Array &angularlines);//获得一条折线上的所有角平分线的方程
	static void  getEquidistanceLines(osg::Vec3Array &polylineformula, float width, osg::Vec3Array &equidistancelineformula);//根据已知的折线方程，求距其width的等距离折线方程
	static float distanceOfPolyLine(osg::Vec3Array &polygonf);//求折线的长度：各个折线段的长度和
	static bool  getPointOnPolyline(osg::Vec3 &Q, int &nPos, osg::Vec3Array &polygonf, osg::Vec3 &P, float width);
	//20110316编写
	static bool getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length);//根据点p来求解其矩形的折线，flag为1表示在左下方画矩形，flag为0表示在右下方画矩形（主要用于霜冻线算法）
};

