/*==================================================================================
项目名： 人机交互平台
作 者： 韩猛
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
-------------------------------------------------------------------------------------
文件名：CElement.h CElement.cpp
类 名： CElement
父 类： NULL
子 类： 无
功能说明：定义要素基本信息
调用说明：

-------------------------------------版本更新-----------------------------------------
V 1.2
作 者：输入作者（或修改者）名字
完成日期：
更新说明：
V 1.1
作 者：输入作者（或修改者）名字
完成日期：
更新说明：
V 1.0
原作者 ：韩猛
完成日期：2011年04月08日
=====================================================================================*/
#pragma once
#include "cdataelement_global.h"

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QByteArray>
#include <QtGui/QColor>
#include <QVector>

#include "BasicData.h"
#include "Data.h"
#include "Level.h"
#include "LonLatRange.h"
#include "Period.h"
#include "Station.h"
#include "TimeScale.h"
#include <QCoreApplication>
#include <QDir>


// 等经纬区域
typedef struct  
{
	float lonBegin;
	float lonDelt;
	float lonCount;
	float latBegin;
	float latDelt;
	float latCount;
	bool  valid; //是否有效
}AreaRange;

typedef struct TransformDB
{
	QString sDataType;	//站点数据类型
	int nStationCount;	//站点个数
	QString sName;		//站名
	float sID;			//ID
	QString sType;		//值类型，（要素）
	float sValue;		//值
	float sLever;		//级别
	float sLat;			//纬度
	float sLon;			//经度
	float sTime1;		//时间1
	float sTime2;		//时间2
	QString sCale;		//尺度
	float fTime;		//时间
}TRANSFORMDB;

//typedef struct Interval
//{
//	float fValue;
//	QVector4D qColor;
//	QString strInterval;
//}Interval;

class CDATAELEMENT_EXPORT CInterval
{
public:
	CInterval();
	~CInterval();

	float fValue;
	QColor *qColor;
	QString strInfo;
};

// 要素
class CDATAELEMENT_EXPORT CElement
{
public:
	typedef float  ValueType;

	CElement();
	CElement( const CElement& element);
	CElement& operator=(const CElement& element);

	~CElement();
	QString ID();
	void ID(const QString& id);

	////站点数据
	//void setStationData(QList<TRANSFORMDB *> l_tranDB);
	//inline QList<TRANSFORMDB *> getStationData(){return m_list_transDB;}

	QString name();
	void  name(const QString& name);

	QString GetChineseName();

	QString unit();
	void unit(const QString& unit);

	//时间尺度
	//CTimeScale& GetTimeScale();
	//void SetTimeScale(const CTimeScale& ts);

	//时间范围   （YYYYMMDDHHmmSS)
	CPeriod& ActualPeriod();
	void ActualPeriod(const CPeriod&);
	CPeriod& SelectedPeriod();
	void SelectedPeriod(const CPeriod&);
	void ResetSelectedPeriod(const QDateTime& begin, const QDateTime& end);
	void ResetSelectedPeriod(const QList<QDateTime> times);

	//经纬度范围 经纬尺度（精度）
	CLonLatRange& SelectedLon();
	void SelectedLon(const CLonLatRange &range);
	void ResetSelectedLon(float begin, float end);

	CLonLatRange& ActualLon();
	void ActualLon(const CLonLatRange &range);

	CLonLatRange& SelectedLat();
	void SelectedLat(const CLonLatRange &range);
	void ResetSelectedLat(float begin, float end);

	CLonLatRange& ActualLat();
	void ActualLat(const CLonLatRange &range);

	//高度层次
	CLevel& Level();
	void Level(const CLevel&);
	void ResetLevel(QList<float> levels);

	bool selected() const;
	void selected(bool selected);

	//放大、缩小倍数
	float  ScaleFactor() const;
	void ScaleFactor(const float  sf);

	// 偏移量
	float AddOffset() const;
	void  AddOffset(const float offset);

	// 无效值
	float MissingValue() const;
	void  MissingValue(const float  mv);

	// 要素值域范围
	QPair<ValueType, ValueType> ValueRange();
	void ValueRange(const QPair<ValueType, ValueType> &range);

	////设置维数大小
	//int  DataBlocks();
	//void DataBlocks(int size);
	//设置数据块
	CData& Data();
	void Data(const CData& data);
	void Data(long *pSize, long iDims, float *pArray);
	void Data(QList<long> dimensions, float *pArray);

	// 增加基础数据块
	void AddData(long *pSize, long iDims, float *pArray);
	void AddData(QList<long> dimensions, float *pArray);

	// 删除数据块
	void RemoveData();
	// 删除某个基础数据块
	bool RemoveDataBlock(int blockIndex);

	//获得基础数据块数量
	int DataBlocksCount() const;

	//获得某个数据块的维数
	long DataBlockDimensions(int blockIndex) const;
	// 获得某个数据块某维的大小
	long DataBlockDimensionSize(int blockIndex, long dimensionIndex) const;

	//获得某个数据
	bool GetData(float &retVal, int blockIndex, long *pSize, long size);
	bool GetData(float &retVal, int blockIndex, QList<long> indexes) const;
	//设置某个数据
	bool SetData(float value,   int blockIndex, long *pSize, long size);
	bool SetData(float value, int blockIndex, QList<long> indexes) const;

	float* GetDataArray(int blockIndex) const;
	//某个数据块中数据的大小
	long GetTotalSize(int blockIndex);

	bool NeedDisplay() const;
	void NeedDisplay(bool display);

	void SetMicapsDeclaration(QString strDeclaration);
	QStringList GetMicapsDeclaration() const;
	// 产品列表
	QList<int> GetProducts();
	static QString GetProductName(int key);

	void SetFileType(const QString& fileType);
	QString GetFileType() const;

	void Inited();
	bool IsInited() const;

	void SetFileName(const QString& fileName);
	QString GetFileName() const;

	void LongName(const QString& fileName);
	QString LongName() const;

	bool IsMemory() const;
	void IsMemory(bool type);

	//QList<float> GetContourValues() const;
	//void SetContourValues(const QList<float>& values);

//	QList<Interval> GetInterval() const;
//	void SetInterval(const QList<Interval>&);

	QVector<CInterval*> GetCInterval();
	void SetCInterval(QVector<CInterval*>);

	QByteArray GetByteArray() const;
	void SetByteArray(const QByteArray& values);

	//////////////////////////////////保存打开文件路径////////////////////////////////////////
	static QString m_strFilePath; //添加保存文件路径静态变量
	static void FilePath(QString strFilePath);
	static QString FilePath();

	inline bool GetRCheck(){return m_isRain;};
	inline void SetRCheck(bool b){m_isRain = b;};

	////////BUG阿东和嘉祥沟通不畅的结果
	QStringList m_showList;
private:
	QString m_id;     //对应数据规格书名书中的 Vxxxxxx
	QString m_name; // 要素名称
	QString m_ch_name; // 要素中文名称
	QString m_unit; // 要素单位
	//CTimeScale m_timeScale; //时间尺度

	CPeriod m_actualPeriod; //时间范围
	CPeriod m_selectedPeriod;

	CLevel m_level; //高度层次
	float  m_scaleFactor; //放大、缩小倍数
	float  m_addOffset; // 增加偏移量
	float  m_missingValue; //缺省值

    //经纬度范围 经纬尺度（精度）
	CLonLatRange m_selectedLon;
	CLonLatRange m_actualLon;
	CLonLatRange m_selectedLat;
	CLonLatRange m_actualLat;

	// 要素取值范围
	QPair<ValueType, ValueType> m_valueRange;
	bool m_selected;
	bool m_needDisplay; //是否需要显示
	CData m_Data;
	//int m_DataSize; //表示 m_pData 大小

	//micaps使用的数据（赵高攀）
	QStringList m_MicapsDeclaration;//存储顺序：类型，时间日期，文件描述,各个要素描述...

	QString m_fileType; //

	bool m_inited; //要素基本信息是否初始化

	QString m_fileName; //文件名称

	QString m_longName; //要素完整名称

	bool m_isMemory; //是否是内存数据

	QByteArray m_ByteArray;

	QVector<CInterval*> m_CInterval;
	//QList<Interval> m_Interval;
	//QList<float> m_contourValues;
	//QList<TRANSFORMDB *> m_list_transDB;
	bool m_isRain;
};