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
�ļ�����FileType.h
�� ���� ��
�� �ࣺ ��
�� �ࣺ �� 
����˵���� 
����˵���� ��
============================================================*/
#include "cdataelement_global.h"
#include <QString>

class CDATAELEMENT_EXPORT CFileType
{
public:
	CFileType(int key, QString suffixName, QString name,  QString chName, QString libName);
	~CFileType(void);

	int Key() const;
	QString SuffixName() const;
	QString Name() const;
	QString ChineseName();
	QString LibraryName();

private:
	int m_key;  // �ļ�����
	QString m_suffixName; // �ļ���׺
	QString m_name; //�ļ���������
	QString m_chName; //�ļ�������������	
	QString m_libName; // ��̬������
};

