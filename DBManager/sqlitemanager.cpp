#include <QCoreApplication>
#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>
#include <QSettings>
#include <QMessageBox>
#include "sqlitemanager.h"

SqliteManager::SqliteManager()
{
	m_db_sqlite=QSqlDatabase::addDatabase("QSQLITE","QSQLITE");    //添加数据库驱动      
	QString iniPath = QCoreApplication::applicationDirPath()+tr("/Config/Config.ini");
	QSettings *configIni = new QSettings(iniPath, QSettings::IniFormat); 
	QString dbPath = configIni->value("/QSQLITE/QSQLITEDB").toString();//监控目录
	m_db_sqlite.setDatabaseName(dbPath);  //在工程目录新建一个RadarDB.db的文件 
	if (!m_db_sqlite.open()) 
	{
		QSqlError err = m_db_sqlite.lastError();
		m_db_sqlite.close();
		QMessageBox::critical(NULL,tr("提示"),err.text());
	}
}

SqliteManager::~SqliteManager()
{

}

Q_GLOBAL_STATIC(SqliteManager, sqliteManager)
	SqliteManager *SqliteManager::Instance() 
{ 
	return sqliteManager(); 
}


bool SqliteManager::CreateRadarDBTb(const QString& tbName)
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_sqlite.isOpen())
	{
		m_db_sqlite.open();
	}
	QSqlQuery query(m_db_sqlite);//以下执行相关QSL语句

	bool bsuccess = false;
	if (tbName == tr("radarProductTb"))//主表
	{
		bsuccess = query.exec("create table radarProductTb(pk_id varchar(300) primary key,productId varchar(150),stationId varchar(150),productPath varchar(800),productDate date,productTime time)");
	}
	//CREATE TABLE r_work_station (
	//	pk_id int(11) NOT NULL  primary key,
	//	code varchar(50) NOT NULL ,
	//	name varchar(50) DEFAULT NULL,
	//	longitude float DEFAULT NULL,
	//	latitude float DEFAULT NULL ,
	//	reach float DEFAULT NULL ,
	//	direction float DEFAULT NULL ,
	//	range float DEFAULT NULL ,
	//	type varchar(255) DEFAULT NULL ,
	//	equipment varchar(255) DEFAULT NULL ,
	//	quantity int(11) DEFAULT NULL ,
	//	province varchar(50) DEFAULT NULL ,
	//	city varchar(50) DEFAULT NULL ,
	//	area varchar(50) DEFAULT NULL ,
	//	town varchar(50) DEFAULT NULL,
	//	airspace varchar(50) DEFAULT NULL ,
	//	admin_name varchar(50) DEFAULT NULL ,
	//	tel varchar(50) DEFAULT NULL
	//	) 

	
	else if (tbName == tr("stationTb"))//站点名称子表
	{
		QString sql;
		sql.append("create table stationTb(guid varchar(50) primary key,");
		sql.append("stationId varchar(50),");
		sql.append("stationName varchar(50),");
		sql.append("cProvince varchar(50),");
		sql.append("cRadType varchar(50),");
		sql.append("nLat int,");
		sql.append("nLon int,");
		sql.append("nHei int,");
		sql.append("nDataRange int,");
		sql.append("nTime int,");
		sql.append("nWestLon int,");
		sql.append("nEastLon int,");
		sql.append("nNorthLat int,");
		sql.append("nSouthLat int,");
		sql.append("cSpare varchar(50) )");
		bsuccess = query.exec(sql);
	}
	else if (tbName == tr("productTb"))//产品类型子表
	{
		bsuccess =  query.exec("create table productTb(guid varchar(50) primary key,productId varchar(50),productName varchar(120))");
	}
	return bsuccess;
}


bool SqliteManager::InsertRadarProductTb(const QString& guid,const QString& productId,const QString& stationId,const QString& productPath,const QString& productDate,const QString& productTime)
{



	QMutexLocker locker(&m_db_lock);
	if (!m_db_sqlite.isOpen())
	{
		m_db_sqlite.open();
	}
	QSqlQuery query(m_db_sqlite);//以下执行相关QSL语句
	//create table radarProductTb(guid varchar(120) primary key,productId varchar(50),stationId varchar(50),productPath varchar(300),productDate date,productTime time)
	QString strSql = tr("insert into radarProductTb(pk_id,productId,stationId,productPath,productDate,productTime) VALUES ('%1' , '%2' , '%3' , '%4' , '%5' ,'%6')").arg(guid).arg(productId).arg(stationId).arg(productPath).arg(productDate).arg(productTime);
	//QString strSql = tr("insert into radarProductTb(guid,productId,stationId,productPath,productDate,productTime) value('%1' , '%2' ,'%3' , '%4' ,'%5')").arg(guid).arg(productId).arg(stationId).arg(productPath).arg(productDate).arg(productTime);
	//QString sql = tr("insert into radarProductTb values('814E4C76-3D2F-4DEB-8CD5-826CDDEDA284','CAP','z9070','Z9070_CAP_20101008_080826.bin.bz2')");
	//QString sql;
	//sql.append(tr("insert into radarProductTb values"));
	//sql.append(tr("('"));
	//sql.append(guid);
	//sql.append(tr("','"));
	//sql.append(productId);
	//sql.append(tr("','"));
	//sql.append(stationId);
	//sql.append(tr("','"));
	//sql.append(productPath);
	//sql.append(tr("','"));

	//转换date
	//QDate tDate(1969,12,31);
	//tDate = tDate.addDays(static_cast<int>(productDate.toInt()));
	//sql.append(productDate/*tDate.toString("yyyy-MM-dd")*/);

	//转换time
	//QTime time = QTime::fromString(productTime, "hhmmss");
	//sql.append(tr("','"));
	//sql.append(productTime/*time.toString("hh:mm:ss")*/);

	//sql.append(tr("')"));
	bool bsuccess = query.exec(strSql);
	QString querys = query.lastError().text();

	qDebug()<<querys;
	return bsuccess;
}

bool SqliteManager::InsertProductTb(const QString& guid,const QString& productId,const QString& productName)
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_sqlite.isOpen())
	{
		m_db_sqlite.open();
	}
	QSqlQuery query(m_db_sqlite);//以下执行相关QSL语句
	//QString sql = tr("insert into productTb values('814E4C76-3D2F-4DEB-8CD5-826CDDEDA284','CAP','')");
	QString sql;
	sql.append(tr("insert into productTb values"));
	sql.append(tr("('"));
	sql.append(guid);
	sql.append(tr("','"));
	sql.append(productId);
	sql.append(tr("','"));
	sql.append(productName);
	sql.append(tr("')"));
	bool bsuccess = query.exec(sql);
	return bsuccess;
}

bool SqliteManager::InsertStationTb(const QString& guid,
	const QString& stationId,
	const QString& stationName,
	const QString& cProvince,
	const QString& cRadType,

	const int& nLat,
	const int& nLon,
	const int& nHei,
	const int& nDataRange,
	const int& nTime,

	const int& nWestLon,
	const int& nEastLon,
	const int& nNorthLat,
	const int& nSouthLat,

	const QString& cSpare)
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_sqlite.isOpen())
	{
		m_db_sqlite.open();
	}
	QSqlQuery query(m_db_sqlite);
	//QString sql = tr("insert into stationTb values('814E4C76-3D2F-4DEB-8CD5-826CDDEDA284','z9070','三亚')");
	QString sql;
	sql.append(tr("insert into stationTb values"));
	sql.append(tr("('"));
	sql.append(guid);
	sql.append(tr("','"));
	sql.append(stationId);
	sql.append(tr("','"));
	sql.append(stationName);
	sql.append(tr("','"));
	sql.append(cProvince);
	sql.append(tr("','"));
	sql.append(cRadType);
	sql.append(tr("',"));
	sql.append(QString::number(nLat));
	sql.append(tr(","));
	sql.append(QString::number(nLon));
	sql.append(tr(","));
	sql.append(QString::number(nHei));
	sql.append(tr(","));
	sql.append(QString::number(nDataRange));
	sql.append(tr(","));
	sql.append(QString::number(nTime));
	sql.append(tr(","));
	sql.append(QString::number(nWestLon));
	sql.append(tr(","));
	sql.append(QString::number(nEastLon));
	sql.append(tr(","));
	sql.append(QString::number(nNorthLat));
	sql.append(tr(","));
	sql.append(QString::number(nSouthLat));
	sql.append(tr(",'"));
	sql.append(cSpare);
	sql.append(tr("')"));
	bool bsuccess = query.exec(sql);
	return bsuccess;
}


QList<QList<QString>> SqliteManager::QueryRadarListDB( const QString& tbName , QString strSql )
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_sqlite.isOpen())
	{
		m_db_sqlite.open();
	}
	QSqlQuery query(m_db_sqlite);
	QList<QList<QString>> fList;

	QString sql;
	if (tbName == tr("radarProductTb"))//主表
	{
		sql = tr("select * from radarProductTb ");
	}
	else if (tbName == tr("stationTb"))//站点名称子表
	{
		sql = tr("select * from stationTb ");
	}
	else if (tbName == tr("productTb"))//产品类型子表
	{
		sql = tr("select * from productTb ");
	}
	else if (tbName == tr("r_work_station"))
	{
		sql = tr("select * from  r_work_station");
	}
	sql.append(strSql).toLower();
	bool bsuccess = query.exec(sql);
	if (!bsuccess)
	{
		return fList;
	}
	while(query.next())
	{

		QList<QString> cList;
		QString guid=query.value(0).toString();

		if (tbName == tr("radarProductTb"))//主表
		{
			QString productId=query.value(1).toString();
			QString stationId=query.value(2).toString();
			QString productPath=query.value(3).toString();
			QString productDate = query.value(4).toString();
			QString productTime=query.value(5).toString();
			cList.push_back(guid);
			cList.push_back(productId);
			cList.push_back(stationId);
			cList.push_back(productPath);
			cList.push_back(productDate);
			cList.push_back(productTime);
			fList.push_back(cList);
		}
		else if (tbName == tr("stationTb"))//站点名称子表
		{
			QString stationId=query.value(1).toString();
			QString stationName=query.value(2).toString();
			cList.push_back(guid);
			cList.push_back(stationId);
			cList.push_back(stationName);
			fList.push_back(cList);
		}
		else if (tbName == tr("productTb"))//产品类型子表
		{
			QString productId=query.value(1).toString();
			QString productName=query.value(2).toString();
			cList.push_back(guid);
			cList.push_back(productId);
			cList.push_back(productName);
			fList.push_back(cList);
		}	
		else if (tbName == tr("r_work_station"))
		{
			QString name=query.value(2).toString();
			QString lon =query.value(3).toString();
			QString lat = query.value(4).toString();

			cList.push_back(name);
			cList.push_back(lon);
			cList.push_back(lat);
			fList.push_back(cList);
		}
	}
	return fList;
}
