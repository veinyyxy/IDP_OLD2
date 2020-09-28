#include "ProductTypeManager.h"
#include "ProductType.h"

CProductTypeManager* CProductTypeManager::m_pPTMgr = 0;

CProductTypeManager* CProductTypeManager::GetInstance()
{
	if(!m_pPTMgr)
	{
		m_pPTMgr = new CProductTypeManager();
	}

	return m_pPTMgr;
}

CProductTypeManager::~CProductTypeManager()
{
	m_keyNames.clear();
	int key = 0;
	CProductType *pProduct = 0;
	while(!m_keyProducts.isEmpty())
	{
		key =  m_keyProducts.begin().key();
		CProductType *pProduct = m_keyProducts.take(key);
		if(pProduct)
		{
			delete pProduct;
			pProduct = 0;
		}
	}
}

QList<int> CProductTypeManager::Keys()
{
	QList<int> keys;
	CProductType *pProduct = 0;
	QHash<int, CProductType*>::const_iterator it = m_keyProducts.constBegin();
	for(; it != m_keyProducts.constEnd(); ++it)
	{
		pProduct = it.value();
		if(pProduct && pProduct->Valid())
		{
			keys << pProduct->Key();
		}
	}

	return keys;
}

QStringList CProductTypeManager::ChineseNames()
{
	QList<QString> names;
	CProductType *pProduct = 0;
	QHash<int, CProductType*>::const_iterator it = m_keyProducts.constBegin();
	for(; it != m_keyProducts.constEnd(); ++it)
	{
		pProduct = it.value();
		if(pProduct && pProduct->Valid())
		{
			names << pProduct->ChineseName();
		}
	}

	return names;
}

int CProductTypeManager::Key(const QString& ChineseName) const
{
	return m_keyNames.key(ChineseName);
}

QString CProductTypeManager::ChineseName(int key) const
{
	return m_keyNames.value(key);
}

CProductTypeManager::CProductTypeManager(void)
{
	//AddProduct( new CProcudtType(key, name));
	//NETCDF文件格式
	AddProduct(new CProductType(1,QObject::tr("等值线")));
	AddProduct(new CProductType(2,QObject::tr("色斑图")));
	AddProduct(new CProductType(3,QObject::tr("风矢图")));
	AddProduct(new CProductType(4,QObject::tr("风羽图")));
	AddProduct(new CProductType(5,QObject::tr("流线图")));
	AddProduct(new CProductType(22,QObject::tr("格点图")));
	AddProduct(new CProductType(23,QObject::tr("折线图")));
	AddProduct(new CProductType(24,QObject::tr("云 图")));
	//MICAPS3文件格式
	AddProduct(new CProductType(6,QObject::tr("站点图")));
	AddProduct(new CProductType(7,QObject::tr("等值线")));
	AddProduct(new CProductType(8,QObject::tr("色斑图")));

	//MICAPS1文件格式
	AddProduct(new CProductType(9,QObject::tr("站点图")));
	AddProduct(new CProductType(16,QObject::tr("地面填图")));

	//MICAPS2文件格式
	AddProduct(new CProductType(10,QObject::tr("站点图")));
	AddProduct(new CProductType(17,QObject::tr("高空填图")));

	//MICAPS4文件格式
	AddProduct(new CProductType(11,QObject::tr("格点图")));
	AddProduct(new CProductType(14,QObject::tr("等值线")));
	AddProduct(new CProductType(15,QObject::tr("色斑图")));

	//MICAPS5文件格式 站点图等值线色斑图
	AddProduct(new CProductType(12,QObject::tr("时间剖面图")));
	AddProduct(new CProductType(13,QObject::tr("空间剖面图")));
	
	//MICAPS7文件格式 台风路径
	AddProduct(new CProductType(21,QObject::tr("台风路径")));

	//MICAPS11文件格式
	AddProduct(new CProductType(18,QObject::tr("风矢图")));
	AddProduct(new CProductType(19,QObject::tr("风羽图")));
	AddProduct(new CProductType(20,QObject::tr("流线图")));

}

void CProductTypeManager::AddProduct(CProductType* pProduct)
{
	if(pProduct && pProduct->Valid())
	{
		int key = pProduct->Key();
		if(!m_keyNames.contains(key))
		{
			m_keyNames.insert(key, pProduct->ChineseName());
		}

		if(!m_keyProducts.contains(key))
		{
			m_keyProducts.insert(key, pProduct);
		}
	}
}





