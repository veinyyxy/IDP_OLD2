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
������ڣ�2012��03��06��
============================================================*/
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>
#include "RadReadStationFile.h"


CRadReadStationFile::CRadReadStationFile()
{	
}

CRadReadStationFile::~CRadReadStationFile(void)
{
	//���չ�ϣ������ڴ�ռ䣻
	if(!m_RadStation.isEmpty())
	{
		RADSTATION_HASH_ITERATOR locIter(m_RadStation);
		while(locIter.hasNext())
		{
			locIter.next();
			delete locIter.value();
		}
		m_RadStation.clear();
	}
}

/************************************************************************
 ����˵�������״�վ��Ķ������ļ�
 ����˵����fileName�ǰ���·�����ļ������ַ���
           ����ֵΪbool�ͣ���ȷִ�з���true��������쳣����false
 ��    �ߣ�renxue
 ������ڣ�2012��03��07��
************************************************************************/
bool CRadReadStationFile::ReadFile( const QString &fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) 	return false;
	
	m_BinaryStream.setDevice(&file);

	//����ļ�ͷ�Ƿ�ΪROSE_CINRAD_CLASS
	quint64 tHead;
	m_BinaryStream>>tHead;
	if(tHead != 0x524F53455F43494E) return false;
	m_BinaryStream>>tHead;
	if(tHead != 0x5241445F434C4153) return false;
	m_BinaryStream>>tHead;
	if(tHead != 0x530000009A000000) return false;
	
	//���ζ�ȡվ����Ϣ
	int nCount = 0;
	char cTemp[20];
	qint32 nTemp;
	while(!m_BinaryStream.atEnd())
	{
		P_CINRAD_STATIONINFO pSite = new CINRAD_STATIONINFO;		

		m_BinaryStream.readRawData(cTemp, 8);
		pSite->cID = cTemp;
		m_BinaryStream.readRawData(cTemp, 20);
		pSite->cName = cTemp;
		m_BinaryStream.readRawData(cTemp, 20);
		pSite->cProvince = cTemp;
		m_BinaryStream.readRawData(cTemp, 12);
		pSite->cRadType = cTemp;

		m_BinaryStream>>nTemp;			
		pSite->nLat = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nLon = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nHei = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nDataRange = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nTime = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nWestLon = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;	
		pSite->nEastLon = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;		
		pSite->nNorthLat = ChangeOrder((int)nTemp);
		m_BinaryStream>>nTemp;
		pSite->nSouthLat = ChangeOrder((int)nTemp);
		m_BinaryStream.readRawData(cTemp, 4);
		pSite->cSpare = cTemp;

		m_RadStation.insert(pSite->cID, pSite);
		nCount = m_RadStation.count();
	}
	file.close();
	
	if(file.error() != QFile::NoError)
	{
		return false;
	}
	return true;
}

/************************************************************************
 ����˵�����������������ֽڽ�����ת
 ����˵���������ֽ������������
 ��    �ߣ�renxue
 ������ڣ�2012��03��14��
************************************************************************/
int CRadReadStationFile::ChangeOrder( int &n )
{
	int cLeft_1  = (n & 0xff000000)>>24;
	int cLeft_2  = (n & 0x00ff0000)>>8;
	int cRight_1 = (n & 0x0000ff00)<<8;
	int cRight_2 = (n & 0x000000ff)<<24;
	return cLeft_1 | cLeft_2 | cRight_1 | cRight_2;
}

RADSTATION_HASH CRadReadStationFile::RadStaion()
{
	return m_RadStation;
}

void CRadReadStationFile::PrintHash()
{
	RADSTATION_HASH_ITERATOR locIter(m_RadStation);
	while(locIter.hasNext())
	{
		locIter.next();
		qDebug()<<locIter.key()<<endl;
		PrintStation(locIter.value());
	}
}

void CRadReadStationFile::PrintStation(P_CINRAD_STATIONINFO p)
{
	if(!p) return;
	qDebug()<<"վ�ţ�"<<p->cID<<"  "<<"վ�����ƣ�"<<p->cName<<"  "<<"ʡ�ݣ�"<<p->cProvince<<"  "<<"���ͣ�"<<p->cRadType<<endl;
	qDebug()<<p->nLat<<"  "<<p->nLon<<"  "<<p->nHei<<"  "<<endl;
	qDebug()<<"���ݷ�Χ��"<<p->nDataRange<<"  "<<"ʱ�䣺"<<p->nTime<<endl;
	qDebug()<<p->nWestLon<<"  "<<p->nEastLon<<"  "<<p->nNorthLat<<"  "<<p->nSouthLat<<"  "<<p->cSpare<<endl;
}


