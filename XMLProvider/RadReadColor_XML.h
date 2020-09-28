/*============================================================ 
�ļ�����RadReadColor_XML.h  
�� ���� CRadReadColor_XML
�� �ࣺ QWidget
�� �ࣺ �� 
����˵������XML�����ļ��ж�ȡ��Ʒ����ɫ��Ϣ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2012��03��08��
============================================================*/
#pragma once
#include "XML_DPConfig.h"
#include <QtGui/QPixmap>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QHash>
#include <QtGui/QVector3D>


typedef struct ColorNode_Product
{
	double fStart;
	double fEnd;
	int   nRed;
	int   nGreen;
	int   nBlue;
}COLORNODE_PRODUCT, *P_COLORNODE_PRODUCT;

typedef QVector<P_COLORNODE_PRODUCT>  COLORTABLE_VECTOR, *P_COLORTABLE_VECTOR;
typedef QHash<int, P_COLORTABLE_VECTOR>  COLORTABLE_HASH;
typedef QHashIterator<int, P_COLORTABLE_VECTOR> COLORTABLE_HASH_ITERATOR;

typedef struct SymbolLegend
{
	std::string labelText;
	QPixmap pixmap;
}SymbolLegend_Product, *P_SymbolLegend_Product;
typedef QVector<P_SymbolLegend_Product>  SymbolLegend_Vector, *P_SymbolLegend_Vector;

class  XML_EXPORT_IMPORT CRadReadColor_XML 
{
public:
	CRadReadColor_XML(void);
	~CRadReadColor_XML(void);

	bool ReadFile(const QString &fileName);		
	inline COLORTABLE_HASH ColorTable(){return m_ColorTableHash;}
	void PrintHash();
	void PrintColor(P_COLORTABLE_VECTOR pVet);

private:	
	bool ReadColorTable(QXmlStreamReader &xml);
	bool ReadItem(QXmlStreamReader &xml, P_COLORTABLE_VECTOR pVector);

private:
	QXmlStreamReader m_XMLreader;
	COLORTABLE_HASH  m_ColorTableHash;
};

