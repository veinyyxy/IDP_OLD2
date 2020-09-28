
#include <QDomDocument>
#include <QDomElement>

#include "CipasComposerLegendItem.h"

CCipasComposerLegendItem::CCipasComposerLegendItem(): QStandardItem()
{
}

CCipasComposerLegendItem::CCipasComposerLegendItem(const QString& text ): QStandardItem( text )
{
}

CCipasComposerLegendItem::CCipasComposerLegendItem(const QIcon& icon, const QString& text ): QStandardItem( icon, text )
{
}


CCipasComposerLegendItem::~CCipasComposerLegendItem(void)
{
}

void CCipasComposerLegendItem::writeXMLChildren( QDomElement& elem, QDomDocument& doc ) const
{
	int numRows = rowCount();
	CCipasComposerLegendItem* currentItem = 0;
	for ( int i = 0; i < numRows; ++i )
	{
		currentItem = dynamic_cast<CCipasComposerLegendItem*>( child( i, 0 ) );
		if ( currentItem )
		{
			currentItem->writeXML( elem, doc );
		}
	}
}

////////////////////CCipasComposerRasterSymbolItem

CCipasComposerRasterSymbolItem::CCipasComposerRasterSymbolItem(): CCipasComposerLegendItem()
{
}

CCipasComposerRasterSymbolItem::CCipasComposerRasterSymbolItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerRasterSymbolItem::CCipasComposerRasterSymbolItem( const QIcon& icon, const QString& text ): CCipasComposerLegendItem( icon, text )
{
}

CCipasComposerRasterSymbolItem::~CCipasComposerRasterSymbolItem()
{
}

QStandardItem* CCipasComposerRasterSymbolItem::clone() const
{
	CCipasComposerRasterSymbolItem* cloneItem  = new CCipasComposerRasterSymbolItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerRasterSymbolItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement rasterClassElem = doc.createElement( "RasterClassificationItem" );
	rasterClassElem.setAttribute( "layerId", mLayerID );
	rasterClassElem.setAttribute( "text", text() );
	elem.appendChild( rasterClassElem );
}

void CCipasComposerRasterSymbolItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	setLayerID( itemElem.attribute( "layerId", "" ) );

// 	QgsRasterLayer* rLayer = qobject_cast<QgsRasterLayer*>( QgsMapLayerRegistry::instance()->mapLayer( mLayerID ) );
// 	if ( rLayer && xServerAvailable )
// 	{
// 		setIcon( QIcon( rLayer->legendAsPixmap( true ) ) );
// 	}
}


////////////////////CCipasComposerLayerItem

CCipasComposerLayerItem::CCipasComposerLayerItem(): CCipasComposerLegendItem()
{
}

CCipasComposerLayerItem::CCipasComposerLayerItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerLayerItem::~CCipasComposerLayerItem()
{
}

QStandardItem* CCipasComposerLayerItem::clone() const
{
	CCipasComposerLayerItem* cloneItem  = new CCipasComposerLayerItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerLayerItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement layerItemElem = doc.createElement( "LayerItem" );
	layerItemElem.setAttribute( "layerId", mLayerID );
	//QString colors=QString::number(mColor.red())+":"+QString::number(mColor.green())+":"+QString::number(mColor.blue());
	//layerItemElem.setAttribute("colors",colors);
	layerItemElem.setAttribute( "text", text() );
	writeXMLChildren( layerItemElem, doc );
	elem.appendChild( layerItemElem );
}

void CCipasComposerLayerItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	setLayerID( itemElem.attribute( "layerId", "" ) );

	//now call readXML for all the child items
	QDomNodeList childList = itemElem.childNodes();
	QDomNode currentNode;
	QDomElement currentElem;
	CCipasComposerLegendItem* currentChildItem = 0;

	int nChildItems = childList.count();
	for ( int i = 0; i < nChildItems; ++i )
	{
		currentNode = childList.at( i );
		if ( !currentNode.isElement() )
		{
			continue;
		}

		currentElem = currentNode.toElement();
		QString elemTag = currentElem.tagName();
		if ( elemTag == "ShadedLayerItem" )
		{
			currentChildItem = new CCipasComposerShadedLayerItem();
		}
		else if ( elemTag == "RainLayerItem" )
		{
			currentChildItem = new CCipasComposerRainLayerItem();
		}
		else if ( elemTag == "TemperatureLayerItem" )
		{
			currentChildItem = new CCipasComposerTemperatureLayerItem();
		}
		else
		{
			continue; //unsupported child type//StationLayerItem
		}
		currentChildItem->readXML( currentElem, xServerAvailable );
		appendRow( currentChildItem );
	}
}

////////////////////CCipasComposerGroupItem

CCipasComposerGroupItem::CCipasComposerGroupItem(): CCipasComposerLegendItem()
{
}

CCipasComposerGroupItem::CCipasComposerGroupItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerGroupItem::~CCipasComposerGroupItem()
{
}

QStandardItem* CCipasComposerGroupItem::clone() const
{
	CCipasComposerGroupItem* cloneItem = new CCipasComposerGroupItem();
	*cloneItem = *this;
	return cloneItem;
}

void CCipasComposerGroupItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement layerGroupElem = doc.createElement( "GroupItem" );
	layerGroupElem.setAttribute( "text", text() );
	writeXMLChildren( layerGroupElem, doc );
	elem.appendChild( layerGroupElem );
}

void CCipasComposerGroupItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );

	//now call readXML for all the child items
	QDomNodeList childList = itemElem.childNodes();
	QDomNode currentNode;
	QDomElement currentElem;
	CCipasComposerLegendItem* currentChildItem = 0;

	int nChildItems = childList.count();
	for ( int i = 0; i < nChildItems; ++i )
	{
		currentNode = childList.at( i );
		if ( !currentNode.isElement() )
		{
			continue;
		}

		currentElem = currentNode.toElement();
		QString elemTag = currentElem.tagName();

		if ( elemTag == "GroupItem" )
		{
			currentChildItem = new CCipasComposerGroupItem();
		}
		else if ( elemTag == "LayerItem" )
		{
			currentChildItem = new CCipasComposerLayerItem();
		}
		else
		{
			continue; //unsupported child item type
		}
		currentChildItem->readXML( currentElem, xServerAvailable );
		appendRow( currentChildItem );
	}
}



//色版图层项目


CCipasComposerShadedLayerItem::CCipasComposerShadedLayerItem(): CCipasComposerLegendItem()
{
}

CCipasComposerShadedLayerItem::CCipasComposerShadedLayerItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerShadedLayerItem::~CCipasComposerShadedLayerItem()
{
}

QStandardItem* CCipasComposerShadedLayerItem::clone() const
{
	CCipasComposerShadedLayerItem* cloneItem  = new CCipasComposerShadedLayerItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerShadedLayerItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	//QDomElement layerItemElem = doc.createElement( "ShadedLayerItem" );
	//layerItemElem.setAttribute( "layerId", mLayerID );
	//layerItemElem.setAttribute( "text", text() );
	//writeXMLChildren( layerItemElem, doc );
	//elem.appendChild( layerItemElem );

	QDomElement layerItemElem = doc.createElement( "ShadedLayerItem" );
	//layerItemElem.setAttribute( "layerId", mLayerID );
	QString colors=QString::number(mColor.red())+":"+QString::number(mColor.green())+":"+QString::number(mColor.blue());
	layerItemElem.setAttribute("colors",colors);
	layerItemElem.setAttribute( "text", text() );
	writeXMLChildren( layerItemElem, doc );
	elem.appendChild( layerItemElem );

}

void CCipasComposerShadedLayerItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	//if ( itemElem.isNull() )
	//{
	//	return;
	//}
	//setText( itemElem.attribute( "text", "" ) );
	//setLayerID( itemElem.attribute( "layerId", "" ) );
	//QString itemtex=itemElem.attribute( "text", "" );
	////now call readXML for all the child items
	//QDomNodeList childList = itemElem.childNodes();
	//QDomNode currentNode;
	//QDomElement currentElem;
	//CCipasComposerLegendItem* currentChildItem = 0;

	//int nChildItems = childList.count();
	//for ( int i = 0; i < nChildItems; ++i )
	//{
	//	currentNode = childList.at( i );
	//	if ( !currentNode.isElement() )
	//	{
	//		continue;
	//	}

	//	currentElem = currentNode.toElement();
	//	QString elemTag = currentElem.tagName();
	//	if ( elemTag == "VectorClassificationItem" )
	//	{
	//		//	currentChildItem = new QgsComposerSymbolItem();
	//	}
	//	else if ( elemTag == "VectorClassificationItemNg" )
	//	{
	//		//		currentChildItem = new QgsComposerSymbolV2Item();
	//	}
	//	else if ( elemTag == "RasterClassificationItem" )
	//	{
	//		currentChildItem = new CCipasComposerRasterSymbolItem();
	//	}
	//	else
	//	{
	//		continue; //unsupported child type
	//	}
	//	currentChildItem->readXML( currentElem, xServerAvailable );
	//	appendRow( currentChildItem );
	//}

	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	QString inColors=itemElem.attribute("colors","0:0:0");
	QStringList inColorLs=inColors.split(":");
	QColor inColor(inColorLs.at(0).toUInt(),inColorLs.at(1).toUInt(),inColorLs.at(2).toUInt());
	setColor(inColor);

}

//站点图层


CCipasComposerStationLayerItem::CCipasComposerStationLayerItem(): CCipasComposerLegendItem()
{
}

CCipasComposerStationLayerItem::CCipasComposerStationLayerItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerStationLayerItem::~CCipasComposerStationLayerItem()
{
}

QStandardItem* CCipasComposerStationLayerItem::clone() const
{
	CCipasComposerStationLayerItem* cloneItem  = new CCipasComposerStationLayerItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerStationLayerItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement layerItemElem = doc.createElement( "StationLayerItem" );
	layerItemElem.setAttribute( "layerId", mLayerID );
	layerItemElem.setAttribute( "text", text() );
	writeXMLChildren( layerItemElem, doc );
	elem.appendChild( layerItemElem );
}

void CCipasComposerStationLayerItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	setLayerID( itemElem.attribute( "layerId", "" ) );

	//now call readXML for all the child items
	QDomNodeList childList = itemElem.childNodes();
	QDomNode currentNode;
	QDomElement currentElem;
	CCipasComposerLegendItem* currentChildItem = 0;

	int nChildItems = childList.count();
	for ( int i = 0; i < nChildItems; ++i )
	{
		currentNode = childList.at( i );
		if ( !currentNode.isElement() )
		{
			continue;
		}

		currentElem = currentNode.toElement();
		QString elemTag = currentElem.tagName();
		if ( elemTag == "VectorClassificationItem" )
		{
			//	currentChildItem = new QgsComposerSymbolItem();
		}
		else if ( elemTag == "VectorClassificationItemNg" )
		{
			//		currentChildItem = new QgsComposerSymbolV2Item();
		}
		else if ( elemTag == "RasterClassificationItem" )
		{
			currentChildItem = new CCipasComposerRasterSymbolItem();
		}
		else
		{
			continue; //unsupported child type
		}
		currentChildItem->readXML( currentElem, xServerAvailable );
		appendRow( currentChildItem );
	}
}

//温度图层项目


CCipasComposerTemperatureLayerItem::CCipasComposerTemperatureLayerItem(): CCipasComposerLegendItem()
{
}

CCipasComposerTemperatureLayerItem::CCipasComposerTemperatureLayerItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerTemperatureLayerItem::~CCipasComposerTemperatureLayerItem()
{
}

QStandardItem* CCipasComposerTemperatureLayerItem::clone() const
{
	CCipasComposerTemperatureLayerItem* cloneItem  = new CCipasComposerTemperatureLayerItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerTemperatureLayerItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement layerItemElem = doc.createElement( "RainLayerItem" );
	//layerItemElem.setAttribute( "layerId", mLayerID );
	QString colors=QString::number(mColor.red())+":"+QString::number(mColor.green())+":"+QString::number(mColor.blue());
	layerItemElem.setAttribute("colors",colors);
	layerItemElem.setAttribute( "text", text() );
	writeXMLChildren( layerItemElem, doc );
	elem.appendChild( layerItemElem );
}

void CCipasComposerTemperatureLayerItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	QString inColors=itemElem.attribute("colors","0:0:0");
	QStringList inColorLs=inColors.split(":");
	QColor inColor(inColorLs.at(0).toUInt(),inColorLs.at(1).toUInt(),inColorLs.at(2).toUInt());
	setColor(inColor);
}

//Rain图层项目


CCipasComposerRainLayerItem::CCipasComposerRainLayerItem(): CCipasComposerLegendItem()
{
}

CCipasComposerRainLayerItem::CCipasComposerRainLayerItem( const QString& text ): CCipasComposerLegendItem( text )
{
}

CCipasComposerRainLayerItem::~CCipasComposerRainLayerItem()
{
}

QStandardItem* CCipasComposerRainLayerItem::clone() const
{
	CCipasComposerRainLayerItem* cloneItem  = new CCipasComposerRainLayerItem();
	*cloneItem = *this;
	cloneItem->setLayerID( mLayerID );
	return cloneItem;
}

void CCipasComposerRainLayerItem::writeXML( QDomElement& elem, QDomDocument& doc ) const
{
	QDomElement layerItemElem = doc.createElement( "RainLayerItem" );
	//layerItemElem.setAttribute( "layerId", mLayerID );
	QString colors=QString::number(mColor.red())+":"+QString::number(mColor.green())+":"+QString::number(mColor.blue());
	layerItemElem.setAttribute("colors",colors);
	layerItemElem.setAttribute( "text", text() );
	writeXMLChildren( layerItemElem, doc );
	elem.appendChild( layerItemElem );
}

void CCipasComposerRainLayerItem::readXML( const QDomElement& itemElem, bool xServerAvailable )
{
	if ( itemElem.isNull() )
	{
		return;
	}
	setText( itemElem.attribute( "text", "" ) );
	QString inColors=itemElem.attribute("colors","0:0:0");
	QStringList inColorLs=inColors.split(":");
	QColor inColor(inColorLs.at(0).toUInt(),inColorLs.at(1).toUInt(),inColorLs.at(2).toUInt());
	setColor(inColor);

}