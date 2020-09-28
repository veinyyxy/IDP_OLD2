#pragma once

#include "PluginManagerConfig.h"
#include <QObject>
#include "../include/IDPApplication.h"

class PLUGIN_MANAGER_IMPORT_EXPORT PluginManager
{
public:
	PluginManager* GetInstance();
	~PluginManager(void);
	static QObject* GetPlugin(QString fileName, QString path = QString());

protected:
	PluginManager(void);
	static PluginManager* s_instance;
};