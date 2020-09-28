/************************************************************************/
/* Grads产品生产设置对话框实现文件                                       */
/************************************************************************/
#include <QSettings>
#include <QStringList>
#include "GridDataDialog.h"

QString GridDataDialog::pBasePath  = QDir::currentPath();//静态变量保存当前打开文件路径


/************************************************************************
 功能说明：初始化对话框
 作      者：易家祥
 完成日期：2011年5月10日
************************************************************************/
GridDataDialog::GridDataDialog(const QString& filePath, CDataProviderManager* dPMgr, QString var, int nTime ,QString time , int nLevel ,QString level ,QStringList EleNames, QWidget *parent)
	: QDialog(parent), m_pFilePath(filePath), m_EleTypeNames_(EleNames), m_VarName(var), m_time(time)
{
	setAttribute(Qt::WA_DeleteOnClose);
	this->resize(800, 600);
	this->InitDialog();
	this->InitUISlots();
	m_pDPMgr = dPMgr;
	m_pElement = NULL;

	m_fileType = "txt";

	CElement* element1 = dPMgr->GetElementByChineseName(var, m_pFilePath);
	if (element1 != NULL)
	{
		if (element1->GetFileType() == "MICAPS1")
		{
			InitializeModelM1( dPMgr ,nTime,time,nLevel,level);
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			connect(m_TbView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(ZoomToStation(const QModelIndex&)));

			m_fileType = tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS2")
		{
			InitializeModelM2( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS3")
		{
			InitializeModelM3( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType = tr("CIPAS站点数据(*.cps);;MICAPS3数据(*.000)");
		}
		else if (element1->GetFileType() == "MICAPS5")
		{
			InitializeModelM5( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "TXT")
		{
			InitializeModelTXT( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS7")
		{
			InitializeModelM7( dPMgr );
		}
		else if (element1->GetFileType() == "MICAPS8")
		{
			InitializeModelM8( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "STADA")
		{
			InitializeModelSTADA(dPMgr);
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS11")
		{
			InitializeModelM11( dPMgr,var,nTime,time,nLevel,level );
		}
		else if (element1->GetFileType() == "CIPASM")
		{
			InitializeModelCIPASM( dPMgr  ,nTime,time,nLevel,level);
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		}
		else if (element1->GetFileType() == "STATIONS")
		{
			//InitializeModelM3( dPMgr );
			m_EleTypeNames_ = element1->m_showList;
			//InitializeModelStaion(dPMgr);
			InitializeModelTXT( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPAS站点数据(*.cps)");
		//	m_fileType = "CIPAS站点数据(*.cps);;MICAPS3数据(*.000)";
		}
		else if (element1->GetFileType() == "EXPONENT")
		{
			InitializeModelEXPONENT( var,dPMgr );
			m_fileType =  tr("指数数据(*.tms)");
		}
		else 
		{
			InitializeModel(dPMgr,var,nTime,time,nLevel,level);
			m_fileType =  tr("NetCDF数据(*.nc);;CIPAS格点数据(*.cps)");
			if (element1->GetFileType() == "MICAPS4")
				m_fileType = tr("NetCDF数据(*.nc);;MICAPS4数据(*.000);;CIPAS站点数据(*.cps)");
		}
	}
	setWindowTitle(tr("显示选择数据列表"));
	setWindowFlags(windowFlags() | Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint);
}

GridDataDialog::~GridDataDialog(void)
{
	m_pElement = NULL;
	m_pDPMgr = NULL;

	QItemSelectionModel * pItem = m_TbView->selectionModel();
	
	if(pItem)
		pItem->clearSelection();

	QStringList stlist;
    SendStationItems(stlist);
}

/************************************************************************
 功能说明：初始化对话框控件
 作      者：易家祥
 完成日期：2011年5月10日
************************************************************************/
void GridDataDialog::InitDialog()
{
	m_MGridLayout = new QGridLayout(this);
	this->setLayout(m_MGridLayout);

	m_TopToolBar = new QToolBar(this);
	m_TopToolBar->setObjectName(tr("打开数据0"));
	m_MGridLayout->addWidget(m_TopToolBar,0,0);

	m_ActionSave = new QAction(QIcon(":/images/save.png"),tr("保存"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionSave);
	//m_ActionSave->setEnabled(false);

	m_TbView = new QTableView(this);
	m_TbView->setSortingEnabled(true);
	m_MGridLayout->addWidget(m_TbView,1,0);
	m_TbView->show();
}

/************************************************************************
 功能说明：初始化model数据
 作      者：易家祥
 完成日期：2011年5月10日
************************************************************************/
void GridDataDialog::InitializeModel(CDataProviderManager* dPMgr , QString var, int nTime ,QString time , int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);
	m_pElement = dPMgr->GetElementByChineseName(var, m_pFilePath);//dPMgr->GetElementByName(var);
	if (m_pElement == NULL)
	{
		return;
	}
	m_pElement->SelectedLon().m_count = m_pElement->ActualLon().m_count;
	m_pElement->SelectedLat().m_count = m_pElement->ActualLat().m_count;

	int x =  m_pElement->ActualLon().m_count;
	int y = m_pElement->ActualLat().m_count;

	m_pElement->SelectedPeriod().m_begin = QDateTime::fromString(time,"yyyy'-'MM'-'dd'-'hh:mm:ss");
	m_pElement->SelectedPeriod().m_end = QDateTime::fromString(time,"yyyy'-'MM'-'dd'-'hh:mm:ss");

	m_pElement->SelectedPeriod().m_hashDate.clear();
	m_pElement->SelectedPeriod().m_hashDate.insert(nTime, m_pElement->SelectedPeriod().m_begin);
	m_pElement->SelectedPeriod().m_count = 1;

	m_pElement->Level().m_selected.clear();
	m_pElement->Level().m_selected.insert(nLevel,level.toFloat());

	if(!(dPMgr->ReadDataFromFile(m_pElement)))
	{
		return;
	}

	CLonLatRange rLon = m_pElement->SelectedLon();
	m_ItemModel->setColumnCount(x);
	for (int i = 0 ; i < x ; i++)
	{
		m_ItemModel->setHeaderData(i, Qt::Horizontal, rLon.Value(i));
	}

	CLonLatRange rLat = m_pElement->SelectedLat();
	m_ItemModel->setRowCount(y);
	for (int i = 0 ; i < y; i++)
	{
		m_ItemModel->setHeaderData(i, Qt::Vertical, rLat.Value(i));
	}
	int i = 0;
	int j = 0;

	float *pDataArray = m_pElement->Data().GetDataArray(0);
	if (NULL == pDataArray)
	{
		return;
	}

	//long dimension = m_pElement->Data().DataBlockDimensions(0);

	QStandardItem *newItem=0;
	for (int i = 0 ; i<y;i++)//lat  144
	{
		for (int j = 0 ;j<x;j++)//lon 73
		{
//			if (i*x+j<dimension)
			{
				QString currValue = QString::number(pDataArray[i*x+j],'g',8);
				//qDebug()<<currValue;
				newItem = new QStandardItem(currValue);
				m_ItemModel->setItem(i ,j, newItem);
			}			
		}
	}
	m_TbView->setModel(m_ItemModel);
	m_TbView->setSortingEnabled(false);

	QFileInfo temDir(m_pElement->GetFileName());
	QString suffix =  temDir.suffix().toLower();
	
	if (QString("格点数据") == var)
	{
		if (level.isEmpty() || level.isNull())
		{
			if (nTime>=0)
			{
				m_nLevel = nTime;
				m_elename = time;
			}
		}
		else if (time.isEmpty() || time.isNull())
		{
			if (nLevel>0)
			{
				m_nLevel = nLevel;
				m_elename = level;
			}
		}
		else
		{
			m_nLevel = nLevel*nTime;
		}
		connect(m_ItemModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(modifyCipasr(QStandardItem *)));
	}
	if (QString("nc") == suffix || m_pElement->GetFileType() == QString("MICAPS4")|| m_pElement->GetFileType() == QString("GRIB"))
	{
		//m_ActionSave->setEnabled(true);
	}
	else
	{
		//m_ActionSave->setEnabled(false);
	}	
}

/************************************************************************
 功能说明：初始化对话框事件
 作      者：易家祥
 完成日期：2011年5月10日
************************************************************************/
void GridDataDialog::InitUISlots()
{
	m_TopToolBar->connect(m_TopToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(DataToolBarEvent(QAction*)));
}

void GridDataDialog::writeGridData(QString& fileName)
{
	//--------------------------------------------------------------------------写格点数据文件Start
	QFileInfo temFDir(m_pFilePath);
	QString suffixF =  temFDir.suffix().toLower();		 

	if (QString("nc") == suffixF || QString("NetCDF") == m_pElement->GetFileType() || QString("GRIB") == m_pElement->GetFileType() || QString("AWX") == m_pElement->GetFileType())//netCDF to cipas 
		//	if (QString("GRIB") == m_pElement->GetFileType() || QString("AWX") == m_pElement->GetFileType())
	{
		m_pLstElement.clear();
		m_pLstElement.append(m_pElement);
		if (m_pDPMgr->WriteCiapsRaster(fileName,m_pLstElement)>0)
		{
			QMessageBox::information(this,tr("提示"),tr("保存成功!"));
			//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存cps格点文件。") );
		}
		else
		{
			QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
			//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存cps格点文件失败！") );
		}
		return;
	}
	//--------------------------------------------------------------------------写格点数据文件END
	//--------------------------------------------------------------------------写站点数据文件Start
	m_pLstElement.clear();
	CElement *pTempEle = NULL;
	QString strTemp;
	//--------------------------------------------------------------------------文件头Start
	CElement *pElement = new CElement;
	pElement->name(tr("文件描述"));
	pElement->SetMicapsDeclaration(tr("1"));//cipas  数据类型
	pElement->SetMicapsDeclaration(tr("3"));
	pElement->SetMicapsDeclaration(tr("8"));
	pElement->SetMicapsDeclaration(tr("10"));
	pElement->SetMicapsDeclaration(m_pDPMgr->GetFileDescription(m_pFilePath));
	pElement->SetMicapsDeclaration(tr("CIPAS"));
	QString PDay=tr("PDAY 10 ");
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("站点编号"),m_pFilePath);
	if (NULL == pTempEle ||pTempEle->ActualPeriod().m_hashDate.size()<1)
	{
		QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存cps文件失败！") );
		return;
	}
	PDay += pTempEle->ActualPeriod().m_hashDate.value(0).toString("yyyy MM dd hh");
	PDay+=QString(" ");
	PDay += pTempEle->ActualPeriod().m_hashDate.value(0).toString("yyyy MM dd hh");
	//pElement->SetMicapsDeclaration(tr("PDAY 10 2011 03 06 -999 2011 03 15 -999"));
	pElement->SetMicapsDeclaration(PDay);
	if (pTempEle->Level().m_valus.size()>0)
	{
		pElement->SetMicapsDeclaration(QString::number(pTempEle->Level().m_valus.size()));
		strTemp = QString(" ");
		for (int i = 0;i<pTempEle->Level().m_valus.size();i++)
		{
			strTemp += QString::number(pTempEle->Level().m_valus.value(0));
			strTemp += QString(" ");
		}
		pElement->SetMicapsDeclaration(strTemp);
	}else
	{
		pElement->SetMicapsDeclaration(tr("1"));
		pElement->SetMicapsDeclaration(tr("1000"));
	}

	pElement->SetMicapsDeclaration(QString::number(pTempEle->Data().DataBlockDimensionSize(0,0)));//站点总数
	pElement->SetMicapsDeclaration(QString::number(m_HeaderItemList.size()));					  //属性个数
	strTemp = QString(" ");
	for(int i= 0;i<m_HeaderItemList.size();i++)
	{
		pElement->SetMicapsDeclaration(m_HeaderItemList.at(i));
		strTemp += QString("f10.2  ");
	}
	pElement->SetMicapsDeclaration(strTemp);

	pElement->SetMicapsDeclaration(tr("LONGLAT"));
	pElement->SetMicapsDeclaration(tr("73.0 54.0 135.0 4.0"));
	m_pLstElement.append(pElement);
	//--------------------------------------------------------------------------文件头END
	//--------------------------------------------------------------------------要素数据Start
	for(int i= 0;i<m_HeaderItemList.size();i++)
	{
		pTempEle = m_pDPMgr->GetElementByChineseName(m_HeaderItemList.at(i),m_pFilePath);
		m_pLstElement.append(pTempEle);
	}
	//--------------------------------------------------------------------------要素数据END
	if (m_pDPMgr->WriteCipas(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
		//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存cps站点数据文件。") );
	}
	else
	{
		QMessageBox::warning(this,tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存cps站点数据文件失败！") );
	}
	pElement = NULL;
	pTempEle = NULL;
	//--------------------------------------------------------------------------写站点数据文件END
}


void GridDataDialog::writeNCData(QString& fileName)
{
	m_pLstElement.clear();
	m_pLstElement.append(m_pElement);
	if (m_pDPMgr->Write(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
		//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存nc文件。") );
	}
	else
	{
		QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存nc文件失败！") );
	}
}

void GridDataDialog::writeCipasGridData(QString& fileName)
{
	m_pLstElement.clear();

	//m_pDPMgr->CloseFilter();
	//QStringList eleNames  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);   //转译中文名list　得到所有头信息

	m_pDPMgr->OpenFilter();
	CElement* pCheckElement = m_pDPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);

	if(pCheckElement == NULL) //格点保存
	{
		m_pLstElement.clear();
		m_pLstElement.append(m_pElement);
		if (m_pDPMgr->WriteCiapsRaster(fileName,m_pLstElement)>0)
		{
			QMessageBox::information(this,tr("提示"),tr("保存成功!"));
			//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存cps格点文件。") );
		}
		else
		{
			QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
			//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存cps格点文件失败！") );
		}

		return;
	}

	////取得站点编号的Element对象，从而取得对象
	//CElement* pElement = m_pDPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	//if (!pElement)
	//{
	//	pElement = m_pDPMgr->GetElementByChineseName(tr("格点数据"), m_pFilePath);
	//	if (!pElement)
	//	{
	//		return;
	//	}

	//	QList<CElement*> elements_v;
	//	elements_v.push_back(pElement);
	//	if (m_pDPMgr->WriteCiapsRaster(fileName,elements_v)>0)
	//	{
	//		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
	//		CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存nc文件。") );
	//	}
	//	else
	//	{
	//		QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
	//		CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存nc文件失败！") );
	//	}
	//	return;
	//}

	// 以下为站点保存 韩猛 2012-10-18
	//文件头信息创建
	CElement *pElement = new CElement;
	pElement->name(QString::fromLocal8Bit("文件描述"));
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("1"));
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("2"));
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("13"));
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("14"));
	//pElement->SetMicapsDeclaration(m_pFilePath);
	pElement->SetMicapsDeclaration(fileName);
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("CIPAS"));
	QString PDay=QString::fromLocal8Bit("PDAY 10 ");

	//QHash<long, QDateTime> hashDate = m_pElement->ActualPeriod().m_hashDate;
	QHash<long, QDateTime> hashDate = pElement->ActualPeriod().m_hashDate;
	QHash<long, QDateTime>::Iterator it; 
	QString startDate,endDate;
	for(it = hashDate.begin(); it != hashDate.end(); it++)
	{
		 startDate = it.value().toString("yyyy'-'MM'-'dd'");
		 endDate =   it.value().toString("yyyy'-'MM'-'dd'");
	}
	startDate.replace('-'," ");
	PDay+=startDate;
	PDay+=" -999 ";
	endDate.replace('-'," ");
	PDay+=endDate;
	PDay+=" -999";
	pElement->SetMicapsDeclaration(PDay);
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("1"));
	if (m_elename == "")
	{
		m_elename = "1-1-1";
	}
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("%1").arg(m_elename));
	pElement->SetMicapsDeclaration(QString::number(m_nNumData));

	int nNumEl  = m_HeaderItemList.count();
	pElement->SetMicapsDeclaration(QString("%1").arg(nNumEl));

	QList<QString > ElementNames;
	QString strType;
	if(m_HeaderItemList.count() > 0)
	{
		for (int i = 0 ; i < m_HeaderItemList.count();i++)
		{
			pElement->SetMicapsDeclaration(m_HeaderItemList.at(i));
			ElementNames.append(m_HeaderItemList.at(i));
			strType = strType +"f10.2  ";
		}	
	}
	pElement->SetMicapsDeclaration(strType);
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("LONGLAT"));
	pElement->SetMicapsDeclaration(QString::fromLocal8Bit("73.0 54.0 135.0 4.0"));
	pElement->SetMicapsDeclaration(QString::number(0)); //等值线条数，测试用
	m_pLstElement.append(pElement);

	//取得站点编号
	pElement = m_pDPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	m_pLstElement.append(pElement);
	//取得站点经度
	pElement = m_pDPMgr->GetElementByChineseName(tr("站点经度"), m_pFilePath);
	m_pLstElement.append(pElement);

	//取得站点纬度
	pElement = m_pDPMgr->GetElementByChineseName(tr("站点纬度"), m_pFilePath);
	m_pLstElement.append(pElement);
	pElement = NULL;


	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);   //转译中文名list　得到所有头信息

	//取得各个列数据
	for(int i = 0 ; i < headerItemList.count(); i++)
	{
		QString itemList = headerItemList.at(i);

		pElement = m_pDPMgr->GetElementByChineseName(headerItemList.at(i), m_pFilePath);
		m_pLstElement.append(pElement);
	}

	if (m_pDPMgr->WriteCipas(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
		//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存nc文件。") );
	}
	else
	{
		QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存nc文件失败！") );
	}
}

void GridDataDialog::writeMICAPS3(QString& fileName)
{
	CElement *pTempEle= NULL;
	m_pLstElement.clear();
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("描述信息"),m_pFilePath);
	m_pLstElement.append(pTempEle);
	for(int i= 0;i<m_HeaderItemList.size();i++)
	{
		pTempEle = m_pDPMgr->GetElementByChineseName(m_HeaderItemList.at(i),m_pFilePath);
		m_pLstElement.append(pTempEle);
	}
	if (m_pDPMgr->WriteMicaps3(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
		//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存MICAPS3文件") );
	}
	else
	{
		QMessageBox::warning(this, tr("警告"), tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存MICAPS3文件失败！") );
	}
	pTempEle = NULL;
}
void GridDataDialog::writeMICAPS4(QString& fileName)
{
	CElement *pTempEle= NULL;
	m_pLstElement.clear();
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("描述信息"),m_pFilePath);
	m_pLstElement.append(pTempEle);
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("格点数据"),m_pFilePath);
	m_pLstElement.append(pTempEle);

	if (m_pDPMgr->WriteMicaps4(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("提示"),tr("保存成功!"));
		//CDiskFileLog::WriteLog(tr("信息"), tr("DataToolBarEvent::成功保存MICAPS4文件。") );
	}
	else
	{
		QMessageBox::warning(this,tr("警告"),tr("保存失败"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("警告"), tr("DataToolBarEvent::保存MICAPS4文件失败！") );
	}
	pTempEle = NULL;
}

void GridDataDialog::ZoomToStation(const QModelIndex& index)
{
	QModelIndex idxs = m_TbView->selectionModel()->currentIndex();
	int row = idxs.row();
	QStandardItemModel *pModel = (QStandardItemModel *)m_TbView->model();
	if (pModel)
	{
		//QStringList strlist;
		int id = pModel->item(row, 0)->data(Qt::DisplayRole).toInt();		
		float flon = pModel->item(row, 1)->data(Qt::DisplayRole).toFloat();	
		float flat  = pModel->item(row, 2)->data(Qt::DisplayRole).toFloat();
		//strlist.append(QString::number(id));
		//strlist.append(QString::number(flon));
		//strlist.append(QString::number(flat));
		emit SendZoomStation(id,flon,flat);
	}	
}

void GridDataDialog::FocusStations()
{
	QModelIndexList idxs = m_TbView->selectionModel()->selectedRows();
	
	QStandardItemModel *pModel = (QStandardItemModel *)m_TbView->model();
	
	//QVector<STATION_ITEM> stationitems;
	QStringList strlist;
	for (int i = 0; i < idxs.size(); i++)
	{
		int row = idxs[i].row();
		int id = pModel->item(row, 0)->data(Qt::DisplayRole).toInt();		
		float flon = pModel->item(row, 1)->data(Qt::DisplayRole).toFloat();	
		float flat  = pModel->item(row, 2)->data(Qt::DisplayRole).toFloat();
		strlist.append(QString::number(id));
		strlist.append(QString::number(flon));
		strlist.append(QString::number(flat));
	}
	SendStationItems(strlist);
}

/************************************************************************
 功能说明：ToolBar事件
 作      者：易家祥
 完成日期：2011年5月10日
************************************************************************/
void GridDataDialog::DataToolBarEvent(QAction* action)
{
	QString cmd = action->text();
	//QString filter = "气候数据 (*."+m_fileType+")";
	if (cmd == tr("保存"))
	{
		//取得上次记录的路径
		QSettings myQSettings;  // where we keep last used filter in persistent state
		QString lastSaveDir = myQSettings.value( "CIPASUI/MainWinSaveDir", "/home/" ).toString();

		if (m_fileType == tr("txt"))
		{
			//CDiskFileLog::WriteLog(tr("警告"),tr("不支持保存类型！") );
			QMessageBox::information(this,tr("提示"),tr("不支持保存类型！"));
			return;
		}

		QString fileName = QFileDialog::getSaveFileName(this,tr("保存数据文件"), lastSaveDir ,m_fileType);
		if (fileName.isEmpty())
		{
			//CDiskFileLog::WriteLog(tr("警告"),tr("DataToolBarEvent::文件名或文件路径为空！") );
			return;
		}

		QFileInfo temDir(fileName);
		QString suffix =  temDir.suffix().toLower();
		if (QString("nc") == suffix)
		{
			writeNCData(fileName); //refact by zhanglei
		}
		else if (QString("000") == suffix)
		{
				QString filetype = m_pElement->GetFileType();
				if (QString("MICAPS3") == m_pElement->GetFileType())
				{
					writeMICAPS3(fileName); //refact by zhanglei
				}
				if (QString("MICAPS4") == m_pElement->GetFileType())
				{
					writeMICAPS4(fileName); //refact by zhanglei
				}
		}
		else if (QString("cps") == suffix)
		{
			//writeGridData(fileName);	 //refact by zhanglei		
			writeCipasGridData(fileName);
		}

		//保存本次打开的路径
		QFileInfo fi(fileName);
		myQSettings.setValue("CIPASUI/MainWinSaveDir",fi.absolutePath());
	}
}

/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年5月30日
************************************************************************/
void GridDataDialog::InitializeModelM1(CDataProviderManager* dPMgr ,int nTime ,QString time ,int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("站点编号" )<<tr("站点经度") <<tr("站点纬度") <<tr("站点海拔")<<tr("站点级别")
					<< tr("总云量" )<<tr("风向") <<tr("风速")<<tr("海平面气压") <<tr("3小时变压")
					<<tr("过去天气1")<< tr("过去天气2") <<tr("6小时降水" )<<tr("低云状") <<tr("低云量")
					<<tr("低云高")<< tr("露点") <<tr("能见度") <<tr("现在天气") <<tr("温度")<<tr("中云状")
					<< tr("高云状") ;
	QStandardItem *newItem=0;
	m_stationname.clear();
	for(int n = 0;n<22;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<m_nNumData;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			if (n<3)
			{
				newItem->setEditable(false);
			}
			if (n == 0)
			{
				m_stationname.insert(i,newItem->text().toFloat());
			}
			newItem->setData((qulonglong)m_pElement, Qt::UserRole);
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	if (level.isEmpty() || level.isNull())
	{
		if (nTime>=0)
		{
			m_nLevel = nTime;
			m_elename = time;
		}
	}
	else if (time.isEmpty() || time.isNull())
	{
		if (nLevel>0)
		{
			m_nLevel = nLevel;
			m_elename = level;
		}
	}
	else
	{
		m_nLevel = nLevel*nTime;
	}
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
	connect(m_ItemModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(modifyItemData(QStandardItem *)));
	//connect(m_TbView,SIGNAL(clicked(),m_TbView,SLOT(SelectRow())))
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年5月30日
************************************************************************/
void GridDataDialog::InitializeModelM2(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("站点编号") <<tr("站点经度") <<tr("站点纬度" )<<tr("站点海拔" )<<tr("站点级别")
		<< tr("高度") <<tr("温度") <<tr("温度露点差")<<tr("风向") <<tr("风速");
	QStandardItem *newItem=0;

	for(int n = 0;n<10;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<m_nNumData;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年5月30日
************************************************************************/
void GridDataDialog::InitializeModelM3(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("站点编号") <<tr("站点经度") <<tr("站点纬度") <<tr("海拔高度")<<tr("站点值1")
		<< tr("站点值2");
	QStandardItem *newItem=0;

	for(int n = 0;n<6;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<m_nNumData;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年7月28日
************************************************************************/
void GridDataDialog::InitializeModelM5(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<<tr("站点编号")<<tr("站点经度")<<tr("站点纬度")<<tr("站点海拔")<<tr("内容长度")<<tr("气压")<<tr("高度")<<tr("温度")<<tr("露点")<<tr("风向")<<tr("风速");
	QStandardItem *newItem=0;

	for(int n = 0;n<m_HeaderItemList.size();n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<ncount;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);
	m_TbView->setSortingEnabled(false);
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年6月10日
************************************************************************/
void GridDataDialog::InitializeModelM8(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("站点编号") <<tr("站点经度") <<tr("站点纬度") <<tr("站点海拔")<<tr("天气现象1")
					<< tr("风向1") <<tr("风速1") <<tr("最低温度")<<tr("最高温度") <<tr("天气现象2")
					<<tr("风向2")<< tr("风速2") ;
	QStandardItem *newItem=0;

	for(int n = 0;n<12;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<ncount;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年6月13日
************************************************************************/
void GridDataDialog::InitializeModelM7(CDataProviderManager* dPMgr )
{
	QStringList tempStrList;
	int nPointNum;
	float *pDataArray;
	CData tempData;

	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("文件描述"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}

	tempStrList = m_pElement->GetMicapsDeclaration();
	nPointNum = tempStrList.at(tempStrList.size()-1).toInt();

	m_pElement = dPMgr->GetElementByChineseName(tr("台风路径"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	tempData = m_pElement->Data();
	pDataArray = tempData.GetDataArray(0);

	QStringList headerItemList ;
	headerItemList<< tr("年") <<tr("月" )<<tr("日" )<<tr("时次")<<tr("时效")
					<< tr("中心经度" )<<tr("中心纬度") <<tr("最大风速")<<tr("中心最低气压") <<tr("七级风圈半径")
					<<tr("十级风圈半径")<< tr("移向") << tr("移速") ;

	QStandardItem *newItem=0;
	for(int i = 0;i<13;i++)//添加标题
	{
		newItem = new QStandardItem(headerItemList.at(i));
		m_ItemModel->setHorizontalHeaderItem(i, newItem);
	}

	for(int n = 0;n<nPointNum;n++)
	{
		for(int i = 0;i<13;i++)
		{
			newItem = new QStandardItem(QString::number(pDataArray[n*13+i]));
			m_ItemModel->setItem(n,i,newItem);
		}
	}
	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(false);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年6月13日
************************************************************************/
void GridDataDialog::InitializeModelM11(CDataProviderManager* dPMgr , QString var, int nTime ,QString time , int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);
	m_pElement = dPMgr->GetElementByChineseName(var, m_pFilePath);//dPMgr->GetElementByName(var);
	if (m_pElement == NULL)
	{
		return;
	}
	m_pElement->SelectedLon().m_count = m_pElement->ActualLon().m_count;
	m_pElement->SelectedLat().m_count = m_pElement->ActualLat().m_count;

	int x =  m_pElement->ActualLon().m_count;
	int y = m_pElement->ActualLat().m_count;

	m_pElement->SelectedPeriod().m_begin = QDateTime::fromString(time,"yyyy'-'MM'-'dd'-'hh:mm:ss");
	m_pElement->SelectedPeriod().m_end = QDateTime::fromString(time,"yyyy'-'MM'-'dd'-'hh:mm:ss");

	m_pElement->SelectedPeriod().m_hashDate.clear();
	m_pElement->SelectedPeriod().m_hashDate.insert(nTime, m_pElement->SelectedPeriod().m_begin);
	m_pElement->SelectedPeriod().m_count = 1;

	m_pElement->Level().m_selected.clear();
	m_pElement->Level().m_selected.insert(nLevel,level.toFloat());

	if(!(dPMgr->ReadDataFromFile(m_pElement)))
	{
		return;
	}

	CLonLatRange rLon = m_pElement->SelectedLon();
	m_ItemModel->setColumnCount(x);
	for (int i = 0 ; i < x ; i++)
	{
		m_ItemModel->setHeaderData(i, Qt::Horizontal, rLon.Value(i));
	}

	CLonLatRange rLat = m_pElement->SelectedLat();
	m_ItemModel->setRowCount(y);
	for (int i = 0 ; i < y; i++)
	{
		m_ItemModel->setHeaderData(i, Qt::Vertical, rLat.Value(i));
	}
	int i = 0;
	int j = 0;

	CElement *pElementU,*pElementV;
	float *pDataArrayU,*pDataArrayV;
	QString tempStr;
	pElementU = dPMgr->GetElementByName("WIND UA", m_pFilePath);
	pElementV = dPMgr->GetElementByName("WIND VA", m_pFilePath);
	if (NULL == pElementU || NULL == pElementV )
	{
		return;
	}
	pDataArrayU= pElementU->Data().GetDataArray(0);
	pDataArrayV= pElementV->Data().GetDataArray(0);
	if (NULL == pDataArrayU || NULL == pDataArrayV )
	{
		return;
	}

	QStandardItem *newItem=0;
	for (int i = 0 ; i<y;i++)//lat  144
	{
		for (int j = 0 ;j<x;j++)//lon 73
		{
			tempStr = QString::number(pDataArrayU[i*x+j]) + "/" + QString::number(pDataArrayV[i*x+j]);
			newItem = new QStandardItem(tempStr);
			m_ItemModel->setItem(i ,j, newItem);
		}
	}
	m_TbView->setModel(m_ItemModel);
	m_TbView->setSortingEnabled(false);

	//QFileInfo temDir(m_pElement->GetFileName());
	//QString suffix =  temDir.suffix().toLower();
	//if (m_pElement->GetFileType() == QString("MICAPS11"))
	//{
	//	m_ActionSave->setEnabled(true);
	//}
	//else
	//{
	//	m_ActionSave->setEnabled(false);
	//}
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年5月30日
************************************************************************/
void GridDataDialog::InitializeModelCIPASM(CDataProviderManager* dPMgr  ,int nTime ,QString time ,int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);
	m_HeaderItemList.clear();
	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//转译中文名list
	m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(0), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	int ncountlevels = m_pElement->Level().m_valus.size();

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);

	QStandardItem *newItem=0;
	newItem = new QStandardItem(tr("站点编号"));
	m_ItemModel->setHorizontalHeaderItem(0, newItem);
	newItem = new QStandardItem(tr("站点经度"));
	m_ItemModel->setHorizontalHeaderItem(1, newItem);
	newItem = new QStandardItem(tr("站点纬度"));
	m_ItemModel->setHorizontalHeaderItem(2, newItem);
	m_HeaderItemList<<tr("站点编号")<<tr("站点经度")<<tr("站点纬度");
	for(int i =0;i<headerItemList.size();i++)
	{
		newItem = new QStandardItem(headerItemList.at(i));
		m_ItemModel->setHorizontalHeaderItem(i+3, newItem);
		m_HeaderItemList<<headerItemList.at(i);
	}
	m_stationname.clear();
	for(int i =nLevel;i<nLevel+1;i++)
	{
		for(int n = 0;n<m_nNumData;n++)
		{
			m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			newItem->setEditable(false);
			m_stationname.insert(n,newItem->text().toFloat());
			m_ItemModel->setItem(i*m_nNumData+n,0,newItem);
			m_pElement = dPMgr->GetElementByChineseName(tr("站点经度"), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			newItem->setEditable(false);
			m_ItemModel->setItem(i*m_nNumData+n,1,newItem);
			m_pElement = dPMgr->GetElementByChineseName(tr("站点纬度"), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			newItem->setEditable(false);
			m_ItemModel->setItem(i*m_nNumData+n,2,newItem);

			for(int j = 0;j<headerItemList.size();j++)
			{
				m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(j), m_pFilePath);
				if (m_pElement == NULL)
				{
					return;
				}
				newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i*m_nNumData+n]));
				m_ItemModel->setItem(i*m_nNumData+n,j+3,newItem);
			}
		}
	}

	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);

	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	if (level.isEmpty() || level.isNull())
	{
		if (nTime>=0)
		{
			m_nLevel = nTime;
			m_elename = time;
		}
	}
	else if (time.isEmpty() || time.isNull())
	{
		if (nLevel>0)
		{
			m_nLevel = nLevel;
			m_elename = level;
		}
	}
	else
	{
		m_nLevel = nLevel*nTime;
	}
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
	connect(m_ItemModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(modifyItemData(QStandardItem *)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年6月10日
************************************************************************/
void GridDataDialog::InitializeModelSTADA(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<<tr("站点编号")<<tr("站点纬度")<<tr("站点经度")<<"Tim"<<"Level"<<"Flag"<<tr("气温")<<tr("气温距平")<<tr("降水量")<<tr("降水距平百分率");
	QStandardItem *newItem=0;

	for(int n = 0;n<10;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		for(int i = 0;i<ncount;i++)
		{
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			m_ItemModel->setItem(i,n,newItem);
		}
	}
	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);

	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 功能说明：初始化model数据
 作      者：赵高攀
 完成日期：2011年6月10日
************************************************************************/
void GridDataDialog::InitializeModelTXT(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);
	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//转译中文名list
	m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(0), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}

	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);		//取得要素的数量

	m_HeaderItemList.clear();

	//创建表头
	QStandardItem *newItem=0;
	newItem = new QStandardItem(tr("站点编号"));
	m_ItemModel->setHorizontalHeaderItem(0, newItem);
	newItem = new QStandardItem(tr("站点经度"));
	m_ItemModel->setHorizontalHeaderItem(1, newItem);
	newItem = new QStandardItem(tr("站点纬度"));
	m_ItemModel->setHorizontalHeaderItem(2, newItem);
	m_HeaderItemList<<tr("站点编号")<<tr("站点经度")<<tr("站点纬度");

	for(int i = 0 ; i< headerItemList.count(); i++)
	{
		newItem = new QStandardItem(headerItemList.at(i));
		m_ItemModel->setHorizontalHeaderItem(i+3, newItem);
			m_HeaderItemList<<headerItemList.at(i);
	}

	//初始化数据功能
	for(int n = 0;n<m_nNumData;n++)
	{
		m_pElement = dPMgr->GetElementByChineseName(tr("站点编号"), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
		m_ItemModel->setItem(n,0,newItem);
		m_pElement = dPMgr->GetElementByChineseName(tr("站点经度"), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
		m_ItemModel->setItem(n,1,newItem);
		m_pElement = dPMgr->GetElementByChineseName(tr("站点纬度"), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
		m_ItemModel->setItem(n,2,newItem);

		for(int j = 0;j<headerItemList.size();j++)
		{
			m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(j), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			m_ItemModel->setItem(n,j+3,newItem);
		}
	}
	

	m_TbView->setModel(m_ItemModel);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));

}

void GridDataDialog::InitializeModelStaion( CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName("站点编号", m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< "站点编号"/*<<"站点名称"*/<<"站点经度" <<"站点纬度";
	m_HeaderItemList.append(m_EleTypeNames_);
	QStandardItem *newItem=0;

	for(int n = 0;n<m_EleTypeNames_.size() + 3;n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);
		m_pElement = dPMgr->GetElementByChineseName(m_HeaderItemList.at(n), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}

		for(int i = 0;i<ncount;i++)
		{
			
			if (n == 3)
			{
				char cTempFloat[128];
				sprintf(cTempFloat,"%.2lf",m_pElement->Data().GetDataArray(0)[i]);
				newItem = new QStandardItem(QString(cTempFloat));
			}
			else
			{
				newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			}
			
			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}

void GridDataDialog::InitializeModelEXPONENT( QString var , CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(var, m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	if(!(dPMgr->ReadDataFromFile(m_pElement)))
	{
		return;
	}

	//for (int i = 0;i < m_EleTypeNames_.count();i++)
	//	qDebug()<<m_EleTypeNames_.at(i);
	
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<<"年份"<<"月份";
	m_HeaderItemList.append(var);
	QStandardItem *newItem=0;

	for(int n = 0;n<m_HeaderItemList.size();n++)
	{
		newItem = new QStandardItem(m_HeaderItemList.at(n));
		m_ItemModel->setHorizontalHeaderItem(n, newItem);

		for(int i = 0;i<ncount;i++)
		{

			if (n == 0)
			{
				QDate tempData = m_pElement->SelectedPeriod().m_hashDate.value(i+1).date();
				//qDebug()<<n<<i<<tempData.year();
				newItem = new QStandardItem(QString::number(tempData.year()));
			}
			else if (n == 1)
			{
				QDate tempData = m_pElement->SelectedPeriod().m_hashDate.value(i+1).date();
				//qDebug()<<n<<i<<tempData.month();
				newItem = new QStandardItem(QString::number(tempData.month()));
			}
			else
			{
				newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[i]));
			}

			m_ItemModel->setItem(i,n,newItem);
		}
	}

	m_TbView->setModel(m_ItemModel);

	//m_ActionSave->setEnabled(true);
	QHeaderView *headerGoods = m_TbView->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}

void GridDataDialog::modifyItemData( QStandardItem * modItem)
{
	QString newValue = modItem->text();
	int iColumn = modItem->column();
	int iRow = modItem->row();

	QStandardItem* moItem = m_ItemModel->item(iRow,0);
	float newalue = moItem->text().toFloat();
	int orlocal = m_stationname.key(newalue);
 	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//转译中文名list
	headerItemList.push_front("站点纬度");
	headerItemList.push_front("站点经度");
	headerItemList.push_front("站点编号");

 	CElement* pElement = m_pDPMgr->GetElementByChineseName(headerItemList.at(iColumn), m_pFilePath);
	pElement->Data().GetDataArray(0)[m_nLevel*m_nNumData+orlocal] = newValue.toFloat();
}

void GridDataDialog::modifyCipasr( QStandardItem * modItem)
{
	QString newValue = modItem->text();
	int iColumn = modItem->column();
	int iRow = modItem->row();

	CElement* pElement = m_pDPMgr->GetElementByChineseName("格点数据", m_pFilePath);
	int x =  pElement->ActualLon().m_count;
	int y = pElement->ActualLat().m_count;

	pElement->Data().GetDataArray(0)[x*y*m_nLevel+iRow*x+iColumn] = newValue.toFloat();
}
