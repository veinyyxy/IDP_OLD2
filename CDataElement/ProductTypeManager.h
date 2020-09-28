#pragma once
#include "cdataelement_global.h"
#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>

class CProductType;

class  CDATAELEMENT_EXPORT  CProductTypeManager
{
public:
	static CProductTypeManager* GetInstance();
	~CProductTypeManager(void);
	QList<int> Keys();
	QStringList ChineseNames();

	int Key(const QString& ChinseName) const;
	QString ChineseName(int) const;
private:
	CProductTypeManager(void);
	void AddProduct(CProductType* pProduct);
private:
	static CProductTypeManager* m_pPTMgr;
	QHash<int, QString> m_keyNames;
	QHash<int, CProductType*> m_keyProducts;
};

