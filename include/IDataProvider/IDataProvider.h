#pragma once
#include <QtPlugin>
#include <QHash>
#include <qstringlist.h>

class CElement;

class IDataProvider
{
public:
	typedef QHash<QString, CElement*> HashElement;
	virtual ~IDataProvider() {}
	virtual  QStringList keys() = 0;
	virtual bool LoadFile(const std::string& filename) = 0;
	//virtual bool LoadFile(const QString& filePath) = 0;//装载数据源文件，初始化各种辅助信息
	virtual HashElement GetAllElements(const QString& filePath) const = 0; //获取所有要素信息
	virtual CElement*  GetElement(const QString&  elementName)=0;//获取要素
	virtual bool StoreDetailValues(  CElement* pElement)=0;//存储相关要素所有数据
	virtual int Write(const QString& fileTypeName) = 0;//写文件
	virtual bool CloseFileAndRemoveElements(const QString& fileName)=0;
	virtual QString GetDataDescription(const QString& filePath) = 0; //若是Micaps，返回数据描述信息，否则返回 QString（）空串
	virtual QStringList GetElementNames(const QString& filePath) = 0; ////若是Micaps，返回元素名称列表，否则返回 QStringList（）空列表

};
Q_DECLARE_INTERFACE(IDataProvider, "css.IDataProvider.IDataProvider/1.0")
