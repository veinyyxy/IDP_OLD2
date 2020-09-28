#ifndef ODBCMANAGER_H
#define ODBCMANAGER_H
#include <QVariant>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"

class ODBCManager : public DBManager
{
public:
	ODBCManager();
	~ODBCManager();
	static ODBCManager *Instance();//得到数据库操作类实例
	bool StartExeSQL(const QString& strSql);
private:
	void InitDBXMLConfig();
	
private:
	QSqlDatabase m_db_odbc;
	QMutex m_db_lock;

	QString m_dsn;
	QString m_dbName;
	QString m_host;
	int m_port;
	QString m_user;
	QString m_passwd;
};

#endif // ODBCMANAGER_H
