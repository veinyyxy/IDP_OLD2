/*============================================================ 
文件名：RadReadColor_XML.h  
类 名： CRadReadColor_XML
父 类： 无
子 类： 无 
功能说明：从XML数据文件中读取站点信息
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2012年03月08日
============================================================*/

#include "RadReadColor_XML.h"
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QDebug>
#include <QtCore/QFile>
CRadReadColor_XML::CRadReadColor_XML(void)
{	
}

CRadReadColor_XML::~CRadReadColor_XML(void)
{
	if(!m_ColorTableHash.isEmpty())
	{
		COLORTABLE_HASH_ITERATOR iter(m_ColorTableHash);
		P_COLORTABLE_VECTOR pVector;
		int i, nLen;
		while(iter.hasNext())
		{
			iter.next();
			pVector = iter.value();
			nLen = pVector->size();
			i = 0;
			while(i<nLen)
			{
				delete pVector->at(i);
				//pVector->at(i) = NULL;
				i++;
			}
			pVector->clear();
			delete pVector;
			pVector = NULL;
		}//while
		m_ColorTableHash.clear();
	}
}
/************************************************************************
 功能说明：读xml文件
 作    者：RENXUE
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadColor_XML::ReadFile( const QString &fileName)
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
			ReadColorTable(m_XMLreader);
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
 功能说明：处理颜色表元素tr的内容
 作    者：renxue
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadColor_XML::ReadColorTable(QXmlStreamReader &xml)
{
	QString strID;
	QXmlStreamAttributes attbutes;

	//读表中的产品标号
	attbutes = xml.attributes();
	if(!attbutes.hasAttribute("name")) return false;
	strID = attbutes.value("", "name").toString();

	//读产品的颜色表
	P_COLORTABLE_VECTOR pColorVector = new COLORTABLE_VECTOR;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isStartElement() && xml.name() == "tr")
		{
			ReadItem(xml, pColorVector);
		}	
		if (xml.isEndElement() && xml.name() == "table")
		{
			m_ColorTableHash.insert(strID.toInt(), pColorVector);
			return true;
		}
	}//while
	return false;
}

/************************************************************************
 功能说明：处理颜色表元项td的内容
 作    者：renxue
 完成日期：2012年3月09日
************************************************************************/
bool CRadReadColor_XML::ReadItem(QXmlStreamReader &xml, P_COLORTABLE_VECTOR pVector)
{
	if(pVector == NULL)  return false;

	QXmlStreamAttributes attributes;
	QString strText;
	float   fStart, fEnd;
	int     nRed, nGreen, nBlue;
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
			if(strText == "start")
			{
				xml.skipCurrentElement();
				return false;
			}
		
			switch(i)
			{
			case 0:
				fStart = strText.toDouble();
				break;
			case 1:
				fEnd = strText.toDouble();
				break;
			case 2:
				nRed = strText.toInt();
				break;
			case 3:
				nGreen = strText.toInt();
				break;
			case 4:
				nBlue = strText.toInt();
				break;
			default:
				break;
			}	
			i++;
		}//if  
		if(i == 5) break;
	}//while

	P_COLORNODE_PRODUCT pColorNode = new COLORNODE_PRODUCT;
	pColorNode->fStart = fStart;
	pColorNode->fEnd = fEnd;
	pColorNode->nRed = nRed;
	pColorNode->nGreen = nGreen;
	pColorNode->nBlue = nBlue;
	pVector->push_back(pColorNode);
	//m_ColorTable.insert(fStart, )
	return true;
}

//for debug
void CRadReadColor_XML::PrintHash()
{
	COLORTABLE_HASH_ITERATOR locIter(m_ColorTableHash);
	while(locIter.hasNext())
	{
		locIter.next();
		qDebug()<<locIter.key()<<endl;
		PrintColor(locIter.value());
	}
}

//for debug
void CRadReadColor_XML::PrintColor(P_COLORTABLE_VECTOR pVet)
{
	int i, nLen;
	nLen = pVet->size();
	P_COLORNODE_PRODUCT p;
	for(i=0; i<nLen; i++)
	{
		p = pVet->at(i);
		qDebug()<<"start："<<p->fStart<<"  "<<"end:"<<p->fEnd<<endl;
		qDebug()<<p->nRed<<"  "<<p->nGreen<<"  "<<p->nBlue<<"  "<<endl;
	}
}