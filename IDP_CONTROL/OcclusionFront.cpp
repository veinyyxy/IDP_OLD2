/*============================================================ 
项目名： 人机交互平台
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：OcclusionFront.h OcclusionFront.cpp 
类 名： COcclusionFront
父 类： CAlgorithmInterface
子 类： 无 
功能说明： 锢囚锋算法
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年2月24日
============================================================*/

#include "OcclusionFront.h"
#include "VectorAlgorithm.h"


COcclusionFront::COcclusionFront(void)
{
}


COcclusionFront::~COcclusionFront(void)
{
}


void COcclusionFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* 函数功能： 锢囚锋算法：根据输入的折线点集,计算输出的锢囚锋点集       */
/* 参数说明:  该算法隐含通过成员变量设置锢囚锋上三角形的边长       		*/
/*            inputLine为输入的折线点集									*/
/*            outputLine为输出的锢囚锋点集								*/
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 24 日										*/
/************************************************************************/
bool COcclusionFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	float width = m_sideLength;
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);
	float len;
	float h1, h2, h3;

	float occlusionfrontsize = 4*width;//暖锋线的单位长度，暂设定直径的四倍
	osg::Vec3 startp = inputLine[0];
	//outputLine<<inputLine;
	outputLine.push_back(osg::ref_ptr<osg::Vec3Array>(&inputLine));

	if (length < 2*width)  return false;//折线长度小于直径的两倍
	else //求半圆突起的关键顶点
	{ 
		int num, i;
		num = length/occlusionfrontsize;
		i = 0;
		if(num == 0)//折线长度大于直径的两倍，小于直径的四倍
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*occlusionfrontsize)/2;//折线长度大于直径的四倍

		//计算在折线上距离某点为width的点的坐标
		h1 = width*sqrt(5.0)/8;
		h2 = width*sqrt(3.0)/4;
		h3 = width*sqrt(15.0)/8;
		osg::Vec3 p1, p2, p3, p4, p5, M;	
		osg::Vec3 ml1, ml2, ml3, mr1, mr2, mr3;
		osg::Vec3 QL1, QL2, QL3, QR1, QR2, QR3;
		int nPos1, nPos3, nPos5, tempPos;
		while(i<num)
		{
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);
			linehelper.getMidpoint(p1, p3, M);
			linehelper.getPointToPOnRightHand(p1, p3, M, width/2, 1, p2);			

			//收集三角形点集
			osg::ref_ptr<osg::Vec3Array> triangle(new osg::Vec3Array), semicircle(new osg::Vec3Array);
			tempPos = nPos3;
			//triangle<<p1<<p2<<p3;
			triangle->push_back(p1);
			triangle->push_back(p2);
			triangle->push_back(p3);

			while(tempPos > nPos1)
			{
				triangle->push_back(inputLine[tempPos--]);
			}
			triangle->push_back(p1);

			linehelper.getMidpoint(p3, p5, M);
			linehelper.getPointToPOnRightHand(p3, p5, M, width/2, 1, p4);
			linehelper.getMidpoint(p3, M, ml2);
			linehelper.getMidpoint(p3, ml2, ml1);
			linehelper.getMidpoint(ml2, M, ml3);
			linehelper.getMidpoint(M, p5, mr2);
			linehelper.getMidpoint(M, mr2, mr3);
			linehelper.getMidpoint(mr2, p5, mr1);
			linehelper.getPointToPOnRightHand(p3, p5, ml1, h1, 1, QL1);
			linehelper.getPointToPOnRightHand(p3, p5, ml2, h2, 1, QL2);
			linehelper.getPointToPOnRightHand(p3, p5, ml3, h3, 1, QL3);
			linehelper.getPointToPOnRightHand(p3, p5, mr1, h1, 1, QR1);
			linehelper.getPointToPOnRightHand(p3, p5, mr2, h2, 1, QR2);
			linehelper.getPointToPOnRightHand(p3, p5, mr3, h3, 1, QR3);

			//收集半圆点集
			//semicircle<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			semicircle->push_back(p3);
			semicircle->push_back(QL1);
			semicircle->push_back(QL2);
			semicircle->push_back(QL3);
			semicircle->push_back(p4);
			semicircle->push_back(QR3);
			semicircle->push_back(QR2);
			semicircle->push_back(QR1);
			semicircle->push_back(p5);
			while(nPos5 > nPos3)
			{
				semicircle->push_back(inputLine[nPos5--]);
			}
			semicircle->push_back(p3);

			//outputLine<<triangle<<semicircle;
			outputLine.push_back(triangle);
			outputLine.push_back(semicircle);

			len += occlusionfrontsize;
			i++;
		}
	}	
	return true;
}