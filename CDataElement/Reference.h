#pragma once
/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CReference.h  CReference.cpp
�� ���� CReference
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ʱ�䷶Χ
����˵���� ��
============================================================*/
#include "cdataelement_global.h"

class CDATAELEMENT_EXPORT CReference
{
public:
	CReference();
	CReference(const CReference& ref);
	CReference& operator = (const CReference& ref);
	~CReference();
	bool Reattach(const	CReference& ref);
	bool Only();
	bool MakeOnly();
	const int GetRefCount();


private:
	int *pCount;
};

