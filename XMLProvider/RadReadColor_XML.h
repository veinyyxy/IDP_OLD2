/*============================================================ 
文件名：RadReadColor_XML.h  
类 名： CRadReadColor_XML
父 类： QWidget
子 类： 无 
功能说明：从XML数据文件中读取产品的颜色信息
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2012年03月08日
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

