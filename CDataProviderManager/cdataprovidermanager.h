/*
* 功能：数据文件管理，对不同类型的文件提供不同的插件
* 日期：2011-04-14
* 作者：韩猛
*
*/

#ifndef CDATAPROVIDERMANAGER_H
#define CDATAPROVIDERMANAGER_H

#include "cdataprovidermanager_global.h"
#include <qstringlist.h>
#include <qhash.h>
#include <QList>
#include <QTextCodec>

#include "..\include\\IDataProvider\IDataProvider.h"
#include "../CDataElement/CElement.h"



class CDATAPROVIDERMANAGER_EXPORT CDataProviderManager : public QObject
{
		Q_OBJECT
public:

	~CDataProviderManager();
	static CDataProviderManager* GetDPManager();
	const QStringList& SuffixNames(); //支持的文件名后缀

	// 本地文件
	QHash<QString, CElement*> GetAllElement(const QString&fileName); // 获得要素名称与要素对应关系
	QHash<QString, CElement*> GetAllElementNoListW(const QString&fileName); // 获得要素名称与要素对应关系
	QStringList GetAllElementNames(const QString&fileName); //获得要素名称
	QStringList GetAllElementChineseNames(const QString&fileName); //获得要素名称（中文)
	QStringList GetAllElementChineseNamesNoListW(const QString&fileName); //获得要素名称（中文)但不灌入列表
	CElement* GetElementByName(const QString &elementName, const QString&fileName); // 根据名称，获得要素信息
	CElement* GetElementByChineseName(const QString &elementName, const QString&fileName); // 根据中文名称，获得要素信息
	bool ReadDataFromFile(CElement* pElement);
	//int Write(const QString& fileTypeName); //写文件
	int Write(const QString& fileName, QList<CElement*> elements_v);//写文件
	bool CloseFileAndRemoveElements(const QString& fileName);
	QStringList GetAllOpenedFiles();
	QString GetFileDescription(const QString&fileName);
	QString GetFileTypeFromXMl(const QString&suffixName);
	int KillNoListElement(); //删除分析工具箱用到但不显示的数据

	// 远程数据库
	QStringList GetAllElementNamesFromDB(); //获得要素名称（中文)
	QStringList GetAllElementChineseNamesFromDB();
	CElement* GetElementByNameFromDB(const QString &elementName); // 根据名称，获得要素信息
	CElement* GetElementByChineseNameFromDB(const QString &elementName); // 根据中文名称，获得要素信息
	int WriteCipas(const QString& fileName, QList<CElement*> elements_v);//写文件
	int WriteCiapsRaster(const QString& fileName, QList<CElement*> elements_v);//写文件
	int WriteMicaps3(const QString& fileName, QList<CElement*> elements_v);//写文件
	int WriteMicaps4(const QString& fileName, QList<CElement*> elements_v);//写文件
	void InitElementsFromDB();

	// 内存数据
	bool AddMemoryElements(QList<CElement*> elements);
	bool AddMemoryElement(CElement* pElement);

	//本地指数文件
	QHash<QString, QString> GetAllExponentElements();

	//过滤器设置
	void OpenFilter();
	void CloseFilter();
	bool Filter() const;

	bool FileHasRead(const QString& fileName); //判断文件是否已经读取
signals:
	void OpenedFilesCountChanged(const QString& fileName, bool addFile, bool isMemory);
private:
	CDataProviderManager();
	bool LoadFiles(const QStringList &fileNames); //加载插件，解析文件
	IDataProvider* GetDataProvider(const QString& fileName); //根据数据源文件名，加载 DLL 动态库
	IDataProvider* CreateDataProvider(const QString& dllName, const QString& path = "IDataProvider"); //加载 DLL 动态库
	const QStringList& GetDLLNames(const QString& path = "IDataProvider"); //获得所有动态库名称
	QString DLLFileName(const QString& type); //根据文件类型，获得 DLL 动态库名称
	QString FileType(const QString& fileName); //获得文件类型
	QString MicapsFileType(const QString& sfilename); //获得MICAPS文件类型
	QString TxtFileType(const QString& sfilename); //获得Txt文件类型
	QString CpsFileType(const QString& sfilename); //获取cps文件类型
	QString AWXFileType(const QString& sfilename); //获取awx文件类型

private:
	bool m_filter;
	QStringList m_dllNames;   //所有的 DLL 动态库名称
	QStringList m_suffixNames; // 文件后缀名称
	QStringList m_FileNames; // 已经打开本地文件名列表与ElementsFileMap文件名列表一部分相同
	typedef QHash<QString, IDataProvider*> FileProviderMap; // 文件名与IDataProvider的映射关系
	//typedef QHash<QString, QString> ElementFileMap; // 要素名称与文件映射关系
	typedef QHash<QString, QString> ElementNamesMap; // 要素名称与中文名称映射关系
	typedef QHash<QString, CElement*> ElementNameObjectMap; //要素名称与对象映射关系
	typedef QHash<QString, CElement*> ElementsFileMap; //要素与文件名映射关系
	typedef QList<QString> ElementNames;
	typedef QHash<QString, ElementNames > SortedNamesMap;

	// 本地文件
	FileProviderMap m_fileProvidersMap;
	//ElementFileMap m_elementFilesMap;
	//ElementNamesMap m_namesMap;
	//ElementNameObjectMap m_nameObjectsMap;
	ElementsFileMap m_elementsFileMap;  //保留 “/", "\" 替换为"/"
	SortedNamesMap m_sortedNamesMap;

	// 分析工具箱文件
	QStringList m_FileNamesNoListW; // 分析工具箱已经打开本地文件名列表与ElementsFileMap文件名列表一部分相同

	//远程数据文件
	typedef QHash<QString, CElement*> NameElementMap; // 文件名与IDataProvider的映射关系
	QStringList m_elementNamesDB;
	ElementNamesMap m_namesMapDB;
	NameElementMap m_nameElementMapDB;

	static CDataProviderManager* s_pDPManager;
};

#endif // CDATAPROVIDERMANAGER_H