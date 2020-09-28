/*============================================================ 
�ļ�����DrawLineSymbol.h DrawLineSymbol.cpp 
�� ���� CDrawLineSymbol
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ʸ�������ߵ��㷨
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

--------------------------�汾����----------------------------
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
������ڣ�2011��7��27��
============================================================*/
#pragma once
#include <osg/Geometry>
#include "IDPDrawGeometryData.h"
#include "AlgorithmInterface.h"
#include "CardinalSpline.h"
#include "Layer.h"

class CDrawLineSymbol : public IDPDrawGeometryData
{
public:
	CDrawLineSymbol(int nType);
	~CDrawLineSymbol(void);
	int GetType(){return m_nType;};
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );

private:
	osg::Geometry* CreateLine();
	int    m_nType;	
	osg::ref_ptr<osg::Geometry> m_pDisplayLine;	
	bool   m_bFinished, m_bFirst;			//�ж��Ƿ��������
	osg::ref_ptr<osg::Vec3Array>  m_pArray;
	osg::ref_ptr<osg::Vec3Array>  m_InputData, m_OutputData;
	//osg::Geometry* m_pSymbolLine;
	CCardinalSpline *m_pSpLine;
};

