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
文件名：VectorAlgorithm.h VectorAlgorithm.cpp 
类 名： CVectorAlgorithm 
父 类： 无
子 类： 无 
功能说明： 实现对双线、箭头、四种锋线、霜冻线算法的基础函数
调用说明： osg::Vec3里的数都是采用float型

-------------------------------------版本更新-----------------------------
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：renxue
完成日期：2011年04月15日
============================================================*/ 

#include "VectorAlgorithm.h"
#include <math.h>

#define PI 3.1415926

/************************************************************************/
/* 函数功能： 根据两点求直线方程(型如ax+by+c=0)          				*/
/* 参数说明:  p1和p2为已知点的坐标  									*/
/*            formula是求得的型如ax+by+c=0格式的方程的系数  			*/
/* 返 回 值:  null                              						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::getLineFormula(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &formula)
{
	float a, b, c;
	
	if(abs(p1.x()-p2.x())<1e-5)
	{
		b = 0.0;
		a = 1.0;
		c = -p1.x();
	}
	else
	{
		b = -1.0;
		a = (p2.y()-p1.y())/(p2.x()-p1.x());
		c = p1.y() - p1.x()*a;
	}
	formula.set(a, b, c);
}

/************************************************************************/
/* 函数功能： 根据两点求直线方程(型如ax+by+c=0)          				*/
/* 参数说明:  P为直线上的点，fDirection是直线与x轴正方向的夹角（0~360）	*/
/*            formula是求得的型如ax+by+c=0格式的方程的系数  			*/
/* 返 回 值:  null                              						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::LineFormula(osg::Vec3 &P, float fDirection, osg::Vec3 &formula)
{
	//处理异常的角度
	while(abs(fDirection-0.0)<1e-5)
	{
		fDirection += 360.0;
	}
	while(abs(fDirection-360.0)<1e-5)
	{
		fDirection -= 360.0;
	}

	float a, b, c;
	if(abs(fDirection-0.0)<1e-5 || abs(fDirection-180.0)<1e-5 || abs(fDirection-360.0)<1e-5)//0度、180度、360度方向
	{
		a = 0;
		b = -1;
		c = P.y();
	}
	else if(abs(fDirection-90.0)<1e-5 || abs(fDirection-270.0)<1e-5)//90度或270度方向
	{
		a = -1;
		b = 0;
		c = P.x();
	}
	else
	{
		b = -1;
		float fAngle = fDirection/(2.0*PI);
		a = tan(fAngle);
		c = P.y()-P.x()*a;
	}
	
	formula.set(a, b, c);
}


/************************************************************************/
/* 函数功能： 求两点之间的距离          								*/
/* 参数说明:  p1和p2为已知点的坐标  									*/
/* 返 回 值:  float p1和p2之间的距离                              		*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
double CVectorAlgorithm::distancePointFToPointF(osg::Vec3 &p1, osg::Vec3 &p2)
{
	double distance = 0.0;
	distance = sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()));
	return distance;
}

/************************************************************************/
/* 函数功能： 获得两条线的角平分线         								*/
/* 参数说明:  line1和line2为输入的直线方程								*/
/* 返 回 值:  bool平行线或同一条直线返回false，正常运行返回true         */
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getTwoAngularBisectrixFormula(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3Array &angularlines)
{
	//平行线or同一条直线
	if(abs(line1.x()-line2.x())<1e-6 && abs(line1.y()-line2.y())<1e-6)
	{
		return false;
	}

	float a1, b1, c1, a2, b2, c2;
	float temp1, temp2;
	//两直线相交
	temp1 = hypot(line1.x(), line1.y());
	temp2 = hypot(line2.x(), line2.y());
	a1 = line1.x()*temp2 + line2.x()*temp1;
	b1 = line1.y()*temp2 + line2.y()*temp1;
	c1 = line1.z()*temp2 + line2.z()*temp1;
	if(b1 != 0.0)
	{
		a1 /= -b1;
		c1 /= -b1;
		b1 = -1;
	}

	a2 = line1.x()*temp2 - line2.x()*temp1;
	b2 = line1.y()*temp2 - line2.y()*temp1;
	c2 = line1.z()*temp2 - line2.z()*temp1;
	if(b2 != 0.0)
	{
		a2 /= -b2;
		c2 /= -b2;
		b2 = -1;
	}
	angularlines.push_back(osg::Vec3(a1, b1, c1));
	angularlines.push_back(osg::Vec3(a2,b2,c2));
	return true;
}

/************************************************************************/
/* 函数功能： 已知某条直线方程，求距其width的等距离直线方程         	*/
/* 参数说明:  line为输入的直线方程								      	*/
/*            lines是输出的两条等距线直线方程					      	*/
/* 返 回 值:  bool 正常运行返回true，否则返回false        			    */
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getEquidistanceLines(osg::Vec3 &line, float width, osg::Vec3Array &lines)
{
	if(abs(line.x())<1e-6 && abs(line.y())<1e-6 && abs(line.z())<1e-6) return false;

	float a, b, c1, c2;
	if(abs(line.y()) < 1e-6)
	{
		a = 1.0;
		b = 0.0;
		c1 = line.z() + width;
		c2 = line.z() - width;
	}
	else
	{
		b = -1.0;
		a = line.x();
		float ydistance;
		ydistance = width * sqrt(1.0 + a * a);
		c1 = line.z() + ydistance;
		c2 = line.z() - ydistance;
	}
	
	lines.push_back(osg::Vec3(a,b,c1));
	lines.push_back(osg::Vec3(a,b,c2));
	return true;
}

/************************************************************************/
/* 函数功能： 判断line1和line2是否相交  								*/
/* 参数说明:  line1和line2为已知直线的方程系数  						*/
/* 返 回 值:  bool true表示两条线相交，false表示不相交                  */
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::isLinesIntersected(osg::Vec3 &line1, osg::Vec3 &line2)
{
	if(abs(line1.x()-line2.x())<1e-5 && abs(line1.y()-line2.y())<1e-5) return false;
	else return true;
}

/************************************************************************/
/* 函数功能： 求直线line1和line2的交点  								*/
/* 参数说明:  line1和line2为已知直线的方程系数  						*/
/*            p是返回的交点                                             */
/* 返 回 值:  void                                                      */
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::getPointofIntersection(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3 &P)
{	
	double a, b;
	a = (line2.z()*line1.y() - line1.z()*line2.y())/(line1.x()*line2.y() - line2.x()*line1.y());
	b = (line2.z()*line1.x() - line1.z()*line2.x())/(line1.y()*line2.x() - line2.y()*line1.x());

	P.set(a, b, 0.0);
}

/************************************************************************/
/* 函数功能： 求过p点的line的法线,点p只使用xy坐标  						*/
/* 参数说明:  line1为输入直线的方程系数，p为已知点  					*/
/*            normaline是返回法线的方程                                 */
/* 返 回 值:  bool正常运行返回true，否则返回false                       */
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getNormalFormula(osg::Vec3 &p, osg::Vec3 &line, osg::Vec3 &normaline)
{
	if(abs(line.x())<1e-6 && abs(line.y())<1e-6 && abs(line.z())<1e-6) return false;
	
	double a, b, c;	
	if(abs(line.y())<1e-5) 
	{
		a = 0.0;
		b = -1.0;
		c = p.y();
	}
	else if(abs(line.x())<1e-5) 
	{
		b = 0.0;
		a = 1.0;
		c = -p.x();
	}
	else
	{
		a = -(1.0/line.x());
		b = -1.0;
		c = p.y() + p.x()/line.x();
	}
	normaline.set(a, b, c);
	return true;
}

/************************************************************************/
/* 函数功能： 求线段p1p2的中点          								*/
/* 参数说明:  M是求得的中点  											*/
/*            p1和p2为已知点的坐标			        					*/
/* 返 回 值:  null                              						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
void  CVectorAlgorithm::getMidpoint(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &M)
{
	/*M.setX((p1.x() + p2.x())/2.0);
	M.setY((p1.y() + p2.y())/2.0);*/

	M.set((p1.x() + p2.x())/2.0, (p1.y() + p2.y())/2.0, 0);
}

/************************************************************************/
/* 函数功能： 求点到直线的距离          								*/
/* 参数说明:  line是直线方程											*/
/*            p为已知点的坐标			            					*/
/* 返 回 值:  double p到line线的距离             						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
double CVectorAlgorithm::distancePointToLine(osg::Vec3 &p, osg::Vec3 &line)
{
	double temp = hypot(line.x(), line.y());
	temp = (line.x()*p.x() + line.y()*p.y() + line.z())/temp;
	return abs(temp);
}


/************************************************************************/
/* 函数功能： 在直线line上计算到p点的距离为width的两点的坐标Qs          */
/* 参数说明:  line是直线方程											*/
/*            Qs为得到的两个三维点的坐标								*/
/* 返 回 值:  bool 正常执行返回true否则返回false						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getPointDistantToP(osg::Vec3 &line, osg::Vec3 &p, float width, osg::Vec3Array &Qs)
{
	if(abs(line.x())<1e-6 && abs(line.y())<1e-6) return false;//判断点是否在线上，线的方程是否正确
	
	float ta1,ta2,tb1,tb2;	
	if(abs(line.y()) < 1e-6)//纵向的直线
	{		
		ta1 = ta2 = p.x();
		tb1 = p.y()+width;
		tb2 = p.y()-width;		
	}
	else if(abs(line.x()) < 1e-6)//横向的直线
	{		
		ta1 = p.x()+width;
		ta2 = p.x()-width;
		tb1 = tb2 = p.y();
	}
	else
	{
		//用一元二次方程计算横坐标
		double a, b, c;
		a = 1.0 + line.x()*line.x();
		b = 2*(line.x() * (line.z()-p.y()) - p.x());
		c = p.x()*p.x() + (line.z()-p.y())*(line.z()-p.y()) - width*width;
		osg::Vec3 equation(a,b,c);
		
		double delta = b*b -4*a*c;
	    if(delta<0.0)	return false;
		if (delta > 1e-6)
		{
			delta = sqrt(delta);
			ta1 =  (delta-equation.y())/(2*equation.x());
			ta2 = -(delta+equation.y())/(2*equation.x());		
		} 
		else 
		{
			ta1 = ta2 = -equation.y()/(2*equation.x());
		}

		tb1 = ta1 * line.x() + line.z();
		tb2 = ta2 * line.x() + line.z();
	}
	
	Qs.push_back(osg::Vec3(ta1, tb1, 0.0f));
	Qs.push_back(osg::Vec3(ta2, tb2, 0.0f));	
	return true;
}

/************************************************************************/
/* 函数功能： 判断向量AB和向量CD方向的相同，则返回true，否则返回false   */
/* 参数说明:  A、B、C、D是任意的三维点							    	*/
/* 返 回 值:  bool 向量AB和CD方向相同返回true，否则返回false			*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool  CVectorAlgorithm::isABCDSameDirection(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &C, osg::Vec3 &D)
{
	if((B.x()-A.x())*(D.x()-C.x())>1e-6 )		return true;
	else if(abs(B.x()-A.x())*(D.x()-C.x())<1e-4)
	{
		if((B.y()-A.y())*(D.y()-C.y())>1e-6) return true;
		else 			return false;
	}
	else
		return false;
}

/************************************************************************/
/* 函数功能： 在向量AB的正方向上，求解距离p点为width的点                */
/* 参数说明:  A、B、P是任意的三维点										*/
/*            Q是得到的三维点的坐标，只有一个							*/
/* 返 回 值:  bool 正常执行返回true否则返回false						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getPointInVectorABToP(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &p, float width, osg::Vec3 &Q)
{
	osg::Vec3 formula;
	getLineFormula(A, B, formula);

	//求解在直线上计算到某点的距离为width的点的坐标
	osg::ref_ptr<osg::Vec3Array> tpoint = new osg::Vec3Array; 
	getPointDistantToP(formula, p, width, *tpoint);

	int i = tpoint->size();
	if(i < 2) return false;
	if(isABCDSameDirection(A, B, p, (*tpoint)[i-1]))
	{
		Q = (*tpoint)[i-1];
	} 
	else
		Q = (*tpoint)[i-2];
	return true;
}

/************************************************************************/
/* 函数功能： p不在向量AB的直线上，判断p与矢量AB是否满足右手定理        */
/* 参数说明:  A、B、P是任意的三维点										*/
/* 返 回 值:  bool 满足右手定理返回true,否则返回false					*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool  CVectorAlgorithm::rightHandRule(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P)
{
	osg::Vec3  tB, tP;
	//tB.setX(B.x() - A.x());
	////tB.setY(A.y() - B.y());//在右方向为x轴正向，向下为y轴正方向的坐标系下的判断
	//tB.setY(B.y() - A.y());//在右方向为x轴正向，向上为y轴正方向的坐标系下的判断

	tB.set(B.x() - A.x(), B.y() - A.y(), 0);

	//tP.setX(P.x() - A.x());
	////tP.setY(A.y() - P.y());////在右方向为x轴正向，向下为y轴正方向的坐标系下的判断
	//tP.setY(P.y() - A.y());////在右方向为x轴正向，向上为y轴正方向的坐标系下的判断

	tP.set(P.x() - A.x(), P.y() - A.y(), 0);
	osg::Vec3 tO(1.0, 0.0, 0.0);

	float angleB, angleP;
	angleB = getAngleOfVectorAB(tO, tB);	
	angleB = getAngleToX(angleB, tB);	
	angleP = getAngleOfVectorAB(tO, tP);	
	angleP = getAngleToX(angleP, tP);

	if(angleB>=0.0 && angleB<270.0) //20110420修改的
	{
		if((angleP>angleB) && (angleP-angleB)<PI) return true;//第一、二、三象限
		else return false;
	}
	if((angleB>270.0 && angleB<360.0) && ((angleB>angleP) && (angleB-angleP)<PI)) return false;	//第四象限
	else return true;
}

/************************************************************************/
/* 函数功能： //P是线段AB上的任意点，获取QP 	                        */
/* 参数说明:  A、B、P是已知点											*/
/*            Q是得到的三维点，QP垂直AB，且QP的长度为width              */
/*            flag=1表示Q与向量满足右手定理，为0表示不满足；            */
/* 返 回 值:  bool                  									*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getPointToPOnRightHand(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P, float width, int flag, osg::Vec3 &Q)
{
	if (abs(A.x()-B.x())<1e-6 && abs(A.y()-B.y())<1e-6 && abs(A.z()-B.z())<1e-6) return false;

	osg::Vec3 line, normal;
	getLineFormula(A, B, line);	
	getNormalFormula(P, line, normal);	

	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array;
	getPointDistantToP(normal, P, width, *point);
	
	
	int size = point->size();
	if (size!=2) return false;
	else
	{
		if(flag)
		{
			if(bool bt = rightHandRule(A, B, (*point)[size-2])) Q = (*point)[size-2];
			else Q = (*point)[size-1];
		}
		else
		{
			if(rightHandRule(A, B, (*point)[size-2])) Q = (*point)[size-1];
			else Q = (*point)[size-2];
		}
		
		return true;
	}	
}

/************************************************************************/
/* 函数功能： 以原点O为起点，求向量OA和OB之间的夹角,用弧度表示          */
/* 参数说明:  A、B是任意点	                    						*/
/* 返 回 值:  float 向量OA和OB之间的夹角,用弧度表示                   	*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 11 日										*/
/************************************************************************/
float  CVectorAlgorithm::getAngleOfVectorAB(osg::Vec3 &A, osg::Vec3 &B)
{
	float cosine;
	cosine = (A.x()*B.x() + A.y()*B.y())/(hypot(A.x(),A.y()) * hypot(B.x(),B.y()));
	
	if(abs(1.0-cosine) < 1e-6) return 0.0;//0度值
	else if(abs(cosine+1.0) < 1e-6) return 180.0;//360度值(416改为180)
	else return acos(cosine);
}

/************************************************************************/
/* 函数功能： 已知P的坐标和op与x轴的夹角，求OP的角                      */
/* 参数说明:  坐标系：右方向为x轴正向，向上为y轴正方向  				*/
/*            angle是OP与x轴正方向的夹角    							*/
/* 返 回 值:  float OP与x轴正方向的角                               	*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 11 日										*/
/************************************************************************/
float CVectorAlgorithm::getAngleToX(float angle, osg::Vec3 &P)
{
	if(angle > 180.0)	return -1.0;
	
	if(abs(angle)<1e-6) return angle;//夹角为零
	if(P.y()>0) return angle;//第一、二象限
	else return (360.0-angle);//第三、四象限
}

/************************************************************************/
/* 函数功能： 在xy轴确定的平面上，判断点p与ab的相对位置                 */
/* 参数说明:  A、B是任意点	    										*/
/* 返 回 值:  int                      									*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 11 日										*/
/************************************************************************/
int CVectorAlgorithm::direction(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &p)//
{
	float zero=1e-6;
	float tmp=(p.x()-a.x())*(p.y()-b.y())-(p.x()-b.x())*(p.y()-a.y());
	if (tmp > zero) return 1;
	if (tmp < -1*zero) return 4;
	return 0;
}

/************************************************************************/
/* 函数功能： 在xy轴确定的平面上，判断p是否在三角形区域内 	            */
/* 参数说明:  A、B、C是三角形的顶点	，P是任意点							*/
/* 返 回 值:  bool 在三角形内返回true，否则返回false                 	*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::isPointInTriangle(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &c, osg::Vec3 &p)
{
	int t= direction(a,b,p)+ direction(b,c,p) + direction(c,a,p);

	if (t>=1 && t<=4) return true;
	if (t==8 || t==12) return true;
	return false;    
}

/************************************************************************/
/* 函数功能： 根据一条折线点集求各个线段的直线方程(型如ax+by+c=0)       */
/* 参数说明:  polygonf是输入的折线点集									*/
/*            polylineformula是输出的折线段方程集						*/
/* 返 回 值:  Null                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::getPolyLineFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula)
{
	osg::Vec3 temp;
	int i, size = polygonf.size();
	for(i = 0; i < size - 1; i++)
	{
		getLineFormula(polygonf[i], polygonf[i+1], temp);
		//polylineformula<<temp;
		polylineformula.push_back(temp);
	}
}

/************************************************************************/
/* 函数功能： 根据已知的折线方程，求距其width的等距离折线方程           */
/* 参数说明:  polygonf是输入的折线点集									*/
/*            equidistancelineformula是输出的等距离折线段方程			*/
/* 返 回 值:  Null                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::getEquidistanceLines(osg::Vec3Array &polylineformula, float width, osg::Vec3Array &equidistancelineformula)
{
	int i, size = polylineformula.size();
	for(i = 0; i < size; i++)
	{
		getEquidistanceLines(polylineformula[i], width, equidistancelineformula);				
	}
}

/************************************************************************/
/* 函数功能： 获得一条折线上的所有夹角角平分线的方程          		    */
/* 参数说明:  polygonf是输入的折线点集									*/
/*            polylineformula是输入的等距离折线段方程					*/
/*            angularlines是输出所有合理的角平分线方程					*/
/* 返 回 值:  Null                 										*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
void CVectorAlgorithm::getAngularBisectrixFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula, osg::Vec3Array &angularlines)
{ 
	int i, size = polylineformula.size();	
	osg::Vec3  point[2], tp[2];
	osg::Vec3  templine;
	for(i = 0; i < size - 1; i++)
	{
		//计算两条直线之间的角平分线
		osg::ref_ptr<osg::Vec3Array> angularformula = new osg::Vec3Array;

		if(getTwoAngularBisectrixFormula(polylineformula[i],polylineformula[i+1],*angularformula))		
		{
			//筛选合适的角平分线
			getLineFormula(polygonf[i], polygonf[i+2], templine);//计算折线组成三角形的第三条边
			if(isLinesIntersected(templine, (*angularformula)[0]) && isLinesIntersected(templine, (*angularformula)[1]))
			{
				//三角形的第三条边和两条角平分线都有交点
				getPointofIntersection(templine, (*angularformula)[0], tp[0]);
				getPointofIntersection(templine, (*angularformula)[1], tp[1]);
				getMidpoint(tp[0], polygonf[i+1], point[0]);
				getMidpoint(tp[1], polygonf[i+1], point[1]);

				//判断中点是否在三角形区域内
				if(isPointInTriangle(polygonf[i], polygonf[i+1], polygonf[i+2], point[0]))
					angularlines.push_back((*angularformula)[0]);//angularlines<<angularformula[0];
				else 
					angularlines.push_back((*angularformula)[1]);//angularlines<<angularformula[1];		
			}
			else if(isLinesIntersected(templine, (*angularformula)[0]))
			{
				angularlines.push_back((*angularformula)[0]);//angularlines<<angularformula[0];
			}
			else
				angularlines.push_back((*angularformula)[1]);//angularlines<<angularformula[1];
		}
		else
			angularlines.push_back(osg::Vec3(0.0f, 0.0f, 0.0f));//angularlines<<osg::Vec3(0.0f, 0.0f, 0.0f);
	}
}

/************************************************************************/
/* 函数功能： 获得一条折线上长度          		    					*/
/* 参数说明:  polygonf是输入的折线点集									*/
/* 返 回 值:  float 折线的长度                 							*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 02 月 11 日										*/
/************************************************************************/
float  CVectorAlgorithm::distanceOfPolyLine(osg::Vec3Array &polygonf)
{
	int i, size = polygonf.size();
	float length = 0.0;
	for(i=0; i<size-1; i++)
	{
		length += distancePointFToPointF(polygonf[i], polygonf[i+1]);
	}
	return length;
}

/************************************************************************
 功能说明：在折线polygonf的正方向上求解距离P点为width的点Q
 参	   数：polygonf是折线的3D坐标数组；
           P是polygonf的一个3D坐标点；
		   Q是输出的3D点；
		   nPos是polygonf正方向上，在Q之前polygonf上的最近的点；
 作    者：renxue
 完成日期：2010年4月11日
************************************************************************/
bool CVectorAlgorithm::getPointOnPolyline(osg::Vec3 &Q, int &nPos, osg::Vec3Array &polygonf, osg::Vec3 &P, float width)
{	
	int size = polygonf.size();
	int i = 0;
	while((P!=polygonf[i]) && (i<size))
	{
		i++;
	}
	if (i == size) return false;//P不在折线的点集上

	double sumdistance, distance;
	sumdistance = distance = 0.0;
	while (sumdistance < width)
	{
		if(i==size) return false;
		else
		{
			distance = distancePointFToPointF(polygonf[i], polygonf[i+1]);
			sumdistance += distance;
			i++;
		}
	}
	if(abs(sumdistance-width-distance)<1e-6) Q = polygonf[i];
	else
		getPointInVectorABToP(polygonf[i-1],polygonf[i],polygonf[i-1],width+distance-sumdistance, Q);

	nPos= i-1;

	return true;
}

/************************************************************************/
/* 函数功能： 根据点p来求解其矩形的折线,主要用于霜冻线算法              */
/* 参数说明:  该算法适用的坐标系：x轴右方向为正，y轴上方向为正          */
/*            flagflag为1表示在左下方画矩形，为0表示在右下方画矩形		*/
/*            length用来控制矩形的宽高									*/
/*            rectline用来存放得到的矩形顶点							*/  
/* 返 回 值:  bool 正常执行返回true，否则返回false                 		*/
/* 作    者:  renxue													*/
/* 日    期:  2011 年 03 月 11 日										*/
/************************************************************************/
bool CVectorAlgorithm::getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length)
{//x轴右方向为正方向，y轴上方向为正方向
	if(flag != 0 && flag != 1) return false;
	if(length < 1e-6) return false;

	osg::Vec3 p1, p2, p3, p4;
	float width = length;
	/*p1.setX(p.x());
	p1.setY(p.y() - width/2);
	p1.setZ(0.0f);
	p2.setX(p.x());
	p2.setY(p1.y() - width);
	p2.setZ(0.0f);*/
	p1.set(p.x(), p.y() - width/2.0f, 0.0f);
	p2.set(p.x(), p1.y() - width, 0);

	if(flag)
	{
		/*p3.setX(p2.x() - width);
		p3.setY(p2.y());
		p3.setZ(0.0f);
		p4.setX(p3.x());
		p4.setY(p1.y());
		p4.setZ(0.0f);*/

		p3.set(p2.x() - width, p2.y(), 0);
		p4.set(p3.x(), p1.y(), 0);
	}
	else
	{
		/*p3.setX(p2.x() + width);
		p3.setY(p2.y());
		p3.setZ(0.0f);
		p4.setX(p3.x());
		p4.setY(p1.y());
		p4.setZ(0.0f);*/

		p3.set(p2.x() + width, p2.y(), 0);
		p4.set(p3.x(), p1.y(), 0);
	}
	//rectline<<p1<<p2<<p3<<p4;
	rectline.push_back(p1);
	rectline.push_back(p2);
	rectline.push_back(p3);
	rectline.push_back(p4);
	return true;
}

////根据点p来求解其矩形的折线，flag为1表示在左下方画矩形，flag为0表示在右下方画矩形（主要用于霜冻线算法），该算法与坐标系的方向有关
//bool CVectorAlgorithm::getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length)
//{//x轴右方向为正方向，y轴下方向为正方向
//	osg::Vec3 p1, p2, p3, p4;
//	float width = length;
//	p1.setX(p.x());
//	p1.setY(p.y() + width/2);
//	p1.setZ(0.0f);
//	p2.setX(p.x());
//	p2.setY(p1.y() + width);
//	p2.setZ(0.0f);
//	if(flag)
//	{
//		p3.setX(p2.x() - width);
//		p3.setY(p2.y());
//		p3.setZ(0.0f);
//		p4.setX(p3.x());
//		p4.setY(p1.y());
//		p4.setZ(0.0f);
//	}
//	else
//	{
//		p3.setX(p2.x() + width);
//		p3.setY(p2.y());
//		p3.setZ(0.0f);
//		p4.setX(p3.x());
//		p4.setY(p1.y());
//		p4.setZ(0.0f);
//	}
//	rectline<<p1<<p2<<p3<<p4;
//	return true;
//}