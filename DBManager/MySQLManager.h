#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H

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

class MySQLManager  : public DBManager
{
public:
	MySQLManager();
	~MySQLManager();

	static MySQLManager *Instance();//得到数据库操作类实例
	virtual int GetData_ID(QString code);
	//sprintf(sql, "insert into r_product(title,start_longitude,start_latitude,end_longitude,end_latitude,img_Dir,img_name,GenTime,ts,data_id,dateDirectory) value(\'%s\',%.2f,%.2f,%.2f,%.2f,\'%s\',\'%s\',now(),\'%s\',%d,\'%s\')",monitorPathVector[i].productType,slon,slat,elon,elat,prodPath,str1.c_str(),ts,monitorPathVector[i].parentID,nowHour);
	virtual bool Insert_r_productTb(const QString& strSql);
private:
	void InitDBXMLConfig();
	
private:
	QSqlDatabase m_db_mysql;
	QMutex m_db_lock;

	QString m_dbName;
	QString m_host;
	int m_port;
	QString m_user;
	QString m_passwd;
};

#endif // MYSQLMANAGER_H
