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
�ļ�����MeteElement.h  MeteElement.cpp
�� ���� CMeteElement
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ���
����˵���� ��
============================================================*/
#include "cdataelement_global.h"
#include <QString>

class  CDATAELEMENT_EXPORT CMeteElement
{
public:
	CMeteElement();
	float  x;
	float   y;
	float   z;
	QString t;
	float   value;
};
