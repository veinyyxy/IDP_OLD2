/************************************************************************/
/* Grads��Ʒ�������öԻ���ʵ���ļ�                                       */
/************************************************************************/
#include <QSettings>
#include <QStringList>
#include "GridDataDialog.h"

QString GridDataDialog::pBasePath  = QDir::currentPath();//��̬�������浱ǰ���ļ�·��


/************************************************************************
 ����˵������ʼ���Ի���
 ��      �ߣ��׼���
 ������ڣ�2011��5��10��
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

			m_fileType = tr("CIPASվ������(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS2")
		{
			InitializeModelM2( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPASվ������(*.cps)");
		}
		else if (element1->GetFileType() == "MICAPS3")
		{
			InitializeModelM3( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType = tr("CIPASվ������(*.cps);;MICAPS3����(*.000)");
		}
		else if (element1->GetFileType() == "MICAPS5")
		{
			InitializeModelM5( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPASվ������(*.cps)");
		}
		else if (element1->GetFileType() == "TXT")
		{
			InitializeModelTXT( dPMgr );
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPASվ������(*.cps)");
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
			m_fileType =  tr("CIPASվ������(*.cps)");
		}
		else if (element1->GetFileType() == "STADA")
		{
			InitializeModelSTADA(dPMgr);
			m_TbView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_TbView->setSelectionMode(QAbstractItemView::MultiSelection);	
			connect(m_TbView, SIGNAL(clicked(QModelIndex )), this, SLOT(FocusStations()));
			m_fileType =  tr("CIPASվ������(*.cps)");
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
			m_fileType =  tr("CIPASվ������(*.cps)");
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
			m_fileType =  tr("CIPASվ������(*.cps)");
		//	m_fileType = "CIPASվ������(*.cps);;MICAPS3����(*.000)";
		}
		else if (element1->GetFileType() == "EXPONENT")
		{
			InitializeModelEXPONENT( var,dPMgr );
			m_fileType =  tr("ָ������(*.tms)");
		}
		else 
		{
			InitializeModel(dPMgr,var,nTime,time,nLevel,level);
			m_fileType =  tr("NetCDF����(*.nc);;CIPAS�������(*.cps)");
			if (element1->GetFileType() == "MICAPS4")
				m_fileType = tr("NetCDF����(*.nc);;MICAPS4����(*.000);;CIPASվ������(*.cps)");
		}
	}
	setWindowTitle(tr("��ʾѡ�������б�"));
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
 ����˵������ʼ���Ի���ؼ�
 ��      �ߣ��׼���
 ������ڣ�2011��5��10��
************************************************************************/
void GridDataDialog::InitDialog()
{
	m_MGridLayout = new QGridLayout(this);
	this->setLayout(m_MGridLayout);

	m_TopToolBar = new QToolBar(this);
	m_TopToolBar->setObjectName(tr("������0"));
	m_MGridLayout->addWidget(m_TopToolBar,0,0);

	m_ActionSave = new QAction(QIcon(":/images/save.png"),tr("����"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionSave);
	//m_ActionSave->setEnabled(false);

	m_TbView = new QTableView(this);
	m_TbView->setSortingEnabled(true);
	m_MGridLayout->addWidget(m_TbView,1,0);
	m_TbView->show();
}

/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��׼���
 ������ڣ�2011��5��10��
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
	
	if (QString("�������") == var)
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
 ����˵������ʼ���Ի����¼�
 ��      �ߣ��׼���
 ������ڣ�2011��5��10��
************************************************************************/
void GridDataDialog::InitUISlots()
{
	m_TopToolBar->connect(m_TopToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(DataToolBarEvent(QAction*)));
}

void GridDataDialog::writeGridData(QString& fileName)
{
	//--------------------------------------------------------------------------д��������ļ�Start
	QFileInfo temFDir(m_pFilePath);
	QString suffixF =  temFDir.suffix().toLower();		 

	if (QString("nc") == suffixF || QString("NetCDF") == m_pElement->GetFileType() || QString("GRIB") == m_pElement->GetFileType() || QString("AWX") == m_pElement->GetFileType())//netCDF to cipas 
		//	if (QString("GRIB") == m_pElement->GetFileType() || QString("AWX") == m_pElement->GetFileType())
	{
		m_pLstElement.clear();
		m_pLstElement.append(m_pElement);
		if (m_pDPMgr->WriteCiapsRaster(fileName,m_pLstElement)>0)
		{
			QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
			//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����cps����ļ���") );
		}
		else
		{
			QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
			//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����cps����ļ�ʧ�ܣ�") );
		}
		return;
	}
	//--------------------------------------------------------------------------д��������ļ�END
	//--------------------------------------------------------------------------дվ�������ļ�Start
	m_pLstElement.clear();
	CElement *pTempEle = NULL;
	QString strTemp;
	//--------------------------------------------------------------------------�ļ�ͷStart
	CElement *pElement = new CElement;
	pElement->name(tr("�ļ�����"));
	pElement->SetMicapsDeclaration(tr("1"));//cipas  ��������
	pElement->SetMicapsDeclaration(tr("3"));
	pElement->SetMicapsDeclaration(tr("8"));
	pElement->SetMicapsDeclaration(tr("10"));
	pElement->SetMicapsDeclaration(m_pDPMgr->GetFileDescription(m_pFilePath));
	pElement->SetMicapsDeclaration(tr("CIPAS"));
	QString PDay=tr("PDAY 10 ");
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("վ����"),m_pFilePath);
	if (NULL == pTempEle ||pTempEle->ActualPeriod().m_hashDate.size()<1)
	{
		QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����cps�ļ�ʧ�ܣ�") );
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

	pElement->SetMicapsDeclaration(QString::number(pTempEle->Data().DataBlockDimensionSize(0,0)));//վ������
	pElement->SetMicapsDeclaration(QString::number(m_HeaderItemList.size()));					  //���Ը���
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
	//--------------------------------------------------------------------------�ļ�ͷEND
	//--------------------------------------------------------------------------Ҫ������Start
	for(int i= 0;i<m_HeaderItemList.size();i++)
	{
		pTempEle = m_pDPMgr->GetElementByChineseName(m_HeaderItemList.at(i),m_pFilePath);
		m_pLstElement.append(pTempEle);
	}
	//--------------------------------------------------------------------------Ҫ������END
	if (m_pDPMgr->WriteCipas(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
		//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����cpsվ�������ļ���") );
	}
	else
	{
		QMessageBox::warning(this,tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����cpsվ�������ļ�ʧ�ܣ�") );
	}
	pElement = NULL;
	pTempEle = NULL;
	//--------------------------------------------------------------------------дվ�������ļ�END
}


void GridDataDialog::writeNCData(QString& fileName)
{
	m_pLstElement.clear();
	m_pLstElement.append(m_pElement);
	if (m_pDPMgr->Write(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
		//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����nc�ļ���") );
	}
	else
	{
		QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����nc�ļ�ʧ�ܣ�") );
	}
}

void GridDataDialog::writeCipasGridData(QString& fileName)
{
	m_pLstElement.clear();

	//m_pDPMgr->CloseFilter();
	//QStringList eleNames  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);   //ת��������list���õ�����ͷ��Ϣ

	m_pDPMgr->OpenFilter();
	CElement* pCheckElement = m_pDPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);

	if(pCheckElement == NULL) //��㱣��
	{
		m_pLstElement.clear();
		m_pLstElement.append(m_pElement);
		if (m_pDPMgr->WriteCiapsRaster(fileName,m_pLstElement)>0)
		{
			QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
			//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����cps����ļ���") );
		}
		else
		{
			QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
			//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����cps����ļ�ʧ�ܣ�") );
		}

		return;
	}

	////ȡ��վ���ŵ�Element���󣬴Ӷ�ȡ�ö���
	//CElement* pElement = m_pDPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	//if (!pElement)
	//{
	//	pElement = m_pDPMgr->GetElementByChineseName(tr("�������"), m_pFilePath);
	//	if (!pElement)
	//	{
	//		return;
	//	}

	//	QList<CElement*> elements_v;
	//	elements_v.push_back(pElement);
	//	if (m_pDPMgr->WriteCiapsRaster(fileName,elements_v)>0)
	//	{
	//		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
	//		CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����nc�ļ���") );
	//	}
	//	else
	//	{
	//		QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
	//		CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����nc�ļ�ʧ�ܣ�") );
	//	}
	//	return;
	//}

	// ����Ϊվ�㱣�� ���� 2012-10-18
	//�ļ�ͷ��Ϣ����
	CElement *pElement = new CElement;
	pElement->name(QString::fromLocal8Bit("�ļ�����"));
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
	pElement->SetMicapsDeclaration(QString::number(0)); //��ֵ��������������
	m_pLstElement.append(pElement);

	//ȡ��վ����
	pElement = m_pDPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	m_pLstElement.append(pElement);
	//ȡ��վ�㾭��
	pElement = m_pDPMgr->GetElementByChineseName(tr("վ�㾭��"), m_pFilePath);
	m_pLstElement.append(pElement);

	//ȡ��վ��γ��
	pElement = m_pDPMgr->GetElementByChineseName(tr("վ��γ��"), m_pFilePath);
	m_pLstElement.append(pElement);
	pElement = NULL;


	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);   //ת��������list���õ�����ͷ��Ϣ

	//ȡ�ø���������
	for(int i = 0 ; i < headerItemList.count(); i++)
	{
		QString itemList = headerItemList.at(i);

		pElement = m_pDPMgr->GetElementByChineseName(headerItemList.at(i), m_pFilePath);
		m_pLstElement.append(pElement);
	}

	if (m_pDPMgr->WriteCipas(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
		//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����nc�ļ���") );
	}
	else
	{
		QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����nc�ļ�ʧ�ܣ�") );
	}
}

void GridDataDialog::writeMICAPS3(QString& fileName)
{
	CElement *pTempEle= NULL;
	m_pLstElement.clear();
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("������Ϣ"),m_pFilePath);
	m_pLstElement.append(pTempEle);
	for(int i= 0;i<m_HeaderItemList.size();i++)
	{
		pTempEle = m_pDPMgr->GetElementByChineseName(m_HeaderItemList.at(i),m_pFilePath);
		m_pLstElement.append(pTempEle);
	}
	if (m_pDPMgr->WriteMicaps3(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
		//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����MICAPS3�ļ�") );
	}
	else
	{
		QMessageBox::warning(this, tr("����"), tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����MICAPS3�ļ�ʧ�ܣ�") );
	}
	pTempEle = NULL;
}
void GridDataDialog::writeMICAPS4(QString& fileName)
{
	CElement *pTempEle= NULL;
	m_pLstElement.clear();
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("������Ϣ"),m_pFilePath);
	m_pLstElement.append(pTempEle);
	pTempEle = m_pDPMgr->GetElementByChineseName(tr("�������"),m_pFilePath);
	m_pLstElement.append(pTempEle);

	if (m_pDPMgr->WriteMicaps4(fileName,m_pLstElement)>0)
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɹ�!"));
		//CDiskFileLog::WriteLog(tr("��Ϣ"), tr("DataToolBarEvent::�ɹ�����MICAPS4�ļ���") );
	}
	else
	{
		QMessageBox::warning(this,tr("����"),tr("����ʧ��"), QMessageBox::Retry, QMessageBox::Cancel);
		//CDiskFileLog::WriteLog(tr("����"), tr("DataToolBarEvent::����MICAPS4�ļ�ʧ�ܣ�") );
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
 ����˵����ToolBar�¼�
 ��      �ߣ��׼���
 ������ڣ�2011��5��10��
************************************************************************/
void GridDataDialog::DataToolBarEvent(QAction* action)
{
	QString cmd = action->text();
	//QString filter = "�������� (*."+m_fileType+")";
	if (cmd == tr("����"))
	{
		//ȡ���ϴμ�¼��·��
		QSettings myQSettings;  // where we keep last used filter in persistent state
		QString lastSaveDir = myQSettings.value( "CIPASUI/MainWinSaveDir", "/home/" ).toString();

		if (m_fileType == tr("txt"))
		{
			//CDiskFileLog::WriteLog(tr("����"),tr("��֧�ֱ������ͣ�") );
			QMessageBox::information(this,tr("��ʾ"),tr("��֧�ֱ������ͣ�"));
			return;
		}

		QString fileName = QFileDialog::getSaveFileName(this,tr("���������ļ�"), lastSaveDir ,m_fileType);
		if (fileName.isEmpty())
		{
			//CDiskFileLog::WriteLog(tr("����"),tr("DataToolBarEvent::�ļ������ļ�·��Ϊ�գ�") );
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

		//���汾�δ򿪵�·��
		QFileInfo fi(fileName);
		myQSettings.setValue("CIPASUI/MainWinSaveDir",fi.absolutePath());
	}
}

/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��5��30��
************************************************************************/
void GridDataDialog::InitializeModelM1(CDataProviderManager* dPMgr ,int nTime ,QString time ,int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("վ����" )<<tr("վ�㾭��") <<tr("վ��γ��") <<tr("վ�㺣��")<<tr("վ�㼶��")
					<< tr("������" )<<tr("����") <<tr("����")<<tr("��ƽ����ѹ") <<tr("3Сʱ��ѹ")
					<<tr("��ȥ����1")<< tr("��ȥ����2") <<tr("6Сʱ��ˮ" )<<tr("����״") <<tr("������")
					<<tr("���Ƹ�")<< tr("¶��") <<tr("�ܼ���") <<tr("��������") <<tr("�¶�")<<tr("����״")
					<< tr("����״") ;
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
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
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��5��30��
************************************************************************/
void GridDataDialog::InitializeModelM2(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("վ����") <<tr("վ�㾭��") <<tr("վ��γ��" )<<tr("վ�㺣��" )<<tr("վ�㼶��")
		<< tr("�߶�") <<tr("�¶�") <<tr("�¶�¶���")<<tr("����") <<tr("����");
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��5��30��
************************************************************************/
void GridDataDialog::InitializeModelM3(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("վ����") <<tr("վ�㾭��") <<tr("վ��γ��") <<tr("���θ߶�")<<tr("վ��ֵ1")
		<< tr("վ��ֵ2");
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��7��28��
************************************************************************/
void GridDataDialog::InitializeModelM5(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<<tr("վ����")<<tr("վ�㾭��")<<tr("վ��γ��")<<tr("վ�㺣��")<<tr("���ݳ���")<<tr("��ѹ")<<tr("�߶�")<<tr("�¶�")<<tr("¶��")<<tr("����")<<tr("����");
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
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��6��10��
************************************************************************/
void GridDataDialog::InitializeModelM8(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< tr("վ����") <<tr("վ�㾭��") <<tr("վ��γ��") <<tr("վ�㺣��")<<tr("��������1")
					<< tr("����1") <<tr("����1") <<tr("����¶�")<<tr("����¶�") <<tr("��������2")
					<<tr("����2")<< tr("����2") ;
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��6��13��
************************************************************************/
void GridDataDialog::InitializeModelM7(CDataProviderManager* dPMgr )
{
	QStringList tempStrList;
	int nPointNum;
	float *pDataArray;
	CData tempData;

	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("�ļ�����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}

	tempStrList = m_pElement->GetMicapsDeclaration();
	nPointNum = tempStrList.at(tempStrList.size()-1).toInt();

	m_pElement = dPMgr->GetElementByChineseName(tr("̨��·��"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	tempData = m_pElement->Data();
	pDataArray = tempData.GetDataArray(0);

	QStringList headerItemList ;
	headerItemList<< tr("��") <<tr("��" )<<tr("��" )<<tr("ʱ��")<<tr("ʱЧ")
					<< tr("���ľ���" )<<tr("����γ��") <<tr("������")<<tr("���������ѹ") <<tr("�߼���Ȧ�뾶")
					<<tr("ʮ����Ȧ�뾶")<< tr("����") << tr("����") ;

	QStandardItem *newItem=0;
	for(int i = 0;i<13;i++)//��ӱ���
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��6��13��
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
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��5��30��
************************************************************************/
void GridDataDialog::InitializeModelCIPASM(CDataProviderManager* dPMgr  ,int nTime ,QString time ,int nLevel ,QString level)
{
	m_ItemModel = new QStandardItemModel(this);
	m_HeaderItemList.clear();
	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//ת��������list
	m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(0), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	int ncountlevels = m_pElement->Level().m_valus.size();

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);

	QStandardItem *newItem=0;
	newItem = new QStandardItem(tr("վ����"));
	m_ItemModel->setHorizontalHeaderItem(0, newItem);
	newItem = new QStandardItem(tr("վ�㾭��"));
	m_ItemModel->setHorizontalHeaderItem(1, newItem);
	newItem = new QStandardItem(tr("վ��γ��"));
	m_ItemModel->setHorizontalHeaderItem(2, newItem);
	m_HeaderItemList<<tr("վ����")<<tr("վ�㾭��")<<tr("վ��γ��");
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
			m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			newItem->setEditable(false);
			m_stationname.insert(n,newItem->text().toFloat());
			m_ItemModel->setItem(i*m_nNumData+n,0,newItem);
			m_pElement = dPMgr->GetElementByChineseName(tr("վ�㾭��"), m_pFilePath);
			if (m_pElement == NULL)
			{
				return;
			}
			newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
			newItem->setEditable(false);
			m_ItemModel->setItem(i*m_nNumData+n,1,newItem);
			m_pElement = dPMgr->GetElementByChineseName(tr("վ��γ��"), m_pFilePath);
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
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
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��6��10��
************************************************************************/
void GridDataDialog::InitializeModelSTADA(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<<tr("վ����")<<tr("վ��γ��")<<tr("վ�㾭��")<<"Tim"<<"Level"<<"Flag"<<tr("����")<<tr("���¾�ƽ")<<tr("��ˮ��")<<tr("��ˮ��ƽ�ٷ���");
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));
}
/************************************************************************
 ����˵������ʼ��model����
 ��      �ߣ��Ը���
 ������ڣ�2011��6��10��
************************************************************************/
void GridDataDialog::InitializeModelTXT(CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);
	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//ת��������list
	m_pElement = dPMgr->GetElementByChineseName(headerItemList.at(0), m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}

	m_nNumData = m_pElement->Data().DataBlockDimensionSize(0,0);		//ȡ��Ҫ�ص�����

	m_HeaderItemList.clear();

	//������ͷ
	QStandardItem *newItem=0;
	newItem = new QStandardItem(tr("վ����"));
	m_ItemModel->setHorizontalHeaderItem(0, newItem);
	newItem = new QStandardItem(tr("վ�㾭��"));
	m_ItemModel->setHorizontalHeaderItem(1, newItem);
	newItem = new QStandardItem(tr("վ��γ��"));
	m_ItemModel->setHorizontalHeaderItem(2, newItem);
	m_HeaderItemList<<tr("վ����")<<tr("վ�㾭��")<<tr("վ��γ��");

	for(int i = 0 ; i< headerItemList.count(); i++)
	{
		newItem = new QStandardItem(headerItemList.at(i));
		m_ItemModel->setHorizontalHeaderItem(i+3, newItem);
			m_HeaderItemList<<headerItemList.at(i);
	}

	//��ʼ�����ݹ���
	for(int n = 0;n<m_nNumData;n++)
	{
		m_pElement = dPMgr->GetElementByChineseName(tr("վ����"), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
		m_ItemModel->setItem(n,0,newItem);
		m_pElement = dPMgr->GetElementByChineseName(tr("վ�㾭��"), m_pFilePath);
		if (m_pElement == NULL)
		{
			return;
		}
		newItem = new QStandardItem(QString::number(m_pElement->Data().GetDataArray(0)[n]));
		m_ItemModel->setItem(n,1,newItem);
		m_pElement = dPMgr->GetElementByChineseName(tr("վ��γ��"), m_pFilePath);
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), m_TbView, SLOT (sortByColumn(int)));

}

void GridDataDialog::InitializeModelStaion( CDataProviderManager* dPMgr )
{
	m_ItemModel = new QStandardItemModel(this);

	m_pElement = dPMgr->GetElementByChineseName("վ����", m_pFilePath);
	if (m_pElement == NULL)
	{
		return;
	}
	long ncount = m_pElement->Data().DataBlockDimensionSize(0,0);
	m_HeaderItemList.clear();
	m_HeaderItemList<< "վ����"/*<<"վ������"*/<<"վ�㾭��" <<"վ��γ��";
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
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
	m_HeaderItemList<<"���"<<"�·�";
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
	//SortIndicatorΪˮƽ�����������Աߵ�����ָʾ��
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
 	QStringList headerItemList  = m_pDPMgr->GetAllElementChineseNames(m_pFilePath);//ת��������list
	headerItemList.push_front("վ��γ��");
	headerItemList.push_front("վ�㾭��");
	headerItemList.push_front("վ����");

 	CElement* pElement = m_pDPMgr->GetElementByChineseName(headerItemList.at(iColumn), m_pFilePath);
	pElement->Data().GetDataArray(0)[m_nLevel*m_nNumData+orlocal] = newValue.toFloat();
}

void GridDataDialog::modifyCipasr( QStandardItem * modItem)
{
	QString newValue = modItem->text();
	int iColumn = modItem->column();
	int iRow = modItem->row();

	CElement* pElement = m_pDPMgr->GetElementByChineseName("�������", m_pFilePath);
	int x =  pElement->ActualLon().m_count;
	int y = pElement->ActualLat().m_count;

	pElement->Data().GetDataArray(0)[x*y*m_nLevel+iRow*x+iColumn] = newValue.toFloat();
}
