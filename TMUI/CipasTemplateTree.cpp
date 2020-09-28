
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QMenu>

#include "CipasTemplateTree.h"

CCipasTemplateTree::CCipasTemplateTree(QWidget *parent)
	: QTreeWidget(parent)
	, m_contextMenu(0)
	, fromTW(0)
	, toTW(0)
	, dragdropItem(0)
{
	QStringList labels;
	labels << tr("Name");
	setEditTriggers(QTreeWidget::NoEditTriggers);
	header()->setResizeMode(QHeaderView::Stretch);
	setHeaderLabels(labels);
	//this->setStyleSheet("QTreeView::branch {image:none;}");
	this->setRootIsDecorated(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(rightMenu(const QPoint&)));
	isDropEventChangedItem=false;
	connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*))
		,this,SLOT(slot_currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
}


CCipasTemplateTree::~CCipasTemplateTree(void)
{
	if (m_contextMenu)
	{
		delete m_contextMenu;
		m_contextMenu = NULL;
	}
	if (fromTW)
	{
		delete fromTW;
		fromTW=NULL;
	}
	/*由于代码中 dragdropItem 父节点赋给了 toTW 所以 要先释放dragdropItem*/
	if (dragdropItem)
	{
		delete dragdropItem;
		dragdropItem=NULL;
	}
	if (toTW)
	{
		delete toTW;
		toTW=NULL;
	}

	
}



bool CCipasTemplateTree::read(QIODevice *device)
{
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!domDocument.setContent(device, true, &errorStr, &errorLine,
		&errorColumn)) {
			QMessageBox::information(window(), tr("DOM Bookmarks"),
				tr("Parse error at line %1, column %2:\n%3")
				.arg(errorLine)
				.arg(errorColumn)
				.arg(errorStr));
			return false;
	}

	QDomElement root = domDocument.documentElement();
	if (root.tagName() != "xbel") {
		QMessageBox::information(window(), tr("DOM Bookmarks"),
			tr("The file is not an XBEL file."));
		return false;
	} else if (root.hasAttribute("version")
		&& root.attribute("version") != "1.0") {
			QMessageBox::information(window(), tr("DOM Bookmarks"),
				tr("The file is not an XBEL version 1.0 "
				"file."));
			return false;
	}

	clear();

	disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
		this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

	QDomElement child = root.firstChildElement("Group");
	while (!child.isNull()) {
		parseFolderElement(child);
		child = child.nextSiblingElement("Group");
	}

	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
		this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

	return true;
}

bool CCipasTemplateTree::write(QIODevice *device)
{
	const int IndentSize = 4;

	QTextStream out(device);
	domDocument.save(out, IndentSize);
	return true;
}

void CCipasTemplateTree::updateDomElement(QTreeWidgetItem *item, int column)
{
	QDomElement element = domElementForItem.value(item);
	if (!element.isNull()) {
		if (column == 0) {
			QDomElement oldTitleElement = element.firstChildElement("Name");
			QDomElement newTitleElement = domDocument.createElement("Name");
			if (element.tagName()=="Group")
			{
				if (Folded)
				{
					element.setAttribute("folded","yes");
				} 
				else
				{
					element.setAttribute("folded","no");
				}
			} 
			else if(element.tagName()=="Item")
			{
				if (isDefaultT)
				{
					element.setAttribute("isDefault","yes");
				} 
				else
				{
					element.setAttribute("isDefault","no");
				}
				
				element.setAttribute("des",currentItemDes);
			}
			
			QDomText newTitleText = domDocument.createTextNode(item->text(0));
			QString t0= item->text(0);
			newTitleElement.appendChild(newTitleText);

			element.replaceChild(newTitleElement, oldTitleElement);
			//QString ps=element.attribute("path");
			//QString des=element.attribute("des");
			//int j=0;
		} else {
			if (element.tagName() == "Item")
				element.setAttribute("path", item->text(1));
		}
	}
}

void CCipasTemplateTree::slot_currentItemChanged(QTreeWidgetItem* cur, QTreeWidgetItem* prev)
{
	if (isDropEventChangedItem)
	{
		if (prev)
		{
			dragdropItem=prev;
		}
		//拖动项目对应的domelement
		QDomElement selectedElement = domElementForItem.value(prev);
		QDomElement FromPElement=domElementForItem[prev->parent()];
		FromPElement.removeChild(selectedElement);
		QDomNode oldDomNode= selectedElement.parentNode();
		QString sn= oldDomNode.nodeName();
		QString sv= oldDomNode.nodeValue();
		QString s=prev->text(0);
		QTreeWidgetItem *newParent=prev->parent();
		QString ps=newParent->text(0);
		int j=0;
	}
}

void CCipasTemplateTree::dropItem()
{
	toTW=dragdropItem->parent();
	QDomElement ToPElement=domElementForItem[toTW];
	QDomElement selectedElement = domElementForItem.value(dragdropItem);
	ToPElement.appendChild(selectedElement);
	QString ps=toTW->text(0);
	int j=0;
}

void CCipasTemplateTree::dropEvent(QDropEvent *event)
{
	//QDomElement selectedElement = domElementForItem.value(currentItem());
	//QTreeWidgetItem *oldParent=currentItem()->parent();
	//if (oldParent)
	//{
	//	QDomElement oldSelectedElementP = domElementForItem.value(oldParent);
	//}
	isDropEventChangedItem=true;
	QTreeWidget::dropEvent(event);
	isDropEventChangedItem=false;
	//QTreeWidgetItem *newParent=currentItem()->parent();
	//if (newParent)
	//{
	//	QDomElement newSelectedElementP = domElementForItem.value(newParent);
	//}
	//int j=0;
	dropItem();
}

void CCipasTemplateTree::parseFolderElement(const QDomElement &element,
	QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item = createItem(element,true, parentItem);

	QString Name = element.firstChildElement("Name").text();
	if (Name.isEmpty())
		Name = QObject::tr("Group");
	if (Name==tr("浏览更多……"))
	{
		item->setIcon(0, QIcon(":/images/thematicMap/mActionFileOpen.png" ));
	}
	else
	{
		//item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setIcon(0, QIcon(":/images/thematicMap/open.png" ));
	}
	//QString groupTitle=tr("<b>%1</b>").arg(Name);
	item->setText(0, Name);

	bool folded = (element.attribute("folded") != "no");
	setItemExpanded(item, !folded);

	QDomElement child = element.firstChildElement();
	while (!child.isNull()) {
		if (child.tagName() == "Group") {
			parseFolderElement(child, item);
		} else if (child.tagName() == "Item") {
			QTreeWidgetItem *childItem = createItem(child, false,item);

			QString Name = child.firstChildElement("Name").text();
			if (Name.isEmpty())
				Name = QObject::tr("Group");

			childItem->setFlags(item->flags() | Qt::ItemIsEditable);
			childItem->setIcon(0, QIcon(":/images/thematicMap/mComposer.png" ));
			childItem->setText(0, Name);
			childItem->setText(1, child.attribute("path"));
			childItem->setText(2, child.attribute("des"));
			childItem->setText(3, child.attribute("isGroup"));
			childItem->setText(4, child.attribute("isDefault"));
		} else if (child.tagName() == "separator") {
			QTreeWidgetItem *childItem = createItem(child, true,item);
			childItem->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
			childItem->setText(0, QString(30, 0xB7));
		}
		child = child.nextSiblingElement();
	}
}

QTreeWidgetItem *CCipasTemplateTree::createItem(const QDomElement &element,
	bool isGroup,
	QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item;
	if (parentItem) {
		item = new QTreeWidgetItem(parentItem);
	} else {
		item = new QTreeWidgetItem(this);
	}
	if (isGroup)
	{
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable
			|Qt::ItemIsDragEnabled
			|Qt::ItemIsDropEnabled);
	}
	else
	{
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable 
			|Qt::ItemIsDragEnabled
			);
	}
	domElementForItem.insert(item, element);
	return item;
}

void CCipasTemplateTree::removeItem(QTreeWidgetItem *currentItem )
{
	QString Name= currentItem->text(0);
	if (Name==tr("系统模板")||Name==tr("用户模板"))
	{
		return;
	}
	QDomElement elementp = domElementForItem.value(currentItem);
	for (int i=0;i<currentItem->childCount();i++)
	{
		QTreeWidgetItem *childItem=currentItem->child(i);
		currentItem->takeChild(i);
		QDomElement element = domElementForItem.value(childItem);
		QString sn= element.tagName();
		QString sn1=element.text();
		//domDocument.removeChild(element);
		elementp.removeChild(element);
		domElementForItem.remove(childItem);
	}
	
	
	//int rs= domElementForItem.remove(currentItem);
	QTreeWidgetItem *parent=currentItem->parent();
	if (parent)
	{
		parent->removeChild(currentItem);
		QDomElement elementpp = domElementForItem.value(parent);
		elementpp.removeChild(elementp);
	}
	else
	{

		int topIndex=this->indexOfTopLevelItem(currentItem);
		//QDomElement elementpp = domElementForItem.value(this->invisibleRootItem()->child(topIndex));
		//elementpp.removeChild(elementp);
		QHash<QTreeWidgetItem *, QDomElement>::const_iterator j=domElementForItem.constBegin();
		while(j!=domElementForItem.constEnd())
		{
			if (j.value().nodeName()=="Group")
			{
				QString Name = j.value().firstChildElement("Name").text();
				QTreeWidgetItem *rItem=j.key();
				if (Name=="用户模板")
				{
					int i=0;
				}
			}
			++j;
		}
		QTreeWidgetItem *r1Item=this->invisibleRootItem()->child(1);
		QDomElement elementpp = domElementForItem.value(r1Item);
		if (!elementpp.isNull())
		{
			int ss=0;
		}
		this->takeTopLevelItem(topIndex);
	}
	//domDocument.removeChild(elementp);
}

void CCipasTemplateTree::insertItem(QTreeWidgetItem *currentItem ,bool isGroup)
{
	QDomElement userP = domElementForItem.value(currentItem);
	if (isGroup)
	{
		QDomElement userC=domDocument.createElement("Group");
		userC.setAttribute("folded","no");
		userC.setAttribute("isGroup","yes");
		userP.appendChild(userC);
		QDomElement newTitleElement = domDocument.createElement("Name");
		newTitleElement.setNodeValue(tr("新建分类"));
		userC.appendChild(newTitleElement);
		QTreeWidgetItem *childItem = createItem(userC, true,currentItem);
		childItem->setIcon(0, QIcon(":/images/thematicMap/open.png" ));
		childItem->setText(0, tr("新建分类"));
	}
	else
	{
		return;
		/*
		QDomElement userC=domDocument.createElement("Item");
		userC.setAttribute("isDefault","no");
		userC.setAttribute("isGroup","no");
		userC.setAttribute("path","");
		userC.setAttribute("des","");
		userP.appendChild(userC);
		QDomElement newTitleElement = domDocument.createElement("Name");
		newTitleElement.setNodeValue(tr("新建项"));
		userC.appendChild(newTitleElement);
		QTreeWidgetItem *childItem = createItem(userC, true,currentItem);
		childItem->setIcon(0, QIcon(":/images/mComposer.png" ));
		childItem->setText(0, tr("新建项"));
		*/
	}
	
}

void CCipasTemplateTree::removeTemplate(QTreeWidgetItem *item)
{
	removeItem(item);
}

void CCipasTemplateTree::insertTemplate(TemplateStruct *item)
{
	//生成dom节点
	//在用户模板下面创建node
	QDomElement root = domDocument.documentElement();
	QDomElement child = root.firstChildElement("Group");
	while (!child.isNull()) {
		QString vns=child.firstChildElement("Name").text();
		if (child.firstChildElement("Name").text()=="用户模板")
		{
			//插入新节点
			QHash<QTreeWidgetItem *, QDomElement>::const_iterator j=domElementForItem.constBegin();
			while(j!=domElementForItem.constEnd())
			{
				QString Name = j.value().firstChildElement("Name").text();
				if (Name=="用户模板")
				{
					int i=0;
					QTreeWidgetItem *rItem=j.key();
					QDomElement userP = domElementForItem.value(rItem);
					//QDomElement userC=userP.firstChildElement();//domDocument
					QDomElement userC=domDocument.createElement("Item");
					userC.setAttribute("path",item->Path);
					userC.setAttribute("des",item->Description);
					userC.setAttribute("isGroup",item->IsGroup);
					userC.setAttribute("isDefault",item->IsDefault);
					userP.appendChild(userC);
					QDomElement newTitleElement = domDocument.createElement("Name");
					newTitleElement.setNodeValue(item->Name);
					userC.appendChild(newTitleElement);
					QTreeWidgetItem *childItem = createItem(userC, false,rItem);
					//childItem->setFlags(item->flags() | Qt::ItemIsEditable);
					childItem->setIcon(0, QIcon(":/images/thematicMap/mComposer.png" ));
					childItem->setText(0, item->Name);
					//childItem->setText(1, item->Path);
					//childItem->setText(2, item->Description);
					this->setCurrentItem(childItem);
				}
				++j;
			}
			break;
		}
		child = child.nextSiblingElement("Group");
	}

	
	//QDomNodeList rnl=root.childNodes();
	//for (int k=0;k<rnl.count();k++)
	//{
	//	QDomNode userNode= rnl.at(k);
	//	QString Name=userNode.firstChildElement("Name").text();
	//	if (Name=="用户模板")
	//	{
	//		QDomElement newTitleElement = domDocument.createElement("Name");
	//	}
	//}
	//
	//QDomElement newTitleElement = domDocument.createElement("Name");
	//写入dom 由 主调函数调用updateDomElement 函数实现
	//
}

QString CCipasTemplateTree::ItemPath()
{
	
	QDomElement element = domElementForItem.value(currentItem());
	QString s=element.attribute("path");
	return element.attribute("path");

}

QString CCipasTemplateTree::ItemDescription() const
{
	QDomElement element = domElementForItem.value(currentItem());
	QString str1= element.attribute("des");
	return str1;
}

void CCipasTemplateTree::rightMenu(const QPoint& point)
{
	QTreeWidgetItem *clickedItem=this->itemAt(point);
	if (clickedItem->text(0)==tr("浏览更多……"))
	{
		return;
	}
	QDomElement element = domElementForItem.value(clickedItem);
	m_contextMenu=new QMenu(this);
	QString s=element.attribute("isGroup");
	if (s==tr("no"))
	{
		//m_contextMenu->addAction(QIcon(":/images/mComposer.png" )
		//	,tr("增加项目")
		//	,this
		//	,SLOT(slot_newItem()));
		m_contextMenu->addAction(QIcon(":/images/thematicMap/symbologyRemove.png" )
			,tr("删除项")
			,this
			,SLOT(slot_remove()));
		//m_contextMenu->addAction(QIcon(":/images/config_.png" )
		//	,tr("设为默认模板")
		//	,this
		//	,SLOT(slot_setDefaultT()));
	}
	else if (s==tr("yes"))
	{
		m_contextMenu->addAction(QIcon(":/images/thematicMap/open.png" )
			,tr("增加分类")
			,this
			,SLOT(solt_newGroup()));
		QString Name = element.firstChildElement("Name").text();
		if (Name!=tr("系统模板")&&Name!=tr("用户模板"))
		{
			m_contextMenu->addAction(QIcon(":/images/thematicMap/symbologyRemove.png" )
				,tr("删除分类")
				,this
				,SLOT(slot_remove()));
		}
		//m_contextMenu->addAction(QIcon(":/images/mComposer.png" )
		//	,tr("增加项目")
		//	,this
		//	,SLOT(slot_newItem()));
	}
	else
		return;
	m_contextMenu->exec(QCursor::pos());
}

void CCipasTemplateTree::newGroup()
{
	solt_newGroup();
}

void CCipasTemplateTree::solt_newGroup()
{
	//QMessageBox::warning( 0, tr("信息提示"), tr("增加新分类") );
	QDomElement element = domElementForItem.value(currentItem());
	QString s=element.attribute("isGroup");
	bool isG=false;
	if (s==tr("yes")) isG=true;
	insertItem(currentItem(),isG);
}

void CCipasTemplateTree::slot_newItem()
{
	QMessageBox::warning( 0, tr("信息提示"), tr("增加新项目") );
}

void CCipasTemplateTree::slot_remove()
{
	removeItem(this->currentItem());
}

void CCipasTemplateTree::slot_setDefaultT()
{
	QDomElement element = domElementForItem.value(currentItem());
	QDomNodeList ItemList=domDocument.elementsByTagName( "Item" );
	for (int i=0;i<ItemList.count();i++)
	{
		QDomElement currentItemElem = ItemList.at( i ).toElement();
		currentItemElem.setAttribute("isDefault","no");
	}
	isDefaultT=true;
	//updateDomElement(createItem(),0);
}

void CCipasTemplateTree::setCurrentItemDescription(const QString &text)
{
	currentItemDes=text;
}

QString CCipasTemplateTree::getCurrentItemDescription()const
{
	return currentItemDes;
}