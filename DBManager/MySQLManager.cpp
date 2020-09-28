#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>
#include<QDebug>
#include <QSqlError>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QVariant>

#include "MySQLManager.h"

MySQLManager::MySQLManager()
{
	InitDBXMLConfig();

	m_db_mysql = QSqlDatabase::addDatabase("QMYSQL","QMYSQL");
	m_db_mysql.setDatabaseName(m_dbName);
	m_db_mysql.setHostName(m_host);
	m_db_mysql.setPort(m_port);
	m_db_mysql.setUserName(m_user);
	m_db_mysql.setPassword(m_passwd);
	if (!m_db_mysql.open()) 
	{
		QSqlError err = m_db_mysql.lastError();
		m_db_mysql.close();
		QMessageBox::critical(NULL,tr("提示"),err.text());
	}
}

MySQLManager::~MySQLManager()
{

}

Q_GLOBAL_STATIC(MySQLManager, mysqlManager)
	MySQLManager *MySQLManager::Instance() 
{ 
	return mysqlManager(); 
}


int MySQLManager::GetData_ID(QString code)
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_mysql.isOpen())
	{
		m_db_mysql.open();
	}
	QSqlQuery query(m_db_mysql);//以下执行相关QSL语句
	QString sql = QObject::tr("SELECT pk_id FROM r_radar_data where code='%1'").arg(code);
	bool bsuccess = query.exec(sql);
	while(query.next())
	{
		return query.value(0).toInt();
	}
	return 0;
}

bool MySQLManager::Insert_r_productTb( const QString& strSql )
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_mysql.isOpen())
	{
		m_db_mysql.open();
	}
	QSqlQuery query(m_db_mysql);//以下执行相关QSL语句
	bool bsuccess = query.exec(strSql);
	return bsuccess;
}

void MySQLManager::InitDBXMLConfig()
{
	QString filepath = QCoreApplication::applicationDirPath()+tr("/Config/DB.xml");
	QFile file(filepath);
	QXmlStreamReader  xmlReader;
	if(file.open(QFile::ReadOnly|QFile::Text))
	{
		xmlReader.setDevice(&file);
		QDomDocument pDomDoc;
		if (!pDomDoc.setContent(&file)) 
		{
			file.close();
			return;
		}
		QDomElement docElem = pDomDoc.documentElement();
		QDomNode n = docElem.firstChild();
		while (!n.isNull()) 
		{
			QDomElement e = n.toElement(); //将节点转换为元素
			if (!e.isNull()) 
			{
				if (e.attribute("name") == "dbName") m_dbName = e.attribute("value");
				else if (e.attribute("name") == "host") m_host = e.attribute("value");
				else if (e.attribute("name") == "user") m_user = e.attribute("value");
				else if (e.attribute("name") == "passwd") m_passwd = e.attribute("value");
				else if (e.attribute("name") == "port") m_port = e.attribute("value").toInt();
			}
			n = n.nextSibling();
		}
	}
	file.close();
}
