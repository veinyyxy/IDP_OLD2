#include "PluginManager.h"
#include <QPluginLoader>
#include <QCoreApplication>
#include <qdebug.h>

PluginManager* PluginManager::s_instance = 0;

PluginManager* PluginManager::GetInstance()
{
	if(!s_instance)
	{
		s_instance = new PluginManager();
	}

	return s_instance;
}

QObject* PluginManager::GetPlugin(QString fileName, QString path)
{
	if(path.isEmpty())
		path = IDPApplication::PluginPath();

	if(fileName.isEmpty())
		return NULL;

	path = IDPApplication::PluginPath() + path;
	QDir pluginDir(path);
	QString absolutePath = pluginDir.absoluteFilePath(fileName);
	QPluginLoader loader(absolutePath);
	//if(!loader.load())
	//	return NULL;

	QObject *pObject = loader.instance();
	if(!pObject)
		qDebug() << "load plugin failed:" << loader.errorString();
	return  pObject;
}

PluginManager::PluginManager(void)
{
}

PluginManager::~PluginManager(void)
{
}