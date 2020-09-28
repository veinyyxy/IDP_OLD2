#include "cdataprovidermanager.h"
#include <qdir.h>
#include <CDataElement>
#include "../include/IDPApplication.h"
#include "../PluginManager/PluginManager.h"
//#include "../IDataProvider/IDataProvider.h"
////#include "../CDataElement/CElement.h"
#include "netcdfcpp.h"
#include <QCoreApplication>
#include <QList>
#include <QMutexLocker>
#include <QMutex>
#include <qtextstream.h>
#include <QDebug>
#include "QtXml/QXmlStreamReader"

CDataProviderManager* CDataProviderManager::s_pDPManager = 0;
QMutex mutex;
CDataProviderManager::~CDataProviderManager()
{
	QHash< QString, CElement*> :: iterator it = m_elementsFileMap.begin();
	while(it != m_elementsFileMap.end())
	{
		CElement *pElement = it.value();
		delete pElement;
		pElement = 0;
		it = m_elementsFileMap.erase(it);
	}
	FileProviderMap::iterator itp = m_fileProvidersMap.begin();
	//QList<IDataProvider*> pLists =  m_fileProvidersMap.values();

	QList<QString> types;
	QList<IDataProvider*> provides;
	while(itp != m_fileProvidersMap.end())
	{
		IDataProvider *pidp = itp.value();
		itp = m_fileProvidersMap.erase(itp);
		if(pidp)
		{
			QString type = pidp->keys().at(0);
			if(!types.contains(type))
			{
				types.append(type);
				provides.append(pidp);
			}
			//delete pidp;
			//pidp = NULL;
		}
	}

	while(provides.count())
	{
		IDataProvider *pidp = provides.takeFirst();
		delete pidp;
	}

}

//��� CDataProviderManager ���
CDataProviderManager* CDataProviderManager::GetDPManager()
{
	if(!s_pDPManager)
	{
		s_pDPManager = new CDataProviderManager();
	}

	return s_pDPManager;
}

//��������Դ�ļ��������� DLL ��̬��
IDataProvider* CDataProviderManager::GetDataProvider(const QString& fileName)
{
	if(m_fileProvidersMap.contains(fileName))
	{
		return m_fileProvidersMap.value(fileName);
	}

	QString type = FileType(fileName);
	QString dllName = DLLFileName(type);
	IDataProvider* pDP = CreateDataProvider(dllName);
	if(pDP)
	{
		m_fileProvidersMap.insert(fileName, pDP);
	}

	return pDP;
}

//���������ṩ�� IDataProvider ���
IDataProvider* CDataProviderManager::CreateDataProvider(const QString& dllName, const QString& path)
{
	IDataProvider* pDP = NULL;
	if(dllName.isNull() || dllName.isEmpty())
	{
		return pDP;
	}

	QObject *pObject = PluginManager::GetPlugin(dllName, path);
	if(!pObject)
	{
		return pDP;
	}

	pDP = dynamic_cast<IDataProvider*>(pObject);

	return pDP;
}

//������ж�̬������
const QStringList&  CDataProviderManager::GetDLLNames(const QString& path)
{
	//INIT
	m_dllNames.clear();
	QDir dir(path);
	QStringList filters;

#ifdef WIN32
    filters << "*.dll";
#else
	filters << "*.so";
#endif
    dir.setNameFilters(filters);
	m_dllNames = dir.entryList();
	return	m_dllNames;
}

// ��������֧�ֵ��ļ���ʽ��׺
const QStringList& CDataProviderManager::SuffixNames()
{
	return m_suffixNames;
}

//���ز���������ļ�
bool CDataProviderManager::LoadFiles(const QStringList &fileNames)
{
	if(fileNames.isEmpty())
	{
		return false;
	}

	bool retFlag = true;
	for(int i = 0; i < fileNames.size(); ++i)
	{
		IDataProvider *pDB = GetDataProvider(fileNames.at(i));
		if(pDB)
		{
			if(!pDB->LoadFile(fileNames.at(i).toStdString()))
			{
				retFlag = false;
			}

			emit OpenedFilesCountChanged(fileNames.at(i), true, false);
		}
		else
		{
			retFlag = false;
		}
	}

	return retFlag;
}

// ���Ҫ��������Ҫ�ض�Ӧ��ϵ
QHash<QString, CElement*> CDataProviderManager::GetAllElement(const QString&fileNameSrc)
{
	//�ļ�û�н�����
	//ElementNameObjectMap elements;

	QHash<QString, CElement*> elements;
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	QList<CElement*> elementList = m_elementsFileMap.values(fileName);

	if(elementList.isEmpty())
	{
		IDataProvider *pDP = GetDataProvider(fileName);

		if(!pDP || !pDP->LoadFile(fileName.toStdString()))
		{
			return elements;
		}

		//m_nameObjectsMap = pDP->GetAllElements();
		elements = pDP->GetAllElements(fileName);
		ElementNames names = pDP->GetElementNames(fileName);
		m_sortedNamesMap.insert(fileName, names);
		//if(!sortedNames.isEmpty())
		//{
		//	sortedNames.clear();
		//	sortedNames = pDP->GetElementNames();
		//}
		// ��һ�δ��ļ�������Ҫ��Ӣ����������������ӳ���ϵ
		m_FileNames.append(fileName);//�洢�´򿪵��ļ���
		QHash<QString, CElement*>::iterator it = elements.begin();
		for(; it != elements.end(); ++it)
		{
			CElement *pElement = it.value();
			QString name = it.key();
			QString chName = name;
			if(pElement)
			{
				chName = pElement->GetChineseName();
				//m_namesMap.insert(name, chName);
				m_elementsFileMap.insertMulti(fileName, pElement);
			}
		}// END FOR
		//QMutexLocker locker(&mutex);
		emit OpenedFilesCountChanged(fileName, true, false);
		return elements;
	} // END���ɣ�

	// �ļ��Ѿ���������ֱ����ȡ����
	elementList = m_elementsFileMap.values(fileName);
	int nCount = elementList.size();
	for(int i = 0; i < nCount; ++i)
	{
		CElement *pElement = elementList.at(i);
		if(pElement)
		{
			elements.insert(pElement->name(), pElement);
		}
	}

	return elements;
}

QHash<QString, CElement*> CDataProviderManager::GetAllElementNoListW( const QString&fileNameSrc )
{
	//�ļ�û�н�����
	QHash<QString, CElement*> elements;
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	QList<CElement*> elementList = m_elementsFileMap.values(fileName);

	if(elementList.isEmpty())
	{
		IDataProvider *pDP = GetDataProvider(fileName);

		if(!pDP || !pDP->LoadFile(fileName.toStdString()))
		{
			return elements;
		}

		elements = pDP->GetAllElements(fileName);
		ElementNames names = pDP->GetElementNames(fileName);
		m_sortedNamesMap.insert(fileName, names); //
		//m_FileNames.append(fileName); //�洢�´򿪵��ļ���
		m_FileNamesNoListW.append(fileName); //�洢�´򿪵��ļ�������ʾ�ڶ�����Ƭ
		QHash<QString, CElement*>::iterator it = elements.begin();
		for(; it != elements.end(); ++it)
		{
			CElement *pElement = it.value();
			QString name = it.key();
			QString chName = name;
			if(pElement)
			{
				chName = pElement->GetChineseName();
				//m_namesMap.insert(name, chName);
				m_elementsFileMap.insertMulti(fileName, pElement);
			}
		}// END FOR
		//QMutexLocker locker(&mutex);
		//emit OpenedFilesCountChanged(fileName, true, false);
		return elements;
	} // END���ɣ�

	// �ļ��Ѿ���������ֱ����ȡ����
	elementList = m_elementsFileMap.values(fileName);
	for(int i = 0; i < elementList.size(); ++i)
	{
		CElement *pElement = elementList.at(i);
		if(pElement)
		{
			elements.insert(pElement->name(), pElement);
		}
	}

	return elements;
}

//���Ҫ������
QStringList CDataProviderManager::GetAllElementNames(const QString&fileName)
{
	ElementNameObjectMap elements = GetAllElement(fileName);
	QStringList elementNames;

	// init maps
	IDataProvider::HashElement::const_iterator it = elements.constBegin();
	for(; it != elements.constEnd(); ++it)
	{
		CElement *pElement = it.value();
		QString name = it.key();
		if(!Filter() || pElement->NeedDisplay())
		{
			elementNames.append(name);
		}
	}// END FOR

	return elementNames;
}

bool CDataProviderManager::FileHasRead(const QString& fileName)
{
	if (m_elementsFileMap.contains(fileName))
	{
		return true; //�ļ��Ѿ���ȡ
	}
	else
	{
		return false; //�ļ���δ��ȡ
	}
}

//���Ҫ�����ƣ�����)
QStringList CDataProviderManager::GetAllElementChineseNames(const QString&fileNameSrc)
{
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	QStringList sortedNames;
	ElementNameObjectMap elements = GetAllElement(fileName);

	sortedNames = m_sortedNamesMap.value(fileName);
	if(!sortedNames.isEmpty())
	{
		return sortedNames;
	}

	QStringList elementNames;

	// init maps
	IDataProvider::HashElement::iterator it;
	for(it = elements.begin(); it != elements.end(); ++it)
	{
		CElement *pElement = it.value();
		if(pElement)
		{
			QString name = pElement->GetChineseName();
			if(!Filter() || pElement->NeedDisplay())
			{
				elementNames.append(name);
			}
		}
	}// END FOR

	return elementNames;
}

QStringList CDataProviderManager::GetAllElementChineseNamesNoListW( const QString&fileNameSrc )
{
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	QStringList sortedNames;
	ElementNameObjectMap elements = GetAllElementNoListW(fileName);

	sortedNames = m_sortedNamesMap.value(fileName);
	if(!sortedNames.isEmpty())
	{
		return sortedNames;
	}

	QStringList elementNames;

	// init maps
	IDataProvider::HashElement::const_iterator it = elements.constBegin();
	for(; it != elements.constEnd(); ++it)
	{
		CElement *pElement = it.value();
		if(pElement)
		{
			QString name = pElement->GetChineseName();
			if(!Filter() || pElement->NeedDisplay())
			{
				elementNames.append(name);
			}
		}
	}// END FOR

	return elementNames;
}

// �������ƣ����Ҫ����Ϣ
CElement* CDataProviderManager::GetElementByName(const QString &elementName, const QString&fileNameSrc)
{
	CElement *pElement = 0;
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	//QHash< CElement*, QString>::const_iterator it = m_elementsFileMap.constBegin();
	QHash<QString, CElement* > :: iterator it = m_elementsFileMap.begin();
	for(; it != m_elementsFileMap.constEnd(); ++it)
	{
		pElement = it.value();
		if(pElement && pElement->name() == elementName && pElement->GetFileName() == fileName)
		{
			break;
			//return pElement;
		}
	}
	return pElement;
}

// �����������ƣ����Ҫ����Ϣ
CElement* CDataProviderManager::GetElementByChineseName(const QString &chName, const QString&fileNameSrc)
{
	CElement *pElement = 0;
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	//QHash< CElement*, QString>::const_iterator it = m_elementsFileMap.constBegin();
	QHash<QString, CElement* > ::const_iterator it = m_elementsFileMap.constBegin();
	for(; it != m_elementsFileMap.constEnd(); ++it)
	{
		pElement = it.value();
		if(pElement && pElement->GetChineseName() == chName && pElement->GetFileName() == fileName)
		{
			return pElement;
		}
	}

	return NULL;
}

bool CDataProviderManager::ReadDataFromFile(CElement* pElement)
{
	if(!pElement)
	{
		return false;
	}

	//�����ڴ��ļ���ֱ�ӷ��سɹ�
	if(pElement->IsMemory())
	{
		return true;
	}

	QString elementName = pElement->name();
	QString fileName = m_elementsFileMap.key(pElement);
	if(fileName.isEmpty())
	{
		fileName = pElement->GetFileName();
	}
	if(fileName.isEmpty())
	{
		return false;
	}

	IDataProvider *pDP = GetDataProvider(fileName);
	if(!pDP)
	{
		return false;
	}

	return pDP->StoreDetailValues(pElement);
}

//д�ļ�
int CDataProviderManager::Write(const QString& fileNameString, QList<CElement*> elements_v)
{
	if(fileNameString.isEmpty() || elements_v.isEmpty())
	{
		return 0;
	}

	std::string stdFileName = fileNameString.toStdString();

	const char *fileName = stdFileName.data();
	NcFile* pNcFile = new NcFile(fileName,NcFile::Replace,0,0U,NcFile::Classic);//ʵ����NcFile����
	if(!pNcFile || !pNcFile->is_valid())
	{
		return 0;
	}

	// ��ʼ��ά�� dimname = elementName + dimName
	for(int i = 0; i < elements_v.count(); ++i)
	{
		NcDim *pDimLon = 0;
		NcDim *pDimLat = 0;
		NcDim *pDimLevel = 0;
		NcDim *pDimTime = 0;

		CElement *pElement = elements_v.at(i);
		if(!pElement)
		{
			continue;
		}

		//QString name = pElement->name();
		QString name = "Saved";
		// ����
		//QString dimNameStr = QString("lon_%1").arg(name); // wangjl ͳһ����
		QString dimNameStr = QString("lon");
		std::string stdString = dimNameStr.toStdString();
		NcToken dimName = stdString.data(); //dimNameStr.toLocal8Bit().data();
		long size = pElement->SelectedLon().m_count;
		if(size > 0)
		{
			//���ά��
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return 0;
			}
			pDimLon = pDim;
			NcVar* pVar = pNcFile->add_var(dimName, ncFloat, pDim);
			if(pVar)
			{
				int iAtt = 0;
				float range[2];
				range[0] = pElement->SelectedLon().m_begin;
				range[1] = pElement->SelectedLon().m_end;
				pVar->add_att("units",  "degrees_east");
				pVar->add_att("long_name", "Longitude");
				pVar->add_att("actual_range", 2,  range);
				// add values

				float *pValues = new float[size];
				float delt = pElement->SelectedLon().m_delt;
				for(int j = 0; j < size; ++j)
				{
					pValues[j] = range[0] + j * delt;
				}
				long *pCount = new long(size);
				NcBool ret = pVar->put(pValues, pCount);

				delete pValues;
				delete pCount;
				if(ret == FALSE)
				{
					return 0;
				}
			} // pVar
		} // end size > 0

		//γ��
		//dimNameStr = QString("lat_%1").arg(name); // wangjl ͳһ����
		dimNameStr = QString("lat");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data();
		size = pElement->SelectedLat().m_count;
		if(size > 0)
		{
			//���ά��
			NcDim * pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return 0;
			}
			pDimLat = pDim;
			NcVar* pVar = pNcFile->add_var(dimName, ncFloat, pDim);
			if(pVar)
			{
				int iAtt = 0;
				float range[2];
				range[0] = pElement->SelectedLat().m_begin;
				range[1] = pElement->SelectedLat().m_end;
				pVar->add_att("units",  "degrees_north");
				pVar->add_att("long_name",  "Latitude");
				pVar->add_att("actual_range", 2, range);
				// add values
				long size = pElement->SelectedLat().m_count;

				float *pValues = new float[size];
				float delt = pElement->SelectedLat().m_delt;
				for(int j = 0; j < size; ++j)
				{
					pValues[j] = range[0] + j * delt;
				}
				long *pCount = new long(size);
				NcBool ret = pVar->put(pValues, pCount);

				delete pValues;
				delete pCount;
				if(ret == FALSE)
				{
					return 0;
				}
			} // end pVar
		}// end Size > 0

		//���
		//dimNameStr = QString("level_%1").arg(name); // wangjl ͳһ����
		dimNameStr = QString("level");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data();
		size = pElement->Level().m_selected.size();
		if(size > 0)
		{
			//���ά��
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return 0;
			}
			pDimLevel = pDim;
			NcVar* pVar = pNcFile->add_var(dimName, ncFloat, pDim);
			if(pVar)
			{
				int iAtt = 0;
				float range[2];
				float *pValues = new float[size];
				QHash<int, float>::const_iterator it = pElement->Level().m_selected.constBegin();
				for(int j = 0; it != pElement->Level().m_selected.constEnd(); ++j, ++it)
				{
					pValues[j] = it.value();
				}
				range[0] = pValues[0];
				range[1] = pValues[size-1];
				//pVar->add_att("units", iAtt++, "degrees_north");
				pVar->add_att("long_name",  "Level");
				pVar->add_att("actual_range", 2, range);
				// add values
				long *pCount = new long(size);
				NcBool ret = pVar->put(pValues, pCount);
				delete pValues;
				delete pCount;
				if(ret == FALSE)
				{
					return 0;
				}
			}// end pVar
		}// end size > 0

		//ʱ��
		//dimNameStr = QString("time_%1").arg(name); // wangjl ͳһ����
		dimNameStr = QString("time");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data();
		size = pElement->SelectedPeriod().m_hashDate.size();
		if(size > 0)
		{
			//���ά��
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return 0;
			}
			pDimTime = pDim;
			NcVar* pVar = pNcFile->add_var(dimName, ncFloat, pDim);
			if(pVar)
			{
				int iAtt = 0;
				long range[2];
				long *pValues = new long[size];
				QDateTime begin = pElement->SelectedPeriod().m_begin;
				CTimeScale timeScale = pElement->SelectedPeriod().m_timeScale;
				long steps = 0;
				QHash<long, QDateTime>::const_iterator it = pElement->SelectedPeriod().m_hashDate.constBegin();
				QList<QDateTime> dates;
				for(; it != pElement->SelectedPeriod().m_hashDate.constEnd(); ++it)
				{
					dates << it.value();
				}
				qSort(dates);
				for(int j = 0; j < dates.size(); ++j)
				{
					QDateTime dateTime = dates.at(j);
					if(GetSteps(steps, dateTime, begin, timeScale))
					{
						pValues[j] = steps;
					}
				}

				range[0] = pValues[0];
				range[1] = pValues[size-1];
				QString beginStr = begin.toString("yyyy-MM-dd hh:mm:ss");
				QString unitsStr = timeScale.GetUnitDescription();
				unitsStr = QString("%1s since %2").arg(unitsStr).arg(beginStr);
				stdString = unitsStr.toStdString();
				//dimName = stdString.data();
				NcToken units = stdString.data();
				pVar->add_att("units",  units);
				pVar->add_att("long_name",  "Time");
				pVar->add_att("actual_range", 2, range);
				// add values
				long *pCount = new long(size);
				NcBool ret = pVar->put(pValues, pCount);
				delete pValues;
				delete pCount;
				if(ret == FALSE)
				{
					return 0;
				}
			}// end pvar
		} // end size > 0

		// ��ӱ���
		int ndims = 0;
		NcDim ** ppDim = new NcDim*[4];
		if(pDimTime)
		{
			ppDim[ndims++] = pDimTime;
		}
		if(pDimLevel)
		{
			ppDim[ndims++] = pDimLevel;
		}
		if(pDimLat)
		{
			ppDim[ndims++] = pDimLat;
		}
		if(pDimLon)
		{
			ppDim[ndims++] = pDimLon;
		}

		if(!ndims)
		{
			return 0;
		}
		stdString = name.toStdString();
		NcToken sName = stdString.data();
		NcVar *pVar = pNcFile->add_var(sName,  ncFloat, ndims, (const NcDim**)ppDim);
		if(!pVar)
		{
			return 0;
		}
		stdString = pElement->LongName().toStdString();
		NcToken longName = stdString.data();
		pVar->add_att("long_name",  longName);
		stdString = pElement->unit().toStdString();
		NcToken units = stdString.data();
		pVar->add_att("units",  units);
		pVar->add_att("missing_value",  pElement->MissingValue());
		long *pCount = new long[ndims];
		ndims = 0;
		long totalSize = 1;
		if(pDimTime)
		{
			pCount[ndims++] = pElement->SelectedPeriod().m_hashDate.size();
			totalSize *= pCount[ndims-1];
		}
		if(pDimLevel)
		{
			pCount[ndims++] = pElement->Level().m_selected.size();
			totalSize *= pCount[ndims-1];
		}
		if(pDimLat)
		{
			pCount[ndims++] = pElement->SelectedLat().m_count;
			totalSize *= pCount[ndims-1];
		}
		if(pDimLon)
		{
			pCount[ndims++] = pElement->SelectedLon().m_count;
			totalSize *= pCount[ndims-1];
		}

		//
		float *pVals = pElement->Data().GetDataArray(0);
		if(!pVar->put(pVals, pCount))
		{
			return 0;
		}
	}// end for

	delete pNcFile;

	return 1;
} // end fuction

/************************************************************************/
/* �������ܣ� CIPAS�Զ������ݸ�ʽ�ļ���д��
/* ����˵��:  elements_v Ҫ���� ���ļ�������
/* �� �� ֵ:   int 1��д��ɹ���0д��ʧ��
/* ��    ��:  �Ը���
/* ��    ��:  2011 �� 06 �� 28��
/************************************************************************/
int CDataProviderManager::WriteCipas(const QString& fileName, QList<CElement*> elements_v)
{
	if(fileName.isEmpty() || elements_v.isEmpty())
	{
		return 0;
	}
	QFile file(fileName);
	QString StrTemp;
	int nLevels = 0;
	int nDataCount = 0;
	int nAttributes = 0;
	int nAbuteIndex = 1;
	int nHeaderInfo = 0;
	CElement *pTempElement;
	QStringList DetialList;
	QStringList AttributeList;

	if (!file.open (QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return 0;
	}
	
	// sort elements_v  �ļ�����  վ����  ����  γ��  add by hanmeng 2012-10-16
	int index = -1;
	for(int i = 0; i< elements_v.size(); ++i) // �ļ�����
	{
		if(elements_v.at(i)->name() == QObject::tr("�ļ�����"))
		{
			index = i; 
			break;
		}
	}

	if(index != 0 && index != -1)
	{
		elements_v.swap(0, index);
	}

	index = -1;
	for(int i = 1; i< elements_v.size(); ++i) // վ����
	{
		if(elements_v.at(i)->name() == QObject::tr("վ����"))
		{
			index = i; 
			break;
		}
	}
	if(index != 1&& index != -1)
	{
		elements_v.swap(1, index);
	}

	index = -1;
	for(int i = 2; i< elements_v.size(); ++i) // վ�㾭��
	{
		if(elements_v.at(i)->name() == QObject::tr("վ�㾭��"))
		{
			index = i; 
			break;
		}
	}
	if(index != 2&& index != -1)
	{
		elements_v.swap(2, index);
	}

	index = -1;
	for(int i = 3; i< elements_v.size(); ++i) // վ��γ��
	{
		if(elements_v.at(i)->name() == QObject::tr("վ��γ��"))
		{
			index = i; 
			break;
		}
	}
	if(index != 3&& index != -1)
	{
		elements_v.swap(3, index);
	}

	index = -1;
	for(int i = 4; i< elements_v.size(); ++i) // վ�㺣�� 
	{
		if(elements_v.at(i)->name() == QObject::tr("վ�㺣��") || elements_v.at(i)->name().contains("����") )
		{
			index = i; 
			break;
		}
	}
	if(index != 4&& index != -1)
	{
		elements_v.swap(4, index);
	}

	pTempElement = elements_v.first();
	if (pTempElement->name() != QObject::tr("�ļ�����"))
	{
		return 0;
	}
	DetialList = pTempElement->GetMicapsDeclaration();
	if (DetialList.size()<11)
	{
		return 0;
	}
	QTextStream stream (&file);
	//------------------------------------------------------------headerSTART
	stream<<"CIPAS  ";
	StrTemp = DetialList.at(nHeaderInfo);	//�������ͣ�������1վ������
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"  ";
	StrTemp = DetialList.at(nHeaderInfo);	//������Ϣ��ʼ����
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"  ";
	StrTemp = DetialList.at(nHeaderInfo);	//��ͼ������ʼ����
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"  ";
	StrTemp = DetialList.at(nHeaderInfo);	//������Ϣ��ʼ����
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"\r\n";

	stream<<"#HEADINFO";
	stream<<"\r\n";

	stream<<"#DESCRIPTION   ";
	StrTemp = DetialList.at(nHeaderInfo);	//�ļ�������Ϣ
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"\r\n";

	stream<<"#SCD_CATEGORY   ";
	StrTemp = QString::fromLocal8Bit("-999");	//�ļ�������Ϣ
	stream<<StrTemp;
	stream<<"\r\n";

	stream<<"#DATA_STORAGE_FORMAT   ";
	StrTemp = DetialList.at(nHeaderInfo);	//�ļ�������Ϣ
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"\r\n";

	stream<<"#TIME  ";
	StrTemp = DetialList.at(nHeaderInfo);	//ʱ�䣺��PDAY 10 2011 03 06 -999 2011 03 15 -999��
	nHeaderInfo++;
	stream<<StrTemp;//10����ֵ
	stream<<"\r\n";

	stream<<"#LEVEL  ";
	StrTemp = DetialList.at(nHeaderInfo);
	nHeaderInfo++;
	stream<<StrTemp;
	nLevels = StrTemp.toInt();				//�������
	stream<<"  ";
	StrTemp = DetialList.at(nHeaderInfo);	//����Ϣ����1000 900 850��
	nHeaderInfo++;
	stream<<StrTemp;
	stream<<"\r\n";

	stream<<"#MISSING_VALUE  ";
	stream<<999999;
	stream<<"\r\n";

	stream<<"#ATTRIBUTES  ";
	StrTemp = DetialList.at(nHeaderInfo);
	nHeaderInfo++;
	stream<<StrTemp;
	nDataCount = StrTemp.toInt();			//����վ������
	stream<<"  ";
	StrTemp = DetialList.at(nHeaderInfo);
	nHeaderInfo++;
	stream<<StrTemp;
	nAttributes = StrTemp.toInt();			//������������
 	int nsize = 15+nAttributes; 
	if (DetialList.size()!= nsize)
	{
		return 0;
	}
	for (int i= 0;i<nAttributes;i++)
	{
		stream<<"  ";
		StrTemp = DetialList.at(nHeaderInfo);	//������������
		nHeaderInfo++;
		StrTemp = elements_v.at(i+1)->name(); //��ʹ��DetialList�е�����
		stream<<StrTemp;
		//stream<<"  float   ";					//Ĭ�϶���float����
		AttributeList << StrTemp;
	}
	stream<<"\r\n";

	StrTemp = DetialList.at(nHeaderInfo);		//�������Զ��壬�����Ը���ƥ��ո�ֿ� f10.2 f10.2 f10.2 f10.2 f10.2
	nHeaderInfo++;
	stream<<"#FORMATDEFINE  ";
	stream<<StrTemp;
	stream<<"\r\n";

	StrTemp = DetialList.at(nHeaderInfo);		//����ͶӰ����
	nHeaderInfo++;
	stream<<"#PROJECTION  ";
	stream<<StrTemp;
	stream<<"\r\n";

	StrTemp = DetialList.at(nHeaderInfo);		//ͶӰ��Χ�����ϽǾ�γ�����½Ǿ�γ �ĸ������ÿո�ֿ�
	nHeaderInfo++;
	stream<<"#SPATAIL_BOX  ";
	stream<<StrTemp;
	stream<<"\r\n";

	StrTemp = DetialList.at(nHeaderInfo);		//��ֵ������
	nHeaderInfo++;
	stream<<"#DRAWSET 0";						//��ֵ������Ĭ����
	stream<<StrTemp;
	stream<<"\r\n";
	//-----------------------------------------------------------------------headerinfo END
	if (elements_v.size()<nAttributes +1)
	{
		return 0;
	}
	CData *tempData = new CData[nAttributes];
	//-----------------------------------------------------------------------Data Start
	stream<<"#DATASET";
	stream<<"\r\n";
	for (int n =0;n<nAttributes ;n++)
	{
		tempData[n] = elements_v.at(n+1)->Data();
	}
	float tempvalue;
	for (int i = 0;i<nLevels;i++)
	{
		stream<<"#DATA  ";
		stream<<i+1;
		stream<<"\r\n";
		for(int j= 0;j<nDataCount;j++)
		{
			stream.setFieldWidth(15);
			 tempvalue= tempData[0].GetDataArray(0)[j];
			stream<<QString::number(tempvalue);
			 tempvalue = tempData[1].GetDataArray(0)[j];
			stream<<QString::number(tempvalue);
			 tempvalue = tempData[2].GetDataArray(0)[j];
			stream<<QString::number(tempvalue);
			for (int n =3;n<nAttributes ;n++)
			{
				 tempvalue = tempData[n].GetDataArray(0)[i*nDataCount+j];
				stream<<QString::number(tempvalue);
				//stream<< " " ;
			}
			stream<<"\r\n";
		}
	}
	//-----------------------------------------------------------------------Data END
	delete []tempData;
	return 1;
}

bool CDataProviderManager::CloseFileAndRemoveElements(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return false;
	}

	//ɾ�����������е�ӳ���ϵ��ɾ��element����
	IDataProvider *pDP = GetDataProvider(fileName);
	if(pDP)
	{
		pDP->CloseFileAndRemoveElements(fileName);
	}
	else
	{
		/**** ɾ���������㷨 element ****/
		QHash<QString, CElement* > :: iterator it = m_elementsFileMap.begin();
		while(it != m_elementsFileMap.end())
		{
			if(it.key() == fileName)
			{
				//ע��������룬��Ȼ���������,add by zhanglei
// 				CElement *pElement = it.value();
// 				delete pElement;
// 				pElement = 0;

				m_elementsFileMap.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	//����ļ���CElement��ӳ��
	{
		QHash<QString, CElement* > :: iterator it = m_elementsFileMap.begin();
		while(it != m_elementsFileMap.end())
		{
			if(it.key() == fileName)
			{
				//CElement *pElement = it.key();
				//delete pElement;
				//pElement = 0;
				//it = 
				m_elementsFileMap.erase(it++);
			}
			else
			{
				++it;
			}
		}
		QStringList :: iterator itn = m_FileNames.begin();
		while(itn != m_FileNames.end())
		{
			if (*itn == fileName)
			{
				itn = m_FileNames.erase(itn);
			}
			else
				++itn;
		}
		itn = m_FileNamesNoListW.begin();
		while(itn != m_FileNamesNoListW.end())
		{
			if (*itn == fileName)
			{
				itn = m_FileNamesNoListW.erase(itn);
			}
			else
				++itn;
		}
	}

	//����ļ���Ҫ�����Ƶ�ӳ��
	{
		QHash<QString, ElementNames >::iterator it = m_sortedNamesMap.begin();
		while(it != m_sortedNamesMap.end())
		{
			if(it.key() == fileName)
			{
				it.value().clear();
				it = m_sortedNamesMap.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	emit OpenedFilesCountChanged(fileName ,false, false);

	return true;
}

// ��ȡ�򿪵ı����ļ�
QStringList  CDataProviderManager::GetAllOpenedFiles()
{
	//QStringList files;
	//QString file;
	//QHash<QString, CElement*> ::const_iterator it = m_elementsFileMap.constBegin();
	//for(; it != m_elementsFileMap.constEnd(); ++it)
	//{
	//	file = it.key();
	//	if(!files.contains(file))
	//	{
	//		files.append(file);
	//	}
	//}

	return m_FileNames;

	//return m_elementsFileMap.keys();
}

// ��ȡ�ļ�������Ϣ��micaps�ļ���������������Ϣ�����򷵻��ļ����ƣ���·����
QString CDataProviderManager::GetFileDescription(const QString&fileNameSrc)
{
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");

	QString fileType = FileType(fileName);
	if(fileType.contains("MICAPS")||fileType.contains("STADA")||fileType.contains("CIPASM"))// MICAPS file
	{
		IDataProvider *pDP = GetDataProvider(fileName);
		if(pDP)
		{
			pDP->LoadFile(fileName.toStdString());
			return pDP->GetDataDescription(fileName);
		}
	}

	int index = fileName.lastIndexOf("/");
	int length = fileName.length();
	int n = length - index -1;
	QString suffixName = fileName.right(n);
	if(!suffixName.isEmpty())
	{
		suffixName = suffixName.toUpper();
	}

	return suffixName;
}

//��Զ�����ݿ���Ҫ������
QStringList CDataProviderManager::GetAllElementNamesFromDB()
{
	QStringList names;
	ElementNamesMap ::const_iterator it = m_namesMapDB.constBegin();
	for(; it != m_namesMapDB.constEnd(); ++it)
	{
		names << it.key();
	}

	return names;
}

//��Զ�����ݿ���Ҫ�����ƣ�����)
QStringList CDataProviderManager::GetAllElementChineseNamesFromDB()
{
	QStringList names;
	ElementNamesMap ::const_iterator it = m_namesMapDB.constBegin();
	for(; it != m_namesMapDB.constEnd(); ++it)
	{
		names << it.value();
	}

	return names;
}

// �������ƣ����Ҫ����Ϣ
CElement* CDataProviderManager::GetElementByNameFromDB(const QString &elementName)
{
	return m_nameElementMapDB.value(elementName);
}
// �����������ƣ����Ҫ����Ϣ
CElement* CDataProviderManager::GetElementByChineseNameFromDB(const QString &chName)
{
	QString elementName = m_namesMapDB.key(chName);
	return m_nameElementMapDB.value(elementName);
}

void CDataProviderManager::InitElementsFromDB()
{
	m_elementNamesDB.clear();
	// add SST
	m_elementNamesDB << QString("SST");
	if(!m_namesMapDB.contains("SST"))
	{
		m_namesMapDB.insert("SST",QObject::tr("����") );
	}
	if(!m_nameElementMapDB.contains("SST"))
	{
		CElement *pElement = new CElement();
		//(ValueType begin, ValueType end, ValueType delt, long int count, QString units, ValueType precision = 0.01);
		CLonLatRange lonRange(0.5, 359.5, 1, 360, QString("west to east"));
		CLonLatRange latRange(-89.5, 90, 1, 180, QString ("south to north"));
		CTimeScale ts(1, 2); // one time per month
		//CLevel level();
		CPeriod period;
		period.m_timeScale = ts;
		QDateTime begin = QDateTime(QDate::fromString("19500101", "yyyyMMdd"));
		QDateTime end = QDateTime(QDate::fromString("20101201", "yyyyMMdd"));
		period.m_begin = begin;
		period.m_end = end;
		for(int i = 0; i < 732; i++)
		{
			period.m_hashDate.insert(0, QDateTime(begin.addMonths(i)));
		}

		//init pElement
		pElement->ActualLon(lonRange);
		pElement->SelectedLon(lonRange);
		pElement->ActualLat(latRange);
		pElement->SelectedLat(latRange);
		pElement->ActualPeriod(period);
		pElement->SelectedPeriod(period);

		m_nameElementMapDB.insert("SST", pElement);
	}
}

bool CDataProviderManager::AddMemoryElements(QList<CElement*> elements)
{
	if(elements.isEmpty())
	{
		return false;
	}

	bool flag = true;
	CElement *pElement;
	QString strfilename;

	for(int i = 0; i < elements.size(); ++i)
	{
		pElement = elements.at(i);
		if(!pElement)
		{
			flag = false;
			continue;
		}

		if(m_elementsFileMap.key(pElement).isEmpty())
		{
 			strfilename = pElement->GetFileName();
			if(m_FileNames.contains(strfilename))
			{
				m_elementsFileMap.insertMulti(strfilename,pElement);
			}else
			{
				if (!(m_FileNames.contains(strfilename)))
				{
					m_FileNames.append(strfilename);
				}
				m_elementsFileMap.insert(strfilename, pElement);

				//emit OpenedFilesCountChanged(strname, true, true);
			}	
		}
	}
	emit OpenedFilesCountChanged(strfilename, true, true);

	return flag;
}

bool CDataProviderManager::AddMemoryElement(CElement* pElement)
{
	if(!pElement)
	{
		return false;
	}

	if(m_elementsFileMap.key(pElement).isEmpty())
	{
		QString strname = pElement->GetFileName();
	//	if (m_elementsFileMap.contains(strname))
		if(m_FileNames.contains(strname))
		{
			m_elementsFileMap.insertMulti(strname,pElement);
		}else
		{
			if (!(m_FileNames.contains(strname)))
			{
				m_FileNames.append(strname);
			}
			m_elementsFileMap.insert(strname, pElement);
			if (pElement->NeedDisplay())
			{
				emit OpenedFilesCountChanged(strname, true, true);
			}
		}		
	}

	return true;
}

//��ñ���ָ���ļ�����, ����ʾ�ڽ����ϣ��ʲ������ź�
QHash<QString, QString> CDataProviderManager::GetAllExponentElements()
{
	QHash<QString, QString> names;
	QString path = IDPApplication::DataPath() + QObject::tr("/ָ���ļ�");
	QDir dir(path, "*.tms *.txt");
	QStringList files = dir.entryList();
	if(files.isEmpty())
	{
		return names;
	}

	foreach(QString file, files)
	{
		file = path + "/" + file;
		IDataProvider* pDP = GetDataProvider(file);
		if(pDP && pDP->LoadFile(file.toStdString()))
		{
			QHash<QString, CElement*> elements = pDP->GetAllElements(file);
			QHash<QString, CElement*>::const_iterator it = elements.constBegin();
			for(; it != elements.constEnd(); ++it)
			{
				if(it.value())
				{
					names.insert(it.value()->GetChineseName(), file);
					m_elementsFileMap.insertMulti(file, it.value());
				}
			}
		}
	} // end foreach

	return names;
}
void CDataProviderManager::OpenFilter()
{
	m_filter = true;
}
void CDataProviderManager::CloseFilter()
{
	m_filter = false;
}
bool CDataProviderManager::Filter() const
{
	return m_filter;
}

// ˽�й��캯��
CDataProviderManager::CDataProviderManager()
{
	//m_dllPath = path;
	m_suffixNames << "nc" << "grb1" << "grb2" << "000" << "tms" << "ctl";
}

//�����ļ����ͣ���� DLL ��̬������
QString CDataProviderManager::DLLFileName(const QString& type)
{
	if(type == "NotValid")
	{
		return type;
	}

	QString path = IDPApplication::PluginPath() + QString("/IDataProvider/");
	GetDLLNames(path);
	for(int i = 0; i < m_dllNames.count(); ++i)
	{
		QString dllName = m_dllNames.at(i);
		if(dllName.contains(type, Qt::CaseInsensitive))
		{
			return dllName;
		}
	}

	return "NotVaild";
}

//����ļ�����
QString CDataProviderManager::FileType(const QString& fileName)
{
	QString type = "NotValid";
	if(fileName.isNull() || fileName.isEmpty())
	{
		return type;
	}

	int index = fileName.lastIndexOf(".");
	int length = fileName.length();
	int n = length - index -1;
	QString suffixName = fileName.right(n);
	if(!suffixName.isEmpty())
	{
		suffixName = suffixName.toUpper();
	}

	if(suffixName == "TXT") //
	{
		type = TxtFileType(fileName);
		return type;
	}
	else if(suffixName == "CPS") //CIPASM/CIPASR
	{
		type = CpsFileType(fileName);
		return type;
	}
	else if (suffixName == "AWX")
	{
		type = AWXFileType(fileName);
		return type;
	}
	else if (suffixName == "TIF")
	{
		type = "TIF";
		return type;
	}
	else
	{	//if(!suffixName.contains(QRegExp(QString("[A-Z]"))) || suffixName == "DAT" ) //micaps
		type = GetFileTypeFromXMl(suffixName);
		if (type != "NotValid")
		{
			return type;
		}
		else
		{
			type = MicapsFileType(fileName);
			return type;
		}
	}
	return type;
}

QString CDataProviderManager::MicapsFileType(const QString& sfilename)
{
	FILE *fp=NULL;

	std::string cfilename  = sfilename.toStdString();

	const char *filename = cfilename.data();
	fp = fopen(filename, "r");

	QString notValid = "NotValid";

	if (fp == NULL)
	{
		printf ("cannot open file");
		return notValid;
	}

	char	strTitle[10];			// diamond
	int		iMicapsType =0;			// Micaps��������
	char	strMicapsInfo[100];		// Micaps����˵����Ϣ

	fscanf(fp,"%s %d %s", strTitle, &iMicapsType, strMicapsInfo);
	fclose(fp);

	if (QString(strTitle) == "diamond")
	{
		switch (iMicapsType)
		{
		case 1:
			return "MICAPS1";
			break;
		case 2:
			return "MICAPS2";
			break;
		case 3:
			return "MICAPS3";
			break;
		case 4:
			return "MICAPS4";
			break;
		case 5:
			return "MICAPS5";
			break;
		case 7:
			return "MICAPS7";
			break;
		case 8:
			return "MICAPS8";
			break;
		case 11:
			return "MICAPS11";
			break;
		case 13:
			return "MICAPS13";
			break;
		default:
			return notValid;
			break;
		}
	}
	else
		if (QString(strTitle).toInt() > 100)
		{
			return "STADA";
		}else
			return notValid;
}
/************************************************************************/
/* �������ܣ� �����ļ���׺�����������ļ���Ѱ��ƥ��Ķ�̬������
/* ����˵��:  �ļ���׺��
/* �� �� ֵ:  ��̬�����ƻ򲿷����ƣ���Ϊ�ļ�����
/* ��    ��:  �Ը���
/* ��    ��:  2011 �� 08 �� 11��
/************************************************************************/
QString CDataProviderManager::GetFileTypeFromXMl(const QString&suffixName)
{
	QString notValid = "NotValid";
	QString xmlName = IDPApplication::ConfigPath() + "/BasicInfo/DataEngine.xml";
	QFile file(xmlName);
	if(file.open(QFile::ReadOnly|QFile::Text))
	{
		QXmlStreamReader   XmlReader;
		XmlReader.setDevice(&file);
		XmlReader.readNext();
		while (!XmlReader.atEnd())
		{
			if (XmlReader.isStartElement())
			{
				if (XmlReader.name() == "ConfigDLL")
				{
					XmlReader.readNext();
					while (!XmlReader.atEnd())
					{
						if (XmlReader.isStartElement())
						{
							if (XmlReader.name() == "Info")
							{
								QString tempsName = XmlReader.attributes().value("suffixName").toString();
								if (suffixName == tempsName.toUpper())
								{
									QString tempdName = XmlReader.attributes().value("DllName").toString();
									if (!tempdName.isEmpty())
									{
										file.close();
										return tempdName;
									}
								}
							}else
								//if (XmlReader.isEndElement())
								//{
								//	XmlReader.readNext();
								//	break;
								//}
								XmlReader.readNext();
						}
						XmlReader.readNext();
					}
				}
				else
				{
					file.close();
					return notValid;
				}
			}
			else
			{
				XmlReader.readNext();
			}
		}
		file.close();
	}
	else
	{
		return notValid;
	}
	return notValid;
}
/************************************************************************/
/* �������ܣ� micaps3���ݸ�ʽ�ļ���д��
/* ����˵��:  elements_v Ҫ���� ���ļ�������
/* �� �� ֵ:   int 1��д��ɹ���0д��ʧ��
/* ��    ��:  �Ը���
/* ��    ��:  2011 �� 06 �� 28��
/************************************************************************/
int CDataProviderManager::WriteMicaps3(const QString& fileName, QList<CElement*> elements_v)
{
	if(fileName.isEmpty() || elements_v.isEmpty())
	{
		return 0;
	}
	QFile file(fileName);
	QString StrTemp;
	CElement *pTempElement;
	QStringList DetialList;

	if (!file.open (QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return 0;
	}
	pTempElement = elements_v.first();
	if (pTempElement->name() != QObject::tr("������Ϣ") ||elements_v.size()<6)
	{
		return 0;
	}
	DetialList = pTempElement->GetMicapsDeclaration();
	if (DetialList.size()<15)
	{
		return 0;
	}
	QTextStream stream (&file);
	//------------------------------------------------------------headerSTART
	for (int i= 0;i<DetialList.size();i++)
	{
		stream<<DetialList.at(i);
		stream<<QString(" ");
		if (i%10 ==1)
		{
			stream<<"\r\n";
		}
	}
	stream<<"\r\n";
	int n = 0;
	//------------------------------------------------------------headerEnd
	int nElementNum;
	int nStationNum;
	nStationNum = DetialList.at(DetialList.size()-1).toInt();
	nElementNum = DetialList.at(DetialList.size()-2).toInt();
	n = 4+nElementNum;
	if (n<5 || n>6)
	{
		return 0;
	}
	//------------------------------------------------------------DataSTART
	for (int i = 0;i<nStationNum;i++)
	{
		stream.setFieldWidth(10);
		for(int j = 0;j<n;j++)
		{
			stream<<elements_v.at(j+1)->Data().GetDataArray(0)[i];
		}
		stream<<"\r\n";
	}
	//------------------------------------------------------------DataEnd
	return 1;
}
/************************************************************************/
/* �������ܣ� micaps4���ݸ�ʽ�ļ���д��
/* ����˵��:  elements_v Ҫ���� ���ļ�������
/* �� �� ֵ:   int 1��д��ɹ���0д��ʧ��
/* ��    ��:  �Ը���
/* ��    ��:  2011 �� 06 �� 28��
/************************************************************************/
int CDataProviderManager::WriteMicaps4(const QString& fileName, QList<CElement*> elements_v)
{
	if(fileName.isEmpty() || elements_v.isEmpty())
	{
		return 0;
	}
	QFile file(fileName);
	QString StrTemp;
	CElement *pTempElement;
	QStringList DetialList;

	if (!file.open (QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return 0;
	}
	pTempElement = elements_v.first();
	if (NULL == pTempElement )
	{
		return 0;
	}
	if (pTempElement->name() != QObject::tr("������Ϣ"))
	{
		return 0;
	}
	DetialList = pTempElement->GetMicapsDeclaration();
	if (DetialList.size()<22)
	{
		return 0;
	}
	QTextStream stream (&file);
	//------------------------------------------------------------headerSTART
	for (int i= 0;i<DetialList.size();i++)
	{
		stream<<DetialList.at(i);
		stream<<QString(" ");
		if (i%10 ==1)
		{
			stream<<"\r\n";
		}
	}
	stream<<"\r\n";
	//------------------------------------------------------------headerEnd
	pTempElement = elements_v.last();
	if (NULL == pTempElement)
	{
		return 0;
	}
	float *pDataArray = pTempElement->Data().GetDataArray(0);
	int x =  pTempElement->ActualLon().m_count;
	int y =  pTempElement->ActualLat().m_count;
	//------------------------------------------------------------DataSTART
	for (int i = 0 ; i<y;i++)//lat  144
	{
		stream.setFieldWidth(7);
		for (int j = 0 ;j<x;j++)//lon 73
		{
			stream<<QString::number(pDataArray[i*x+j]);
		}
		stream<<"\r\n";
	}
	//------------------------------------------------------------DataEnd
	pDataArray = NULL;
	return 1;
}

QString CDataProviderManager::TxtFileType(const QString& sfilename)//���Txt�ļ�����
{
	//------------------------------------------------------------�ж�CIPAS---Start
	QFile file(sfilename);
	if (!file.open (QIODevice::ReadOnly))
	{
		return "NotValid";
	}
	QTextStream stream (&file);
	QString strLine = stream.readLine();
	file.close();

	QStringList firstLine = strLine.split("	");
	if (firstLine.count() == 1)
	{
		firstLine = strLine.split(" ");

		if (firstLine.count() == 1)
		{
			return "NotValid";
		}
	}

	if (strLine.isEmpty())
	{
		return "NotValid";
	}
	else if (strLine.contains(QObject::tr("ָ��")))
	{
		return "EXPONENT";
	}
	else if (firstLine.at(0) == (QObject::tr("CIPAS")))
	{
		if (firstLine.at(2) == (QObject::tr("1")))
		{
			return "CIPASM";
		}
		else if (firstLine.at(2) == (QObject::tr("2")))
		{
			return "CIPASR";
		}
	}

	return "NotValid";
}

QString CDataProviderManager::CpsFileType(const QString& sfilename)//��ȡcps�ļ�����
{
	FILE *fp=NULL;

	std::string cfilename  = sfilename.toStdString();

	const char *filename = cfilename.c_str();
	fp = fopen(filename, "r");

	QString notValid = "NotValid";

	if (fp == NULL)
	{
		printf ("cannot open file");
		return notValid;
	}

	char	strTitle[10];			// CIPAS
	int		icpsType =0;			// cps��������

	fscanf(fp,"%s %d", strTitle, &icpsType);
	fclose(fp);

	if (QString(strTitle) == "CIPAS")
	{
		if (1==icpsType)
		{
			return "CIPASM";
		}
		else if (2==icpsType)
		{
			return "CIPASR";
		}
		else if (3==icpsType)
		{
			return "CIPASE";
		}
		else
			return notValid;
	}
	else
		return notValid;
}

QString CDataProviderManager::AWXFileType( const QString& sfilename )
{
	FILE *fp=NULL;

	//QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	//QByteArray encodedString = codec->fromUnicode(sfilename);
	//const char * path = encodedString.data();

	QByteArray tempByte1 = sfilename.toLocal8Bit();
	char * path = tempByte1.data();

	fp = fopen(path, "rb");

	QString notValid = "NotValid";

	if (fp == NULL)
	{
		printf ("cannot open file");
		return notValid;
	}

	char	strTitle[12];			//awx

	//fscanf(fp,"%s", strTitle);
	fread(strTitle,12,1,fp);
	fclose(fp);

	if (QString(strTitle[0]) == "T"||QString(strTitle[0]) == "D")
	{
		return "AWX";
	}
	else
		return notValid;
}
/************************************************************************/
/* �������ܣ� micaps4���ݸ�ʽ�ļ���д��
/* ����˵��:  elements_v Ҫ���� ���ļ�������
/* �� �� ֵ:   int 1��д��ɹ���0д��ʧ��
/* ��    ��:  �Ը���
/* ��    ��:  2011 �� 06 �� 28��
/************************************************************************/
int CDataProviderManager::WriteCiapsRaster(const QString& fileName, QList<CElement*> elements_v)//д�ļ�
{
	if(fileName.isEmpty() || elements_v.isEmpty())
	{
		return 0;
	}

	QFile file(fileName);
	QString StrTemp;
	CElement *pTempElement;
	QStringList DetialList;

	if (!file.open (QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return 0;
	}
	pTempElement = elements_v.first();
	QTextStream stream (&file);
	//------------------------------------------------------------headerSTART
	stream<<"CIPAS  ";
	stream<<2;								//�������ͣ��������� 1 (վ������) 2 ��������ݣ�
	stream<<"  ";
	stream<<2;								//������Ϣ��ʼ����
	stream<<"  ";
	stream<<7;								//��ͼ������ʼ����
	stream<<"  ";
	stream<<17;								//������Ϣ��ʼ����
	stream<<"\r\n";
	stream<<"#HEADINFO";
	stream<<"\r\n";
	stream<<"#ELEMENT_NAME  ";
	stream<<pTempElement->name().replace(QString(" "),QString("-"));			//Ҫ�ر�ʶ��
	stream<<"  ";
	stream<<pTempElement->name().replace(QString(" "),QString("-"));			//Ҫ�ر�ʶ��д
	stream<<"\r\n";
	stream<<"#DESCRIPTION   ";
	stream<<pTempElement->LongName();		//�ļ�������Ϣ
	stream<<"\r\n";

	stream<<"#DATA_STORAGE_FORMAT   ";
	stream<<"TXT";							//�ļ�������Ϣ
	stream<<"\r\n";

	stream<<"#COORD_SEQ    ";
	stream<<"XY";							//��ʶ����˳��ı�ʶ������XY��XT��YT��
	stream<<"\r\n";

	stream<<"#FORMATDEFINE    ";
	stream<<"f10.6 DEFAULT";				//�ļ����ݵ������ʽ����f14.10 100 6 "," ";"
	stream<<"\r\n";

	stream<<"#PROJECTION    ";
	stream<<"WSG84";						//ͶӰ��ʽ����WS584
	stream<<"\r\n";

	stream<<"#MISSING_VALUE    ";
	//stream<<QString::number(pTempElement->MissingValue());//���ݼ��е�ȱʡֵĬ��Ϊ999999
	stream<<QString::number(999999);
	stream<<"\r\n";

	stream<<"#DEFINE_X    ";
	stream<<"LONGITUDE FLOAT \"��\" ";			//X��ı�ʶ�� �������� ��λ X��ֵ�ĸ��� X��ķֱ��� X�����Сֵ X������ֵ
	stream<<QString::number(pTempElement->SelectedLon().m_count);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLon().m_delt);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLon().m_begin);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLon().m_end);
	stream<<"\r\n";

	stream<<"#DEFINE_Y    ";
	stream<<"LATITUDE FLOAT \"��\" ";			//Y��ı�ʶ�� �������� ��λ Y��ֵ�ĸ��� Y��ķֱ��� Y�����Сֵ Y������ֵ
	stream<<QString::number(pTempElement->SelectedLat().m_count);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLat().m_delt);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLat().m_begin);
	stream<<"  ";
	stream<<QString::number(pTempElement->SelectedLat().m_end);
	stream<<"\r\n";

	stream<<"#DEFINE_Z    ";
	stream<<"TIME INTEGER \"yyyyMMdd\" 1 CUSTOM  ";	//Z��ı�ʶ�� �������� ��λ Y��ֵ�ĸ��� Y��ķֱ��� Y�����Сֵ Y������ֵ
	stream<<pTempElement->SelectedPeriod().m_begin.toString(QString("yyyyMMdd"));
	stream<<"  ";
	stream<<pTempElement->SelectedPeriod().m_begin.toString(QString("yyyyMMdd"));
	stream<<"\r\n";

	stream<<"#TIME  ";
	stream<<"PDAY 365  ";							//ʱ��߶� ʱ��߶�ֵ ��ʼ�� ��ʼ�� ��ʼ�� ʱ�� ��ֹ�� ��ֹ�� ��ֹ�� ʱ��
	stream<<pTempElement->SelectedPeriod().m_begin.toString(QString("yyyy  MM  dd  hh"));
	stream<<"  ";
	stream<<pTempElement->SelectedPeriod().m_begin.toString(QString("yyyy  MM  dd  hh"));
	stream<<"\r\n";

	stream<<"#Z_LIST    ";							//Z[0]��ֵ Z[1]��ֵ ...�磺19980331 19980630 19980930 19981231
	stream<<pTempElement->SelectedPeriod().m_begin.toString(QString("yyyyMMdd"));
	stream<<"\r\n";

	stream<<"#DRAWSET -999";						//ȱʡֵΪ-999
	stream<<"\r\n";
	//-----------------------------------------------------------------------headerinfo END

	//-----------------------------------------------------------------------Data Start
	stream<<"#DATASET";
	stream<<"\r\n";
	stream<<"#DATA  ";
	stream<<1;
	stream<<"\r\n";

	int x =  pTempElement->ActualLon().m_count;
	int y = pTempElement->ActualLat().m_count;
	float *pDataArray = pTempElement->Data().GetDataArray(0);
	if (NULL == pDataArray)
	{
		return 0;
	}
	for (int i = 0 ; i<y;i++)//lat
	{
		stream.setFieldWidth(15);
		for (int j = 0 ;j<x;j++)//lon
		{
			stream<<QString::number(pDataArray[i*x+j],'f',6);
		}
		stream<<"\r\n";
	}
	stream<<";";
	//-----------------------------------------------------------------------Data END
	stream<<"#FILEOVER";						//
	stream<<"\r\n";

	return 1;
}

int CDataProviderManager::KillNoListElement()
{
	int i;
	for(i = 0;i < m_FileNamesNoListW.count();i++)
	{
		if (!CloseFileAndRemoveElements(m_FileNamesNoListW.at(i)))
			return 1;
	}
	m_FileNamesNoListW.clear();
	return 0;
}
