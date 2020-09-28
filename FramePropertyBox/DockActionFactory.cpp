#include "StdAfx.h"
#include "DockActionFactory.h"
#include "AllAcions.h"

DockActionFactory::DockActionFactory(void)
{
	m_ActionMap.insert("MoveUpAction", new MoveUpAction(QIcon(":/images/MapLayer/move_task_up.png"), QString::fromLocal8Bit("����"), 0));
	m_ActionMap.insert("MoveDownAction", new MoveDownAction(QIcon(":/images/MapLayer/move_task_down.png"), QString::fromLocal8Bit("����"), 0));
	m_ActionMap.insert("AddBasicAction", new AddBasicAction(QIcon(":/images/MapLayer/map_add.png"), QString::fromLocal8Bit("��ӻ���ͼ��"), 0));
	m_ActionMap.insert("ShowLayerPropertyAction", new ShowLayerPropertyAction(QIcon(":/images/MapLayer/map_property.png"), QString::fromLocal8Bit("ͼ������"), 0));
	m_ActionMap.insert("DeleteLayerAction", new DeleteLayerAction(QIcon(":/images/MapLayer/map_delete.png"), QString::fromLocal8Bit("ɾ��ͼ��"), 0));
	m_ActionMap.insert("ShowLayerAction", new ShowLayerAction(QIcon(":/images/MapLayer/show.png"), QString::fromLocal8Bit("��ʾͼ��"), 0));
	m_ActionMap.insert("HideLayerAction", new HideLayerAction(QIcon(":/images/MapLayer/hide.png"), QString::fromLocal8Bit("����ͼ��"), 0));
}

DockActionFactory::~DockActionFactory(void)
{
}

MyActionInterface* DockActionFactory::GetActionFromName( const QString& name )
{
	return m_ActionMap.value(name);
}
