#pragma once
#include "ToolBoxInterface.h"

class StandardTools :
	public ToolBoxInterface
{
public:
	StandardTools(void);
	virtual ~StandardTools(void);

	virtual QWidget* GetToolTableWidget();

	virtual QString GetToolTableName();

	virtual MyActionInterface* GetActionFromName( const QString& name );

	virtual void AddAction( const QString& name, MyActionInterface* pAction );

	virtual void RemoveAction( const QString& name );

protected:
	QPointer<QToolBar> m_pStandardToolsBar;
	QString m_strName;
	QMap<QString, MyActionInterface*> m_ActionMap;
};

