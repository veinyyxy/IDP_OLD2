#include <QCoreApplication>
#include <QMessageBox>
#include <QFile>
#include<QDebug>
#include <QSqlError>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QVariant>
#include "OracleManager.h"

OracleManager::OracleManager()
{
	InitDBXMLConfig();

	m_db_oraclel = QSqlDatabase::addDatabase("QOCI","QOCI");
	m_db_oraclel.setDatabaseName(m_dbName);
	m_db_oraclel.setHostName(m_host);
	m_db_oraclel.setPort(m_port);
	m_db_oraclel.setUserName(m_user);
	m_db_oraclel.setPassword(m_passwd);
	if (!m_db_oraclel.open()) 
	{
		QSqlError err = m_db_oraclel.lastError();
		m_db_oraclel.close();
		QMessageBox::critical(NULL,tr("提示"),err.text());
	}
}

OracleManager::~OracleManager()
{

}

Q_GLOBAL_STATIC(OracleManager, oracleManager)
	OracleManager *OracleManager::Instance() 
{ 
	return oracleManager(); 
}


void OracleManager::InitDBXMLConfig()
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
