#ifndef ORACLEMANAGER_H
#define ORACLEMANAGER_H

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

class OracleManager : public DBManager
{
public:
	OracleManager();
	~OracleManager();

	static OracleManager *Instance();//得到数据库操作类实例
private:
	void InitDBXMLConfig();
private:
	QSqlDatabase m_db_oraclel;
	QMutex m_db_lock;

	QString m_dbName;
	QString m_host;
	int m_port;
	QString m_user;
	QString m_passwd;
};

#endif // ORACLEMANAGER_H
