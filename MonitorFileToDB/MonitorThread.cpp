#include <QUuid>
#include "radarMosaicDataProvider.h"
#include "radarsingledataprovider.h"
#include "radarstructdataprovider.h"
#include "MonitorThread.h"

using namespace goto_gis;

SqliteManager* MonitorThread::m_pSqliteManager = 0;
MySQLManager* MonitorThread::m_pMySQLManager = 0;

MySQLDBPNG::MySQLDBPNG( MySQLManager* pDB,QString path , QString productId , QString ts)
{
	this->setAutoDelete(true);
	m_pDB = pDB;
	m_path = path;
	m_productId = productId;
	m_ts = ts;
}

MySQLDBPNG::~MySQLDBPNG()
{

}

void MySQLDBPNG::run()
{
	try
	{
		QMutexLocker locker(&m_lock);
		QString filename = m_path;

		QString pngPath;
		QRectF r(0,0,0,0);
		if (filename.contains(QObject::tr("MVIL"))||filename.contains(QObject::tr("MVILD"))||filename.contains(QObject::tr("BREF"))||filename.contains(QObject::tr("CREF"))||filename.contains(QObject::tr("PSP"))||filename.contains(QObject::tr("HM"))||filename.contains(QObject::tr("MET"))) 
		{
			RadarMosaicDataProvider * mdrivder = new RadarMosaicDataProvider;
			if (mdrivder)
			{
				pngPath = mdrivder->DrawThumbs(m_path);
				r = *(mdrivder->GetRect());
			}
		}
		else if (filename.contains(QObject::tr("VWP"))||filename.contains(QObject::tr("VAD"))||filename.contains(QObject::tr("SBT")) || filename.contains(QObject::tr("EVP"))||filename.contains(QObject::tr("_M_"))||filename.contains(QObject::tr("SCI"))||filename.contains(QObject::tr("HI")))
		{
			RadarStructDataProvider* sdrivder = new RadarStructDataProvider;
			if (sdrivder)
			{
				pngPath = sdrivder->DrawThumbs(m_path);
				sdrivder->GetRect(r);
			}
		} 
		else
		{
			RadarSingleDataProvider* sdrivder = new RadarSingleDataProvider;
			if (sdrivder)
			{
				QStringList list = sdrivder->DrawThumbs(m_path);
				pngPath = list.at(0);
				sdrivder->GetRect(r);
			}
		}

		QString title = m_productId.toUpper();
		double start_longitude = 0;
		double start_latitude = 0;
		double end_longitude = 0;
		double end_latitude = 0;

		start_longitude = r.left();
		end_longitude = r.right();

		end_latitude = r.top();
		start_latitude = r.bottom();

		QString img_Dir ="";
		QString img_name ="";
		//GenTime
		QString ts = m_ts;
		int data_id = 0;
		QString date_Dir = "";

		QFileInfo info(pngPath);
		QString path = info.absolutePath();
		img_Dir = path.remove(path.left(2));
		img_name = info.fileName();
		QString tempDir = img_Dir;
		date_Dir = tempDir.remove(tempDir.left(tempDir.lastIndexOf("/")+1));
		data_id = m_pDB->GetData_ID(m_productId);
		//monitorPathVector[i].productType,slon,slat,elon,elat,prodPath,str1.c_str(),ts,monitorPathVector[i].parentID,nowHour
		QString strSql = QObject::tr("insert into r_product(title,start_longitude,start_latitude,end_longitude,end_latitude,img_Dir,img_name,GenTime,ts,data_id,dateDirectory) value('%1',%2,%3,%4,%5,'%6','%7',now(),'%8',%9,'%10')").arg(title).arg(start_longitude).arg(start_latitude).arg(end_longitude).arg(end_latitude).arg(img_Dir).arg(img_name).arg(m_ts).arg(data_id).arg(date_Dir);
		m_pDB->Insert_r_productTb(strSql);
	}
	catch (...)
	{
	}
}




//
//void MonitorThread::OnFileCreate(const cFileSystemEvent& evt)
//{
//	QString path = QString::fromStdString(evt.GetFullPath());
//	QFileInfo fileInfo(path);
//	bool bisDir=fileInfo.isDir();
//	if(!bisDir)     
//	{
//		if (fileInfo.suffix().toLower() == "bin" || fileInfo.suffix().toLower() == "bz2")
//		{
//			//DataAnalyzeToDB(fileInfo.absoluteFilePath());
//			QMap<QString,QStringList> nameMap;
//			QString tempstring;
//			QStringList tempList;
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-R";
//			tempList.append("1");//产品名
//			tempList.append("2");//站名
//			tempList.append("4");//时间
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-V";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-W";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-CR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-CRH";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-CAP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-ET";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-VIL";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-VILD";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-HBR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-QPR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-OHP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-WHP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-THP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-SHP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-SCI";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-SCT";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-SBT";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-VAD";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-VWP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-EVP";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-TSR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-OSR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC-WSR";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L2_O_UF_QC";
//			tempList.clear();
//			tempList.append("1");//产品名
//			tempList.append("3");//站名
//			tempList.append("4");//时间
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_ST_QC";
//			tempList.clear();
//			tempList.append("1");//产品名
//			tempList.append("2");//站名
//			tempList.append("4");//时间
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L2_O_UF";
//			tempList.clear();
//			tempList.append("3");//产品名
//			tempList.append("1");//站名
//			tempList.append("4");//时间
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_MST_QC";
//			tempList.clear();
//			tempList.append("1");//产品名
//			tempList.append("0");//站名
//			tempList.append("2");//时间
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_MST_QC-CREF";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_MST_QC-MVIL";
//			nameMap.insert(tempstring,tempList);
//
//			tempstring = "RADA_CHN_DOR_L3_MST_QC-MVILD";
//			nameMap.insert(tempstring,tempList);
//
//			QStringList attribute = QDir(fileInfo.fileName()).dirName().split("-");
//			if (QStringList() != nameMap.value(attribute.at(0)))
//			{
//
//				//QFileInfo info(fName);
//				//if(info.suffix() == "bin")
//				//{
//				//	QString temp = attribute.last();
//				QStringList templist = attribute;
//				QStringList temp = nameMap.value(attribute.at(0));
//				int i = temp.at(0).toInt();//产品名
//				int j = temp.at(1).toInt();//站名
//				int k = temp.at(2).toInt();//时间
//				attribute.clear();
//				attribute.append(templist.at(j));
//				attribute.append(templist.at(i));
//				QString tempstr = templist[k].left(templist[k].indexOf("."));
//				attribute.append(tempstr);
//
//			}else 
//			{
//				QStringList temlist = attribute[0].split("_");
//				QString str_time = temlist[3].left(temlist[3].indexOf("."));
//				attribute.clear();
//				attribute = temlist;
//				attribute[2].append(str_time);
//			}
//
//
//			QString guid, productId, stationId, productPath, productDate, productTime;
//			QStringList list = attribute;
//			guid= QUuid::createUuid().toString();
//			productId = list.at(0).toLower();
//			stationId = list.at(1).toLower();
//			productPath = path;
//			QDateTime dt = QDateTime::fromString(list.at(2),"yyyyMMddhhmmss");
//			productDate = dt.date().toString("yyyy'-'MM'-'dd");
//			productTime = dt.time().toString("hh:mm:ss");
//			productPath.replace("/","\\");
//			bool bsuccess = m_pSqliteManager->InsertRadarProductTb(guid,productId,stationId,productPath,productDate,productTime);
//		}
//	}
//}




MonitorThread::MonitorThread(QStringList pathList): QThread()
{
	InitDBManager();
	m_fsPathList = pathList;
}

MonitorThread::MonitorThread( QString path )
{
	InitDBManager();
	m_path = path;

	//watcher = new cFileSystemWatcher(m_path.toStdString());
	//watcher->SetEventHandler(cFileSystemEvent::CREATED, OnFileCreate);
}

MonitorThread::~MonitorThread()
{
	//watcher->Close();
}

void MonitorThread::StartMonitorThread()
{
	start(HighestPriority);
}



void MonitorThread::run()
{
	ReadDirToList(m_path);
}

void MonitorThread::ReadDirToList(const QString& path)
{
	QDir dir(path);
	if (!dir.exists())
		return;
	dir.setFilter(QDir::Dirs|QDir::Files);
	//QStringList filters;
	//filters << "*.bin"<< "*.bz2";
	//dir.setNameFilters(filters);
	dir.setSorting(QDir::DirsFirst);
	QFileInfoList list = dir.entryInfoList();
	int i=0;
	do
	{
		QFileInfo fileInfo = list.at(i);
		if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
		{
			i++;
			continue;
		}
		bool bisDir=fileInfo.isDir();
		if(bisDir)
		{
			ReadDirToList(fileInfo.filePath());
		}
		else
		{
			if (fileInfo.suffix().toLower() == "bin" || fileInfo.suffix().toLower() == "bz2")
			{
				DataAnalyzeToDB(fileInfo.absoluteFilePath());
			}
		}
		i++;
	}while(i<list.size());
}

//
//void MonitorThread::ReadDirToList()
//{
//	foreach(QString watcherDir ,m_fsPathList)
//	{
//		QDir dir(watcherDir); //需要解码文件保存路径
//		dir.setFilter(QDir::Files|QDir::NoDotAndDotDot);  
//		QStringList filters;
//		filters << "*.bin"<< "*.bz2";
//		dir.setNameFilters(filters);
//		dir.setSorting(QDir::Time);
//		//转化成一个list
//		QFileInfoList fileInfoList = dir.entryInfoList();
//		foreach(QFileInfo fileInfo , fileInfoList)
//		{
//			DataAnalyzeToDB(fileInfo.absoluteFilePath());
//		}
//	}
//}

void MonitorThread::InitDBManager()
{
	typedef QObject* (*ClsFactory)(QString  Name,DBManager::EnumDBType dbType);
	ClsFactory avg = (ClsFactory) QLibrary::resolve("DBManager.dll", "CreateInstance");
	m_pSqliteManager =(SqliteManager*)avg("MDMS_DBManager",DBManager::Sqlite);

	m_pMySQLManager = (MySQLManager*)avg("MDMS_DBManager",DBManager::MySQL);
}

void MonitorThread::DataAnalyzeToDB( QString fileName )
{
	QString guid, productId, stationId, productPath, productDate, productTime;
	QFileInfo info(fileName);
	QStringList list;
	QFileInfo fileInfo(fileName);
	if (fileInfo.suffix().toLower() == "bz2")
	{
		list = Bz2FileNameAnalyes(info.fileName());
	}
	else if (fileInfo.suffix().toLower() == "bin")
	{
		list = BinFileNameAnalyes(info.fileName());
	}
	guid= info.fileName();//QUuid::createUuid().toString();
	productId = list.at(1).toLower();
	stationId = list.at(0).toLower();
	productPath = fileName;
	QDateTime dt = QDateTime::fromString(list.at(2),"yyyyMMddhhmmss");
	productDate = dt.date().toString("yyyy'-'MM'-'dd");
	productTime = dt.time().toString("hh:mm:ss");
	productPath.replace("/","\\");
	bool bsuccess = m_pSqliteManager->InsertRadarProductTb(guid,productId,stationId,productPath,productDate,productTime);
	if (bsuccess)//插入成功
	{   
		QString ts = productDate+" "+ productTime;
		QString filename = productPath;
		//if (filename.contains(QObject::tr("MVIL"))||filename.contains(QObject::tr("MVILD"))||filename.contains(QObject::tr("BREF"))||filename.contains(QObject::tr("CREF"))||filename.contains(QObject::tr("PSP"))||filename.contains(QObject::tr("HM"))||filename.contains(QObject::tr("MET"))) 
		//{
			QThreadPool::globalInstance()->start(new MySQLDBPNG(m_pMySQLManager,productPath,productId,ts));
		//}
	}
}

QStringList MonitorThread::Bz2FileNameAnalyes( QString fName )
{
	QStringList tempList = QFileInfo(fName).baseName().split("_");
	QStringList attribute;
	attribute.append(tempList.at(0));//站号
	attribute.append(tempList.at(1));//产品名
	attribute.append(tempList.at(2)+tempList.at(3));
	return attribute;
}


QStringList MonitorThread::BinFileNameAnalyes( QString fName )
{
	QMap<QString,QStringList> nameMap;
	QString tempstring;
	QStringList tempList;

	tempstring = "RADA_CHN_DOR_L3_ST_QC-R";
	tempList.append("1");//产品名
	tempList.append("2");//站名
	tempList.append("4");//时间
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-V";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-W";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-CR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-CRH";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-CAP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-ET";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-VIL";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-VILD";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-HBR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-QPR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-OHP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-WHP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-THP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-SHP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-SCI";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-SCT";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-SBT";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-VAD";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-VWP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-EVP";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-TSR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-OSR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC-WSR";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L2_O_UF_QC";
	tempList.clear();
	tempList.append("1");//产品名
	tempList.append("3");//站名
	tempList.append("4");//时间
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_ST_QC";
	tempList.clear();
	tempList.append("1");//产品名
	tempList.append("2");//站名
	tempList.append("4");//时间
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L2_O_UF";
	tempList.clear();
	tempList.append("3");//产品名
	tempList.append("1");//站名
	tempList.append("4");//时间
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_MST_QC";
	tempList.clear();
	tempList.append("1");//产品名
	tempList.append("0");//站名
	tempList.append("2");//时间
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_MST_QC-CREF";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_MST_QC-MVIL";
	nameMap.insert(tempstring,tempList);

	tempstring = "RADA_CHN_DOR_L3_MST_QC-MVILD";
	nameMap.insert(tempstring,tempList);

	QStringList attribute = QDir(fName).dirName().split("-");
	if (QStringList() != nameMap.value(attribute.at(0)))
	{

		//QFileInfo info(fName);
		//if(info.suffix() == "bin")
		//{
		//	QString temp = attribute.last();
		QStringList templist = attribute;
		QStringList temp = nameMap.value(attribute.at(0));
		int i = temp.at(0).toInt();//产品名
		int j = temp.at(1).toInt();//站名
		int k = temp.at(2).toInt();//时间
		attribute.clear();
		attribute.append(templist.at(j));
		attribute.append(templist.at(i));
		QString tempstr = templist[k].left(templist[k].indexOf("."));
		attribute.append(tempstr);

	}else 
	{
		QStringList temlist = attribute[0].split("_");
		QString str_time = temlist[3].left(temlist[3].indexOf("."));
		attribute.clear();
		attribute = temlist;
		attribute[2].append(str_time);
	}
	return attribute;
}

