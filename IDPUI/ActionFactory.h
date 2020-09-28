#pragma once
#include "MyActionInterface.h"

class ActionFactory
{
public:
	virtual MyActionInterface* GetActionFromName(const QString& name) = 0;

};

