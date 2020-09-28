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
文件名：ColdFront.h ColdFront.cpp 
类 名： CColdFront
父 类： CAlgorithmInterface
子 类： 无 
功能说明： 冷锋线算法
调用说明： 

-------------------------------------版本更新-----------------------------------
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：renxue
完成日期：2011年 2月24日
============================================================*/

#include "ColdFront.h"
#include "VectorAlgorithm.h"
#include "MathHelper.h"


CColdFront::CColdFront(void)
{
}

CColdFront::~CColdFront(void)
{
}

void CColdFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* 函数功能： 冷锋线算法：根据输入的折线点集,计算输出的冷锋线点集       */
/* 参数说明:  该算法隐含通过成员变量设置冷锋上三角形的边长       		*/
/*            inputLine为输入的折线点集									*/
/*            outputLine为输出的冷锋线点集								*/
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 24 日										*/
/************************************************************************/
bool CColdFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	float width = m_sideLength;
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);	
	float coldfrontsize = 4 * width;//冷锋线的单位长度，暂设定为三角形边长的四倍
	
	if (length < width * 2)  return false;//折线长度小于三角形边长的两倍
	else //求双三角形的五个顶点
	{
		osg::ref_ptr<osg::Vec3Array> inputLineCopy(&inputLine);
		osg::ref_ptr<osg::Vec3Array> lineVertex = new osg::Vec3Array;
		*lineVertex = inputLine;

		outputLine.push_back(lineVertex);//将原来的折线点集加入到输出line中
		int num, i;
		float len;
		num = length/coldfrontsize;
		if(num == 0)
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*coldfrontsize)/2;		
		
		//计算在折线上距离某点为width的点的坐标
		i = 0;
		osg::Vec3 mid1, mid2, p1, p2, p3, p4, p5;
		osg::Vec3 startp = inputLine[0];
		int nPos1, nPos3, nPos5, tempPos;
		while(i<num)
		{
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);			
			
			linehelper.getMidpoint(p1, p3, mid1);
			linehelper.getPointToPOnRightHand(p1, p3, mid1, width/2.0, 1, p2);
			
			linehelper.getMidpoint(p3, p5, mid2);
			linehelper.getPointToPOnRightHand(p3, p5, mid2, width/2.0, 1, p4);
			tempPos = nPos3;
			osg::ref_ptr<osg::Vec3Array> triangle1(new osg::Vec3Array), triangle2(new osg::Vec3Array);
			//triangle1<<p1<<p2<<p3;//冷锋线上的三角形
			triangle1->push_back(p1);
			triangle1->push_back(p2);
			triangle1->push_back(p3);

			while(tempPos > nPos1)
			{
				//triangle1<<inputLine[tempPos--];
				triangle1->push_back(inputLine[tempPos--]);
			}
			//triangle1<<p1;
			triangle1->push_back(p1);

			//triangle2<<p3<<p4<<p5;
			triangle2->push_back(p3);
			triangle2->push_back(p4);
			triangle2->push_back(p5);

			while(nPos5 > nPos3)
			{
				//triangle2<<inputLine[nPos5--];
				triangle2->push_back(inputLine[nPos5--]);
			}
			//triangle2<<p3;
			triangle2->push_back(p3);

			//outputLine<<triangle1<<triangle2;
			outputLine.push_back(triangle1);
			outputLine.push_back(triangle2);

			len += coldfrontsize;
			i++;
		}
	}	
	return true;
}