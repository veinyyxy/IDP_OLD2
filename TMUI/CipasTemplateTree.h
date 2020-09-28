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
	void slot_currentItemChanged(QTreeWidgetItem* cur, QTreeWidgetItem* prev/*prev���Ǳ���ק����*/);

private:
	void parseFolderElement(const QDomElement &element,
		QTreeWidgetItem *parentItem = 0);
	QTreeWidgetItem *createItem(const QDomElement &element,bool isGroup,
		QTreeWidgetItem *parentItem = 0);
	void removeItem(QTreeWidgetItem *currentItem );
	void insertItem(QTreeWidgetItem *currentItem ,bool isGroup=false);
	void dropItem();
	//��ȡ��dom �ĵ�
	QDomDocument domDocument;
	QHash<QTreeWidgetItem *, QDomElement> domElementForItem;

	QMenu* m_contextMenu;
	QString currentItemDes;			//��ǰģ�������ı�
	bool isDefaultT;				//�Ƿ�ȱʡģ��
	bool Folded;					//�Ƿ�������
	bool isDropEventChangedItem;	//�Ƿ���קʱ����itemChanged �ź�
	QTreeWidgetItem *fromTW;		//�Ϸ�ʱ�뿪��Item
	QTreeWidgetItem *toTW;			//�Ϸ�ʱĿ��Item
	QTreeWidgetItem *dragdropItem;	//��ǰ�Ϸŵ�Item
	
};




