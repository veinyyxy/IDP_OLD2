#pragma once
#include "cdataelement_global.h"
#include <QString>

/*****
CIPAS 产品类型
******/

class  CDATAELEMENT_EXPORT CProductType
{
public:
	CProductType();
	CProductType(int key, QString chName);
	CProductType(const CProductType& other);
	CProductType& operator =(const CProductType& other);
	~CProductType(void);

	// key 是否有效
	bool Valid() const;
	int Key() const;
	QString ChineseName() const;

private:
	int m_key; // 产品类型唯一标示， 大于 0 有效，否则无效
	QString m_chName; // 产品类型名称

};

