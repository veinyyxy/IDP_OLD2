/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� lugsh
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CardinalSpline.h CardinalSpline.cpp 
�� ���� CCardinalSpline
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� ����������ֵ�㷨
����˵���� 

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
ԭ���� ��lugsh
������ڣ�2011�ꣿ�£���
============================================================*/

#include "CardinalSpline.h"
#include <math.h>
#define BOUNDS(pp) {\
	if (pp < 0)\
		pp = 0;\
	else if (pp >= m_ControlPoints->size()-1)\
		pp = m_ControlPoints->size() - 1;\
	}
const  int  CONTROL_NUM = 300 ;

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CCardinalSpline::CCardinalSpline(void) 
	:m_ControlPoints(new osg::Vec3Array),m_delta(0)
{
}

/************************************************************************/
/* �������ܣ� ��������							                        */
/* ����˵��:  NULL														*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CCardinalSpline::~CCardinalSpline(void)
{
}

/************************************************************************/
/* �������ܣ� ���캯��							                        */
/* ����˵��:  NULL														*/
/* �� �� ֵ:  NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
CCardinalSpline::CCardinalSpline(const CCardinalSpline& s)
{
	for (int i = 0; i < (int)s.m_ControlPoints->size(); i++)
		m_ControlPoints->push_back((*s.m_ControlPoints)[i]);
	m_delta = s.m_delta;
}

/************************************************************************/
/* �������ܣ� ���ݴ���ȡ��ƽ����				                        */
/* ����˵��:  NULL														*/
/* �� �� ֵ:  QVector3D   ����ƽ���㷨									*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
osg::Vec3 CCardinalSpline::GetInterpolatedSplinePoint(float t)
{
	int p = (int)(t / m_delta);
	int p0 = p - 1;     BOUNDS(p0);
	int p1 = p;         BOUNDS(p1);
	int p2 = p + 1;     BOUNDS(p2);
	int p3 = p + 2;     BOUNDS(p3);
	float lt = (t - m_delta*(float)p) / m_delta;
	return CCardinalSpline::Eq(lt, (*m_ControlPoints)[p0], (*m_ControlPoints)[p1], (*m_ControlPoints)[p2], (*m_ControlPoints)[p3]);
}

/************************************************************************/
/* �������ܣ� ���ؿ��Ƶ����					                        */
/* ����˵��:  NULL														*/
/* �� �� ֵ:  int   ���ؿ��Ƶ�����										*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
int CCardinalSpline::GetNumPoints()
{
	return m_ControlPoints->size();
}

/************************************************************************/
/* �������ܣ� ȡ��ָ���Ŀ��Ƶ�					                        */
/* ����˵��:  int n   �����Ƶ�˳��										*/
/* �� �� ֵ:  QVector3D  ����ָ�����Ƶ�									*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
osg::Vec3& CCardinalSpline::GetNthPoint(int n)
{
	return (*m_ControlPoints)[n];
}

/************************************************************************/
/* �������ܣ� ���մ���ȡ��ƽ�����ݵ�			                        */
/* ����˵��:  float t       ����������									*/
/*			  QVector3D& p1 ���ָ���1									*/
/*			  QVector3D& p2 ���ָ���2									*/
/*			  QVector3D& p3 ���ָ���3									*/
/*			  QVector3D& p4 ���ָ���4									*/
/* �� �� ֵ:   QVector3D   ��ֵ������   								*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
osg::Vec3 CCardinalSpline::Eq(float t, const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4)
{
	float t2 = t * t;
	float t3 = t2 * t;

	float b1 = .5 * (  -t3 + 2*t2 - t);
	float b2 = .5 * ( 3*t3 - 5*t2 + 2);
	float b3 = .5 * (-3*t3 + 4*t2 + t);
	float b4 = .5 * (   t3 -   t2    );

	return (p1*b1 + p2*b2 + p3*b3 + p4*b4); 
}

/************************************************************************/
/* �������ܣ� ��ӻ�������ƽ�����Ƶ�			                        */
/* ����˵��:   NULL														*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CCardinalSpline::AddSplinePoint(const osg::Vec3& v)
{
	m_ControlPoints->push_back(v);
	m_delta = (float)1.0 / (float)m_ControlPoints->size();
}

/************************************************************************/
/* �������ܣ� �����������������Ƶ�			                        */
/* ����˵��:  NULL														*/
/* �� �� ֵ:   NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CCardinalSpline::ClearSplinePoint()
{
	m_ControlPoints->clear(); 
}

/************************************************************************/
/* �������ܣ� ͨ��������Ƶ����ݣ����ƽ��������                        */
/* ����˵��:  QVector3DArray  inputData  : ���Ƶ�����					*/
/*			  QVector3DArray  inputData  : ���ƽ��������				*/	
/* �� �� ֵ:   NULL														*/
/* ��    ��:  lugsh														*/
/* ��    ��:  2011 �� 04 �� 07 ��										*/
/************************************************************************/
void CCardinalSpline::GetSmoothPoints(osg::Vec3Array &inputData, osg::Vec3Array &OutputData)
{
	int  nSum = 0;
	float Zvalue = 0;
	
	ClearSplinePoint();										//���ԭ�п��Ƶ�����
	if(OutputData.size() > 0 ) OutputData.clear();			//������ƽ��������

	if(inputData.size() <= 0 )  return;

	Zvalue = inputData[0].z();

	//���ÿ��Ƶ�
	for(int i = 0 ; i < inputData.size(); i++)
	{
		AddSplinePoint(inputData[i]);

	}
	osg::Vec3 lastPoint = inputData[inputData.size() -1];

	nSum = inputData.size()*16;

	//ȡ��ƽ����
	for(int j = 0 ; j < nSum; j++)//CONTROL_NUM
	{
		float t = (float)j / (float)nSum;//CONTROL_NUM;
		osg::Vec3 point = GetInterpolatedSplinePoint(t);

		point.set(point.x(), point.y(), Zvalue);
		//point.setZ(Zvalue);
		OutputData.push_back(point);

		float x = point.x() - lastPoint.x();
		float y = point.y() - lastPoint.y();

		if(abs(x) <1 && abs(y) < 1 &&j >=20)
			break;
	}
}

//�ں�����ƽ����
void CCardinalSpline::MergeSmoothLines(osg::Vec3Array &OutPutData, osg::Vec3Array &OutputData2,osg::Vec3Array &OutPutDataSum)
{
	if(OutPutData.size() <= 0 )  return;
	if(OutputData2.size() <= 0 )  return;

	int index_begin = 0;
	int index_end = OutPutData.size()-1;
	int nCount2 = OutputData2.size();
	OutPutDataSum.clear();

	for(int i=OutPutData.size()-1;i>0;i--)
	{
		if (OutPutData[i] == OutputData2[0]) index_end = i;		
	}
	for (int i=0;i<OutPutData.size()-1;i++)
	{
		if (OutPutData[i] == OutputData2[nCount2-1]) index_begin = i;		
	}
	
	//merge data
	for (int i=index_begin;i<index_end;i++)
	{
		OutPutDataSum.push_back(OutPutData[i]);
	}
	for (int i=0;i<nCount2;i++)
	{
		OutPutDataSum.push_back(OutputData2[i]);
	}	
}