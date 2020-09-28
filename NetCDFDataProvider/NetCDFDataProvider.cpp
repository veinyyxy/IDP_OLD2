#include "../CDataElement/CommonFunction.h"
#include "NetCDFDataProvider.h"

NetCDFDataProvider::NetCDFDataProvider():m_prefileName(QString()), m_pNcFile(0)
{

}

NetCDFDataProvider::~NetCDFDataProvider()
{
	ClearVars();
}


QStringList NetCDFDataProvider::keys()
{
	return QStringList() <<QObject::tr("NETCDF") << QObject::tr("NC");
}

//装载数据源文件，初始化各种辅助信息
bool NetCDFDataProvider::LoadFile(const std::string& _filePath)
{	
	QString filePath = QString::fromStdString(_filePath);

	if(filePath.isEmpty())
	{ 
		return false;
	}

	// 与上次打开文件相同，返回 true
	//if(filePath == m_prefileName && m_fileLoaded.contains(filePath))
	//{
	//	return true;
	//}

	std::string stdPath = filePath.toStdString();
	const char * path = stdPath.data();

	NcFile *pNcFile = new NcFile(path);
	if(!pNcFile || pNcFile->is_valid() == FALSE)
	{
		//qDebug() << " NOT valid";
		return false;
	}

	//如果文件已经装载，则返回成功
	if(m_fileLoaded.contains(filePath))
	{
		if(m_pNcFile)
		{
			delete m_pNcFile;
		}
		m_pNcFile = pNcFile;
		m_prefileName = filePath;
		return true;
	}

	m_pNcFile = pNcFile;

	//PNcFileStruct pFileStruct = new NcFileStruct;
	//if(!pFileStruct)
	//{
	//	return false;
	//}
	//pFileStruct->fileName = filePath;

	// 清除上次保存的变量
	ClearVars();

	// 保存变量信息
	const int iVars = pNcFile->num_vars();
	for(int i = 0; i < iVars; ++i)
	{
		NcVar* pVar =  pNcFile->get_var(i);
		if(!pVar)
		{
			return false;
		}
		QString name = pVar->name();
		m_nameVarmap.insert(name, pVar);
	}

	// 初始化维度
	if(!InitDimensions(filePath, pNcFile))
	{
		// delete 
		return false;
	}



	// Init other element
	NcNameVarMap::iterator it = m_nameVarmap.begin();
	for(; it != m_nameVarmap.end(); ++it)
	{
		NcVar* pVar =  it.value();
		if(!pVar)
		{
			return false;
		}
		QString name = pVar->name();
		//init element
		CElement *pElement = m_elements.value(name);
		if(pElement && pElement->GetFileName() == filePath)
		{
			continue;
		}

		pElement = new CElement();
		if(!pElement)
		{
			// delete
			return false;
		}
		if(InitOtherElement(pElement, pVar) && pElement->IsInited())
		{
			pElement->SetFileType(QObject::tr("NetCDF"));
			pElement->SetFileName(filePath);
			m_elements.insertMulti(pElement->name(), pElement);
		}
		else
		{
			// delete 
			delete pElement;
			pElement = 0;
		}
	} // end for

	long id = m_fileLoaded.count() + 1;
	m_fileLoaded.insert(filePath, id);

	//分要素存储数据，存储进elements中
	SetDeclaration(filePath);//存储描述信息以及要素信息

	return true;

}

IDataProvider::HashElement NetCDFDataProvider::GetAllElements(const QString& filePath) const
{
	QHash<QString, CElement*> ret_elements;
	CElement *pElement = 0;
	QHash<QString, CElement*> :: const_iterator it = m_elements.constBegin();
	for(; it != m_elements.constEnd(); ++it)
	{
		pElement = it.value();
		if(pElement && pElement->GetFileName() == filePath)
		{
			ret_elements.insert(pElement->name(), pElement);
		}
	}

	return ret_elements;
}

//获取要素
CElement*  NetCDFDataProvider::GetElement(const QString&  elementName)
{
	return m_elements.value(elementName);
}

//存储相关要素数据
bool NetCDFDataProvider::StoreDetailValues( CElement* pElement)
{
	if(!m_pNcFile || !m_pNcFile->is_valid() || !pElement)
	{
		return false;
	}

	QString fileName = pElement->GetFileName();
	if(!LoadFile(fileName.toStdString()))
	{
		return false;
	}
	std::string stdEleName = pElement->name().toStdString();
	const char * name = stdEleName.data();
	NcVar* pVar = m_pNcFile->get_var(name);
	if(!pVar)
	{
		return false;
	}

	// get type 
	NcType type = pVar->type();

	// get dims
	int iDim_nums = pVar->num_dims();

	long *pBase = new long[iDim_nums];

	if(!pBase)
	{
		return false;
	}

	long *pCount = new long[iDim_nums];
	if(!pCount)
	{
		delete pBase;
		return false;
	}

	long *pSize = new long[iDim_nums];
	if(!pSize)
	{
		delete pBase;
		delete pCount;
		return false;
	}

	long iCounts  = 1; // iCount = latNum * lonNum
	int iDim_time = 0; // 时间所在的维数的下标
	int iDim_level = 0; //层所在维数的下标
	int iTime = 0; //　时间片段总数
	int iLevel = 0; // 层总数

	for(int i = 0; i < iDim_nums; i++) // Init base and count
	{
		// init 0
		pBase[i] = 0;
		pCount[i] = 1;
		NcDim *pDim = pVar->get_dim(i);
		if(!pDim)
		{
			continue;
		}
		QString dimName = QString(pDim->name());
		CElement *pDimElement = m_elements.value(dimName);
		CIPAS_DIM_CATEGORY catetory = m_elementDimMap.value(pDimElement);
		// longitude
		if(catetory  == CIPAS_LON)
		{
			pBase[i] = pElement->ActualLon().Index(pElement->SelectedLon().m_begin);
			pSize[i] = pElement->SelectedLon().m_count;
			pCount[i] = pElement->SelectedLon().m_count;			
			iCounts *= pCount[i];
			continue;
		}

		// latitude
		if(catetory  == CIPAS_LAT)
		{
			pBase[i] = pElement->ActualLat().Index(pElement->SelectedLat().m_begin);
			pSize[i] = pElement->SelectedLat().m_count;
			pCount[i] = pElement->SelectedLat().m_count;
			iCounts *= pCount[i];
			continue;
		}

		// level
		if(catetory  == CIPAS_LEVEL)
		{
			iDim_level = i;
			iLevel = pElement->Level().m_selected.size();
			pSize[i] = iLevel;
			pCount[i] = 1;	
			pBase[i] = 0;
			continue;

		}

		// time
		if(catetory  == CIPAS_TIME)
		{
			iDim_time = i;
			iTime = pElement->SelectedPeriod().m_hashDate.size();
			pSize[i] = iTime;
			pCount[i] = 1;
			pBase[i] = pElement->ActualPeriod().Index(pElement->SelectedPeriod().m_begin);
			continue;
		}

		// other dims
		pBase[i] = 0;
		pCount[i] = 1;



	}// ~~ FOR END Init base and count

	//选择一个或多个层
	if(iLevel > 0)
	{
		float *pVals = new float[iCounts * iLevel * iTime];
		if(!pVals)
		{
			delete pBase;
			delete pCount;
			//qDebug() << "malloc pVals error!";	
			return false;
		}
		//qDebug() << "pVal size:" << iCounts * iLevel * iTime;
		long offset = 0;
		int count = 0;

		QHash<long, QDateTime>::const_iterator itTime = pElement->SelectedPeriod().m_hashDate.constBegin();
		float *pDest = pVals;
		for(; itTime != pElement->SelectedPeriod().m_hashDate.constEnd(); itTime++)
		{   
			int index_time =  itTime.key();
			pBase[iDim_time] = itTime.key();

			QHash<int, float>::const_iterator itLevel = pElement->Level().m_selected.constBegin();
			for(; itLevel != pElement->Level().m_selected.constEnd(); itLevel++)
			{
				long index_level = itLevel.key();
				pBase[iDim_level] = itLevel.key();
				if(pVar->set_cur(pBase) == FALSE)
				{
					delete pBase;
					delete pCount;
					//qDebug() << "set cur error!";
					return false;
				}

				if(pVar->get(pDest, pCount) == FALSE)
				{
					delete pBase;
					delete pCount;
					//qDebug() << "get value error!";
					return false;
				}
				offset +=  iCounts;
				pDest += iCounts;
				count++;
				//qDebug() << "loop count:" << count << "offset:" << offset;
			}
		}


		float scaleFactor = pElement->ScaleFactor();
		float addOffset = pElement->AddOffset();
		float missingValue = pElement->MissingValue();
		float precision = 1;

		for(long i = 0; i < iCounts * iTime * iLevel; ++i)
		{
			if(qAbs(pVals[i] - missingValue) < precision) //缺省值，不做转换
			{
				continue;
			}
			pVals[i] = pVals[i] * scaleFactor + addOffset;
		}

		//long totalSize = 1;
		//for(int i = 0; i < iDim_nums; ++i)
		//{
		//	qDebug() << "pSize[" << i << "]=" << pSize[i];
		//	totalSize *= pSize[i];
		//}

		CData date(pSize, iDim_nums, pVals);
		pElement->Data(date);

		delete pBase;
		delete pCount;
		delete pSize;
		delete pVals;



		//QString new_fileName = QString("%1_%2.nc").arg(fileName).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
		//QVector<CElement*> v;
		//v.push_back(pElement);
		//Write(new_fileName, v);

		return true;
	}// iLevel > 0

	// iLevel <= 0
	float *pVals = new float[iCounts  * iTime];
	if(!pVals)
	{
		delete pBase;
		delete pCount;
		//qDebug() << "malloc pVals error!";	
		return false;
	}
	//qDebug() << "pVal size:" << iCounts  * iTime;
	long offset = 0;
	int count = 0;

	QHash<long, QDateTime>::const_iterator itTime = pElement->SelectedPeriod().m_hashDate.constBegin();
	float *pDest = pVals;
	for(; itTime != pElement->SelectedPeriod().m_hashDate.constEnd(); itTime++)
	{   
		int index_time =  itTime.key();
		pBase[iDim_time] = itTime.key();
		if(pVar->set_cur(pBase) == FALSE)
		{
			delete pBase;
			delete pCount;
			//qDebug() << "set cur error!";
			return false;
		}

		if(pVar->get(pDest, pCount) == FALSE)
		{
			delete pBase;
			delete pCount;
			//qDebug() << "get value error!";
			return false;
		}
		offset +=  iCounts;
		pDest += iCounts;
		count++;
		//qDebug() << "loop count:" << count << "offset:" << offset;

	}


	float scaleFactor = pElement->ScaleFactor();
	float addOffset = pElement->AddOffset();
	float missingValue = pElement->MissingValue();
	float precision = 1;

	for(long i = 0; i < iCounts * iTime; ++i)
	{
#if 0
		if(i < 100)
			qDebug() << "index:" << i << "value:[" << pVals[i] << "]";
#endif
		if(qAbs(pVals[i] - missingValue) < precision) //缺省值，不做转换
		{
			continue;
		}
		pVals[i] = pVals[i] * scaleFactor + addOffset;
		//if(i < 10 || (i < 50090 && i >49900) && i < iCounts*iTime)
		//qDebug() << "val[" << i <<"] =" << pVals[i]; 
	}

	for(int i = 0; i < iDim_nums; ++i)
	{
		//qDebug() << "pSize[" << i << "]=" << pSize[i];
	}

	CData date(pSize, iDim_nums,  pVals);
	pElement->Data(date);

	//QString new_fileName = QString("%1_%2.nc").arg(fileName).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
	//QVector<CElement*> v;
	//v.push_back(pElement);
	//Write(new_fileName, v);

	delete pBase;
	delete pSize;
	delete pCount; 
	delete pVals;

	return true;

}


//写文件

int NetCDFDataProvider::Write(const QString& fileTypeName)
{
	return 0;
}

int NetCDFDataProvider::Write(const QString& fileTypeName, QVector<CElement*> elements_v)
{

	if(fileTypeName.isEmpty() || elements_v.isEmpty())
	{
		return -1;
	}

	std::string stdFileName = fileTypeName.toStdString();

	const char *fileName = stdFileName.data();
	NcFile* pNcFile = new NcFile(fileName,NcFile::Replace,0,0U,NcFile::Classic);//实例化NcFile对象
	if(!pNcFile || !pNcFile->is_valid())
	{
		return -1;
	}

	// typedef data struct
	typedef QVector<NcDim*>P_NcDim;
	typedef QVector<NcVar*>P_NcVar;
	typedef QVector<NcAtt*>P_NcAtt;
	typedef QVector<NcAtt*>EachattArray;

	typedef QHash<QString, NcDim*> NamesDimsMap;
	typedef QHash<QString, NcVar*> NamesVarsMap;
	typedef QHash<QString, NcAtt*> NamesAttsMap;

	NamesDimsMap dimsMap;
	NamesVarsMap varsMap;
	NamesAttsMap attsMap;

	// 初始化维数 dimname = elementName + dimName
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

		QString name = pElement->name();
		// 经度
		//QString dimNameStr = QString("lon_%1").arg(name);
		QString dimNameStr = QString("lon");
		std::string stdString = dimNameStr.toStdString();
		NcToken dimName = stdString.data(); //dimNameStr.toLocal8Bit().data();
		long size = pElement->SelectedLon().m_count;
		if(size > 0)
		{
			//添加维度
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return -1;
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
					return -1;
				}				

			} // pVar

		} // end size > 0

		//纬度
		//dimNameStr = QString("lat_%1").arg(name);
		dimNameStr = QString("lat");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data(); 
		size = pElement->SelectedLat().m_count;
		if(size > 0)
		{
			//添加维度
			NcDim * pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return -1;
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
					return -1;
				}

			} // end pVar
		}// end Size > 0

		//层次
		//dimNameStr = QString("level_%1").arg(name);
		dimNameStr = QString("level");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data(); 
		size = pElement->Level().m_selected.size();
		if(size > 0)
		{
			//添加维度
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return -1;
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
					return -1;
				}

			}// end pVar

		}// end size > 0

		//时间
		//dimNameStr = QString("time_%1").arg(name);
		dimNameStr = QString("time");
		stdString = dimNameStr.toStdString();
		dimName = stdString.data(); 
		size = pElement->SelectedPeriod().m_hashDate.size();
		if(size > 0)
		{
			//添加维度
			NcDim *pDim = pNcFile->add_dim(dimName, size);
			if(!pDim)
			{
				return -1;
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
				for(int j = 0; it != pElement->SelectedPeriod().m_hashDate.constEnd(); ++j, ++it)
				{
					QDateTime dateTime = it.value();
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
					return -1;
				}

			}// end pvar

		} // end size > 0

		// 添加变量
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
			return -1;
		}
		stdString = name.toStdString();
		NcToken sName = stdString.data();
		NcVar *pVar = pNcFile->add_var(sName,  ncFloat, ndims, (const NcDim**)ppDim);
		if(!pVar)
		{
			return -1;
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
		//QFile data("output.txt");
		//if (data.open(QFile::WriteOnly | QFile::Truncate)) 
		//{
		//	QTextStream out(&data);
		//	for(long i = 0; i < totalSize; ++i)
		//	{
		//		if( i%144 == 0)
		//		{
		//			out << "\nline:" << i/144 <<"\t";
		//		}
		//		out << pVals[i] << ",";
		//	}
		//}
		if(!pVar->put(pVals, pCount))
		{
			return -1;
		}

	}// end for

	delete pNcFile;

	return 0;

} // end fuction

bool NetCDFDataProvider::CloseFileAndRemoveElements(const QString& fileName)
{
	//close file

	// remove data
	CElement *pElement = 0;
	QHash<QString, CElement*>::Iterator it = m_elements.begin();
	while( it != m_elements.end())
	{
		pElement = it.value();
		if(pElement && pElement->GetFileName() == fileName)
		{
			delete  pElement;
			pElement = 0;
			it = m_elements.erase(it++);
		}
		else
		{
			++it;
		}
	}

	QHash<QString, long> :: iterator itFile = m_fileLoaded.find(fileName);
	if(itFile != m_fileLoaded.end())
	{
		m_fileLoaded.erase(itFile);
	}
	return true;
}

// 清除变量
void NetCDFDataProvider::ClearVars()
{
	if(!m_pNcFile)
	{
		return;
	}

	QHash<QString, NcVar*>::iterator it; 
	for(it = m_nameVarmap.begin(); it != m_nameVarmap.end(); ++it)
	{
		NcVar* pVar = it.value();
		//if(pVar)
		//{
		//	delete pVar;
		//	pVar = 0;
		//}
		//delete it.value();
	}

	m_nameVarmap.clear();
}

bool NetCDFDataProvider::InitElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		//qDebug() << " Init Element Error";
		return false;
	}

	if(pElement->IsInited())
	{
		return true;
	}

	NcToken var_name = pVar->name();
	pElement->name(var_name);
	QString lowerVarName = pElement->name().toLower();
	//qDebug() << "var name:" << pVar->name();

	//1 经度
	if(lowerVarName == "lon" || lowerVarName == "longitude ")
	{
		return InitLongitudeElement(pElement, pVar);
	}

	//2 纬度
	if(lowerVarName == "lat" || lowerVarName == "latitude")
	{
		return InitLatitudeElement(pElement, pVar);
	}

	//3 level
	if(lowerVarName == "lev" || lowerVarName == "level")
	{
		return InitLevelElement(pElement, pVar);
	}

	//4 time
	if( lowerVarName == "time")
	{
		return InitTimeElement(pElement, pVar);
	}

	return false;

}

//初始化经度
bool NetCDFDataProvider::InitLongitudeElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		return false;
	}

	QString lowerVarName = QString(pVar->name()).toLower();
	pElement->name(lowerVarName);

	int iAtt = pVar->num_atts(); //属性数量
	NcAtt* pAtt = 0; 
	bool initRange = false;
	long num_vals = pVar->num_vals();	// 变量值数量
	CLonLatRange lonRange; //经度对象
	lonRange.m_count = num_vals;
	// get attribute
	for(int i  = 0; i < iAtt; ++i)
	{
		pAtt = pVar->get_att(i);
		QString lowerAttName = QString(pAtt->name()).toLower();

		// 0 long_name
		if(lowerAttName == "long_name")
		{
			pElement->LongName(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		//1 units
		if(lowerAttName == "units")
		{
			//qDebug() << "units Value:" << pAtt->as_string(0);
			lonRange.m_units = QString::fromLocal8Bit(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		//2 actual_range
		if(lowerAttName == "actual_range")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(2 == iNum_Vals)
			{
				lonRange.m_begin = pVar->as_float(0);
				lonRange.m_end =  pVar->as_float(num_vals-1);
				if(lonRange.m_count -1)
				{
					lonRange.m_delt = (lonRange.m_end - lonRange.m_begin)/(lonRange.m_count -1);
				}

				initRange = true;
				//qDebug() <<"longitude begin:" << lonRange.m_begin << "end:" << lonRange.m_end << "delt" << lonRange.m_delt;
			}

			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}// ~~if

	}// ~~ for

	// 未初始化最大最小值
	if(!initRange)
	{
		lonRange.m_begin = pVar->as_float(0);
		lonRange.m_end =  pVar->as_float(num_vals-1);
		if(lonRange.m_count -1)
		{
			lonRange.m_delt = (lonRange.m_end - lonRange.m_begin)/(lonRange.m_count -1);
		}
		//qDebug() <<"longitude begin:" << lonRange.m_begin << "end:" << lonRange.m_end << "delt" << lonRange.m_delt;
	}
	pElement->ActualLon(lonRange);
	pElement->SelectedLon(lonRange);
	pElement->Inited();
	pAtt = 0;
	return true;
}

//初始化纬度
bool NetCDFDataProvider::InitLatitudeElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		return false;
	}

	QString lowerVarName = QString(pVar->name()).toLower();
	pElement->name(lowerVarName);

	NcAtt* pAtt = 0;
	bool initRange = false;
	CLonLatRange latRange;
	long num_vals = pVar->num_vals();	
	int iAtt = pVar->num_atts();
	latRange.m_count = num_vals;
	// get attribute
	for(int i  = 0; i < iAtt; ++i)
	{
		pAtt = pVar->get_att(i);
		QString lowerAttName = QString(pAtt->name()).toLower();

		// 0 long_name
		if(lowerAttName == "long_name")
		{
			pElement->LongName(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		//1 units
		if(lowerAttName == "units")
		{
			//qDebug() << "units Value:" << pAtt->as_string(0);
			latRange.m_units = QString(pAtt->as_string(i));
			delete pAtt;
			continue;
		}

		//2 actual_range
		if(lowerAttName == "actual_range")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(2 == iNum_Vals)
			{
				latRange.m_begin = pVar->as_float(0);
				latRange.m_end =  pVar->as_float(num_vals-1);
				if(latRange.m_count -1)
				{
					latRange.m_delt = (latRange.m_end - latRange.m_begin)/(latRange.m_count -1);
				}

				initRange = true;
				//qDebug() <<"longitude begin:" << latRange.m_begin << "end:" << latRange.m_end << "delt" << latRange.m_delt;
			}

			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}// ~~if

	}// ~~ for

	// 未初始化最大最小值
	if(!initRange)
	{
		latRange.m_begin = pVar->as_float(0);
		latRange.m_end =  pVar->as_float(num_vals-1);
		if(latRange.m_count -1)
		{
			latRange.m_delt = (latRange.m_end - latRange.m_begin)/(latRange.m_count -1);
		}
		//qDebug() <<"longitude begin:" << latRange.m_begin << "end:" << latRange.m_end << "delt" << latRange.m_delt;
	}
	pElement->ActualLat(latRange);
	pElement->SelectedLat(latRange);
	pElement->Inited();
	pAtt = 0;
	return true;
}

//初始化层
bool NetCDFDataProvider::InitLevelElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		return false;
	}

	QString lowerVarName = QString(pVar->name()).toLower();
	pElement->name(lowerVarName);

	// 获得层次数据块值
	long nums = pVar->num_vals();
	float *pVals = new float[nums];
	CLevel levelRange;

	if(pVar->get(pVals,  nums) == TRUE)
	{
		//qDebug() << "get valus OK, level num :" << nums;
		for(int i = 0; i < nums; i++)
		{
			levelRange.m_valus.insert(i, pVals[i]);
			levelRange.m_selected.insert(i, pVals[i]);
		}

		levelRange.m_count = nums;
		levelRange.m_selectedCount = nums;
	}
	else
	{
		//qDebug() << "get Value error, level num :" << nums;
		levelRange.m_count = 0;
	}
	delete pVals;

	// get attribute
	int iAtt = pVar->num_atts();
	NcAtt* pAtt = 0;
	for(int i  = 0; i < iAtt; ++i)
	{
		NcAtt* pAtt = pVar->get_att(i);
		QString lowerAttName  = QString(pAtt->name()).toLower();

		// 0 long_name
		if(lowerAttName == "long_name")
		{
			pElement->LongName(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		// 1 units
		if(lowerAttName == "units")
		{
			//qDebug() << "units Value:" << pAtt->as_string(0);
			levelRange.m_units =  pAtt->as_string(0);
			delete pAtt;
			continue;
		}

		//actual_range
		if(lowerAttName == "actual_range")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(2 == iNum_Vals)
			{
				float begin = pVals->as_float(0);
				float end =  pVals->as_float(1);
				levelRange.m_range = QPair<float, float>(begin, end);
				//qDebug() <<"level begin:" << begin << "end:" << end ;
			}

			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}// ~~if

	}// ~~ for
	pElement->Level(levelRange);
	pElement->Inited();
	pAtt = 0;
	return true;
}

// 初始化时间
bool NetCDFDataProvider::InitTimeElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		return false;
	}

	QString lowerVarName = QString(pVar->name()).toLower();
	pElement->name(lowerVarName);

	// get values
	bool initRange = false;
	int iVal_nums = pVar->num_vals();
	int iAtt = pVar->num_atts();
	NcAtt* pAtt = 0;
	CPeriod dayRange;
	QDateTime NotValidDateTime = QDateTime::currentDateTime();
	QDateTime baseDateTime(NotValidDateTime);
	CTimeScale timeScaleDelt; //步长
	CTimeScale timeScaleDisplay; //存储时的时间表示方式： 小时或天

	//qDebug() << "time val_nums:" << iVal_nums;

	// 获得时间属性
	for(int i  = 0; i < iAtt; ++i)
	{
		NcAtt* pAtt = pVar->get_att(i);
		QString lowerAttName = QString(pAtt->name()).toLower();
		//qDebug()<< "Time" << i << "name:" << pAtt->name() << "value" << pAtt->as_string(0);

		// 0 long_name
		if(lowerAttName == "long_name")
		{
			pElement->LongName(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		//1 units
		if(lowerAttName == "units")
		{
			//qDebug() << "Time units Value:" << pAtt->as_string(0);
			//TODO: 时间尺度转换、显示
			QString timeUnit = pAtt->as_string(0);
			if(GetDateInfoFromUnit(baseDateTime, timeScaleDisplay, timeUnit));
			{
				//dayRange.SetTimeScale(timeScaleDisplay);
			}
			delete pAtt;
			continue;
		}
	}// ~~ for

	// 检验时间尺度有效性
	if(!timeScaleDisplay.Valid())
	{
		////;
	}

	// get value
	long nums = pVar->num_vals();
	float *pVals = new float[nums];
	if(pVar->get(pVals,  nums) == FALSE)
	{
		delete pVals;
		return false;
	}

	// 设置时间尺度
	if(nums < 2) //如果只有一个时间点，设置时间尺度为月
	{
		dayRange.SetTimeScale(CTimeScale(1, 2));
	}
	else
	{
		float deltTime = pVals[1] - pVals[0];
		if(deltTime < 1)
		{
			timeScaleDelt = timeScaleDisplay;
		}
		else
		{
			int unit = timeScaleDisplay.GetUnit();
			switch( unit)
			{
			case 0:
				delete pVals;
				return false;
				break;
			case 1: // year
				timeScaleDelt = CTimeScale(int(deltTime), 1);
				break;

			case 2: // month
				if(deltTime >= 12)
				{
					timeScaleDelt = CTimeScale(int(deltTime/12), 1);
				}
				else
				{
					timeScaleDelt = CTimeScale(int(deltTime), 2);
				}
				break;

			case 3: // day
				if(deltTime >= 365)
				{
					timeScaleDelt = CTimeScale(int(deltTime/365), 1);
				}
				else if(deltTime >= 28)
				{
					timeScaleDelt = CTimeScale(int(deltTime/28), 2);
				}
				else 
				{
					timeScaleDelt = CTimeScale(int(deltTime), 3);
				}
				break;

			case 4: // hour
				if(deltTime >= 365 * 24)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(365*24)), 1);
				}
				else if(deltTime >= 28*24)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(28*24)), 2);
				}
				else if(deltTime >= 24)
				{
					timeScaleDelt = CTimeScale(int(deltTime/24), 3);
				}
				else
				{
					timeScaleDelt = CTimeScale(int(deltTime), 4);
				}
				break;

			case 5: //minute
				if(deltTime >= 365 * 24 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(365*24*60)), 1);
				}
				else if(deltTime >= 28*24 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(28*24*60)), 2);
				}
				else if(deltTime >= 24 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(24*60)), 3);
				}
				else if(deltTime >= 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/60), 4);
				}
				else
				{
					timeScaleDelt = CTimeScale(int(deltTime), 5);
				}
				break;

			case 6: // second
				if(deltTime >= 365 * 24 * 60 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(365*24*60* 60)), 1);
				}
				else if(deltTime >= 28*24 * 60 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(28*24*60* 60)), 2);
				}
				else if(deltTime >= 24 * 60 * 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/(24*60* 60)), 3);
				}
				else if(deltTime >= 60* 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/60* 60), 4);
				}
				else if(deltTime >= 60)
				{
					timeScaleDelt = CTimeScale(int(deltTime/60), 5);
				}
				else
				{
					timeScaleDelt = CTimeScale(int(deltTime), 6);
				}
				break;

			default:
				break;
			}// ~~ end switch
		}
	}
	if(timeScaleDelt.Valid())
	{
		dayRange.SetTimeScale(timeScaleDelt);
	}
	else
	{
		dayRange.SetTimeScale(timeScaleDisplay);
	}
	QDate base  = baseDateTime.date();
	//qDebug() << "........base date, year:" << base.year() << "month" <<base.month() << "day:" << base.day();
	int unit = timeScaleDisplay.GetUnit();
	QDate date;
	switch( unit)
	{
	case 0:
		delete pVals;
		return false;
		break;
	case 1: // year
		for(int i = 0; i< nums; ++i)
		{
			QDate date = baseDateTime.date().addYears(pVals[i]);
			QDateTime inTime(QDate(date.year(), 1, 1), QTime(0,0));
			dayRange.m_hashDate.insert(i,inTime);
		}
		dayRange.m_count = dayRange.m_hashDate.size();

		date = baseDateTime.date().addYears(pVals[0]);
		dayRange.m_begin = QDateTime(QDate(date.year(), 1, 1), QTime(0,0));
		date = baseDateTime.date().addYears(pVals[nums-1]);
		dayRange.m_end =  QDateTime(QDate(date.year(), 1, 1), QTime(0,0));

		break;

	case 2: // month
		for(int i = 0; i< nums; ++i)
		{
			QDate date = baseDateTime.date().addMonths(pVals[i]);
			QDateTime inTime(QDate(date.year(), date.month(), 1), QTime(0,0));
			dayRange.m_hashDate.insert(i,inTime);
		}
		dayRange.m_count = dayRange.m_hashDate.size();

		date = baseDateTime.date().addMonths(pVals[0]);
		dayRange.m_begin = QDateTime(QDate(date.year(), date.month(), 1), QTime(0,0));
		date = baseDateTime.date().addMonths(pVals[nums-1]);
		dayRange.m_end =  QDateTime(QDate(date.year(), date.month(), 1), QTime(0,0));

		break;

	case 3: // day
		for(int i = 0; i< nums; ++i)
		{
			QDate date = baseDateTime.date().addDays(pVals[i]);
			QDateTime inTime(date, QTime(0,0));
			dayRange.m_hashDate.insert(i,inTime);
			//qDebug() << "i = " << i << "year" << inTime.date().year() << "month" << inTime.date().month() << "day" << inTime.date().day();
		}
		dayRange.m_count = dayRange.m_hashDate.size();	

		date = baseDateTime.date().addDays(pVals[0]);
		dayRange.m_begin = QDateTime(date, QTime(0,0));
		date = baseDateTime.date().addDays(pVals[nums-1]);
		dayRange.m_end =  QDateTime(date, QTime(0,0));

		break;

	case 4: // hour
		for(int i = 0; i< nums; ++i)
		{
			QDateTime inTime = baseDateTime.addDays(pVals[i]/24);
			inTime = inTime.addSecs((int(pVals[i])%24)*3600);
			dayRange.m_hashDate.insert(i,inTime);
			//qDebug() << "i = " << i << "year" << inTime.date().year() << "month" << inTime.date().month() << "day" << inTime.date().day();
		}
		dayRange.m_count = dayRange.m_hashDate.size();		
		dayRange.m_begin = baseDateTime.addDays(pVals[0]/24);
		dayRange.m_begin = dayRange.m_begin.addSecs(int(pVals[0])%24);
		dayRange.m_end = baseDateTime.addDays(pVals[nums -1]/24);
		dayRange.m_end = dayRange.m_end.addSecs(int(pVals[nums -1])%24);
		break;

	case 5: //minute
		for(int i = 0; i< nums; ++i)
		{
			QDateTime inTime = baseDateTime.addDays(pVals[i]/(24*60));
			inTime = inTime.addSecs((int(pVals[i])%(24*60))*60);
			dayRange.m_hashDate.insert(i,inTime);
		}
		dayRange.m_count = dayRange.m_hashDate.size();		
		dayRange.m_begin = baseDateTime.addDays(pVals[0]/(24*60));
		dayRange.m_begin = dayRange.m_begin.addSecs(int(pVals[0])%(24*60));
		dayRange.m_end = baseDateTime.addDays(pVals[nums -1]/(24*60));
		dayRange.m_end = dayRange.m_end.addSecs(int(pVals[nums -1])%(24*60));
		break;

	case 6: // second
		for(int i = 0; i< nums; ++i)
		{
			QDateTime inTime = baseDateTime.addDays(pVals[i]/(24*3600));
			inTime = inTime.addSecs(int(pVals[i])%(24*3600));
			dayRange.m_hashDate.insert(i,inTime);
		}
		dayRange.m_count = dayRange.m_hashDate.size();		
		dayRange.m_begin = baseDateTime.addDays(pVals[0]/(24*3600));
		dayRange.m_begin = dayRange.m_begin.addSecs(int(pVals[0])%(24*3600));
		dayRange.m_end = baseDateTime.addDays(pVals[nums -1]/(24*3600));
		dayRange.m_end = dayRange.m_end.addSecs(int (pVals[nums -1])%(24*3600));
		break;

	default:
		break;
	}// ~~ end switch

	pElement->ActualPeriod(dayRange);
	pElement->SelectedPeriod(dayRange);
	pElement->Inited();
	delete pVals;
	pAtt = 0;
	return true;
}


bool NetCDFDataProvider::InitOtherElement(CElement* pElement, const NcVar* pVar)
{
	if(!pElement || !pVar)
	{
		return false;
	}

	QString name = pVar->name();
	pElement->name(name);
	const int iDim_nums = pVar->num_dims();
	//NcDim* pDim = 0;
	for(int i = 0; i < iDim_nums; ++i)
	{
		NcDim* pDim = pVar->get_dim(i);
		if(!pDim || !pDim->is_valid())
		{
			continue;
		}

		QString dimName = pDim->name();
		CElement *pDimElement = m_elements.value(dimName);
		if(pDimElement)
		{
			CIPAS_DIM_CATEGORY dim_category = m_elementDimMap.value(pDimElement);
			switch(dim_category)
			{
			case CIPAS_LON:
				pElement->ActualLon(pDimElement->ActualLon());
				pElement->SelectedLon(pDimElement->SelectedLon());
				break;
			case CIPAS_LAT:
				pElement->ActualLat(pDimElement->ActualLat());
				pElement->SelectedLat(pDimElement->SelectedLat());
				break;
			case CIPAS_LEVEL:
				pElement->Level(pDimElement->Level());
				break;
			case CIPAS_TIME:
				pElement->ActualPeriod(pDimElement->ActualPeriod());
				pElement->SelectedPeriod(pDimElement->SelectedPeriod());
				break;
			default:
				break;
			}// ~~ switch

			// delete pDim;
			pDim = 0;
		}// end if
		else
		{
			// delete pDim;
			pDim = 0;
		}	
	} // ~~ for

	// get attribute
	int iAtt = pVar->num_atts();
	NcAtt* pAtt = 0;
	for(int i  = 0; i < iAtt; ++i)
	{
		NcAtt* pAtt = pVar->get_att(i);
		QString lowerAttName = QString(pAtt->name()).toLower();
		//qDebug()<< "other name:" << pAtt->name() << "value:" << pAtt->as_string(0);

		// 0 long_name
		if(lowerAttName == "long_name")
		{
			pElement->LongName(pAtt->as_string(0));
			delete pAtt;
			continue;
		}

		//units
		if(lowerAttName == "units")
		{
			pElement->unit(pAtt->as_string(i));
			delete pAtt;
			continue;
		}

		//actual_range
		if(lowerAttName == "actual_range")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(2 == iNum_Vals)
			{
				float begin = pVals->as_double(0);
				float end =  pVals->as_double(1);
				QPair<float, float> range(begin, end);
				pElement->ValueRange(range);
				//qDebug() <<"other actual_range begin:" << begin << "end:" << end ;
			}
			delete pVals;
			delete pAtt;
			continue;
		}

		//scale factor
		if(lowerAttName == "scale_factor")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(1 == iNum_Vals)
			{
				float scaleFactor = pVals->as_double(0);
				pElement->ScaleFactor(scaleFactor);
			}
			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}			

		//add offset
		if(lowerAttName == "add_offset")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(1 == iNum_Vals)
			{
				float add_offset = pVals->as_double(0);
				pElement->AddOffset(add_offset);
			}
			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}

		// missing value
		if(lowerAttName == "missing_value")
		{
			NcValues* pVals = pAtt->values();
			int iNum_Vals = pVals->num();
			if(1 == iNum_Vals)
			{
				float mis_val = pVals->as_double(0);
				pElement->MissingValue(mis_val);
				//qDebug() <<" missing_value:" << mis_val;
			}

			delete pVals;
			pVals = 0;
			delete pAtt;
			continue;
		}

	} //~~ for

	if(pElement->ActualLon().m_count > 0 && pElement->ActualLat().m_count > 0)
	{
		pElement->NeedDisplay(true);
	}

	pElement->Inited();
	pAtt = 0;
	return true;
}


void test_print(NcVar *pVar)
{
	if(!pVar)
		return;

	//qDebug() << "days: <<<<<<<" << 17593032/(24*365);
	int iAtt = pVar->num_atts();
	NcAtt* pAtt = 0;
	for(int i  = 0; i < iAtt; ++i)
	{
		NcAtt* pAtt = pVar->get_att(i);
		NcToken att_name = pAtt->name();
		//qDebug()<< "name:" << pAtt->name() << "as_String" << pAtt->as_string(0);
		delete pAtt;
	}

	//qDebug() << " values <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
	NcValues* pVals = pVar->values();
	int iNums  = pVals->num();
	for(int i = 0; i< iNums; ++i)
	{
		printf("%f, ", pVals->as_float(i));
	}
	//pVals->print(std::sSTDOUT);

	delete pVals;


}

QDate NetCDFDataProvider::TransToDate(const int days, QDate baseDate) // 从公元1年1月1日的天数
{
	long offset = baseDate.toJulianDay();
	int jd = days + offset;
	QDate date = QDate::fromJulianDay(jd);
	return date;
}

int NetCDFDataProvider::TransToDays(const QDate& date,  QDate baseDate)  // 日期转换为从公元1年1月1日的天数
{
	long offset = baseDate.toJulianDay();
	int jd = date.toJulianDay();
	jd -= offset;
	return jd;
}

bool NetCDFDataProvider::InitDimensions(const QString &fileName, NcFile* pNcFile)
{
	// get dimensions 
	int iDims = pNcFile->num_dims();
	CIPAS_DIM_CATEGORY dim_category = CIPAS_NOTVALID;
	for(int i = 0; i < iDims; ++i)
	{
		dim_category = CIPAS_NOTVALID;
		NcDim* pDim = pNcFile->get_dim(i);
		if(!pDim)
		{
			return false;
		}
		NcToken pName = pDim->name();
		NcVar *pVar = m_nameVarmap.value(pName, NULL);
		if(!pVar)
		{
			//// delete pDim;
			continue;
		}
		dim_category = DimensionCategory(pVar);
		if(CIPAS_NOTVALID == dim_category)
		{
			//delete pVar;
			return false;
		}

		CElement *pElement = new CElement();
		if(!pElement)
		{
			//delete pVar;
			return false;
		}
		switch(dim_category)
		{
		case CIPAS_LON:
			if(!InitLongitudeElement(pElement, pVar))
			{
				//delete pVar;
				delete pElement;
				return false;
			}
			pElement->SetFileName(fileName);
			m_elements.insertMulti(pName, pElement);
			break;
		case CIPAS_LAT:
			if(!InitLatitudeElement(pElement, pVar))
			{
				//delete pVar;
				delete pElement;
				return false;
			}
			pElement->SetFileName(fileName);
			m_elements.insertMulti(pName, pElement);
			break;
		case CIPAS_LEVEL:
			if(!InitLevelElement(pElement, pVar))
			{
				//delete pVar;
				delete pElement;
				return false;
			}
			pElement->SetFileName(fileName);
			m_elements.insertMulti(pName, pElement);
			break;
		case CIPAS_TIME:
			if(!InitTimeElement(pElement, pVar))
			{
				//delete pVar;
				delete pElement;
				return false;
			}
			pElement->SetFileName(fileName);
			m_elements.insertMulti(pName, pElement);
			break;
		default:
			break;
		}

		if(CIPAS_NOTVALID != dim_category)
		{
			pElement->SetFileType("NetCDF");
			pElement->SetFileName(fileName);
			m_elementDimMap.insert(pElement, dim_category);
			m_elements.insert(pName, pElement);
		}
		//qDebug() << "........Dimension id:" << pDim->id() << ", Name:" << pDim->name() << ", size:" << pDim->size() << ", unlimited:" << pDim->is_unlimited();
	}

	return true;
}



CIPAS_DIM_CATEGORY NetCDFDataProvider::DimensionCategory(NcVar *pVar)
{
	if(!pVar)
	{
		return CIPAS_NOTVALID;
	}

	NcToken name = pVar->name();
	QString lowerName = QString(name).toLower();
	QString longName = lowerName;
	NcAtt *pAtt = pVar->get_att("long_name");
	if(!pAtt)
	{
		pAtt = pVar->get_att("longname");
	}
	if(pAtt)
	{
		//long size = pAtt->num_vals();
		longName = QString(pAtt->as_string(0)).toLower();
	}

	if("lon" == lowerName || "longitude" == lowerName || longName.contains("longitude"))
	{
		return CIPAS_LON;
	}

	if("lat" == lowerName || "latitude" == lowerName || longName.contains("latitude"))
	{
		return CIPAS_LAT;
	}

	if("lev" == lowerName || "level" == lowerName || longName.contains("level") || longName.contains("depth")) 
	{
		return CIPAS_LEVEL;
	}

	if("time" == lowerName || longName.contains("time"))
	{
		return CIPAS_TIME;
	}

	//取坐标投影
	pAtt = pVar->get_att("cartesian_axis");
	if(!pAtt)
	{
		return CIPAS_LON;
	}

	QString cartesian_axis = QString(pAtt->as_string(0)).toLower();
	if(cartesian_axis == "x")
	{
		return CIPAS_LON;
	}

	if(cartesian_axis == "y")
	{
		return CIPAS_LAT;
	}

	if(cartesian_axis == "z")
	{
		return CIPAS_LEVEL;
	}

	if(cartesian_axis == "t")
	{
		return CIPAS_TIME;
	}

	return CIPAS_LON;
}


CIPAS_DIM_CATEGORY NetCDFDataProvider::DimensionCategory(const QString& name, const QString& longName, const QString& cartesian_axis)
{

	QString lowerName = QString(name).toLower();
	QString lowerLongName = QString(longName).toLower();

	if("lon" == lowerName || "longitude" == lowerName || lowerLongName.contains("longitude"))
	{
		return CIPAS_LON;
	}

	if("lat" == lowerName || "latitude" == lowerName || lowerLongName.contains("latitude"))
	{
		return CIPAS_LAT;
	}

	if("lev" == lowerName || "level" == lowerName || lowerLongName.contains("level") || lowerLongName.contains("depth")) 
	{
		return CIPAS_LEVEL;
	}

	if("time" == lowerName || lowerLongName.contains("time"))
	{
		return CIPAS_TIME;
	}

	QString axis = cartesian_axis.toLower();
	if(axis == "x")
	{
		return CIPAS_LON;
	}

	if(axis == "y")
	{
		return CIPAS_LAT;
	}

	if(axis == "z")
	{
		return CIPAS_LEVEL;
	}

	if(axis == "t")
	{
		return CIPAS_TIME;
	}

	return CIPAS_LON;
}

//若是Micaps，返回数据描述信息，否则返回 QString（）空串
QString NetCDFDataProvider::GetDataDescription(const QString& filePath)
{
	return QString();
}

//若是Micaps，返回元素名称列表，否则返回 QStringList（）空列表
QStringList NetCDFDataProvider::GetElementNames(const QString& filePath) 
{
	return QStringList();
}

void NetCDFDataProvider::SetDeclaration( const QString& filePath )
{
	IDataProvider::HashElement curHashEle = GetAllElements(filePath);
	CElement *pcurEleTemp = 0;
	QStringList displayEleList;
	QHash<QString, CElement*> :: const_iterator it = curHashEle.constBegin();
	for(; it != curHashEle.constEnd(); ++it)
	{
		pcurEleTemp = it.value();
		if(pcurEleTemp && pcurEleTemp->NeedDisplay() == true)
		{
			displayEleList.append(pcurEleTemp->name());
		}
	}

	QString strdata;
	//文件描述信息格式化
	CElement *pEle = new CElement();

	pEle->name(QObject::tr("文件描述"));
	pEle->SetMicapsDeclaration("数据类型：NetCDF");

	CElement *pEleTemp = curHashEle.value(QString("lon"));
	if (pEleTemp)
	{
		strdata = QObject::tr("数据经度范围：");
		strdata = strdata + QString::number(pEleTemp->ActualLon().m_begin) + QString(" ~ ") + QString::number(pEleTemp->ActualLon().m_end);
		pEle->SetMicapsDeclaration(strdata);
	}

	pEleTemp = curHashEle.value(QString("lat"));
	if (pEleTemp)
	{
		strdata = QObject::tr("数据纬度范围：");
		strdata = strdata + QString::number(pEleTemp->ActualLat().m_begin) + QString(" ~ ") + QString::number(pEleTemp->ActualLat().m_end);
		pEle->SetMicapsDeclaration(strdata);
	}

	pEleTemp = curHashEle.value(QString("level"));
	if (pEleTemp)
	{
		strdata = QObject::tr("数据层次范围：");
		strdata = strdata + QString::number(pEleTemp->Level().m_range.first) + QString(" ~ ") + QString::number(pEleTemp->Level().m_range.second);
		pEle->SetMicapsDeclaration(strdata);
	}

	pEleTemp = curHashEle.value(QString("time"));
	if (pEleTemp)
	{
		strdata = QObject::tr("数据时间范围：");
		strdata = strdata + pEleTemp->ActualPeriod().m_begin.date().toString() + QString(" ~ ") + pEleTemp->ActualPeriod().m_end.date().toString();
		pEle->SetMicapsDeclaration(strdata);
	}

	strdata = QObject::tr("数据包含元素：");
	pEle->SetMicapsDeclaration(strdata);
	for (int i = 0;i < displayEleList.count();i++)
	{
		strdata = QString(".............") + displayEleList.at(i);
		pEle->SetMicapsDeclaration(strdata);
	}

	pEle->NeedDisplay(false);
	pEle->SetFileType("NetCDF");
	pEle->SetFileName(filePath);
	m_elements.insert(pEle->name(), pEle);
}

Q_EXPORT_PLUGIN2(IDataProvider,  NetCDFDataProvider) 