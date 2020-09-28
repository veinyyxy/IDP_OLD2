/*============================================================ 
文件名：Wind.cpp 
类 名： CWind
父 类： CAlgorithmInterface
子 类： 无 
功能说明： 风标算法
调用说明： 

-------------------------版本更新---------------------------
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：renxue
完成日期：2011年08月23日
============================================================*/

#include "Wind.h"
#include "VectorAlgorithm.h"
#include "MathHelper.h"


CWind::CWind(int iWidth)
{
	CAlgorithmInterface::m_width = iWidth;
	m_nRatio = 6;
}

CWind::~CWind(void)
{
}

void CWind::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
// 函数功能：父类的函数
/************************************************************************/
bool CWind::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	return false;
}

/************************************************************************/
/* 函数功能： 风标算法：根据输入的两个点,计算输出的风标点集             */
/* 参数说明:  该算法隐含通过成员变量设置风标上横线的边长         		*/
/*            P是风标的位置，fAngle是风向，fSpeed是风速
              outputLine是风标的输出点集                                */
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 08 月 23 日										*/
/************************************************************************/
bool CWind::WindAlgorithm(float fSpeed, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	CVectorAlgorithm linehelper;

	osg::Vec3  StartPoint(0, 0, 0), EndPoint(0, m_sideLength*2.0, 0);
	osg::ref_ptr<osg::Vec3Array> sharft_real(new osg::Vec3Array), barb_real(new osg::Vec3Array);
	osg::Vec3Array& shaft = *sharft_real;
	osg::Vec3Array& barb = *barb_real;//shaft是风杆上的点，barb是风羽上的点
	//shaft<<StartPoint<<EndPoint;
	shaft.push_back(StartPoint);
	shaft.push_back(EndPoint);
	//outputLine<<shaft;
	outputLine.push_back(sharft_real);

	int nType = WindScaleBasedSpeed(fSpeed);
	if(nType<0)
	{
		return false;
	}
	if(nType<=36)
	{
		m_nRatio = 6;
	}
	else if(nType>36 && nType<200)
	{
		m_nRatio = 10;
	}
	else if(nType>200)
	{
		m_nRatio = 20;
	}

	//求风羽点
	osg::Vec3 tempA, tempB;
	if(nType == 0)   //无风
	{
		linehelper.getPointInVectorABToP(StartPoint, EndPoint, StartPoint, m_width*(m_nRatio-2), tempA);
		linehelper.getPointToPOnRightHand(StartPoint, EndPoint, tempA, m_width, 0, tempB);
		//barb<<EndPoint<<tempB;
		barb.push_back(EndPoint);
		barb.push_back(tempB);
		//outputLine<<barb;
		outputLine.push_back(barb_real);
	}
	else 
	{
		CaculateBarb(shaft, nType, outputLine);
	}		
	return true;
}

/************************************************************************/
/* 函数功能： 根据风速确定风的级别                                      */
/* 参数说明:  fSpeed是float型的风速       	                        	*/
/*            array是风杆的点集，nType是风级,outputLine是风羽的点集     */
/* 返 回 值:  int 正常执行返回风级，否则返回-1                  		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 09 月 01 日										*/
/************************************************************************/
int CWind::WindScaleBasedSpeed( float fSpeed )
{
	if(fSpeed<0) return -1;
	return (int)fSpeed/2;
}

/************************************************************************/
/* 函数功能： 风羽算法：根据输入的两个点,计算输出的风羽点集             */
/* 参数说明:  该算法隐含通过成员变量设置风羽上横线的边长         		*/
/*            array是风杆的点集，nType是风级,outputLine是风羽的点集     */
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 09 月 01 日										*/
/************************************************************************/
bool CWind::CaculateBarb( osg::Vec3Array &array, int nType, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine )
{
	CVectorAlgorithm linehelper;
	osg::Vec3 tempA, tempB, tempC;
	float     fLongWidth = m_width * 3.0;//风羽长线的长度


	int nShaftRange, nRange;
	nRange = 1.5;
	nShaftRange = m_nRatio + nRange;
	int i=0;
	while(nType)
	{
		osg::ref_ptr<osg::Vec3Array> barb_real(new osg::Vec3Array);
		osg::Vec3Array& barb = *barb_real;
		if(nType>=10)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			//barb<<tempC<<tempA<<tempB<<tempC;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempA);
			barb.push_back(tempB);
			barb.push_back(tempC);
			outputLine.push_back(barb_real);

			nType -= 10;
		}
		else if(nType>=2)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			//barb<<tempC<<tempB;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempB);
			outputLine.push_back(barb_real);

			nType -= 2;
		}
		else if(nType>0)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			linehelper.getMidpoint(tempB, tempC, tempA);
			//barb<<tempC<<tempA;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempA);

			outputLine.push_back(barb_real);

			nType -= 1;
		}
		i++;
	}
	
	return true;
}
