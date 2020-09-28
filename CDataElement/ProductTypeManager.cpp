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
	//NETCDF�ļ���ʽ
	AddProduct(new CProductType(1,QObject::tr("��ֵ��")));
	AddProduct(new CProductType(2,QObject::tr("ɫ��ͼ")));
	AddProduct(new CProductType(3,QObject::tr("��ʸͼ")));
	AddProduct(new CProductType(4,QObject::tr("����ͼ")));
	AddProduct(new CProductType(5,QObject::tr("����ͼ")));
	AddProduct(new CProductType(22,QObject::tr("���ͼ")));
	AddProduct(new CProductType(23,QObject::tr("����ͼ")));
	AddProduct(new CProductType(24,QObject::tr("�� ͼ")));
	//MICAPS3�ļ���ʽ
	AddProduct(new CProductType(6,QObject::tr("վ��ͼ")));
	AddProduct(new CProductType(7,QObject::tr("��ֵ��")));
	AddProduct(new CProductType(8,QObject::tr("ɫ��ͼ")));

	//MICAPS1�ļ���ʽ
	AddProduct(new CProductType(9,QObject::tr("վ��ͼ")));
	AddProduct(new CProductType(16,QObject::tr("������ͼ")));

	//MICAPS2�ļ���ʽ
	AddProduct(new CProductType(10,QObject::tr("վ��ͼ")));
	AddProduct(new CProductType(17,QObject::tr("�߿���ͼ")));

	//MICAPS4�ļ���ʽ
	AddProduct(new CProductType(11,QObject::tr("���ͼ")));
	AddProduct(new CProductType(14,QObject::tr("��ֵ��")));
	AddProduct(new CProductType(15,QObject::tr("ɫ��ͼ")));

	//MICAPS5�ļ���ʽ վ��ͼ��ֵ��ɫ��ͼ
	AddProduct(new CProductType(12,QObject::tr("ʱ������ͼ")));
	AddProduct(new CProductType(13,QObject::tr("�ռ�����ͼ")));
	
	//MICAPS7�ļ���ʽ ̨��·��
	AddProduct(new CProductType(21,QObject::tr("̨��·��")));

	//MICAPS11�ļ���ʽ
	AddProduct(new CProductType(18,QObject::tr("��ʸͼ")));
	AddProduct(new CProductType(19,QObject::tr("����ͼ")));
	AddProduct(new CProductType(20,QObject::tr("����ͼ")));

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





