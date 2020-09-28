#pragma once
#include "TreeViewEventInterface.h"

class TreeViewEventImps :
	public TreeViewEventInterface
{
public:
	TreeViewEventImps(void);
	~TreeViewEventImps(void);
	void ItemClick(const QModelIndex& index);
	void ShowContextMenu(const QPoint& point);
	void ItemDoubleClicked(const QModelIndex& index);
	goto_gis::Map* GetCurentMap();
	QAbstractItemModel* CreateModel();

private:

};

