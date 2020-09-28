/*==================================================================================
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
-------------------------------------------------------------------------------------
�ļ�����CElement.h CElement.cpp
�� ���� CElement
�� �ࣺ NULL
�� �ࣺ ��
����˵��������Ҫ�ػ�����Ϣ
����˵����

-------------------------------------�汾����-----------------------------------------
V 1.2
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵����
V 1.1
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵����
V 1.0
ԭ���� ������
������ڣ�2011��04��08��
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


// �Ⱦ�γ����
typedef struct  
{
	float lonBegin;
	float lonDelt;
	float lonCount;
	float latBegin;
	float latDelt;
	float latCount;
	bool  valid; //�Ƿ���Ч
}AreaRange;

typedef struct TransformDB
{
	QString sDataType;	//վ����������
	int nStationCount;	//վ�����
	QString sName;		//վ��
	float sID;			//ID
	QString sType;		//ֵ���ͣ���Ҫ�أ�
	float sValue;		//ֵ
	float sLever;		//����
	float sLat;			//γ��
	float sLon;			//����
	float sTime1;		//ʱ��1
	float sTime2;		//ʱ��2
	QString sCale;		//�߶�
	float fTime;		//ʱ��
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

// Ҫ��
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

	////վ������
	//void setStationData(QList<TRANSFORMDB *> l_tranDB);
	//inline QList<TRANSFORMDB *> getStationData(){return m_list_transDB;}

	QString name();
	void  name(const QString& name);

	QString GetChineseName();

	QString unit();
	void unit(const QString& unit);

	//ʱ��߶�
	//CTimeScale& GetTimeScale();
	//void SetTimeScale(const CTimeScale& ts);

	//ʱ�䷶Χ   ��YYYYMMDDHHmmSS)
	CPeriod& ActualPeriod();
	void ActualPeriod(const CPeriod&);
	CPeriod& SelectedPeriod();
	void SelectedPeriod(const CPeriod&);
	void ResetSelectedPeriod(const QDateTime& begin, const QDateTime& end);
	void ResetSelectedPeriod(const QList<QDateTime> times);

	//��γ�ȷ�Χ ��γ�߶ȣ����ȣ�
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

	//�߶Ȳ��
	CLevel& Level();
	void Level(const CLevel&);
	void ResetLevel(QList<float> levels);

	bool selected() const;
	void selected(bool selected);

	//�Ŵ���С����
	float  ScaleFactor() const;
	void ScaleFactor(const float  sf);

	// ƫ����
	float AddOffset() const;
	void  AddOffset(const float offset);

	// ��Чֵ
	float MissingValue() const;
	void  MissingValue(const float  mv);

	// Ҫ��ֵ��Χ
	QPair<ValueType, ValueType> ValueRange();
	void ValueRange(const QPair<ValueType, ValueType> &range);

	////����ά����С
	//int  DataBlocks();
	//void DataBlocks(int size);
	//�������ݿ�
	CData& Data();
	void Data(const CData& data);
	void Data(long *pSize, long iDims, float *pArray);
	void Data(QList<long> dimensions, float *pArray);

	// ���ӻ������ݿ�
	void AddData(long *pSize, long iDims, float *pArray);
	void AddData(QList<long> dimensions, float *pArray);

	// ɾ�����ݿ�
	void RemoveData();
	// ɾ��ĳ���������ݿ�
	bool RemoveDataBlock(int blockIndex);

	//��û������ݿ�����
	int DataBlocksCount() const;

	//���ĳ�����ݿ��ά��
	long DataBlockDimensions(int blockIndex) const;
	// ���ĳ�����ݿ�ĳά�Ĵ�С
	long DataBlockDimensionSize(int blockIndex, long dimensionIndex) const;

	//���ĳ������
	bool GetData(float &retVal, int blockIndex, long *pSize, long size);
	bool GetData(float &retVal, int blockIndex, QList<long> indexes) const;
	//����ĳ������
	bool SetData(float value,   int blockIndex, long *pSize, long size);
	bool SetData(float value, int blockIndex, QList<long> indexes) const;

	float* GetDataArray(int blockIndex) const;
	//ĳ�����ݿ������ݵĴ�С
	long GetTotalSize(int blockIndex);

	bool NeedDisplay() const;
	void NeedDisplay(bool display);

	void SetMicapsDeclaration(QString strDeclaration);
	QStringList GetMicapsDeclaration() const;
	// ��Ʒ�б�
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

	//////////////////////////////////������ļ�·��////////////////////////////////////////
	static QString m_strFilePath; //��ӱ����ļ�·����̬����
	static void FilePath(QString strFilePath);
	static QString FilePath();

	inline bool GetRCheck(){return m_isRain;};
	inline void SetRCheck(bool b){m_isRain = b;};

	////////BUG�����ͼ��鹵ͨ�����Ľ��
	QStringList m_showList;
private:
	QString m_id;     //��Ӧ���ݹ���������е� Vxxxxxx
	QString m_name; // Ҫ������
	QString m_ch_name; // Ҫ����������
	QString m_unit; // Ҫ�ص�λ
	//CTimeScale m_timeScale; //ʱ��߶�

	CPeriod m_actualPeriod; //ʱ�䷶Χ
	CPeriod m_selectedPeriod;

	CLevel m_level; //�߶Ȳ��
	float  m_scaleFactor; //�Ŵ���С����
	float  m_addOffset; // ����ƫ����
	float  m_missingValue; //ȱʡֵ

    //��γ�ȷ�Χ ��γ�߶ȣ����ȣ�
	CLonLatRange m_selectedLon;
	CLonLatRange m_actualLon;
	CLonLatRange m_selectedLat;
	CLonLatRange m_actualLat;

	// Ҫ��ȡֵ��Χ
	QPair<ValueType, ValueType> m_valueRange;
	bool m_selected;
	bool m_needDisplay; //�Ƿ���Ҫ��ʾ
	CData m_Data;
	//int m_DataSize; //��ʾ m_pData ��С

	//micapsʹ�õ����ݣ��Ը��ʣ�
	QStringList m_MicapsDeclaration;//�洢˳�����ͣ�ʱ�����ڣ��ļ�����,����Ҫ������...

	QString m_fileType; //

	bool m_inited; //Ҫ�ػ�����Ϣ�Ƿ��ʼ��

	QString m_fileName; //�ļ�����

	QString m_longName; //Ҫ����������

	bool m_isMemory; //�Ƿ����ڴ�����

	QByteArray m_ByteArray;

	QVector<CInterval*> m_CInterval;
	//QList<Interval> m_Interval;
	//QList<float> m_contourValues;
	//QList<TRANSFORMDB *> m_list_transDB;
	bool m_isRain;
};