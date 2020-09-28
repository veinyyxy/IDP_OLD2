/*============================================================ 
�ļ�����RadReadProduct_XML.h  
�� ���� CRadReadProduct_XML
�� �ࣺ ��
�� �ࣺ �� 
����˵������XML�����ļ��ж�ȡ��Ʒ��Ϣ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2012��03��09��
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
 ����˵������xml�ļ�
 ��    �ߣ�RENXUE
 ������ڣ�2012��3��09��
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
 ����˵���������Ʒ��Ԫ��tr������
 ��    �ߣ�renxue
 ������ڣ�2012��3��09��
************************************************************************/
bool CRadReadProduct_XML::ReadTable(QXmlStreamReader &xml)
{
	QString strID;
	QXmlStreamAttributes attbutes;

	//�����еĲ�Ʒ���
	attbutes = xml.attributes();
	if(!attbutes.hasAttribute("name")) return false;
	m_strType = attbutes.value("", "name").toString();

	//����Ʒ����Ϣ
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
 ����˵���������Ʒ��td������
 ��    �ߣ�renxue
 ������ڣ�2012��3��09��
************************************************************************/
bool CRadReadProduct_XML::ReadItem(QXmlStreamReader &xml)
{
	QXmlStreamAttributes attributes;
	QString strText, strAbb, strName, strUnit;
	int     nID;
	int i = 0;

	//����Ʒ��ɫ��
	while (!xml.atEnd())
	{
		xml.readNext();  
		if(xml.isStartElement() && xml.name() == "td")  
		{  
			attributes = xml.attributes();
			if(!attributes.hasAttribute("class")) return false;

			strText = xml.readElementText();
			if(strText == "��Ʒ��")
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
	qDebug()<<"��ţ�"<<p->nID<<"  "<<"��д:"<<p->strAbb;
	qDebug()<<"���֣�"<<p->strName<<"  "<<"��λ:"<<p->strUnit<<endl;
}