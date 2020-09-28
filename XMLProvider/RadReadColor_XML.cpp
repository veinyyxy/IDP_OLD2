/*============================================================ 
�ļ�����RadReadColor_XML.h  
�� ���� CRadReadColor_XML
�� �ࣺ ��
�� �ࣺ �� 
����˵������XML�����ļ��ж�ȡվ����Ϣ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2012��03��08��
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
 ����˵������xml�ļ�
 ��    �ߣ�RENXUE
 ������ڣ�2012��3��09��
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
 ����˵����������ɫ��Ԫ��tr������
 ��    �ߣ�renxue
 ������ڣ�2012��3��09��
************************************************************************/
bool CRadReadColor_XML::ReadColorTable(QXmlStreamReader &xml)
{
	QString strID;
	QXmlStreamAttributes attbutes;

	//�����еĲ�Ʒ���
	attbutes = xml.attributes();
	if(!attbutes.hasAttribute("name")) return false;
	strID = attbutes.value("", "name").toString();

	//����Ʒ����ɫ��
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
 ����˵����������ɫ��Ԫ��td������
 ��    �ߣ�renxue
 ������ڣ�2012��3��09��
************************************************************************/
bool CRadReadColor_XML::ReadItem(QXmlStreamReader &xml, P_COLORTABLE_VECTOR pVector)
{
	if(pVector == NULL)  return false;

	QXmlStreamAttributes attributes;
	QString strText;
	float   fStart, fEnd;
	int     nRed, nGreen, nBlue;
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
		qDebug()<<"start��"<<p->fStart<<"  "<<"end:"<<p->fEnd<<endl;
		qDebug()<<p->nRed<<"  "<<p->nGreen<<"  "<<p->nBlue<<"  "<<endl;
	}
}