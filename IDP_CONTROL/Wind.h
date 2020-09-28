/*============================================================ 
�ļ����� Wind.h 
��  ���� CWind
��  �ࣺ CAlgorithmInterface
��  �ࣺ �� 
����˵���� ����㷨
����˵���� ��Ե�����ͼ���߿���ͼ�Ļ��ƣ������˹��ֻ�ķ��
---------------------------�汾����---------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� �� renxue
������ڣ�2011��08��23��
============================================================*/

#pragma once
#include "AlgorithmInterface.h"

class IDP_CONTROL_EXPORT CWind :public CAlgorithmInterface
{
public:
	CWind(int iWidth);
	//CWind(int nType);
	~CWind(void);
	virtual void SetLineAlgorithmWidth(float width);
	virtual bool SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);
	virtual bool WindAlgorithm(float fSpeed, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);//P�Ƿ���λ�ã�fAngle�Ƿ���fSpeed�Ƿ���

private:
	int  WindScaleBasedSpeed(float fSpeed);                  //���ݷ��ٷ��ط�ļ���
	bool CaculateBarb(osg::Vec3Array &array, int nType, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine);//���ݷ缶nType�ͷ�ˣ�array����������ϵĵ㣻
	int   m_nRatio;
};

