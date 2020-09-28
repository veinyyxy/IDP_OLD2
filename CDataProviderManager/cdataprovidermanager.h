/*
* ���ܣ������ļ������Բ�ͬ���͵��ļ��ṩ��ͬ�Ĳ��
* ���ڣ�2011-04-14
* ���ߣ�����
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
	const QStringList& SuffixNames(); //֧�ֵ��ļ�����׺

	// �����ļ�
	QHash<QString, CElement*> GetAllElement(const QString&fileName); // ���Ҫ��������Ҫ�ض�Ӧ��ϵ
	QHash<QString, CElement*> GetAllElementNoListW(const QString&fileName); // ���Ҫ��������Ҫ�ض�Ӧ��ϵ
	QStringList GetAllElementNames(const QString&fileName); //���Ҫ������
	QStringList GetAllElementChineseNames(const QString&fileName); //���Ҫ�����ƣ�����)
	QStringList GetAllElementChineseNamesNoListW(const QString&fileName); //���Ҫ�����ƣ�����)���������б�
	CElement* GetElementByName(const QString &elementName, const QString&fileName); // �������ƣ����Ҫ����Ϣ
	CElement* GetElementByChineseName(const QString &elementName, const QString&fileName); // �����������ƣ����Ҫ����Ϣ
	bool ReadDataFromFile(CElement* pElement);
	//int Write(const QString& fileTypeName); //д�ļ�
	int Write(const QString& fileName, QList<CElement*> elements_v);//д�ļ�
	bool CloseFileAndRemoveElements(const QString& fileName);
	QStringList GetAllOpenedFiles();
	QString GetFileDescription(const QString&fileName);
	QString GetFileTypeFromXMl(const QString&suffixName);
	int KillNoListElement(); //ɾ�������������õ�������ʾ������

	// Զ�����ݿ�
	QStringList GetAllElementNamesFromDB(); //���Ҫ�����ƣ�����)
	QStringList GetAllElementChineseNamesFromDB();
	CElement* GetElementByNameFromDB(const QString &elementName); // �������ƣ����Ҫ����Ϣ
	CElement* GetElementByChineseNameFromDB(const QString &elementName); // �����������ƣ����Ҫ����Ϣ
	int WriteCipas(const QString& fileName, QList<CElement*> elements_v);//д�ļ�
	int WriteCiapsRaster(const QString& fileName, QList<CElement*> elements_v);//д�ļ�
	int WriteMicaps3(const QString& fileName, QList<CElement*> elements_v);//д�ļ�
	int WriteMicaps4(const QString& fileName, QList<CElement*> elements_v);//д�ļ�
	void InitElementsFromDB();

	// �ڴ�����
	bool AddMemoryElements(QList<CElement*> elements);
	bool AddMemoryElement(CElement* pElement);

	//����ָ���ļ�
	QHash<QString, QString> GetAllExponentElements();

	//����������
	void OpenFilter();
	void CloseFilter();
	bool Filter() const;

	bool FileHasRead(const QString& fileName); //�ж��ļ��Ƿ��Ѿ���ȡ
signals:
	void OpenedFilesCountChanged(const QString& fileName, bool addFile, bool isMemory);
private:
	CDataProviderManager();
	bool LoadFiles(const QStringList &fileNames); //���ز���������ļ�
	IDataProvider* GetDataProvider(const QString& fileName); //��������Դ�ļ��������� DLL ��̬��
	IDataProvider* CreateDataProvider(const QString& dllName, const QString& path = "IDataProvider"); //���� DLL ��̬��
	const QStringList& GetDLLNames(const QString& path = "IDataProvider"); //������ж�̬������
	QString DLLFileName(const QString& type); //�����ļ����ͣ���� DLL ��̬������
	QString FileType(const QString& fileName); //����ļ�����
	QString MicapsFileType(const QString& sfilename); //���MICAPS�ļ�����
	QString TxtFileType(const QString& sfilename); //���Txt�ļ�����
	QString CpsFileType(const QString& sfilename); //��ȡcps�ļ�����
	QString AWXFileType(const QString& sfilename); //��ȡawx�ļ�����

private:
	bool m_filter;
	QStringList m_dllNames;   //���е� DLL ��̬������
	QStringList m_suffixNames; // �ļ���׺����
	QStringList m_FileNames; // �Ѿ��򿪱����ļ����б���ElementsFileMap�ļ����б�һ������ͬ
	typedef QHash<QString, IDataProvider*> FileProviderMap; // �ļ�����IDataProvider��ӳ���ϵ
	//typedef QHash<QString, QString> ElementFileMap; // Ҫ���������ļ�ӳ���ϵ
	typedef QHash<QString, QString> ElementNamesMap; // Ҫ����������������ӳ���ϵ
	typedef QHash<QString, CElement*> ElementNameObjectMap; //Ҫ�����������ӳ���ϵ
	typedef QHash<QString, CElement*> ElementsFileMap; //Ҫ�����ļ���ӳ���ϵ
	typedef QList<QString> ElementNames;
	typedef QHash<QString, ElementNames > SortedNamesMap;

	// �����ļ�
	FileProviderMap m_fileProvidersMap;
	//ElementFileMap m_elementFilesMap;
	//ElementNamesMap m_namesMap;
	//ElementNameObjectMap m_nameObjectsMap;
	ElementsFileMap m_elementsFileMap;  //���� ��/", "\" �滻Ϊ"/"
	SortedNamesMap m_sortedNamesMap;

	// �����������ļ�
	QStringList m_FileNamesNoListW; // �����������Ѿ��򿪱����ļ����б���ElementsFileMap�ļ����б�һ������ͬ

	//Զ�������ļ�
	typedef QHash<QString, CElement*> NameElementMap; // �ļ�����IDataProvider��ӳ���ϵ
	QStringList m_elementNamesDB;
	ElementNamesMap m_namesMapDB;
	NameElementMap m_nameElementMapDB;

	static CDataProviderManager* s_pDPManager;
};

#endif // CDATAPROVIDERMANAGER_H