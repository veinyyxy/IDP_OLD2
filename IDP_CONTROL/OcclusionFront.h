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
�ļ�����OcclusionFront.h OcclusionFront.cpp 
�� ���� COcclusionFront
�� �ࣺ CAlgorithmInterface
�� �ࣺ �� 
����˵���� �������㷨
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
ԭ���� ����ѩ
������ڣ�2011��03��02��
============================================================*/

#pragma once
#include "AlgorithmInterface.h"
class IDP_CONTROL_EXPORT COcclusionFront :public CAlgorithmInterface
{
public:
	COcclusionFront(void);
	~COcclusionFront(void);
	virtual void SetLineAlgorithmWidth(float width);
	virtual bool SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);//����һ�����߷��̵ĵ㼯����������ߵĵ㼯,width�����������ϵ�Բ��ֱ�����������εı߳���

};

