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
文件名：WarmFront.h WarmFront.cpp 
类 名： CWarmFront
父 类： CAlgorithmInterface
子 类： 无 
功能说明： 暖锋算法
调用说明： 

-------------------------------------版本更新------------------------------------------------ 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：renxue
完成日期：2011年2月23日
============================================================*/

#include "WarmFront.h"
#include "VectorAlgorithm.h"

#define LINE_DEBUG_OUT 0

CWarmFront::CWarmFront(void)
{
}


CWarmFront::~CWarmFront(void)
{
}

void CWarmFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* 函数功能： 暖锋线算法：根据输入的折线点集,计算输出的暖锋线点集       */
/* 参数说明:  该算法隐含通过成员变量设置暖锋上半圆的直径        		*/
/*            inputLine为输入的折线点集									*/
/*            outputLine为输出的暖锋线点集								*/
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 23 日										*/
/************************************************************************/
bool CWarmFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)//根据一个折线方程的点集求解暖锋线的点集,width是暖锋线上圆的直径
{
	//************************************************
#if LINE_DEBUG_OUT
	qDebug("******************WarmFront: e-6 = %f, e-7 = %f*********************",1e-6, 1e-7);
	for(int i=0; i<inputLine.size(); i++)//debug--nirissa--20110301
	{
		qDebug("inputLine[%d]:x:%f, y:%f", i, inputLine[i].x(), inputLine[i].y());
	}
#endif
	//************************************************

	float width = m_sideLength;	
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);
	float len, h1, h2, h3;

	float warmfrontsize = 4*width;//暖锋线的单位长度，暂设定直径的四倍
	osg::Vec3 startp = inputLine[0];

	osg::ref_ptr<osg::Vec3Array> inputLineCopy = new osg::Vec3Array;
	*inputLineCopy = inputLine;
	outputLine.push_back(inputLineCopy);
	if (length < 2*width)  return false;//折线长度小于直径的两倍
	else //求半圆突起的关键顶点
	{ 
		int num, i;
		num = length/warmfrontsize;	
		i = 0;
		if(num == 0)//折线长度大于直径的两倍，小于直径的四倍
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*warmfrontsize)/2;//折线长度大于直径的四倍

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
#if LINE_DEBUG_OUT
			qDebug("d:%f, h1:%f, h2:%f, h3:%f, i=%d", width/2, h1, h2, h3, i);//debug--nirissa
#endif
			
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);
			//***************Debug--nirissa*****************
#if LINE_DEBUG_OUT
			float tResult;
			qDebug("p1: x:%f, y:%f；；%f ", p1.x(), p1.y(), width);
			tResult =  sqrt((p3.x()-p1.x())*(p3.x()-p1.x())+ (p3.y()-p1.y())*(p3.y()-p1.y()));
			qDebug("p3: x:%f, y:%f；；%f ", p3.x(), p3.y(), tResult);			
			tResult =  sqrt((p5.x()-p3.x())*(p5.x()-p3.x())+ (p5.y()-p3.y())*(p5.y()-p3.y()));
			qDebug("p5: x:%f, y:%f；；%f ", p5.x(), p5.y(), tResult);
#endif  
   			//****************Debug--nirissa***************
			linehelper.getMidpoint(p1, p3, M);

#if LINE_DEBUG_OUT
			qDebug("M:%f, %f", M.x(), M.y());
			qDebug("**********p1, p3*********");
#endif
			linehelper.getPointToPOnRightHand(p1, p3, M, width/2, 1, p2);

			linehelper.getMidpoint(p1, M, ml2);
			linehelper.getMidpoint(p1, ml2, ml1);
			linehelper.getMidpoint(ml2, M, ml3);
			linehelper.getMidpoint(M, p3, mr2);
			linehelper.getMidpoint(M, mr2, mr3);
			linehelper.getMidpoint(mr2, p3, mr1);
			linehelper.getPointToPOnRightHand(p1, p3, ml1, h1, 1, QL1);
			linehelper.getPointToPOnRightHand(p1, p3, ml2, h2, 1, QL2);
			linehelper.getPointToPOnRightHand(p1, p3, ml3, h3, 1, QL3);
			linehelper.getPointToPOnRightHand(p1, p3, mr1, h1, 1, QR1);
			linehelper.getPointToPOnRightHand(p1, p3, mr2, h2, 1, QR2);
			linehelper.getPointToPOnRightHand(p1, p3, mr3, h3, 1, QR3);
			
			//收集半圆点集
			osg::ref_ptr<osg::Vec3Array> semicircle1(new osg::Vec3Array);
			osg::ref_ptr<osg::Vec3Array> semicircle2(new osg::Vec3Array);
			//semicircle1<<p1<<QL1<<QL2<<QL3<<p2<<QR3<<QR2<<QR1<<p3;
			semicircle1->push_back(p1);
			semicircle1->push_back(QL1);
			semicircle1->push_back(QL2);
			semicircle1->push_back(QL3);
			semicircle1->push_back(p2);
			semicircle1->push_back(QR3);
			semicircle1->push_back(QR2);
			semicircle1->push_back(QR1);
			semicircle1->push_back(p3);
			tempPos = nPos3;
			while(tempPos > nPos1)
			{
				semicircle1->push_back(inputLine[tempPos--]);
			}
			semicircle1->push_back(p1);

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
			//semicircle2<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			semicircle2->push_back(p3);
			semicircle2->push_back(QL1);
			semicircle2->push_back(QL2);
			semicircle2->push_back(QL3);
			semicircle2->push_back(p4);
			semicircle2->push_back(QR3);
			semicircle2->push_back(QR2);
			semicircle2->push_back(QR1);
			semicircle2->push_back(p5);

			while(nPos5 > nPos3)
			{
				semicircle2->push_back(inputLine[nPos5--]);
			}
			semicircle2->push_back(p3);

			//outputLine<<semicircle1<<semicircle2;
			outputLine.push_back(semicircle1);
			outputLine.push_back(semicircle2);
			len += warmfrontsize;
			i++;
		}
	}	
	return true;
}
