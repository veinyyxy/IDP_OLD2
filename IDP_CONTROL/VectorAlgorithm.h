/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ��ѩ
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

-------------------------------------�汾����------------------------------------------------ 
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��04��15��
============================================================*/ 

#pragma once
#include <osg/Vec3>
#include <osg/Array>

class CVectorAlgorithm
{
public:
	static void getLineFormula(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &formula);//����������ֱ�߷���(����ax+by+c=0),bֻ������ȡֵ0��-1��
	static void LineFormula(osg::Vec3 &P, float fDirection, osg::Vec3 &formula);//���ݵ�ͼн���ֱ�߷��̣�
	static double distancePointFToPointF(osg::Vec3 &p1, osg::Vec3 &p2);//������֮��ľ���
	static bool getEquidistanceLines(osg::Vec3 &line, float width, osg::Vec3Array &lines);//������֪��ֱ�߷��̣������width�ĵȾ��������ֱ�߷���
	static bool getTwoAngularBisectrixFormula(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3Array &angularlines);//��������ߵ�������ƽ����
	static bool isLinesIntersected(osg::Vec3 &line1, osg::Vec3 &line2);//�ж�line1��line2�Ƿ��ཻ���Ƿ���true�����򷵻�false
	static void getPointofIntersection(osg::Vec3 &line1, osg::Vec3 &line2, osg::Vec3 &P);//�������ߵĽ���
	static double distancePointToLine(osg::Vec3 &p, osg::Vec3 &line);//��㵽ֱ�ߵľ���
	static bool getNormalFormula(osg::Vec3 &p, osg::Vec3 &line, osg::Vec3 &normaline);//���p���line�ķ���
	static void getMidpoint(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &M);//���߶�p1p2���е�

	static bool getPointDistantToP(osg::Vec3 &line, osg::Vec3 &p, float width, osg::Vec3Array &Qs);//�����ֱ��line�ϼ��㵽p��ľ���Ϊwidth�����������
	static bool isABCDSameDirection(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &C, osg::Vec3 &D);//��A��B��C��D��һ��ֱ���ϣ�����AB������CD������ͬ���򷵻�true�����򷵻�false
	static bool getPointInVectorABToP(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &p, float width, osg::Vec3 &Q);//������AB���������ϣ�������p��Ϊwidth�ĵ�Q��
	static bool rightHandRule(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P);//�ж�p��ʸ��AB�Ƿ��������ֶ������㷵��true�����򷵻�false//δ����
	//static bool getPoint(osg::Vec3 &A, osg::Vec3 &B, float width, osg::Vec3 &Q);//��ȡ�߶�AB�д����Ͼ���AB���е�Ϊwidth�ĵ�Q���õ�������AB�������ֶ���
	static bool getPointToPOnRightHand(osg::Vec3 &A, osg::Vec3 &B, osg::Vec3 &P, float width, int flag, osg::Vec3 &Q);//P���߶�AB�ϵ�����㣬��ȡQP��QP��ֱAB����QP�ĳ���Ϊwidth�����flag=1��Q��������AB�������ֶ������flag=0��Q��������AB���������ֶ���
	static float getAngleOfVectorAB(osg::Vec3 &A, osg::Vec3 &B);//��ԭ��OΪ��㣬������OA��OB֮��ļн�,�û��ȱ�ʾ��
	static float getAngleToX(float angle, osg::Vec3 &P);//���ҷ���Ϊx����������Ϊy�������������ϵ�£���֪P�����꣬��OP��x��������ļнǣ���OP�Ľ�

	static int   direction(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &p);//�ж�p��ab�����λ��
	static bool  isPointInTriangle(osg::Vec3 &a, osg::Vec3 &b, osg::Vec3 &c, osg::Vec3 &p);//�ж�p�Ƿ���abc��������ɵ�������������
	static void  getPolyLineFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula);//����һ�����ߵ㼯������߶ε�ֱ�߷���(����ax+by+c=0),bֻ������ȡֵ0��-1��
	static void  getAngularBisectrixFormula(osg::Vec3Array &polygonf, osg::Vec3Array &polylineformula, osg::Vec3Array &angularlines);//���һ�������ϵ����н�ƽ���ߵķ���
	static void  getEquidistanceLines(osg::Vec3Array &polylineformula, float width, osg::Vec3Array &equidistancelineformula);//������֪�����߷��̣������width�ĵȾ������߷���
	static float distanceOfPolyLine(osg::Vec3Array &polygonf);//�����ߵĳ��ȣ��������߶εĳ��Ⱥ�
	static bool  getPointOnPolyline(osg::Vec3 &Q, int &nPos, osg::Vec3Array &polygonf, osg::Vec3 &P, float width);
	//20110316��д
	static bool getRectLine(osg::Vec3 &p, int flag, osg::Vec3Array &rectline, float length);//���ݵ�p���������ε����ߣ�flagΪ1��ʾ�����·������Σ�flagΪ0��ʾ�����·������Σ���Ҫ����˪�����㷨��
};

