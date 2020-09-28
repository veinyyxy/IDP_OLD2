#include <QLibrary>
#include <QFile>
#include <QTextStream>

#include "dbmanager.h"
#include "sqlitemanager.h"

#include "mainwindow.h"
#include "sysmainwindow.h"

#include "DBDockWidget.h"


DBDockWidget::DBDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);
	m_pSqliteManager = 0;

	InitWidget();
	InitDBManager();
	InitWidgetDB();
	InitSlost();
}

DBDockWidget::~DBDockWidget()
{

}

void DBDockWidget::InitWidget()
{
	ui.dateTimeEdit_s->setDate(QDate::currentDate());
	ui.dateTimeEdit_s->setCalendarPopup(true);
	ui.dateTimeEdit_s->setDisplayFormat("yyyy'-'MM'-'dd'");

	ui.tableWidget_s->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_s->setSelectionBehavior (QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.tableWidget_s->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui.tableWidget_s->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	ui.tableWidget_s->setColumnWidth(0,126);
	ui.tableWidget_s->setColumnWidth(1,126);
	ui.tableWidget_s->setTextElideMode(Qt::ElideMiddle);
	ui.tableWidget_s->verticalHeader()->hide();
	ui.tableWidget_s->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//////////////////////////////////////////////////////////////////////////
	ui.dateTimeEdit_m->setDate(QDate::currentDate());
	ui.dateTimeEdit_m->setCalendarPopup(true);
	ui.dateTimeEdit_m->setDisplayFormat("yyyy'-'MM'-'dd'");

	ui.tableWidget_m->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_m->setSelectionBehavior (QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.tableWidget_m->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui.tableWidget_m->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	ui.tableWidget_m->setColumnWidth(0,126);
	ui.tableWidget_m->setColumnWidth(1,126);
	ui.tableWidget_m->setTextElideMode(Qt::ElideMiddle);
	ui.tableWidget_m->verticalHeader()->hide();
	ui.tableWidget_m->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//////////////////////////////////////////////////////////////////////////
	ui.gridLayout->setContentsMargins(0,0,0,0);
	ui.gridLayout_2->setContentsMargins(0,0,0,0);
	ui.gridLayout_3->setContentsMargins(0,0,0,0);
	ui.gridLayout_4->setContentsMargins(0,0,0,0);
	ui.gridLayout_5->setContentsMargins(0,0,0,0);
	ui.gridLayout_6->setContentsMargins(0,0,0,0);
}

void DBDockWidget::InitDBManager()
{
	typedef QObject* (*ClsFactory)(QString  Name,DBManager::EnumDBType dbType);
	ClsFactory avg = (ClsFactory) QLibrary::resolve("DBManager.dll", "CreateInstance");
	m_pSqliteManager =(SqliteManager*)avg("MDMS_DBManager",DBManager::Sqlite);
}

void DBDockWidget::InitWidgetDB()
{
	if (m_pSqliteManager)
	{
		QList<QList<QString>>  lists = m_pSqliteManager->QueryRadarListDB( tr("productTb"),tr("  where productName not like '组网%'"));
		for (int i = 0 ; i< lists.count() ; i++)
		{
			QString productId = lists.at(i).at(1).toLower();
			QString productName = lists.at(i).at(2);
			ui.cmbName_s->addItem(productName,productId);
		}

		QList<QList<QString>>  listm = m_pSqliteManager->QueryRadarListDB( tr("productTb"),tr("  where productName like '组网%'"));
		for (int i = 0 ; i< listm.count() ; i++)
		{
			QString productId = listm.at(i).at(1).toLower();
			QString productName = listm.at(i).at(2);
			ui.cmbName_m->addItem(productName,productId);
		}

		/////////////////////////////////////区域/////////////////////////////////////
		QFile file(QCoreApplication::applicationDirPath()+tr("/Config/radar_station.ini"));
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		QTextStream in(&file);
		int count = 0;
		while (!in.atEnd()) 
		{
			QString station = in.readLine();
			if (tr("") != station)
			{
				QString stationName = station.split("|").at(0);
				QString stationId = station.split("|").at(1).toLower();
				ui.cmbAre_s->addItem(stationName,stationId);
				count++;
			}
		}
		file.close();
		//////////////////////////////////////////////////////////////////////////
		ui.cmb_hour_s->addItem(tr("1小时"),tr("01:00:00"));
		ui.cmb_hour_s->addItem(tr("2小时"),tr("02:00:00"));
		ui.cmb_hour_s->addItem(tr("3小时"),tr("03:00:00"));
		ui.cmb_hour_s->addItem(tr("4小时"),tr("04:00:00"));
		ui.cmb_hour_s->addItem(tr("5小时"),tr("05:00:00"));
		ui.cmb_hour_s->addItem(tr("6小时"),tr("06:00:00"));

		ui.cmb_hour_m->addItem(tr("1小时"),tr("01:00:00"));
		ui.cmb_hour_m->addItem(tr("2小时"),tr("02:00:00"));
		ui.cmb_hour_m->addItem(tr("3小时"),tr("03:00:00"));
		ui.cmb_hour_m->addItem(tr("4小时"),tr("04:00:00"));
		ui.cmb_hour_m->addItem(tr("5小时"),tr("05:00:00"));
		ui.cmb_hour_m->addItem(tr("6小时"),tr("06:00:00"));
	}
	else
	{

	}
}

void DBDockWidget::InitSlost()
{
	connect(ui.cmbAre_s,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_AreaCurrentIndexChanged_S(int)));

	connect(ui.cmbName_s,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbCurrentIndexChanged_S(int)));
	connect(ui.dateTimeEdit_s,SIGNAL(dateTimeChanged ( const QDateTime &)),this,SLOT(Slot_DateTimeChanged_S(const QDateTime &)));
	connect(ui.chb_day_s,SIGNAL(clicked()),this,SLOT(Slot_Chbclicked_S()));
	connect(ui.chb_hour_s,SIGNAL(clicked()),this,SLOT(Slot_Chbclicked_S()));

	connect(ui.cmb_day_s,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbDayCurrentIndexChanged_S(int)));
	connect(ui.cmb_hour_s,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbHourCurrentIndexChanged_S(int)));

	connect(ui.tableWidget_s,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(Slot_ItemDoubleClicked_S ( QTableWidgetItem *)));
	//////////////////////////////////////////////////////////////////////////
	connect(ui.cmbName_m,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbCurrentIndexChanged_M(int)));
	connect(ui.dateTimeEdit_m,SIGNAL(dateTimeChanged ( const QDateTime &)),this,SLOT(Slot_DateTimeChanged_M(const QDateTime &)));
	connect(ui.chb_day_m,SIGNAL(clicked()),this,SLOT(Slot_Chbclicked_M()));
	connect(ui.chb_hour_m,SIGNAL(clicked()),this,SLOT(Slot_Chbclicked_M()));

	connect(ui.cmb_day_m,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbDayCurrentIndexChanged_M(int)));
	connect(ui.cmb_hour_m,SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_CmbHourCurrentIndexChanged_M(int)));

	connect(ui.tableWidget_m,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(	Slot_ItemDoubleClicked_M ( QTableWidgetItem *)));

	//this->m_pDateTimeEdit->setEnabled(false);
	//slot_cmbCurrentIndexChanged(ui.cmbName_s->currentIndex());
}



void DBDockWidget::Slot_AreaCurrentIndexChanged_S( int index )
{
	if (ui.cmbName_s->currentText() == tr(""))
	{
		return;
	}

	QString stationId = ui.cmbAre_s->itemData(index).toString();

	int iRows  = ui.tableWidget_s->rowCount();
	while(iRows)
	{
		ui.tableWidget_s->removeRow(iRows-1);
		iRows = ui.tableWidget_s->rowCount();
	}
	QString sql;
	sql.append(tr(" where "));
	sql.append(tr(" stationId ='"));
	sql.append(stationId);
	sql.append(tr("'  and "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_s->itemData(ui.cmbName_s->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'"));
	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_S(productPath,list.at(i).at(4),list.at(i).at(5));
	}
	Slot_CmbCurrentIndexChanged_S(ui.cmbName_s->currentIndex());
}

void DBDockWidget::Slot_CmbCurrentIndexChanged_S (int index)
{
	ui.chb_day_s->setChecked(false);
	ui.chb_hour_s->setChecked(false);

	ui.cmb_day_s->setEnabled(false);
	ui.cmb_hour_s->setEnabled(false);
	QString stationId = ui.cmbAre_s->itemData(ui.cmbAre_s->currentIndex()).toString();

	int iRows  = ui.tableWidget_s->rowCount();
	while(iRows)
	{
		ui.tableWidget_s->removeRow(iRows-1);
		iRows = ui.tableWidget_s->rowCount();
	}

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr(" stationId ='"));
	sql.append(stationId);
	sql.append(tr("'  and "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_s->itemData(ui.cmbName_s->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'"));
	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_S(productPath,list.at(i).at(4),list.at(i).at(5));
	}
	ui.dateTimeEdit_s->setEnabled(true);
}

void DBDockWidget::Slot_DateTimeChanged_S ( const QDateTime & datetime )
{
	ui.chb_day_s->setChecked(false);
	ui.chb_hour_s->setChecked(false);

	ui.cmb_day_s->setEnabled(false);
	ui.cmb_hour_s->setEnabled(false);

	QString stationId = ui.cmbAre_s->itemData(ui.cmbAre_s->currentIndex()).toString();

	int iRows  = ui.tableWidget_s->rowCount();
	while(iRows)
	{
		ui.tableWidget_s->removeRow(iRows-1);
		iRows = ui.tableWidget_s->rowCount();
	}

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr(" stationId ='"));
	sql.append(stationId);
	sql.append(tr("'  and "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_s->itemData(ui.cmbName_s->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate ='"));
	sql.append(datetime.date().toString("yyyy'-'MM'-'dd'"));
	sql.append(tr("'"));


	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_S(productPath,list.at(i).at(4),list.at(i).at(5));
	}

}

void DBDockWidget::Slot_Chbclicked_S()
{
	QCheckBox *btn = qobject_cast<QCheckBox *>(sender());
	if (btn->text() == tr("日"))
	{
		if (btn->isChecked())
		{
			ui.chb_hour_s->setChecked(false);
			ui.cmb_hour_s->setEnabled(false);

			ui.cmb_day_s->setEnabled(true);
			Slot_CmbDayCurrentIndexChanged_S(ui.cmb_day_s->currentIndex());
		}
		else
		{
			ui.cmb_day_s->setEnabled(false);
			Slot_DateTimeChanged_S(ui.dateTimeEdit_s->dateTime());
		}

	}
	else if (btn->text() == tr("时"))
	{
		if (btn->isChecked())
		{
			ui.chb_day_s->setChecked(false);
			ui.cmb_day_s->setEnabled(false);

			ui.cmb_hour_s->setEnabled(true);
			Slot_CmbHourCurrentIndexChanged_S(ui.cmb_hour_s->currentIndex());
		}
		else
		{
			ui.cmb_hour_s->setEnabled(false);
			Slot_DateTimeChanged_S(ui.dateTimeEdit_s->dateTime());
		}
	}
}

void DBDockWidget::Slot_CmbDayCurrentIndexChanged_S(int index)
{
	int iRows  = ui.tableWidget_s->rowCount();
	while(iRows)
	{
		ui.tableWidget_s->removeRow(iRows-1);
		iRows = ui.tableWidget_s->rowCount();
	}

	QString stationId = ui.cmbAre_s->itemData(ui.cmbAre_s->currentIndex()).toString();

	int day = ui.cmb_day_s->currentIndex();
	QString beginDate = ui.dateTimeEdit_s->date().addDays(-day).toString("yyyy'-'MM'-'dd'");
	QString endDate = ui.dateTimeEdit_s->date().toString("yyyy'-'MM'-'dd'");

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr(" stationId ='"));
	sql.append(stationId);
	sql.append(tr("'  and "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_s->itemData(ui.cmbName_s->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate between '"));
	//productDate  between '2010-10-08' and '2010-10-09'
	sql.append(beginDate);
	sql.append(tr("'  and '"));
	sql.append(endDate);
	sql.append(tr("'"));

	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_S(productPath,list.at(i).at(4),list.at(i).at(5));
	}

}
void DBDockWidget::Slot_CmbHourCurrentIndexChanged_S(int index)
{
	int iRows  = ui.tableWidget_s->rowCount();
	while(iRows)
	{
		ui.tableWidget_s->removeRow(iRows-1);
		iRows = ui.tableWidget_s->rowCount();
	}

	QString stationId = ui.cmbAre_s->itemData(ui.cmbAre_s->currentIndex()).toString();

	int day = ui.cmb_day_s->currentIndex();
	QString beginDate = ui.dateTimeEdit_s->date().addDays(-day).toString("yyyy'-'MM'-'dd'");
	QString endDate = ui.dateTimeEdit_s->date().toString("yyyy'-'MM'-'dd'");

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr(" stationId ='"));
	sql.append(stationId);
	sql.append(tr("'  and "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_s->itemData(ui.cmbName_s->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate between '"));
	//productDate  between '2010-10-08' and '2010-10-09'
	sql.append(beginDate);
	sql.append(tr("'  and '"));
	sql.append(endDate);
	sql.append(tr("'"));

	QString beginTime = "00:00:00";
	QString endTime = ui.cmb_hour_s->itemData(ui.cmb_hour_s->currentIndex(),Qt::UserRole).toString();

	sql.append(tr("  and  productTime between '"));
	//and productTime between '00:00:00' and '05:30:00'
	sql.append(beginTime);
	sql.append(tr("'  and '"));
	sql.append(endTime);
	sql.append(tr("'"));

	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_S(productPath,list.at(i).at(4),list.at(i).at(5));
	}
}


void DBDockWidget::AddTableWidgetItem_S(QString path,QString date , QString time)
{
	int iRows  = ui.tableWidget_s->rowCount();
	for (int i = 0 ; i<iRows; i++)
	{
		QString dataPath = ui.tableWidget_s->item(i,0)->data(Qt::UserRole).toString();
		if (dataPath == path)
		{
			return;
		}
	}

	ui.tableWidget_s->insertRow(0);
	path.replace("/","\\");
	QStringList dateList = path.split("\\");
	//if (dateList.count() > 1)
	//{
	//	QString time1 = dateList[dateList.count()-1].split("_")[2];
	//	QString time2 = dateList[dateList.count()-1].split("_")[3].split(".")[0];

	//	QString time11 = time1.mid(0,4)+"-"+time1.mid(4,2)+"-"+time1.mid(6,2);//20100213
	//	QString time22 = time2.mid(0,2)+":"+time2.mid(2,2)+":"+time2.mid(4,2);//100213
	QTableWidgetItem* item1 = new QTableWidgetItem(date);
	item1->setData(Qt::UserRole,path);
	QTableWidgetItem* item2 = new QTableWidgetItem(time);
	item2->setData(Qt::UserRole,path);
	ui.tableWidget_s->setItem(0,0,item1);
	item1->setToolTip(path);
	ui.tableWidget_s->setItem(0,1,item2);
	item2->setToolTip(path);
	//}
}


void DBDockWidget::Slot_ItemDoubleClicked_S( QTableWidgetItem* item )
{
	int count = ui.tableWidget_s->currentRow();
	if (count >= 0)
	{
		QString dataPath = ui.tableWidget_s->currentItem()->data(Qt::UserRole).toString();
		MainWindow* mw = SysMainWindow::GetMainWindow();
		mw->LoadData(dataPath);
		//mw->GetDefaultToolBar()->SetEmitForm(dataPath);
		//mw->GetDefaultToolBar()->EmitOpenSigal(tr("DBDockWidget"));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////组网产品///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DBDockWidget::Slot_CmbCurrentIndexChanged_M (int index)
{
	ui.chb_day_m->setChecked(false);
	ui.chb_hour_m->setChecked(false);

	ui.cmb_day_m->setEnabled(false);
	ui.cmb_hour_m->setEnabled(false);

	int iRows  = ui.tableWidget_m->rowCount();
	while(iRows)
	{
		ui.tableWidget_m->removeRow(iRows-1);
		iRows = ui.tableWidget_m->rowCount();
	}

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_m->itemData(ui.cmbName_m->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'"));
	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_M(productPath,list.at(i).at(4),list.at(i).at(5));
	}
	ui.dateTimeEdit_m->setEnabled(true);
}

void DBDockWidget::Slot_DateTimeChanged_M ( const QDateTime & datetime )
{
	ui.chb_day_m->setChecked(false);
	ui.chb_hour_m->setChecked(false);

	ui.cmb_day_m->setEnabled(false);
	ui.cmb_hour_m->setEnabled(false);

	int iRows  = ui.tableWidget_m->rowCount();
	while(iRows)
	{
		ui.tableWidget_m->removeRow(iRows-1);
		iRows = ui.tableWidget_m->rowCount();
	}

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_m->itemData(ui.cmbName_m->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate ='"));
	sql.append(datetime.date().toString("yyyy'-'MM'-'dd'"));
	sql.append(tr("'"));

	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_M(productPath,list.at(i).at(4),list.at(i).at(5));
	}

}

void DBDockWidget::Slot_Chbclicked_M()
{
	QCheckBox *btn = qobject_cast<QCheckBox *>(sender());
	if (btn->text() == tr("日"))
	{
		if (btn->isChecked())
		{
			ui.chb_hour_m->setChecked(false);
			ui.cmb_hour_m->setEnabled(false);

			ui.cmb_day_m->setEnabled(true);
			Slot_CmbDayCurrentIndexChanged_M(ui.cmb_day_m->currentIndex());
		}
		else
		{
			ui.cmb_day_m->setEnabled(false);
			Slot_DateTimeChanged_M(ui.dateTimeEdit_m->dateTime());
		}

	}
	else if (btn->text() == tr("时"))
	{
		if (btn->isChecked())
		{
			ui.chb_day_m->setChecked(false);
			ui.cmb_day_m->setEnabled(false);

			ui.cmb_hour_m->setEnabled(true);
			Slot_CmbHourCurrentIndexChanged_M(ui.cmb_hour_m->currentIndex());
		}
		else
		{
			ui.cmb_hour_m->setEnabled(false);
			Slot_DateTimeChanged_M(ui.dateTimeEdit_m->dateTime());
		}
	}
}

void DBDockWidget::Slot_CmbDayCurrentIndexChanged_M(int index)
{
	int iRows  = ui.tableWidget_m->rowCount();
	while(iRows)
	{
		ui.tableWidget_m->removeRow(iRows-1);
		iRows = ui.tableWidget_m->rowCount();
	}
	int day = ui.cmb_day_m->currentIndex();
	QString beginDate = ui.dateTimeEdit_m->date().addDays(-day).toString("yyyy'-'MM'-'dd'");
	QString endDate = ui.dateTimeEdit_m->date().toString("yyyy'-'MM'-'dd'");

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_m->itemData(ui.cmbName_m->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate between '"));
	//productDate  between '2010-10-08' and '2010-10-09'
	sql.append(beginDate);
	sql.append(tr("'  and '"));
	sql.append(endDate);
	sql.append(tr("'"));

	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_M(productPath,list.at(i).at(4),list.at(i).at(5));
	}

}
void DBDockWidget::Slot_CmbHourCurrentIndexChanged_M(int index)
{
	int iRows  = ui.tableWidget_m->rowCount();
	while(iRows)
	{
		ui.tableWidget_m->removeRow(iRows-1);
		iRows = ui.tableWidget_m->rowCount();
	}
	int day = ui.cmb_day_m->currentIndex();
	QString beginDate = ui.dateTimeEdit_m->date().addDays(-day).toString("yyyy'-'MM'-'dd'");
	QString endDate = ui.dateTimeEdit_m->date().toString("yyyy'-'MM'-'dd'");

	QString sql;
	sql.append(tr(" where "));
	sql.append(tr("   productId ='"));
	QString productId =ui.cmbName_m->itemData(ui.cmbName_m->currentIndex()).toString();
	sql.append(productId);
	sql.append(tr("'  and  productDate between '"));
	//productDate  between '2010-10-08' and '2010-10-09'
	sql.append(beginDate);
	sql.append(tr("'  and '"));
	sql.append(endDate);
	sql.append(tr("'"));

	QString beginTime = "00:00:00";
	QString endTime = ui.cmb_hour_m->itemData(ui.cmb_hour_m->currentIndex(),Qt::UserRole).toString();

	sql.append(tr("  and  productTime between '"));
	//and productTime between '00:00:00' and '05:30:00'
	sql.append(beginTime);
	sql.append(tr("'  and '"));
	sql.append(endTime);
	sql.append(tr("'"));

	QList<QList<QString>>  list =m_pSqliteManager->QueryRadarListDB(tr("radarProductTb"), sql);
	for (int i = 0 ; i< list.count() ; i++)
	{
		QString productPath = list.at(i).at(3);
		AddTableWidgetItem_M(productPath,list.at(i).at(4),list.at(i).at(5));
	}
}


void DBDockWidget::AddTableWidgetItem_M(QString path,QString date , QString time)
{
	int iRows  = ui.tableWidget_m->rowCount();
	for (int i = 0 ; i<iRows; i++)
	{
		QString dataPath = ui.tableWidget_m->item(i,0)->data(Qt::UserRole).toString();
		if (dataPath == path)
		{
			return;
		}
	}

	ui.tableWidget_m->insertRow(0);
	path.replace("/","\\");
	QStringList dateList = path.split("\\");
	//if (dateList.count() > 1)
	//{
	//	QString time1 = dateList[dateList.count()-1].split("_")[2];
	//	QString time2 = dateList[dateList.count()-1].split("_")[3].split(".")[0];

	//	QString time11 = time1.mid(0,4)+"-"+time1.mid(4,2)+"-"+time1.mid(6,2);//20100213
	//	QString time22 = time2.mid(0,2)+":"+time2.mid(2,2)+":"+time2.mid(4,2);//100213
	QTableWidgetItem* item1 = new QTableWidgetItem(date);
	item1->setData(Qt::UserRole,path);
	QTableWidgetItem* item2 = new QTableWidgetItem(time);
	item2->setData(Qt::UserRole,path);
	ui.tableWidget_m->setItem(0,0,item1);
	item1->setToolTip(path);
	ui.tableWidget_m->setItem(0,1,item2);
	item2->setToolTip(path);
	//}
}

void DBDockWidget::Slot_ItemDoubleClicked_M( QTableWidgetItem* item )
{
	int count = ui.tableWidget_m->currentRow();
	if (count >= 0)
	{
		QString dataPath = ui.tableWidget_m->currentItem()->data(Qt::UserRole).toString();
		MainWindow* mw = SysMainWindow::GetMainWindow();
		mw->LoadData(dataPath);
		//mw->GetDefaultToolBar()->SetEmitForm(dataPath);
		//mw->GetDefaultToolBar()->EmitOpenSigal(tr("DBDockWidget"));
	}
}


