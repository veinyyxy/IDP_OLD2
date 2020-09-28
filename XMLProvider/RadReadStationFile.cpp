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
完成日期：2012年03月06日
============================================================*/
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>
#include "RadReadStationFile.h"


CRadReadStationFile::CRadReadStationFile()
{	
}

CRadReadStationFile::~CRadReadStationFile(void)
{
	//回收哈希表里的内存空间；
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
 功能说明：读雷达站点的二进制文件
 参数说明：fileName是包含路径和文件名的字符串
           返回值为bool型，正确执行返回true，错误或异常返回false
 作    者：renxue
 完成日期：2012年03月07日
************************************************************************/
bool CRadReadStationFile::ReadFile( const QString &fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) 	return false;
	
	m_BinaryStream.setDevice(&file);

	//检查文件头是否为ROSE_CINRAD_CLASS
	quint64 tHead;
	m_BinaryStream>>tHead;
	if(tHead != 0x524F53455F43494E) return false;
	m_BinaryStream>>tHead;
	if(tHead != 0x5241445F434C4153) return false;
	m_BinaryStream>>tHead;
	if(tHead != 0x530000009A000000) return false;
	
	//依次读取站点信息
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
 功能说明：将整型数按照字节进行逆转
 参数说明：返回字节逆序的整型数
 作    者：renxue
 完成日期：2012年03月14日
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
	qDebug()<<"站号："<<p->cID<<"  "<<"站点名称："<<p->cName<<"  "<<"省份："<<p->cProvince<<"  "<<"类型："<<p->cRadType<<endl;
	qDebug()<<p->nLat<<"  "<<p->nLon<<"  "<<p->nHei<<"  "<<endl;
	qDebug()<<"数据范围："<<p->nDataRange<<"  "<<"时间："<<p->nTime<<endl;
	qDebug()<<p->nWestLon<<"  "<<p->nEastLon<<"  "<<p->nNorthLat<<"  "<<p->nSouthLat<<"  "<<p->cSpare<<endl;
}


