
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "CipasTemplateModel.h"
CCipasTemplateModel::CCipasTemplateModel(void):parentItem(0)
{
}


CCipasTemplateModel::~CCipasTemplateModel(void)
{
}

void CCipasTemplateModel::setCurrentDir(const QString& dir)
{
	currentDir=dir;
}

void CCipasTemplateModel::LoadConfigXML(const QString& filename)
{

	QFile templateFile( filename );
	if ( !templateFile.open( QIODevice::ReadOnly ) )
	{
		QMessageBox::warning( 0, tr("信息提示"), tr("不能读取模板库文件") );
		return;
	}


	QDomDocument templateDocument;
	if ( !templateDocument.setContent( &templateFile, false ) )
	{
		QMessageBox::warning( 0, tr( "Read error" ), tr( "Content of templateDB file is not valid" ) );
		return;
	}

	readXML( templateDocument );

}

void CCipasTemplateModel::readXML( const QDomDocument& doc)
{
	QDomNodeList dbNodeList = doc.elementsByTagName( "TemplateDB" );
	if ( dbNodeList.size() < 1 )
	{
		return;
	}
	readXMLGroup( dbNodeList.at( 0 ).toElement(), doc );
}

void CCipasTemplateModel::readXMLGroup( const QDomElement& composerElem, const QDomDocument& doc)
{
	QDomNodeList GroupList = composerElem.elementsByTagName( "Group" );
	for ( int i = 0; i < GroupList.size(); ++i )
	{
		QDomElement currentGroupElem = GroupList.at( i ).toElement();
		QStandardItem* currentItem= new QStandardItem(currentGroupElem.attribute( "name" ));
		currentItem->setIcon(QIcon(":/images/thematicMap/open.png" ));
		//currentItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable 
		//	|Qt::ItemIsDragEnabled
		//	|Qt::ItemIsDropEnabled);
		invisibleRootItem()->setChild( invisibleRootItem()->rowCount(), currentItem );
		parentItem=currentItem;
		readXMLItem(currentGroupElem,doc);
	}
	//未划分到Group中的独立item
	QDomNodeList ItemList = composerElem.elementsByTagName( "Item" );
	for ( int i = 0; i < ItemList.size(); ++i )
	{
		QDomElement currentItemElem = ItemList.at( i ).toElement();
		if ((bool)currentItemElem.attribute("isnode").toUInt())
		{
			//做个判断只有磁盘上存在此文件，才添加到模板管理
			if (true/*QFile::exists(currentDir+currentItemElem.attribute("path"))*/)
			{
				QStandardItem* currentItem= new QStandardItem(currentItemElem.attribute( "name" ));
				currentItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable 
					|Qt::ItemIsDragEnabled);
					//|Qt::ItemIsDropEnabled);
				currentItem->setIcon(QIcon(":/images/thematicMap/mComposer.png" ));
				parentItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable
					|Qt::ItemIsDragEnabled
					|Qt::ItemIsDropEnabled);
				invisibleRootItem()->setChild( invisibleRootItem()->rowCount(),currentItem);
			}
		}
	}

}

void CCipasTemplateModel::readXMLItem(const QDomElement& composerElem, const QDomDocument& doc)
{
	QDomNodeList ItemList = composerElem.elementsByTagName( "Item" );
	for ( int i = 0; i < ItemList.size(); ++i )
	{
		QDomElement currentItemElem = ItemList.at( i ).toElement();
		if (!(bool)currentItemElem.attribute("isnode").toUInt())
		{
			if (true/*QFile::exists(currentDir+currentItemElem.attribute("path"))*/)
			{
				QStandardItem* currentItem= new QStandardItem(currentItemElem.attribute( "name" ));
				currentItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable 
					|Qt::ItemIsDragEnabled);
				//	|Qt::ItemIsDropEnabled);
				currentItem->setIcon(QIcon(":/images/thematicMap/mComposer.png" ));
				parentItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable
					|Qt::ItemIsDragEnabled
					|Qt::ItemIsDropEnabled);
				parentItem->setChild(parentItem->rowCount(),0,currentItem);
			}
		}
	}
}


