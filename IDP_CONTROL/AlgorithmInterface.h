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
�ļ�����AlgorithmInterface.h AlgorithmInterface.cpp 
�� ���� CObject
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ˫�ߡ���ͷ�����ַ��ߡ�˪�����㷨���㷨�ӿ�
����˵���� �麯��

---------------------------�汾����-------------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��04��19��
============================================================*/

#pragma once
#include "idp_control_global.h"
#include <osg/Array>
#include <vector>

class IDP_CONTROL_EXPORT CAlgorithmInterface
{
public:
	CAlgorithmInterface() : m_pNextAlgorithm(0), m_sideLength(12), m_pInputLine(0), m_pOutputLine(0){;}
	virtual void SetLineAlgorithmWidth(float width)=0;
	virtual bool SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine) = 0;
	inline void SetNextAlgorithm(CAlgorithmInterface* pNextCalgor){m_pNextAlgorithm = pNextCalgor;}
	virtual void SetLineAttributeType(int nType){m_nAttributeType = nType;}
	virtual bool WindAlgorithm(float fSpeed, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine){return false;}
	virtual void ProcessAlgorithm()
	{
		if(m_pNextAlgorithm)
		{
			;
		}
	}
protected:
	float m_width; //˫�ߵĿ��
	float m_sideLength;//���ַ��ߵ������α߳����Բ��ֱ��
    int   m_nAttributeType;
	osg::Vec3Array* m_pInputLine;
	std::vector<osg::ref_ptr<osg::Vec3Array>>* m_pOutputLine;
	CAlgorithmInterface* m_pNextAlgorithm;
};

