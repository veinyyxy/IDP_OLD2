#pragma once
#include "ActionFactory.h"

class FramTooBoxesActionFactory :
	public ActionFactory
{
public:
	FramTooBoxesActionFactory(void);
	~FramTooBoxesActionFactory(void);

	virtual MyActionInterface* GetActionFromName( const QString& name );
private:
	QMap<QString, MyActionInterface*> m_ActionMap;
};

