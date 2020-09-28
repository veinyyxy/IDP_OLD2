#include "CElement.h"
#include "BasicData.h"
#include "CElementInfo.h"
#include "ProductTypeManager.h"
#include <QDebug>

//#ifdef _DEBUG
//#include "vld.h"
//#endif

QString CElement::m_strFilePath = "";

//////////////////////////////////// CElement
/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CElement::CElement():m_scaleFactor(1.0)
{
	m_needDisplay = false;
	m_inited = false;
	m_addOffset = 0;
	m_missingValue = 0.0;
	m_isMemory = false;
	m_isRain = false;
}

/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CElement::CElement(const CElement& element)
{
	m_id = element.m_id;
	m_name = element.m_name;
	m_unit = element.m_unit; // 要素单位
	m_actualPeriod = element.m_actualPeriod; //时间范围
	m_selectedPeriod = element.m_selectedPeriod; //时间范围
	m_level = element.m_level; //高度层次
	m_scaleFactor = element.m_scaleFactor;
	m_addOffset = element.m_addOffset;
	m_missingValue = element.m_missingValue;
	m_selectedLon = element.m_selectedLon;
	m_actualLon = element.m_actualLon;
	m_selectedLat = element.m_selectedLat;
	m_actualLat = element.m_actualLat;
	m_selected = element.m_selected;
	m_needDisplay = element.m_needDisplay;
	m_Data = element.m_Data;
	m_MicapsDeclaration = element.m_MicapsDeclaration;
	m_fileType = element.m_fileType;
	m_inited = element.m_inited;
	m_fileName = element.m_fileName;
	m_longName = element.m_longName;
	m_isMemory = element.m_isMemory;
	m_CInterval = element.m_CInterval;
	//m_Interval = element.m_Interval;
	//m_contourValues = element.m_contourValues;
	//m_contourValues.clear();
	//for(int i = 0; i <element.m_contourValues.size(); i++ )
	//{
	//	m_contourValues.append(m_contourValues.at(i));
	//}
}

/************************************************************************/
/* 函数功能： 重载运算符构造函数  						                */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CElement& CElement::operator=( const CElement& element)
{
	m_id = element.m_id;
	m_name = element.m_name;
	m_unit = element.m_unit; // 要素单位
	m_actualPeriod = element.m_actualPeriod; //时间范围
	m_selectedPeriod = element.m_selectedPeriod; //时间范围
	m_level = element.m_level; //高度层次
	m_scaleFactor = element.m_scaleFactor;
	m_addOffset = element.m_addOffset;
	m_missingValue = element.m_missingValue;
	m_selectedLon = element.m_selectedLon;
	m_actualLon = element.m_actualLon;
	m_selectedLat = element.m_selectedLat;
	m_actualLat = element.m_actualLat;
	m_selected = element.m_selected;
	m_needDisplay = element.m_needDisplay;
	m_Data = element.m_Data;
	m_MicapsDeclaration = element.m_MicapsDeclaration;
	m_fileType = element.m_fileType;
	m_inited = element.m_inited;
	m_fileName = element.m_fileName;
	m_longName = element.m_longName;
	m_isMemory = element.m_isMemory;
	m_CInterval = element.m_CInterval;
	//m_contourValues = element.m_contourValues;
	//m_contourValues.clear();
	//for(int i = 0; i <element.m_contourValues.size(); i++ )
	//{
	//	m_contourValues.append(m_contourValues.at(i));
	//}
	return *this;
}

/************************************************************************/
/* 函数功能： 析构函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CElement::~CElement()
{
	RemoveData();

	QVector<CInterval*>::iterator iterInterval = m_CInterval.begin();
	while(iterInterval != m_CInterval.end())
	{
		//if (*iterInterval)
		//{
		//	delete *iterInterval;
		//}
		iterInterval = m_CInterval.erase(iterInterval);
	}

	//for (int i = 0;i < m_CInterval.count();i++)
	//{
	//	CInterval *tempEle = m_CInterval.at(i);
	//	QColor tempColor = tempEle->qColor;
	//	if (tempColor.isValid())
	//	{
	//		delete tempEle;
	//		tempEle = NULL;
	//	}
	//}
	//m_CInterval.clear();
}

/************************************************************************/
/* 函数功能： 获得ID                 			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_id				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
QString CElement::ID()
{
	return m_id;
}

/************************************************************************/
/* 函数功能： 获得ID                 			                        */
/* 参数说明:  QString& id												*/
/* 返 回 值： m_id				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void CElement::ID(const QString& id)
{
	m_id = id;
}

/************************************************************************/
/* 函数功能： 获得name                 			                        */
/* 参数说明:  NULL      												*/
/* 返 回 值： m_name			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
QString CElement::name()
{
	return m_name;
}
/************************************************************************/
/* 函数功能： 获得name                 			                        */
/* 参数说明:  QString& name												*/
/* 返 回 值： m_name			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void  CElement::name(const QString& name)
{
	m_name = name;
	CElementFactory* pf = CElementFactory::GetInstance();
	CElementInfo *pInfo = pf->GetElementInfo(QObject::tr("逐月"), m_name);
	if(pInfo)
	{
		m_ch_name =  pInfo->GetChineseName();
	}
	else
	{
		m_ch_name = m_name;
	}

	//设置long_name
	if(m_longName.isEmpty())
	{
		m_longName = name;
	}
}
/************************************************************************/
/* 函数功能： 获得中文名               			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_ch_name			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
QString CElement::GetChineseName()
{
	//return m_ch_name;
	return m_name;
	return m_longName;
}
/************************************************************************/
/* 函数功能： 获得unit                 			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_unit			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
QString CElement::unit()
{
	return m_unit;
}
/************************************************************************/
/* 函数功能： 获得unit                 			                        */
/* 参数说明:  QString& unit												*/
/* 返 回 值： m_unit			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void CElement::unit(const QString& unit)
{
	m_unit = unit;
}

////时间尺度
//CTimeScale& CElement::GetTimeScale()
//{
//	return m_timeScale;
//}
//void CElement::SetTimeScale(const CTimeScale& timeScale)
//{
//	m_timeScale = timeScale;
//}

/************************************************************************/
/* 函数功能： 时间范围                 			                        */
/* 参数说明:  NULL														*/
/* 返 回 值： 	return m_actualPeriod                                   */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CPeriod& CElement::ActualPeriod()
{
	return m_actualPeriod;
}
void CElement::ActualPeriod(const CPeriod& period)
{
	m_actualPeriod = period;
}
CPeriod& CElement::SelectedPeriod()
{
	return m_selectedPeriod;
}
void CElement::SelectedPeriod(const CPeriod& period)
{
	m_selectedPeriod = period;
}

void  CElement::ResetSelectedPeriod(const QDateTime& begin, const QDateTime& end)
{
	if(begin > end)
	{
		return;
	}

	long index1 = m_actualPeriod.Index(begin);
	long index2 = m_actualPeriod.Index(end);
	m_selectedPeriod.m_begin = begin;
	m_selectedPeriod.m_end = end;
	m_selectedPeriod.m_hashDate.clear();
	for(long i = index1; i <= index2; i++)
	{
		QDateTime dt = m_actualPeriod.Value(i);
		m_selectedPeriod.m_hashDate.insert(i, dt);
	}

	m_selectedPeriod.m_count = m_selectedPeriod.m_hashDate.size();
}
/************************************************************************/
/* 函数功能： list 中的时间从小到大排序			                        */
/* 参数说明:  QList<QDateTime>times										*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void  CElement::ResetSelectedPeriod(const QList<QDateTime> times)
{
	m_selectedPeriod.m_hashDate.clear();
	for(int i = 0; i < times.size(); i++)
	{
		QDateTime dt = times.at(i);
		int index = m_actualPeriod.Index(dt);
		m_selectedPeriod.m_hashDate.insert(i, dt);
	}
	m_selectedPeriod.m_count = m_selectedPeriod.m_hashDate.size();
}

/************************************************************************/
/* 函数功能： 经纬度范围 经纬尺度（精度）		                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_selectedLon		                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CLonLatRange& CElement::SelectedLon()
{
	return m_selectedLon;
}
void CElement::SelectedLon(const CLonLatRange &range)
{
	m_selectedLon = range;
}

void CElement::ResetSelectedLon(float begin, float end)
{
	m_selectedLon.m_begin = begin;
	m_selectedLon.m_end = end;
	m_selectedLon.ResetCount();
}
CLonLatRange& CElement::ActualLon()
{
	return m_actualLon;
}
void CElement::ActualLon(const CLonLatRange &range)
{
	m_actualLon = range;
}

CLonLatRange& CElement::SelectedLat()
{
	return m_selectedLat;
}
void CElement::SelectedLat(const CLonLatRange &range)
{
	m_selectedLat = range;
}
void CElement::ResetSelectedLat(float begin, float end)
{
	m_selectedLat.m_begin = begin;
	m_selectedLat.m_end = end;
	m_selectedLat.ResetCount();
}
CLonLatRange& CElement::ActualLat()
{
	return m_actualLat;
}
void CElement::ActualLat(const CLonLatRange &range)
{
	m_actualLat = range;
}

/************************************************************************/
/* 函数功能： 高度层次							                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_level			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CLevel& CElement::Level()
{
	return m_level;
}
void CElement::Level(const CLevel& level)
{
	m_level = level;
}
void CElement::ResetLevel(QList<float> levels)
{
	m_level.m_selected.clear();
	for(int i = 0; i < levels.size(); i++)
	{
		float level = levels.at(i);
		int index = m_level.m_valus.key(level);
		if(index != -1)
		{
			m_level.m_selected.insert(index, level);
		}
	}

	m_level.m_selectedCount = m_level.m_selected.size();
}
//selected
bool CElement::selected() const
{
	return m_selected;
}
void CElement::selected(bool selected)
{
	m_selected = selected;
}
/************************************************************************/
/* 函数功能： 放大、缩小倍数					                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_scaleFactor 	                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
float CElement::ScaleFactor() const
{
	return m_scaleFactor;
}

void CElement::ScaleFactor(const float sf)
{
	m_scaleFactor = sf;
}

/************************************************************************/
/* 函数功能： 偏移量							                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_addOffset		                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
float CElement::AddOffset() const
{
	return m_addOffset;
}
void  CElement::AddOffset(const float offset)
{
	m_addOffset = offset;
}

/************************************************************************/
/* 函数功能： 无效值							                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_missingValue	                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
float CElement::MissingValue() const
{
	return m_missingValue;
}
void CElement::MissingValue(const float mv)
{
	m_missingValue = mv;
}

//// 数据维数设置， 释放原先的内存
//void CElement::DataBlocks(int blocks)
//{
//	if(blocks <= 0)
//	{
//		return;
//	}
//
//	m_Data = CData(blocks);
//
//}
//
//int  CElement::DataBlocks()
//{
//	return m_Data.DataBlocksCount();
//}

/************************************************************************/
/* 函数功能： 数据维数选择						                        */
/* 参数说明:  NULL														*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
CData& CElement::Data()
{
	return m_Data;
}

void CElement::Data(const CData &data)
{
	m_Data = data;
}

void  CElement::Data(long *pSize, long iDims, float *pArray)
{
	m_Data = CData(pSize, iDims, pArray);
}

void  CElement::Data(QList<long> dimensions, float *pArray)
{
	m_Data = CData(dimensions, pArray);
}

/************************************************************************/
/* 函数功能： 增加基础数据块							                */
/* 参数说明:  *pSize iDims *pArray										*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void  CElement::AddData(long *pSize, long iDims, float *pArray)
{
	CBasicData basicData(pSize, iDims, pArray);
	m_Data.AddData(basicData);
}
void  CElement::AddData(QList<long> dimensions, float *pArray)
{
	CBasicData basicData(dimensions, pArray);
	m_Data.AddData(basicData);
}

/************************************************************************/
/* 函数功能： 获得基础数据块数量						                */
/* 参数说明:  NULL														*/
/* 返 回 值： m_Data.DataBlocksCount()                                  */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
int  CElement::DataBlocksCount() const
{
	return m_Data.DataBlocksCount();
}

/************************************************************************/
/* 函数功能： 删除某个基础数据块						                */
/* 参数说明:  blockIndex												*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
void  CElement::RemoveData()
{
	m_Data = CData();
}
/************************************************************************/
/* 函数功能： 删除某个基础数据块						                */
/* 参数说明:  blockIndex												*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
bool  CElement::RemoveDataBlock(int blockIndex)
{
	return m_Data.RemoveDataBlock(blockIndex);
}

/************************************************************************/
/* 函数功能： 获得某个数据块的维数				                        */
/* 参数说明:  blockIndex												*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
long  CElement::DataBlockDimensions(int blockIndex) const
{
	return m_Data.DataBlockDimensions(blockIndex);
}
//
/************************************************************************/
/* 函数功能： 获得某个数据块某维的大小			                        */
/* 参数说明:  blockIndex  dimensionIndex								*/
/* 返 回 值： NULL				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
long  CElement::DataBlockDimensionSize(int blockIndex, long dimensionIndex) const
{
	return m_Data.DataBlockDimensionSize(blockIndex, dimensionIndex);
}

/************************************************************************/
/* 函数功能： 获得某个数据块的数据指针			                        */
/* 参数说明:  blockIndex												*/
/* 返 回 值： m_Data.GetDataArray                                       */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
float* CElement::GetDataArray(int blockIndex) const
{
	return m_Data.GetDataArray(blockIndex);
}

long CElement::GetTotalSize(int blockIndex)
{
	long size = 0;
	CBasicData pData  = m_Data.GetBasicData(blockIndex);
	//if(pData)
	//{
	//	size = pData->TotalSize();
	//}

	size = pData.TotalSize();
	return size;
}

bool CElement::NeedDisplay() const
{
	return m_needDisplay;
}
void CElement::NeedDisplay(bool display)
{
	m_needDisplay = display;
}

/************************************************************************/
/* 函数功能： 要素值域范围						                        */
/* 参数说明:  NULL														*/
/* 返 回 值： m_valueRange		                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 04 月 08 日										*/
/************************************************************************/
QPair<CElement::ValueType, CElement::ValueType>  CElement::ValueRange()
{
	return m_valueRange;
}
void  CElement::ValueRange(const QPair<CElement::ValueType, CElement::ValueType> &range)
{
	m_valueRange.first = range.first;
	m_valueRange.second = range.second;
}

/************************************************************************
功能说明：设置micaps数据的描述信息
作      者：赵高攀
完成日期：2011年5月9日
************************************************************************/
void  CElement::SetMicapsDeclaration(QString strDeclaration)
{
	m_MicapsDeclaration <<strDeclaration;
}
/************************************************************************
功能说明：获取micaps数据的描述信息
作      者：赵高攀
完成日期：2011年5月9日
************************************************************************/
QStringList  CElement::GetMicapsDeclaration() const
{
	return  m_MicapsDeclaration;
}

//
QList<int> CElement::GetProducts()
{
	QList<int> products;

	if("MICAPS1" == m_fileType)
	{
		return products << 9 << 16 << 7 << 8 ;
	}
	else if("MICAPS2" == m_fileType)
	{
		return products << 10 << 17 << 7 << 8 ;
	}
	else if("MICAPS3" == m_fileType || "STATIONS" == m_fileType)
	{
		return products << 6 << 7 << 8;
	}
	else if("CIPASM" == m_fileType)
	{
		return products << 6 << 7 << 8;
	}
	else if("CIPASR" == m_fileType || "GRID" == m_fileType)
	{
		return products << 11 << 14 << 15;
	}
	else if("MICAPS4" == m_fileType)
	{
		return products << 11 << 14 << 15;
	}
	else if("MICAPS5" == m_fileType)
	{
		return products  << 12 << 13 ;
	}
	else if("MICAPS7" == m_fileType)
	{
		return products << 21 ;
	}
	else if("MICAPS8" == m_fileType)
	{
		return products << 10 << 7 << 8 ;
	}
	else if("STADA" == m_fileType)
	{
		return products << 10 << 7 << 8 ;
	}
	else if("MICAPS11" == m_fileType)
	{
		return products << 18 << 19 << 20 ;
	}
	else if("ncfile" == m_fileType)
	{
		return products << 1 << 2 <<22;
	}
	else if ("brokenline" == m_fileType)
	{
		return products<<23;
	}
	else if ("GRD" == m_fileType)
	{
		return products<<1<<2<<22;
	}
	else if("NetCDF" == m_fileType  || "GRIB" == m_fileType || "CTL" == m_fileType )
	{
		products << 1 << 2 <<22;
		if(m_name == "uwnd" || m_name == "vwnd")
		{
			products << 3 << 4 << 5;
		}
		return products;
	}else if("AWX" == m_fileType )
	{
		products << 1 << 2 <<24;
		return products;
	}else if("TXT" == m_fileType )
	{
		products << 6 << 7 <<8;
		return products;
	}

	return  products;
}

// 根据产品编号获得产品名称
QString CElement::GetProductName(int key)
{
	CProductTypeManager* pMgr = CProductTypeManager::GetInstance();
	if(pMgr)
	{
		return pMgr->ChineseName(key);
	}
	return QString();
}
void CElement::SetFileType(const QString &fileType)
{
	m_fileType = fileType;
}

QString CElement::GetFileType() const
{
	return m_fileType;
}

//要素基本信息是否初始化
void  CElement::Inited()
{
		m_inited = true;
}
bool  CElement::IsInited() const
{
	return m_inited;
}

void CElement::SetFileName(const QString& fileNameSrc)
{
	QString fileName = fileNameSrc;
	fileName = fileName.replace("\\", "/");
	m_fileName = fileName;
}

QString CElement::GetFileName() const
{
	return m_fileName;
}

void CElement::LongName(const QString& longName)
{
	m_longName = longName;
}
QString CElement::LongName() const
{
	return m_longName;
}

bool CElement::IsMemory() const
{
	return m_isMemory;
}

void CElement::IsMemory(bool type)
{
	m_isMemory = type;
}


/************************************************************************
功能说明：获取micaps数据
作      者：赵高攀
完成日期：2011年7月22日
************************************************************************/
QByteArray CElement::GetByteArray() const
{
	return m_ByteArray;
}
/************************************************************************
功能说明：存储数据
作      者：赵高攀
完成日期：2011年7月22日
************************************************************************/
void CElement::SetByteArray(const QByteArray& values)
{
	m_ByteArray = values;
}

void CElement::FilePath( QString strFilePath )
{
	m_strFilePath = strFilePath;
}

QString CElement::FilePath()
{
	if (m_strFilePath == "")
	{
		m_strFilePath = QString::fromLocal8Bit("这是个无效路径");//CIPASApplication::DataPath();
		QDir d(m_strFilePath);
		if ( !d.exists())
		{
			d.mkdir(m_strFilePath);
		}
	}
	return m_strFilePath;
};

//void CElement::setStationData( QList<TRANSFORMDB *> l_tranDB )
//{
//	m_list_transDB = l_tranDB;
//}


//QList<float> CElement::GetContourValues() const
//{
//	return m_contourValues;
//}
//
//void CElement::SetContourValues(const QList<float>& values)
//{
//	m_contourValues = values;
//	//m_contourValues.clear();
//	//for(int i = 0; i <values.size(); i++ )
//	//{
//	//	m_contourValues.append(values.at(i));
//	//}
//}


//QList<Interval> CElement::GetInterval() const
//{
//	return m_Interval;
//}
//
//
//void CElement::SetInterval( const QList<Interval>& interval)
//{
//	m_Interval = interval;
//}

QVector<CInterval*> CElement::GetCInterval()
{
	return m_CInterval;
}

void CElement::SetCInterval(QVector<CInterval*> interval)
{
	m_CInterval = interval;
}

//////////////////////////////////// end CElement

CInterval::CInterval()
{
	fValue = 0;
	qColor = NULL;
	strInfo = "";
}

CInterval::~CInterval()
{
	if (qColor)
	{
		delete qColor;
		qColor = NULL;
	}
}
