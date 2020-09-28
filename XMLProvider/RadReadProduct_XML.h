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
#pragma once
#include "XML_DPConfig.h"
#include <QtCore/QXmlStreamReader>
#include <QtGui/QVector3D>
#include <QtCore/QHash>

typedef struct Product
{
	int   nID;
	QString strAbb;//产品缩写
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
	QString       m_strType;//用于区别单站产品还是组网产品
};

