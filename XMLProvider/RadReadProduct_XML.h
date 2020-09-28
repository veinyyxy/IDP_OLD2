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
#pragma once
#include "XML_DPConfig.h"
#include <QtCore/QXmlStreamReader>
#include <QtGui/QVector3D>
#include <QtCore/QHash>

typedef struct Product
{
	int   nID;
	QString strAbb;//��Ʒ��д
	QString strName;
	QString strUnit;
}PRODUCT, *P_PRODUCT;

typedef QHash<int, P_PRODUCT>  PRODUCT_HASH;
typedef QHashIterator<int, P_PRODUCT> PRODUCT_HASH_ITERATOR;

class  XML_EXPORT_IMPORT CRadReadProduct_XML 
{
public:
	CRadReadProduct_XML(void);
	~CRadReadProduct_XML(void);

	bool ReadFile(const QString &fileName);		
	inline PRODUCT_HASH ProductTable(){return m_ProductHash;}
	inline QString      ProductType(){return m_strType;}
	void PrintHash();
	void PrintProduct(P_PRODUCT p);

private:	
	bool ReadTable(QXmlStreamReader &xml);
	bool ReadItem(QXmlStreamReader &xml);

private:
	QXmlStreamReader m_XMLreader;
	PRODUCT_HASH  m_ProductHash;
	QString       m_strType;//��������վ��Ʒ����������Ʒ
};

