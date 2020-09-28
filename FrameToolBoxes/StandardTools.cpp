#include "StdAfx.h"
#include "StandardTools.h"
#include "MyActionInterface.h"

StandardTools::StandardTools(void)
{
	m_pStandardToolsBar = new QToolBar();
	m_strName = QString::fromLocal8Bit("标准工具栏");
	m_pStandardToolsBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}


StandardTools::~StandardTools(void)
{
}

QWidget* StandardTools::GetToolTableWidget()
{
	return m_pStandardToolsBar;
}

QString StandardTools::GetToolTableName()
{
	return m_strName;
}

MyActionInterface* StandardTools::GetActionFromName( const QString& name )
{
	QMap<QString, MyActionInterface*>::Iterator iterFind = m_ActionMap.find(name);
	if(iterFind != m_ActionMap.end())
	{
		return iterFind.value();
	}
	else
		return 0;
}

void StandardTools::AddAction( const QString& name, MyActionInterface* pAction )
{
	m_ActionMap.insert(name, pAction);
	m_pStandardToolsBar->addAction(pAction);
}

void StandardTools::RemoveAction( const QString& name )
{
	MyActionInterface* pAction = 0;
	pAction = m_ActionMap.value(name);
	m_pStandardToolsBar->removeAction(pAction);
	m_ActionMap.remove(name);
}
