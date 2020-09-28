#pragma once
#include "ActionFactory.h"

class DockActionFactory :
	public ActionFactory
{
public:
	DockActionFactory(void);
	~DockActionFactory(void);

	virtual MyActionInterface* GetActionFromName( const QString& name );
private:
	QMap<QString, MyActionInterface*> m_ActionMap;
};

