#pragma once
#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>
#include "Tempalte.h"

class QMenu;

class CCipasTemplateTree :public QTreeWidget
{
	Q_OBJECT
public:
	CCipasTemplateTree(QWidget *parent = 0);
	~CCipasTemplateTree(void);

	bool read(QIODevice *device);
	bool write(QIODevice *device);
	QString ItemPath(/*QTreeWidgetItem *item*/);
	QString ItemDescription()const;
	void removeTemplate(QTreeWidgetItem *item);
	void insertTemplate(TemplateStruct *item);
	void newGroup();
	void setCurrentItemDescription(const QString &text);
	QString getCurrentItemDescription()const;
public slots:
	void updateDomElement(QTreeWidgetItem *item, int column);
protected:
	void dropEvent(QDropEvent *event);
private slots:
	void rightMenu(const QPoint&);
	void solt_newGroup();
	void slot_newItem();
	void slot_remove();
	void slot_setDefaultT();
	void slot_currentItemChanged(QTreeWidgetItem* cur, QTreeWidgetItem* prev/*prev即是被拖拽的项*/);

private:
	void parseFolderElement(const QDomElement &element,
		QTreeWidgetItem *parentItem = 0);
	QTreeWidgetItem *createItem(const QDomElement &element,bool isGroup,
		QTreeWidgetItem *parentItem = 0);
	void removeItem(QTreeWidgetItem *currentItem );
	void insertItem(QTreeWidgetItem *currentItem ,bool isGroup=false);
	void dropItem();
	//读取的dom 文档
	QDomDocument domDocument;
	QHash<QTreeWidgetItem *, QDomElement> domElementForItem;

	QMenu* m_contextMenu;
	QString currentItemDes;			//当前模板描述文本
	bool isDefaultT;				//是否缺省模板
	bool Folded;					//是否缩回树
	bool isDropEventChangedItem;	//是否拖拽时候发射itemChanged 信号
	QTreeWidgetItem *fromTW;		//拖放时离开的Item
	QTreeWidgetItem *toTW;			//拖放时目标Item
	QTreeWidgetItem *dragdropItem;	//当前拖放的Item
	
};




