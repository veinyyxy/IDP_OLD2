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
	osg::Vec3 GetInterpolatedSplinePoint(float t);      //ȡ��ƽ�������ߵ�
	int GetNumPoints();									//�õ�ָ�����߿��Ƶ������
	osg::Vec3& GetNthPoint(int n);						//����ָ�����߶�Ӧ�Ŀ��Ƶ�
	void ClearSplinePoint();							//������ߵ����п��Ƶ�
	void AddSplinePoint(const osg::Vec3& v);			//������ָ��2D\3D���߿��Ƶ�����
	osg::ref_ptr<osg::Vec3Array> m_ControlPoints;
	float m_delta;
};

