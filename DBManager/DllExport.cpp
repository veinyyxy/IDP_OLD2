#include<qglobal.h>
#include<qobject.h>
#include "dbmanager.h"

extern "C" Q_DECL_EXPORT DBManager* CreateInstance(QString  Name,DBManager::EnumDBType dbType)
{
	if (Name==QString("MDMS_DBManager"))
	{
		 return DBManager::Instance()->GetDBManager(dbType);
	}
	return (DBManager*) NULL;
}