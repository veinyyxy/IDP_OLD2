#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>
#include<QDebug>
#include <QSqlError>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QVariant>

#include "ODBCManager.h"

ODBCManager::ODBCManager()
{
	InitDBXMLConfig();

	m_db_odbc = QSqlDatabase::addDatabase("QODBC",tr("QODBC"));
	QString dsn = m_dsn;
	m_db_odbc.setDatabaseName(dsn);
	m_db_odbc.setHostName(m_host);
	m_db_odbc.setUserName(m_user);
	m_db_odbc.setPassword(m_passwd);
	if (!m_db_odbc.open()) 
	{
		QSqlError err = m_db_odbc.lastError();
		m_db_odbc.close();
		QMessageBox::critical(NULL,tr("提示"),err.text());
	}
}

ODBCManager::~ODBCManager()
{

}

Q_GLOBAL_STATIC(ODBCManager, odbcManager)
	ODBCManager *ODBCManager::Instance() 
{ 
	return odbcManager(); 
}

bool ODBCManager::StartExeSQL(const QString& strSql)
{
	QMutexLocker locker(&m_db_lock);
	if (!m_db_odbc.isOpen())
	{
		m_db_odbc.open();
	}
	QSqlQuery query(m_db_odbc);//以下执行相关QSL语句
	bool bsuccess = false;
	bsuccess = query.exec(strSql);
	return bsuccess;
}


void ODBCManager::InitDBXMLConfig()
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
				else if (e.attribute("name") == "dsn") m_dsn = e.attribute("value");
			}
			n = n.nextSibling();
		}
	}
	file.close();
}