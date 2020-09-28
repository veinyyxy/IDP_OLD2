
#include <QApplication>
#include <QMimeData>
#include <QSettings>
#include <QMessageBox>
#include <QMap>
#include <QPainter>
#include "CipasComposerLegendItem.h"
#include "CipasLegendModel.h"

CCipasLegendModel::CCipasLegendModel():QStandardItemModel()
{
	QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
	mHasTopLevelWindow = ( topLevelWidgets.size() > 0 );
}

CCipasLegendModel::~CCipasLegendModel(void)
{
}

void CCipasLegendModel::setLayerSetAndGroups( const QStringList& layerIds, const QList< GroupLayerInfo >& groupInfo )
{
	setLayerSet( layerIds );

	QStandardItem* currentItem = 0;
	QStandardItem* currentGroupItem = 0;
	int i = 0;
	int dd=invisibleRootItem()->rowCount();
	QList< GroupLayerInfo >::const_iterator infoIt = groupInfo.constBegin();
	for ( ; infoIt != groupInfo.constEnd() && i < invisibleRootItem()->rowCount(); )
	{
		currentItem = invisibleRootItem()->child( i, 0 );
		QString infoKey = infoIt->first;
		if ( infoKey.isNull() ) //a toplevel layer
		{
			++i;
		}
		else //a group
		{
			currentGroupItem = addGroup( infoKey, i );
			++i;
			QList<QString> layerList = infoIt->second;
			QList<QString>::const_iterator groupLayerIt = layerList.constBegin();
			for ( ; currentItem && ( groupLayerIt != layerList.constEnd() ); ++groupLayerIt )
			{
				//check if current item is contained in this group
				CCipasComposerLayerItem* layerItem = dynamic_cast<CCipasComposerLayerItem*>( currentItem );
				if ( !layerItem )
				{
					return; //should never happen
				}
				//QString layerID = currentItem->data(Qt::UserRole + 2).toString();
				QString layerID = layerItem->layerID();
				if ( layerList.contains( layerID ) )
				{
					takeRow( i );
					currentGroupItem->setChild( currentGroupItem->rowCount(), 0, currentItem );
				}
				else
				{
					++i;
				}
				currentItem = invisibleRootItem()->child( i, 0 );
			}
		}
		++infoIt;
	}
}

void CCipasLegendModel::setLayerSet( const QStringList& layerIds )
{
	
}

QStandardItem* CCipasLegendModel::addGroup( QString text, int position )
{
	CCipasComposerGroupItem* groupItem = new CCipasComposerGroupItem( text );
	if ( position == -1 )
	{
		invisibleRootItem()->insertRow( invisibleRootItem()->rowCount(), groupItem );
	}
	else
	{
		invisibleRootItem()->insertRow( position, groupItem );
	}
	return groupItem;
}

void CCipasLegendModel::removeLayer( const QString& layerId )
{
	int numRootItems = rowCount();
	for ( int i = 0; i < numRootItems ; ++i )
	{
		CCipasComposerLayerItem* lItem = dynamic_cast<CCipasComposerLayerItem*>( item( i ) );
		if ( !lItem )
		{
			continue;
		}

		if ( layerId == lItem->layerID() )
		{
			removeRow( i ); //todo: also remove the subitems and their symbols...
			emit layersChanged();
			return;
		}
	}
}

bool CCipasLegendModel::writeXML( QDomElement& composerLegendElem, QDomDocument& doc ) const
{
	if ( composerLegendElem.isNull() )
	{
		return false;
	}

	QDomElement legendModelElem = doc.createElement( "Model" );
	legendModelElem.setAttribute( "autoUpdate", mAutoUpdate );
	int nTopLevelItems = invisibleRootItem()->rowCount();
	QStandardItem* currentItem = 0;
	CCipasComposerLegendItem* currentLegendItem = 0;

	for ( int i = 0; i < nTopLevelItems; ++i )
	{
		currentItem = invisibleRootItem()->child( i, 0 );
		currentLegendItem = dynamic_cast<CCipasComposerLegendItem*>( currentItem );
		if ( currentLegendItem )
		{
			currentLegendItem->writeXML( legendModelElem, doc );
		}
	}

	composerLegendElem.appendChild( legendModelElem );

	return true;
}

bool CCipasLegendModel::readXML( const QDomElement& legendModelElem, const QDomDocument& doc )
{
	if ( legendModelElem.isNull() )
	{
		return false;
	}

	clear();

	QDomNodeList topLevelItemList = legendModelElem.childNodes();
	QDomElement currentElem;
	CCipasComposerLegendItem* currentItem = 0;

	int nTopLevelItems =  topLevelItemList.size();
	for ( int i = 0; i < nTopLevelItems; ++i )
	{
		currentElem = topLevelItemList.at( i ).toElement();
		if ( currentElem.isNull() )
		{
			continue;
		}

		//toplevel items can be groups or layers
		if ( currentElem.tagName() == "LayerItem" )
		{
			currentItem = new CCipasComposerLayerItem();
		}
		else if ( currentElem.tagName() == "GroupItem" )
		{
			currentItem = new CCipasComposerGroupItem();
		}
		currentItem->readXML( currentElem, mHasTopLevelWindow );
		appendRow( currentItem );
	}

	setAutoUpdate( legendModelElem.attribute( "autoUpdate", "1" ).toInt() );
	return true;
}

Qt::DropActions CCipasLegendModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

Qt::ItemFlags CCipasLegendModel::flags( const QModelIndex &index ) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if ( !index.isValid() )
	{
		flags |= Qt::ItemIsDropEnabled;
		return flags;
	}

	QStandardItem* item = itemFromIndex( index );
	CCipasComposerLegendItem* cItem = dynamic_cast<CCipasComposerLegendItem*>( item );

	if ( cItem )
	{
		
		flags |= Qt::ItemIsDragEnabled;
	}
	return flags;
}

bool CCipasLegendModel::removeRows( int row, int count, const QModelIndex & parent )
{
	if ( count < 1 )
	{
		return false;
	}

	if ( parent.isValid() )
	{
		for ( int i = row + count - 1; i >= row; --i )
		{
			QStandardItem* item = itemFromIndex( parent );
			if ( item )
			{
				item->takeRow( i );
			}
		}
	}
	else
	{
		for ( int i = row + count - 1; i >= row; --i )
		{
			takeRow( i );
		}
	}
	return true;
}

QMimeData* CCipasLegendModel::mimeData( const QModelIndexList &indexes ) const
{
	QMimeData* mimeData = new QMimeData();
	QByteArray encodedData;
	QDomDocument xmlDoc;
	QDomElement xmlRootElement = xmlDoc.createElement( "LegendModelDragData" );
	xmlDoc.appendChild( xmlRootElement );

	QModelIndexList::const_iterator indexIt = indexes.constBegin();
	for ( ; indexIt != indexes.constEnd(); ++indexIt )
	{
		QStandardItem* sItem = itemFromIndex( *indexIt );
		if ( sItem )
		{
			CCipasComposerLegendItem* mItem = dynamic_cast<CCipasComposerLegendItem*>( sItem );
			if ( mItem )
			{
				//mItem->writeXML( xmlRootElement, xmlDoc );
			}
		}
	}
	mimeData->setData( "text/xml", xmlDoc.toByteArray() );
	return mimeData;
}

QStringList CCipasLegendModel::mimeTypes() const
{
	QStringList types;
	types << "text/xml";
	return types;
}

bool CCipasLegendModel::dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent )
{
	if ( !data->hasFormat( "text/xml" ) )
	{
		return false;
	}

	QStandardItem* dropIntoItem = 0;
	if ( parent.isValid() )
	{
		dropIntoItem = itemFromIndex( parent );
	}
	else
	{
		dropIntoItem = invisibleRootItem();
	}

	//get XML doc
	QByteArray encodedData = data->data( "text/xml" );
	QDomDocument xmlDoc;
	xmlDoc.setContent( encodedData );

	QDomElement dragDataElem = xmlDoc.documentElement();
	if ( dragDataElem.tagName() != "LegendModelDragData" )
	{
		return false;
	}

	QDomNodeList nodeList = dragDataElem.childNodes();
	int nChildNodes = nodeList.size();
	QDomElement currentElem;
	QString currentTagName;
	CCipasComposerLegendItem* currentItem = 0;

	for ( int i = 0; i < nChildNodes; ++i )
	{
		currentElem = nodeList.at( i ).toElement();
		if ( currentElem.isNull() )
		{
			continue;
		}
		currentTagName = currentElem.tagName();
		if ( currentTagName == "LayerItem" )
		{
			currentItem = new CCipasComposerLayerItem();
		}
		else if ( currentTagName == "GroupItem" )
		{
			currentItem = new CCipasComposerGroupItem();
		}
		else
		{
			continue;
		}
		//currentItem->readXML( currentElem );
		if ( row < 0 )
		{
			dropIntoItem->insertRow( dropIntoItem->rowCount(), currentItem );
		}
		else
		{
			dropIntoItem->insertRow( row + i, currentItem );
		}
	}
	emit layersChanged();
	return true;
}

void CCipasLegendModel::setAutoUpdate( bool autoUpdate )
{
	if ( mAutoUpdate == autoUpdate ) //prevent multiple signal/slot connections
	{
		return;
	}

	mAutoUpdate = autoUpdate;
	

}


QPixmap CCipasLegendModel::symbolPreviewPixmap(QColor mColor, QSize size )
{
	QPixmap pixmap( size );
	pixmap.fill( Qt::transparent );
	QPainter painter;
	painter.begin( &pixmap );
	painter.setRenderHint( QPainter::Antialiasing );
	QBrush mBrush;
	mBrush.setStyle(Qt::SolidPattern);
	mBrush.setColor(mColor);
	painter.setBrush(mBrush);
	painter.drawRect(0,0,size.width(),size.height());
	painter.end();
	//pixmap.save("c:/pix.bmp");
	return pixmap;
}

QIcon CCipasLegendModel::symbolPreviewIcon(QColor mColor, QSize size )
{
	return QIcon( symbolPreviewPixmap( mColor, size ) );
}
