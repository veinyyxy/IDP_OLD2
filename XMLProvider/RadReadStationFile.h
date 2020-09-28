/*============================================================ 
文件名：CRadReadStationFile.h  
类 名： CRadReadStationFile
父 类： QWidget
子 类： 无 
功能说明：从二进制数据文件中读取站点信息
调用说明：

----------------------------版本更新----------------------------
V 1.0 
原作者 ：renxue
完成日期：2012年03月07日
============================================================*/
#pragma once
#include "XML_DPConfig.h"

#include <QtGui/QFileDialog>
#include <QtCore/QDataStream>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QHash>


typedef struct CINRAD_StationInfo 
{
	QString cID; //radar site name, in Chinese
	QString cName; //radar site name, in Chinese
	QString cProvince; //province name, in Chinese
	QString cRadType; // CINRAD/XX
	int  nLat; // units:0.0001deg
	int  nLon; // units:0.0001deg
	int  nHei; // units:0.1meter
	int  nDataRange; // units in 1km
	int  nTime; //time clock of products generator
	int  nWestLon; // westest of LatLonGrid of radar coverage, in 0.001deg
	int  nEastLon; // eastest of LatLonGrid of radar coverage, in 0.001deg
	int  nNorthLat; // northest of LatLonGrid of radar coverage, in 0.001deg
	int  nSouthLat; // southest of LatLonGrid of radar coverage, in 0.001deg
	QString cSpare; // tail;
}CINRAD_STATIONINFO, *P_CINRAD_STATIONINFO;

typedef QHash<QString, P_CINRAD_STATIONINFO> RADSTATION_HASH;
typedef QHashIterator<QString, P_CINRAD_STATIONINFO> RADSTATION_HASH_ITERATOR;

class XML_EXPORT_IMPORT CRadReadStationFile : public QObject
{
public:
	CRadReadStationFile();
	~CRadReadStationFile(void);

	/*************************************************************
	功能说明：读十四类数据文件
	参数说明：fileName是包含路径和文件名的字符串
	          返回值为bool型，正确执行返回true，错误或异常返回false
	**************************************************************/
	bool ReadFile(const QString &fileName);	

	RADSTATION_HASH RadStaion();

	//功能说明：输出哈希表中站点的信息，用于调试	
	void PrintHash();
	void PrintStation(P_CINRAD_STATIONINFO p);
private:
	int ChangeOrder(int &n);
private:	
	QDataStream         m_BinaryStream;
	RADSTATION_HASH     m_RadStation;
};

