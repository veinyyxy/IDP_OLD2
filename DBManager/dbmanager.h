#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "dbmanager_global.h"

#include <QObject>
#include <QList>
#include <QStringList>

class  DBManager : public QObject
{
public:
	enum EnumDBType
	{
		Sqlite,
		MySQL,
		ODBC,
		Oracle
	};

public:
	explicit DBManager();
	~DBManager();
	static DBManager *Instance();//得到数据库操作类实例
	DBManager* GetDBManager(EnumDBType dbType);

	//virtual bool CreateRadarDBTb(const QString& tbName) { return true;};
	//virtual bool InsertProductTb(const QString& guid,const QString& productId,const QString& productName) {return true;};
	//virtual bool InsertRadarProductTb(const QString& guid,const QString& productId,const QString& stationId,const QString& productPath,const QString& productDate,const QString& productTime) {return true;};
	//virtual bool InsertStationTb(const QString& guid, const QString& stationId, const QString& stationName, const QString& cProvince, const QString& cRadType, const int& nLat, const int& nLon, const int& nHei, const int& nDataRange, const int& nTime, const int& nWestLon, const int& nEastLon, const int& nNorthLat, const int& nSouthLat, const QString& cSpare) {return true;};
	//virtual QList<QList<QString>> QueryRadarListDB( const QString& tbName , QString strSql ) { QList<QList<QString>> list; return list;};
private:
};

#endif // DBMANAGER_H
