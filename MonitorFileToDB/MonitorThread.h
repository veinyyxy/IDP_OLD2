#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QtGui>
#include <QThread>
#include "dbmanager.h"
#include "sqlitemanager.h"
#include "MySQLManager.h"
#include "FileSystemWatcher.h"
#include "DataProvider.h"

class MySQLDBPNG : public QRunnable
{
public:
	MySQLDBPNG(MySQLManager* pDB,QString path,QString productId,QString ts);
	~MySQLDBPNG();
	void run();
protected:
	QMutex m_lock;
	MySQLManager* m_pDB;
	QString m_path;
	QString m_productId;
	QString m_ts;
};

class MonitorThread : public QThread
{
public:
	MonitorThread(QStringList pathList);
	MonitorThread(QString path);
	~MonitorThread();
	void StartMonitorThread();
protected:
	void run();
	//void ReadDirToList();
	void ReadDirToList(const QString& path);
	void DataAnalyzeToDB(QString fileName);
	void InitDBManager();
	
	QStringList Bz2FileNameAnalyes( QString fName );
	QStringList BinFileNameAnalyes( QString fName );
	 void  static OnFileCreate(const cFileSystemEvent& evt);
private:
	QStringList m_fsPathList;
	QString m_path;
	QMutex m_lock;
	static SqliteManager* m_pSqliteManager;
	static MySQLManager* m_pMySQLManager;
	cFileSystemWatcher* watcher;
};

#endif // MONITORTHREAD_H
