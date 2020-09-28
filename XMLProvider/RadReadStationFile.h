/*============================================================ 
�ļ�����CRadReadStationFile.h  
�� ���� CRadReadStationFile
�� �ࣺ QWidget
�� �ࣺ �� 
����˵�����Ӷ����������ļ��ж�ȡվ����Ϣ
����˵����

----------------------------�汾����----------------------------
V 1.0 
ԭ���� ��renxue
������ڣ�2012��03��07��
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
	����˵������ʮ���������ļ�
	����˵����fileName�ǰ���·�����ļ������ַ���
	          ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
	**************************************************************/
	bool ReadFile(const QString &fileName);	

	RADSTATION_HASH RadStaion();

	//����˵���������ϣ����վ�����Ϣ�����ڵ���	
	void PrintHash();
	void PrintStation(P_CINRAD_STATIONINFO p);
private:
	int ChangeOrder(int &n);
private:	
	QDataStream         m_BinaryStream;
	RADSTATION_HASH     m_RadStation;
};

