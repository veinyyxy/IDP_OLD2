/*=================================================================================== 
项目名： 人机交互平台
作 者： 吴恩平
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年10月30日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CipasLegendModel.h CipasLegendModel.cpp 
类 名： CCipasLegendModel
父 类： QStandardItemModel
子 类： 无 
功能说明：对图例项目使用 项/视图 进行管理，所用的model由此类支持
调用说明：
------------------------------------------------------------------------------------*/
#pragma once

#include <QStandardItemModel>
#include <QStringList>
#include <QSet>
#include <QDomDocument>
#include <QDomElement>

#include "tmui_global.h"

typedef QPair< QString, QList<QString> > GroupLayerInfo;


class TMUI_EXPORT CCipasLegendModel :public QStandardItemModel
{
	
	Q_OBJECT

public:
	enum ItemType
	{
		GroupItem = 0,
		LayerItem,
		ClassificationItem
	};
	CCipasLegendModel();
	~CCipasLegendModel(void);
	/**Sets layer set and groups*/
	void setLayerSetAndGroups( const QStringList& layerIds, const QList< GroupLayerInfo >& groupInfo );
	void setLayerSet( const QStringList& layerIds );
	/**Adds a group to a toplevel position (or -1 if it should be placed at the end of the legend). Returns a pointer to the added group*/
	QStandardItem* addGroup( QString text = tr( "Group" ), int position = -1 );

	/**Tries to automatically update a model entry (e.g. a whole layer or only a single item)*/
	void updateItem( QStandardItem* item );
	/**Updates the whole symbology of a layer*/
	void updateLayer( QStandardItem* layerItem );
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags( const QModelIndex &index ) const;

	/**Implemented to support drag operations*/
	virtual bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );

	/**For the drag operation*/
	QMimeData* mimeData( const QModelIndexList &indexes ) const;
	QStringList mimeTypes() const;

	/**Implements the drop operation*/
	bool dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent );

	void setAutoUpdate( bool autoUpdate );
	bool autoUpdate() { return mAutoUpdate; }

	bool writeXML( QDomElement& composerLegendElem, QDomDocument& doc ) const;

	bool readXML( const QDomElement& legendModelElem, const QDomDocument& doc );

	QPixmap symbolPreviewPixmap(QColor mColor, QSize size );

	QIcon symbolPreviewIcon( QColor mColor, QSize size );
	
public slots:
	void removeLayer( const QString& layerId );

signals:
	void layersChanged();
private:
	///**Adds classification items of vector layers
	// @return 0 in case of success*/
	//int addVectorLayerItems( QStandardItem* layerItem, QgsVectorLayer* vlayer );

protected:
	QStringList mLayerIds;
	/**True if this application has toplevel windows (normally true). If this is false, this means that the application
	might not have a running x-server on unix systems and so QPixmap and QIcon cannot be used*/
	bool mHasTopLevelWindow;

	/**True if the legend is auto updated when layers are added or removed from the map canvas */
	bool mAutoUpdate;
};

