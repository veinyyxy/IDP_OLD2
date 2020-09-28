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
	//virtual bool LoadFile(const QString& filePath) = 0;//װ������Դ�ļ�����ʼ�����ָ�����Ϣ
	virtual HashElement GetAllElements(const QString& filePath) const = 0; //��ȡ����Ҫ����Ϣ
	virtual CElement*  GetElement(const QString&  elementName)=0;//��ȡҪ��
	virtual bool StoreDetailValues(  CElement* pElement)=0;//�洢���Ҫ����������
	virtual int Write(const QString& fileTypeName) = 0;//д�ļ�
	virtual bool CloseFileAndRemoveElements(const QString& fileName)=0;
	virtual QString GetDataDescription(const QString& filePath) = 0; //����Micaps����������������Ϣ�����򷵻� QString�����մ�
	virtual QStringList GetElementNames(const QString& filePath) = 0; ////����Micaps������Ԫ�������б����򷵻� QStringList�������б�

};
Q_DECLARE_INTERFACE(IDataProvider, "css.IDataProvider.IDataProvider/1.0")
