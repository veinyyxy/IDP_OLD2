#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H

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

class SqliteManager : public DBManager
{
public:
	SqliteManager();
	~SqliteManager();
	static SqliteManager *Instance();//得到数据库操作类实例
	virtual bool CreateRadarDBTb(const QString& tbName);
	virtual bool InsertProductTb(const QString& guid,const QString& productId,const QString& productName);
	virtual bool InsertRadarProductTb(const QString& guid,const QString& productId,const QString& stationId,const QString& productPath,const QString& productDate,const QString& productTime);
	virtual bool InsertStationTb(const QString& guid, const QString& stationId, const QString& stationName, const QString& cProvince, const QString& cRadType, const int& nLat, const int& nLon, const int& nHei, const int& nDataRange, const int& nTime, const int& nWestLon, const int& nEastLon, const int& nNorthLat, const int& nSouthLat, const QString& cSpare);
	virtual QList<QList<QString>> QueryRadarListDB( const QString& tbName , QString strSql ="");
	QSqlDatabase getDB(){return m_db_sqlite;};

private:
	QSqlDatabase m_db_sqlite;
	QMutex m_db_lock;
};

#endif // SQLITEMANAGER_H
