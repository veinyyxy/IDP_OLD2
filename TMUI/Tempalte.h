#pragma once

typedef struct _TemplateStruct 
{
	QString Name;			//item��
	QString Path;			//���·����
	QString Description;	//
	QString IsGroup;		//�Ƿ���
	QString IsDefault;		//�Ƿ�ȱʡģ��
	_TemplateStruct():Name(""),Path(""),Description(""),IsGroup("no"),IsDefault("no")
	{
		//ע�⣺ȱʡֵ���޸Ļ�Ӱ����������ṹ��ĵط�
	}
} TemplateStruct;