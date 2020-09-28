/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����VectorAlgorithm.h VectorAlgorithm.cpp 
�� ���� CVectorAlgorithm 
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ʵ�ֶ�˫�ߡ���ͷ�����ַ��ߡ�˪�����㷨�Ļ�������
����˵���� osg::Vec3��������ǲ���float��

-------------------------------------�汾����-----------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��renxue
������ڣ�2011��04��15��
============================================================*/ 

#include "VectorAlgorithm.h"
#include <math.h>

#define PI 3.1415926

/************************************************************************/
/* �������ܣ� ����������ֱ�߷���(����ax+by+c=0)          				*/
/* ����˵��:  p1��p2Ϊ��֪�������  									*/
/*            formula����õ�����ax+by+c=0��ʽ�ķ��̵�ϵ��  			*/
/* �� �� ֵ:  null                              						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
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
/* �������ܣ� ����������ֱ�߷���(����ax+by+c=0)          				*/
/* ����˵��:  PΪֱ���ϵĵ㣬fDirection��ֱ����x��������ļнǣ�0~360��	*/
/*            formula����õ�����ax+by+c=0��ʽ�ķ��̵�ϵ��  			*/
/* �� �� ֵ:  null                              						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
void CVectorAlgorithm::LineFormula(osg::Vec3 &P, float fDirection, osg::Vec3 &formula)
{
	//�����쳣�ĽǶ�
	while(abs(fDirection-0.0)<1e-5)
	{
		fDirection += 360.0;
	}
	while(abs(fDirection-360.0)<1e-5)
	{
		fDirection -= 360.0;
	}

	float a, b, c;
	if(abs(fDirection-0.0)<1e-5 || abs(fDirection-180.0)<1e-5 || abs(fDirection-360.0)<1e-5)//0�ȡ�180�ȡ�360�ȷ���
	{
		a = 0;
		b = -1;
		c = P.y();
	}
	else if(abs(fDirection-90.0)<1e-5 || abs(fDirection-270.0)<1e-5)//90�Ȼ�270�ȷ���
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
/* �������ܣ� ������֮��ľ���          								*/
/* ����˵��:  p1��p2Ϊ��֪�������  									*/
/* �� �� ֵ:  float p1��p2֮��ľ���                              		*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
/************************************************************************/
double CVectorAlgorithm::distancePointFToPointF(osg::Vec3 &p1, osg::Vec3 &p2)
{
	double distance = 0.0;
	distance = sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()));
	return distance;
}

/************************************************************************/
/* �������ܣ� ��������ߵĽ�ƽ����         								*/
/* ����˵��:  line1��line2Ϊ�����ֱ�߷���								*/
/* �� �� ֵ:  boolƽ���߻�ͬһ��ֱ�߷���false���������з���true         */
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::getTwoAngularBisectrixFormula(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3Array &angularlines)
{
	//ƽ����orͬһ��ֱ��
	if(abs(line1.x()-line2.x())<1e-6 && abs(line1.y()-line2.y())<1e-6)
	{
		return false;
	}

	float a1, b1, c1, a2, b2, c2;
	float temp1, temp2;
	//��ֱ���ཻ
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
/* �������ܣ� ��֪ĳ��ֱ�߷��̣������width�ĵȾ���ֱ�߷���         	*/
/* ����˵��:  lineΪ�����ֱ�߷���								      	*/
/*            lines������������Ⱦ���ֱ�߷���					      	*/
/* �� �� ֵ:  bool �������з���true�����򷵻�false        			    */
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
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
/* �������ܣ� �ж�line1��line2�Ƿ��ཻ  								*/
/* ����˵��:  line1��line2Ϊ��ֱ֪�ߵķ���ϵ��  						*/
/* �� �� ֵ:  bool true��ʾ�������ཻ��false��ʾ���ཻ                  */
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::isLinesIntersected(osg::Vec3 &line1, osg::Vec3 &line2)
{
	if(abs(line1.x()-line2.x())<1e-5 && abs(line1.y()-line2.y())<1e-5) return false;
	else return true;
}

/************************************************************************/
/* �������ܣ� ��ֱ��line1��line2�Ľ���  								*/
/* ����˵��:  line1��line2Ϊ��ֱ֪�ߵķ���ϵ��  						*/
/*            p�Ƿ��صĽ���                                             */
/* �� �� ֵ:  void                                                      */
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
void CVectorAlgorithm::getPointofIntersection(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3 &P)
{	
	double a, b;
	a = (line2.z()*line1.y() - line1.z()*line2.y())/(line1.x()*line2.y() - line2.x()*line1.y());
	b = (line2.z()*line1.x() - line1.z()*line2.x())/(line1.y()*line2.x() - line2.y()*line1.x());

	P.set(a, b, 0.0);
}

/************************************************************************/
/* �������ܣ� ���p���line�ķ���,��pֻʹ��xy����  						*/
/* ����˵��:  line1Ϊ����ֱ�ߵķ���ϵ����pΪ��֪��  					*/
/*            normaline�Ƿ��ط��ߵķ���                                 */
/* �� �� ֵ:  bool�������з���true�����򷵻�false                       */
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
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
/* �������ܣ� ���߶�p1p2���е�          								*/
/* ����˵��:  M����õ��е�  											*/
/*            p1��p2Ϊ��֪�������			        					*/
/* �� �� ֵ:  null                              						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
void  CVectorAlgorithm::getMidpoint(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &M)
{
	/*M.setX((p1.x() + p2.x())/2.0);
	M.setY((p1.y() + p2.y())/2.0);*/

	M.set((p1.x() + p2.x())/2.0, (p1.y() + p2.y())/2.0, 0);
}

/************************************************************************/
/* �������ܣ� ��㵽ֱ�ߵľ���          								*/
/* ����˵��:  line��ֱ�߷���											*/
/*            pΪ��֪�������			            					*/
/* �� �� ֵ:  double p��line�ߵľ���             						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
double CVectorAlgorithm::distancePointToLine(osg::Vec3 &p, osg::Vec3 &line)
{
	double temp = hypot(line.x(), line.y());
	temp = (line.x()*p.x() + line.y()*p.y() + line.z())/temp;
	return abs(temp);
}


/************************************************************************/
/* �������ܣ� ��ֱ��line�ϼ��㵽p��ľ���Ϊwidth�����������Qs          */
/* ����˵��:  line��ֱ�߷���											*/
/*            QsΪ�õ���������ά�������								*/
/* �� �� ֵ:  bool ����ִ�з���true���򷵻�false						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::getPointDistantToP(osg::Vec3 &line, osg::Vec3 &p, float width, osg::Vec3Array &Qs)
{
	if(abs(line.x())<1e-6 && abs(line.y())<1e-6) return false;//�жϵ��Ƿ������ϣ��ߵķ����Ƿ���ȷ
	
	float ta1,ta2,tb1,tb2;	
	if(abs(line.y()) < 1e-6)//�����ֱ��
	{		
		ta1 = ta2 = p.x();
		tb1 = p.y()+width;
		tb2 = p.y()-width;		
	}
	else if(abs(line.x()) < 1e-6)//�����ֱ��
	{		
		ta1 = p.x()+width;
		ta2 = p.x()-width;
		tb1 = tb2 = p.y();
	}
	else
	{
		//��һԪ���η��̼��������
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
/* �������ܣ� �ж�����AB������CD�������ͬ���򷵻�true�����򷵻�false   */
/* ����˵��:  A��B��C��D���������ά��							    	*/
/* �� �� ֵ:  bool ����AB��CD������ͬ����true�����򷵻�false			*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
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
/* �������ܣ� ������AB���������ϣ�������p��Ϊwidth�ĵ�                */
/* ����˵��:  A��B��P���������ά��										*/
/*            Q�ǵõ�����ά������ֻ꣬��һ��							*/
/* �� �� ֵ:  bool ����ִ�з���true���򷵻�false						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::getPointInVectorABToP(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &p, float width, osg::Vec3 &Q)
{
	osg::Vec3 formula;
	getLineFormula(A, B, formula);

	//�����ֱ���ϼ��㵽ĳ��ľ���Ϊwidth�ĵ������
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
/* �������ܣ� p��������AB��ֱ���ϣ��ж�p��ʸ��AB�Ƿ��������ֶ���        */
/* ����˵��:  A��B��P���������ά��										*/
/* �� �� ֵ:  bool �������ֶ�����true,���򷵻�false					*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
bool  CVectorAlgorithm::rightHandRule(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P)
{
	osg::Vec3  tB, tP;
	//tB.setX(B.x() - A.x());
	////tB.setY(A.y() - B.y());//���ҷ���Ϊx����������Ϊy�������������ϵ�µ��ж�
	//tB.setY(B.y() - A.y());//���ҷ���Ϊx����������Ϊy�������������ϵ�µ��ж�

	tB.set(B.x() - A.x(), B.y() - A.y(), 0);

	//tP.setX(P.x() - A.x());
	////tP.setY(A.y() - P.y());////���ҷ���Ϊx����������Ϊy�������������ϵ�µ��ж�
	//tP.setY(P.y() - A.y());////���ҷ���Ϊx����������Ϊy�������������ϵ�µ��ж�

	tP.set(P.x() - A.x(), P.y() - A.y(), 0);
	osg::Vec3 tO(1.0, 0.0, 0.0);

	float angleB, angleP;
	angleB = getAngleOfVectorAB(tO, tB);	
	angleB = getAngleToX(angleB, tB);	
	angleP = getAngleOfVectorAB(tO, tP);	
	angleP = getAngleToX(angleP, tP);

	if(angleB>=0.0 && angleB<270.0) //20110420�޸ĵ�
	{
		if((angleP>angleB) && (angleP-angleB)<PI) return true;//��һ������������
		else return false;
	}
	if((angleB>270.0 && angleB<360.0) && ((angleB>angleP) && (angleB-angleP)<PI)) return false;	//��������
	else return true;
}

/************************************************************************/
/* �������ܣ� //P���߶�AB�ϵ�����㣬��ȡQP 	                        */
/* ����˵��:  A��B��P����֪��											*/
/*            Q�ǵõ�����ά�㣬QP��ֱAB����QP�ĳ���Ϊwidth              */
/*            flag=1��ʾQ�������������ֶ���Ϊ0��ʾ�����㣻            */
/* �� �� ֵ:  bool                  									*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
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
/* �������ܣ� ��ԭ��OΪ��㣬������OA��OB֮��ļн�,�û��ȱ�ʾ          */
/* ����˵��:  A��B�������	                    						*/
/* �� �� ֵ:  float ����OA��OB֮��ļн�,�û��ȱ�ʾ                   	*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 03 �� 11 ��										*/
/************************************************************************/
float  CVectorAlgorithm::getAngleOfVectorAB(osg::Vec3 &A, osg::Vec3 &B)
{
	float cosine;
	cosine = (A.x()*B.x() + A.y()*B.y())/(hypot(A.x(),A.y()) * hypot(B.x(),B.y()));
	
	if(abs(1.0-cosine) < 1e-6) return 0.0;//0��ֵ
	else if(abs(cosine+1.0) < 1e-6) return 180.0;//360��ֵ(416��Ϊ180)
	else return acos(cosine);
}

/************************************************************************/
/* �������ܣ� ��֪P�������op��x��ļнǣ���OP�Ľ�                      */
/* ����˵��:  ����ϵ���ҷ���Ϊx����������Ϊy��������  				*/
/*            angle��OP��x��������ļн�    							*/
/* �� �� ֵ:  float OP��x��������Ľ�                               	*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 03 �� 11 ��										*/
/************************************************************************/
float CVectorAlgorithm::getAngleToX(float angle, osg::Vec3 &P)
{
	if(angle > 180.0)	return -1.0;
	
	if(abs(angle)<1e-6) return angle;//�н�Ϊ��
	if(P.y()>0) return angle;//��һ��������
	else return (360.0-angle);//������������
}

/************************************************************************/
/* �������ܣ� ��xy��ȷ����ƽ���ϣ��жϵ�p��ab�����λ��                 */
/* ����˵��:  A��B�������	    										*/
/* �� �� ֵ:  int                      									*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 03 �� 11 ��										*/
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
/* �������ܣ� ��xy��ȷ����ƽ���ϣ��ж�p�Ƿ��������������� 	            */
/* ����˵��:  A��B��C�������εĶ���	��P�������							*/
/* �� �� ֵ:  bool ���������ڷ���true�����򷵻�false                 	*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::isPointInTriangle(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &c, osg::Vec3 &p)
{
	int t= direction(a,b,p)+ direction(b,c,p) + direction(c,a,p);

	if (t>=1 && t<=4) return true;
	if (t==8 || t==12) return true;
	return false;    
}

/************************************************************************/
/* �������ܣ� ����һ�����ߵ㼯������߶ε�ֱ�߷���(����ax+by+c=0)       */
/* ����˵��:  polygonf����������ߵ㼯									*/
/*            polylineformula����������߶η��̼�						*/
/* �� �� ֵ:  Null                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
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
/* �������ܣ� ������֪�����߷��̣������width�ĵȾ������߷���           */
/* ����˵��:  polygonf����������ߵ㼯									*/
/*            equidistancelineformula������ĵȾ������߶η���			*/
/* �� �� ֵ:  Null                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
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
/* �������ܣ� ���һ�������ϵ����мнǽ�ƽ���ߵķ���          		    */
/* ����˵��:  polygonf����������ߵ㼯									*/
/*            polylineformula������ĵȾ������߶η���					*/
/*            angularlines��������к���Ľ�ƽ���߷���					*/
/* �� �� ֵ:  Null                 										*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
/************************************************************************/
void CVectorAlgorithm::getAngularBisectrixFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula, osg::Vec3Array &angularlines)
{ 
	int i, size = polylineformula.size();	
	osg::Vec3  point[2], tp[2];
	osg::Vec3  templine;
	for(i = 0; i < size - 1; i++)
	{
		//��������ֱ��֮��Ľ�ƽ����
		osg::ref_ptr<osg::Vec3Array> angularformula = new osg::Vec3Array;

		if(getTwoAngularBisectrixFormula(polylineformula[i],polylineformula[i+1],*angularformula))		
		{
			//ɸѡ���ʵĽ�ƽ����
			getLineFormula(polygonf[i], polygonf[i+2], templine);//����������������εĵ�������
			if(isLinesIntersected(templine, (*angularformula)[0]) && isLinesIntersected(templine, (*angularformula)[1]))
			{
				//�����εĵ������ߺ�������ƽ���߶��н���
				getPointofIntersection(templine, (*angularformula)[0], tp[0]);
				getPointofIntersection(templine, (*angularformula)[1], tp[1]);
				getMidpoint(tp[0], polygonf[i+1], point[0]);
				getMidpoint(tp[1], polygonf[i+1], point[1]);

				//�ж��е��Ƿ���������������
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
/* �������ܣ� ���һ�������ϳ���          		    					*/
/* ����˵��:  polygonf����������ߵ㼯									*/
/* �� �� ֵ:  float ���ߵĳ���                 							*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 11 ��										*/
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
 ����˵����������polygonf����������������P��Ϊwidth�ĵ�Q
 ��	   ����polygonf�����ߵ�3D�������飻
           P��polygonf��һ��3D����㣻
		   Q�������3D�㣻
		   nPos��polygonf�������ϣ���Q֮ǰpolygonf�ϵ�����ĵ㣻
 ��    �ߣ�renxue
 ������ڣ�2010��4��11��
************************************************************************/
bool CVectorAlgorithm::getPointOnPolyline(osg::Vec3 &Q, int &nPos, osg::Vec3Array &polygonf, osg::Vec3 &P, float width)
{	
	int size = polygonf.size();
	int i = 0;
	while((P!=polygonf[i]) && (i<size))
	{
		i++;
	}
	if (i == size) return false;//P�������ߵĵ㼯��

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
/* �������ܣ� ���ݵ�p���������ε�����,��Ҫ����˪�����㷨              */
/* ����˵��:  ���㷨���õ�����ϵ��x���ҷ���Ϊ����y���Ϸ���Ϊ��          */
/*            flagflagΪ1��ʾ�����·������Σ�Ϊ0��ʾ�����·�������		*/
/*            length�������ƾ��εĿ��									*/
/*            rectline������ŵõ��ľ��ζ���							*/  
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 03 �� 11 ��										*/
/************************************************************************/
bool CVectorAlgorithm::getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length)
{//x���ҷ���Ϊ������y���Ϸ���Ϊ������
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

////���ݵ�p���������ε����ߣ�flagΪ1��ʾ�����·������Σ�flagΪ0��ʾ�����·������Σ���Ҫ����˪�����㷨�������㷨������ϵ�ķ����й�
//bool CVectorAlgorithm::getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length)
//{//x���ҷ���Ϊ������y���·���Ϊ������
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