#pragma once

#include <QStandardItemModel>
#include <QDomDocument>

#include "tmui_global.h"

class TMUI_EXPORT CCipasTemplateModel :public QStandardItemModel
{
public:
	CCipasTemplateModel(void);
	~CCipasTemplateModel(void);
	void LoadConfigXML(const QString& filename);
	void setCurrentDir(const QString& dir);
	//////////////////////////////////////////////////////////////////////////
	//利用xml 方式构建模型
	void readXML( const QDomDocument& doc );
	void readXMLGroup( const QDomElement& composerElem, const QDomDocument& doc);
	void readXMLItem( const QDomElement& composerElem, const QDomDocument& doc);
	//////////////////////////////////////////////////////////////////////////
private:



	QStandardItem* parentItem;
	QString currentDir;

};

