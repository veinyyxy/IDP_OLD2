/*============================================================ 
文件名：RadReadProduct_XML.h  
类 名： CRadReadProduct_XML
父 类： 无
子 类： 无 
功能说明：从XML数据文件中读取产品信息
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2012年03月09日
============================================================*/

#include "RadReadProduct_XML.h"
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QDebug>
#include <QtCore/QFile>

CRadReadProduct_XML::CRadReadProduct_XML(void)
{	
}

CRadReadProduct_XML::~CRadReadProduct_XML(void)
{
	if(!m_ProductHash.isEmpty())
	{
		PRODUCT_HASH_ITERATOR iter(m_ProductHash);
		
		while(iter.hasNext())
		{
			iter.next();
			delete iter.value();
		}//while
		m_ProductHash.clear();
	}
}
/************************************************************************
 功能说明：读xml文件
 作    者：RENXUE
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadProduct_XML::ReadFile( const QString &fileName)
{
	QFile file(fileName);	
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}
	m_XMLreader.setDevice(&file);

	while(!m_XMLreader.atEnd())
	{
		m_XMLreader.readNext();
		if (m_XMLreader.isStartElement() && m_XMLreader.name() == "table")
		{
			ReadTable(m_XMLreader);
		}		
	}
	file.close();
	if (m_XMLreader.hasError())
	{
		return false;
	}
	else if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 功能说明：处理产品表元素tr的内容
 作    者：renxue
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadProduct_XML::ReadTable(QXmlStreamReader &xml)
{
	QString strID;
	QXmlStreamAttributes attbutes;

	//读表中的产品标号
	attbutes = xml.attributes();
	if(!attbutes.hasAttribute("name")) return false;
	m_strType = attbutes.value("", "name").toString();

	//读产品的信息
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isStartElement() && xml.name() == "tr")
		{
			ReadItem(xml);
		}	
		if (xml.isEndElement() && xml.name() == "table")
		{
			return true;
		}
	}//while
	return false;
}

/************************************************************************
 功能说明：处理产品项td的内容
 作    者：renxue
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadProduct_XML::ReadItem(QXmlStreamReader &xml)
{
	QXmlStreamAttributes attributes;
	QString strText, strAbb, strName, strUnit;
	int     nID;
	int i = 0;

	//读产品颜色块
	while (!xml.atEnd())
	{
		xml.readNext();  
		if(xml.isStartElement() && xml.name() == "td")  
		{  
			attributes = xml.attributes();
			if(!attributes.hasAttribute("class")) return false;

			strText = xml.readElementText();
			if(strText == "产品号")
			{
				xml.skipCurrentElement();
				return false;
			}
		
			switch(i)
			{
			case 0:
				nID = strText.toInt();
				break;
			case 1:
				strAbb = strText;
				break;
			case 2:
				strName = strText;
				break;
			case 3:
				strUnit = strText;
				break;
			default:
				break;
			}	
			i++;
		}//if  
		if(i == 4) break;
	}//while

	P_PRODUCT pProduct = new PRODUCT;
	pProduct->nID = nID;
	pProduct->strAbb = strAbb;
	pProduct->strName = strName;
	pProduct->strUnit = strUnit;
	m_ProductHash.insert(nID, pProduct);
	return true;
}

//for debug
void CRadReadProduct_XML::PrintHash()
{
	PRODUCT_HASH_ITERATOR locIter(m_ProductHash);
	while(locIter.hasNext())
	{
		locIter.next();
		qDebug()<<locIter.key()<<endl;
		PrintProduct(locIter.value());
	}
}

//for debug
void CRadReadProduct_XML::PrintProduct(P_PRODUCT p)
{
	qDebug()<<"编号："<<p->nID<<"  "<<"缩写:"<<p->strAbb;
	qDebug()<<"名字："<<p->strName<<"  "<<"单位:"<<p->strUnit<<endl;
}