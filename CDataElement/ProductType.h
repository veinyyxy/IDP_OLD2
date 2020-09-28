#pragma once
#include "cdataelement_global.h"
#include <QString>

/*****
CIPAS ��Ʒ����
******/

class  CDATAELEMENT_EXPORT CProductType
{
public:
	CProductType();
	CProductType(int key, QString chName);
	CProductType(const CProductType& other);
	CProductType& operator =(const CProductType& other);
	~CProductType(void);

	// key �Ƿ���Ч
	bool Valid() const;
	int Key() const;
	QString ChineseName() const;

private:
	int m_key; // ��Ʒ����Ψһ��ʾ�� ���� 0 ��Ч��������Ч
	QString m_chName; // ��Ʒ��������

};

