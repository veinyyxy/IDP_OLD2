#pragma once

typedef struct _TemplateStruct 
{
	QString Name;			//item名
	QString Path;			//相对路径名
	QString Description;	//
	QString IsGroup;		//是否组
	QString IsDefault;		//是否缺省模板
	_TemplateStruct():Name(""),Path(""),Description(""),IsGroup("no"),IsDefault("no")
	{
		//注意：缺省值的修改会影响引用这个结构体的地方
	}
} TemplateStruct;