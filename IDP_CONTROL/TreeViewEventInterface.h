#pragma once
#include <QtGui/QStandardItemModel>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QPoint>

class TreeViewEventInterface
{
public:
	virtual void ItemClick(const QModelIndex& index) = 0;
	virtual void ShowContextMenu(const QPoint& point) = 0;
	virtual void ItemDoubleClicked(const QModelIndex& index) = 0;
	virtual QAbstractItemModel* CreateModel() = 0;
public:
	QStandardItemModel m_StandardItemModel;
};

