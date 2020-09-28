/************************************************************************/
/* Grads产品生产设置对话框实现文件                                       */
/************************************************************************/
//#include "../GradsProvider/GradsProvider.h"
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>

#include "DataProvider.h"
#include "VectorLayer.h"
#include "GeosOnTheOsg.h"

#include "ExtendMap.h"
#include "AdapterWidget.h"
#include "ViewManager.h"

#include "IDPApplication.h"
#include "mainwindow.h"
#include "sysmainwindow.h"
//#include "LeftDockWidget.h"
//#include "QRecentFilesMenu.h"
//#include "DefaultToolBar.h"
#include "LegendWidget.h"

#include "CipasComposer.h"
#include "GridDataDialog.h"
#include "MeteoDataDialog.h"

using namespace goto_gis;

class DataInterface
{
public:
	virtual void DataInput(
		size_t iLonNCCount, 
		size_t iLatNCCount,  
		size_t iTimeNCCount, 
		size_t iLevelNCCount, 
		float fLonBegin, 
		float fLonEnd,
		float fLonDelta,
		float fLatBegin, 
		float fLatEnd,
		float fLatDelta,
		float fMissingValue,
		float fLevelBegin,
		float fLevelEnd,
		float fLevelDelta, 
		std::vector<float>& fLevelVector, 
		std::string& strTimeBegin,
		std::string& strTimeEnd,
		int iTimeDelta,
		double* dData, 
		int nModel, 
		int useNineP) = 0;
	virtual void DataInput( const std::string& ctlFile, const std::string& dataFile ) = 0;
	virtual void DataOutput() = 0;
	virtual void Draw(int iType) = 0;
};

MeteoDataDialog::MeteoDataDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("文件"));
	m_pGridDataDialog = NULL;
	m_bFirstOpenFile = true;
	m_pDPMgr = CDataProviderManager::GetDPManager();

	InitDialog();
	InitSlots();
}

MeteoDataDialog::~MeteoDataDialog(void)
{

}



/************************************************************************
功能说明：初始化对话框控件
作      者：易家祥
完成日期：2011年5月4日
************************************************************************/
void MeteoDataDialog::InitDialog()
{
	mainLayout = new QHBoxLayout;
	this->setLayout(mainLayout);
	mainLayout->setMargin(5);
	mainLayout->setSpacing(5);
	m_MGridLayout = new QGridLayout(this);
	//this->setLayout(m_MGridLayout);
	m_MGridLayout->setMargin(3);
	m_MGridLayout->setSpacing(2);
	m_TopToolBar = new QToolBar(this);
	m_TopToolBar->setIconSize(QSize(18,18));
	m_TopToolBar->setObjectName(tr("打开数据0"));
	m_MGridLayout->addWidget(m_TopToolBar,0,0,Qt::AlignTop);
	m_TopToolBar->setToolTip(tr("产品设置工具栏"));

	m_ActionOpen = new QAction(QIcon(":/images/defaultToolBar/open.png"),tr("打开数据"),m_TopToolBar);
	m_ActionOpen->setObjectName(tr("打开数据1"));

	m_TopToolBar->addAction(m_ActionOpen);

	m_ActionDataInfo  = new QAction(QIcon(":/images/DataInfo.png"),tr("显示数据信息"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionDataInfo);

	m_TopToolBar->addSeparator();

	m_ActionDrawData= new QAction(QIcon(":/images/DrawData.png"),tr("绘制数据信息"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionDrawData);

	m_ActionViewData= new QAction(QIcon(":/images/ViewData.png"),tr("查看数据列表"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionViewData);

	m_TopToolBar->addSeparator();

	m_ActionSectionPlot = new QAction(QIcon(":/images/SectionPlot.png"),tr("剖面图"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionSectionPlot);

	m_ActionDimensionPlot = new QAction(QIcon(":/images/DimensionPlot.png"),tr("折线图"),m_TopToolBar);
	m_TopToolBar->addAction(m_ActionDimensionPlot);

	m_GroupBox = new QGroupBox(tr("产品属性选择"),this);
	m_MGridLayout->addWidget(m_GroupBox,1,0,Qt::AlignTop);
	m_GridLayout = new QGridLayout(m_GroupBox);
	m_GridLayout->setMargin(3);
	m_GridLayout->setSpacing(2);

	m_LblVar = new QLabel(tr("要素："),m_GroupBox);
	m_CmbVar = new QComboBox(m_GroupBox);
	m_CmbVar->setMinimumWidth(160);
	

	m_LblTime = new QLabel(tr("时间："),m_GroupBox);
	m_CmbTime = new QComboBox(m_GroupBox);
	m_CmbTime->setMinimumWidth(160);

	m_LblLevel = new QLabel(tr("垂直层次："),m_GroupBox);
	m_CmbLevel = new QComboBox(m_GroupBox);
	m_CmbLevel->setMinimumWidth(160);

	m_LblDrawType = new QLabel(tr("产品类型："),m_GroupBox);
	m_CmbDrawType = new QComboBox(m_GroupBox);
	m_CmbDrawType->setMinimumWidth(160);

	m_GridLayout->addWidget(m_LblVar,0,0,Qt::AlignLeft);
	m_GridLayout->addWidget(m_CmbVar,0,1,Qt::AlignLeft);
	
	m_GridLayout->addWidget(m_LblLevel,1,0,Qt::AlignLeft);
	m_GridLayout->addWidget(m_CmbLevel,1,1,Qt::AlignLeft);

	m_GridLayout->addWidget(m_LblTime,2,0,Qt::AlignLeft);
	m_GridLayout->addWidget(m_CmbTime,2,1,Qt::AlignLeft);

	m_GridLayout->addWidget(m_LblDrawType,3,0,Qt::AlignLeft);
	m_GridLayout->addWidget(m_CmbDrawType,3,1,Qt::AlignLeft);

	m_GroupBox->setLayout(m_GridLayout);

	m_pFileListWidget = new QListWidget(this);
	m_pFileListWidget->setMinimumHeight(186);
	m_pFileListWidget->setMaximumHeight(186);
	m_pFileListWidget->setObjectName(QString("objFileListWidget"));
	m_pFileListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	m_pFileListWidget->setSelectionBehavior ( QAbstractItemView::SelectItems); //设置选择行为，以行为单位
	m_pFileListWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	m_pFileListWidget->setTextElideMode(Qt::ElideMiddle);

	this->setMinimumHeight(196);
	this->setMaximumHeight(196);

	mainLayout->addWidget(m_pFileListWidget);
	mainLayout->addLayout(m_MGridLayout);
	m_MGridLayout->setSizeConstraint(QLayout::SetFixedSize);
}

void MeteoDataDialog::InitSlots()
{
	connect(m_TopToolBar, SIGNAL(	actionTriggered(QAction*)),this,  SLOT(Slots_ActionTriggered(QAction*)));
}

void MeteoDataDialog::Slots_ActionTriggered( QAction* action )
{
	if (action == m_ActionOpen)
	{
			QString filter =tr("数据文件(*.*)");
			filter +=tr(";;NetCDF数据(*.nc)");
			filter +=tr(";;grib数据(*.grib *.grb2 *.grib2 *.grib1 *.grb1)");
			filter += tr(";;雷达文件 (*.bz2 *.bin)");
			filter += tr(";; 地震文件 (*.meca)");

			
			//filter +=tr(";;grd数据(*.grd)");
			//filter +=tr(";;二进制数据小端(*.ctl)");
			////filter +=tr(";;二进制数据大端(*.ctl)");
			//filter +=tr(";;Micaps数据(*.* *.dat)");
			//filter +=tr(";;M14数据(*.000)");
			//filter +=tr(";;指数数据(*.txt *.tms)");
			//filter +=tr(";;CIPAS数据(*.cps)");
			//filter +=tr(";;CIPAS交互数据(*.xml)");
			//filter +=tr(";;AWX数据（*.awx *.AWX）");
			//filter +=tr(";;站点文本数据(*.txt *.TXT)");
			QString filefilter;
			MainWindow* mw = SysMainWindow::GetMainWindow();
			QString filename = QFileDialog::getOpenFileName(this,tr("选择气候数据"), "/" , filter,&filefilter);

			if (filename.isEmpty())
			{
				return;
			}
			setCursor(Qt::WaitCursor);

			QFileInfo temDir(filename);
			QString fileRex = temDir.suffix();//后缀

			if (tr("bz2").contains(fileRex.toLower()) || tr("bin").contains(fileRex.toLower()) || tr("meca").contains(fileRex.toLower()) || tr("in").contains(fileRex.toLower())  )
			{
				mw->LoadData(filename);
				setCursor(Qt::ArrowCursor);
				return;
			}
			
			OpenFileOrData(filename);
			//mw->GetDefaultToolBar()->GetRecentFilesMenu()->addRecentFile(filename);
			setCursor(Qt::ArrowCursor);
	}
	else if (action == m_ActionDataInfo)
	{

	}
	else if (action == m_ActionDrawData)
	{
		DrawElement();
	}
	else if (action == m_ActionViewData)
	{
		setCursor(Qt::WaitCursor);
		int index = m_CmbVar->currentIndex();
		QString filePath = m_CmbVar->itemData(index).toString();
		if (m_pGridDataDialog)
		{
			delete m_pGridDataDialog;
			m_pGridDataDialog = NULL;
		}
		m_pGridDataDialog = new GridDataDialog(filePath, m_pDPMgr,m_CmbVar->currentText(),m_CmbTime->currentIndex(),m_CmbTime->currentText(),m_CmbLevel->currentIndex(),m_CmbLevel->currentText(), m_EleNames_,this);

		m_pGridDataDialog->move(100, 50);
		//pGridDataDialog->setFixedSize(1200, 600);
		//connect(m_BtnOpenFile, SIGNAL(clicked()), this, SLOT(On_BtnOpenFile()));
		connect(m_pGridDataDialog,SIGNAL(SendStationItems(QStringList)),this,SLOT(DrawFocusStations(QStringList)));
		connect(m_pGridDataDialog,SIGNAL(SendZoomStation(int,float,float)),this,SLOT(ZoomToStation(int,float,float)));
		m_pGridDataDialog->show();
		setCursor(Qt::ArrowCursor);
	}
}

void MeteoDataDialog::DrawElement()
{
	if (m_pFileListWidget->count() == 0)
	{
		return;
	}
	//构造函数初始化

	if(NULL == m_pDPMgr)
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	m_pDPMgr->OpenFilter();
	QListWidgetItem *pTempItem = m_pFileListWidget->currentItem();
	if(NULL == pTempItem)
	{
		pTempItem = m_pFileListWidget->item(m_pFileListWidget->count()-1);
	}
	QString currentfile = pTempItem->data(Qt::UserRole).toString();
	m_HashElement  = m_pDPMgr->GetAllElement(currentfile);

	setCursor(Qt::WaitCursor);
	int index = m_CmbVar->currentIndex();
	QString filePath = m_CmbVar->itemData(index,Qt::UserRole).toString();
	QString strModel = m_CmbDrawType->itemData(m_CmbDrawType->currentIndex(),Qt::UserRole).toString();

	//micaps格式 支持前一时次 后一时次
	if (strModel=="6"
		||strModel=="7"
		||strModel=="8"
		||strModel=="9"
		||strModel=="10"
		||strModel=="11"
		||strModel=="14"
		||strModel=="15"
		||strModel=="16"
		||strModel=="17"
		||strModel=="18"
		||strModel=="19"
		||strModel=="20"
		||strModel=="21")
	{
		/////////////////在此对micaps处理上一个时次，下一个时次 按钮是否可用进行控制进行控制 /////////
		QFileInfo fi(filePath);
		QString filterofMicaps=QString("");
		int i=0;
		while (i++<fi.baseName().length())
		{
			filterofMicaps+=QString("?");
		}
		filterofMicaps+=".";
		i=0;
		while (i++<fi.suffix().length())
		{
			filterofMicaps+=QString("?");
		}
		QStringList filtersl;
		filtersl<<filterofMicaps;
		QDir dir=fi.dir();
		QString dn= dir.path();
		QStringList fileNameList=dir.entryList(filtersl,QDir::Files,QDir::Name);
		if (fileNameList.count()>1)
		{
			int c_index=fileNameList.indexOf(fi.fileName());
		}
		///////////////////////////////////////////////////////////////////////
	}

	if ( strModel == "1"||strModel == "2"||strModel == "3"||strModel == "4"||strModel == "5"||strModel == "12"||strModel == "13"||strModel == "22")
	{
		InitNetCDF2Grads(filePath);
	}
	else if ( strModel == "6"||strModel == "7"||strModel == "8"||strModel == "9"||strModel == "10"||strModel == "11" )
	{
		InitNetCDF2Micaps();

	}
	else if ( strModel == "14"||strModel == "15"||strModel == "16"||strModel == "17" ||strModel == "18"||strModel == "19"||strModel == "20" )
	{
		InitNetCDF2Micaps();
	}
	else if ( strModel == "21")
	{
		InitNetCDF2Micaps();
	}
	
	setCursor(Qt::ArrowCursor);
}

void MeteoDataDialog::InitNetCDF2Micaps()
{
	
	
}

void MeteoDataDialog::OpenFileOrData( QString filename)
{
	QFileInfo temDir(filename);
	QString fileRex = temDir.suffix();//后缀

	if (fileRex.toLower() != "nc"
		&& fileRex.toLower() != "grib"
		&& fileRex.toLower() != "grb1"
		&& fileRex.toLower() != "grib1"
		&& fileRex.toLower() != "grb2"
		//&& fileRex.toLower() != "grd"
		&& fileRex.toLower() != "grib2"
/*		&& fileRex.toLower() != "ctl"
		&& fileRex.toLower() != "dat"
		&& fileRex.toLower() != "tms"
		&& fileRex.toLower() != "cps"
		&& fileRex.toLower() != "tif"
		&& fileRex.toLower() != "tiff"
		&& fileRex.toLower() != "xml"
		&& fileRex.toLower() != "awx"*/)
	{
	
	}

	ClearData();
	//构造函数初始化
	if(NULL == m_pDPMgr)
	{
		setCursor(Qt::ArrowCursor);
		return;
	}

	m_pDPMgr->OpenFilter();
	QStringList listVar = m_pDPMgr->GetAllElementChineseNames(filename);//转译中文名list
	m_HashElement  = m_pDPMgr->GetAllElement(filename);

	if (listVar.isEmpty() && m_HashElement.isEmpty())
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	m_CmbVar->clear();

	for (int i=0; i<listVar.size(); i++)
	{
		QString strVar = listVar.at(i);
		m_CmbVar->addItem(strVar, filename);
		if(m_HashElement.value(strVar) != NULL)
			m_CmbVar->setToolTip(m_HashElement.value(strVar)->LongName());
		m_CmbVar->setItemData(m_CmbVar->count()-1, filename, Qt::UserRole);

		if (m_bFirstOpenFile)//多次打开文件，初始化数据
		{
			On_CmbVar_CurrentIndexChanged(strVar);
			CElement* element1 = m_pDPMgr->GetElementByChineseName(strVar, filename);
			QString fileType = element1->GetFileType();
			CurrentFileType(fileType);
			int index = m_CmbVar->findText(strVar);
			m_CmbVar->setCurrentIndex(index);
		}
		m_bFirstOpenFile = true;
	}

	m_pFileListWidget->clear();

	QStringList sl = m_pDPMgr->GetAllOpenedFiles();//全路径

	foreach(QString fileName, sl)//给listWidget赋值
	{
		if ( fileName.isEmpty() )
		{
			continue;
		}
		QListWidgetItem *item = new QListWidgetItem(m_pFileListWidget);
		item->setData(Qt::UserRole, fileName);
		QString description = m_pDPMgr->GetFileDescription(fileName);
		item->setText(description);
		m_pFileListWidget->addItem(item);
		m_pFileListWidget->setCurrentItem(item);
	}
}

void MeteoDataDialog::ClearData()
{
	m_CmbDrawType->clear();
	m_CmbLevel->clear();
	m_CmbTime->clear();
}

void MeteoDataDialog::On_CmbVar_CurrentIndexChanged( const QString& text )
{
	int index = m_CmbVar->findText(text);
	QString filename = m_CmbVar->itemData(index, Qt::UserRole).toString();
	if ( filename.isEmpty() )
	{
		return;
	}
	if ( !text.isEmpty() )
	{
		ClearData();
		CElement* element1 = m_pDPMgr->GetElementByChineseName(text, filename);
		QString strFileType = element1->GetFileType();

		m_fMissVal = element1->MissingValue();

		QList<int> lst = element1->GetProducts();

		InitDrawType(lst);
		Initata(element1->ActualPeriod().m_hashDate,element1->Level().m_valus);

		/****************屏蔽所有按钮****************/
		//显示数据信息
		m_ActionDataInfo->setEnabled(false);
		//绘制数据
		m_ActionDrawData->setEnabled(false);
		//查看数据列表
		m_ActionViewData->setEnabled(false);

		//剖面图
		m_ActionSectionPlot->setEnabled(false);
		//折线图
		m_ActionDimensionPlot->setEnabled(false);


		/****************数据屏蔽 剖面 折线图绘制****************/
		if (strFileType.contains(QString("MICAPS"))
			|| strFileType.contains(QString("CIPAS"))
			|| strFileType.contains(QString("GRID"))
			|| strFileType.contains(QString("STADA"))
			|| strFileType.contains(QString("AWX"))
			|| strFileType.contains(QString("TXT"))
			|| strFileType.contains(QString("STATIONS")))
		{
			//显示数据信息
			m_ActionDataInfo->setEnabled(true);
			//绘制数据
			m_ActionDrawData->setEnabled(true);
			//查看数据列表
			m_ActionViewData->setEnabled(true);


			if (strFileType == QString("CIPASE") )
			{
				//显示数据信息
				m_ActionDataInfo->setEnabled(false);
				//绘制数据
				m_ActionDrawData->setEnabled(false);
				//查看数据列表
				m_ActionViewData->setEnabled(false);
				//剖面图
				m_ActionSectionPlot->setEnabled(false);
				//折线图
				m_ActionDimensionPlot->setEnabled(true);
			}
			return;
		}

		/****************分析算法结果数据屏蔽****************/
		//if (fileRex.isEmpty()&&path.contains(tr("折线图")))
		if (strFileType.toLower() == "brokenline" )
		{
			//折线图
			m_ActionDimensionPlot->setEnabled(true);
			return;
		}
		//if (path.contains(tr("等值线"))
		//	||path.contains(tr("色斑图"))
		//	||path.contains(tr("格点图")))
		if(strFileType.toLower() == "ncfile" )
		{
			//显示数据信息
			m_ActionDataInfo->setEnabled(true);
			//绘制数据
			m_ActionDrawData->setEnabled(true);
			//查看数据列表
			m_ActionViewData->setEnabled(true);
			return;
		}

		/****************其他默认情况(NetCDF)****************/
		//显示数据信息
		m_ActionDataInfo->setEnabled(true);
		//绘制数据
		m_ActionDrawData->setEnabled(true);
		//查看数据列表
		m_ActionViewData->setEnabled(true);
		//剖面图
		m_ActionSectionPlot->setEnabled(true);
		//折线图
		m_ActionDimensionPlot->setEnabled(true);
	}
}

void MeteoDataDialog::CurrentFileType( QString fileType )
{
	m_CurrentFileType = fileType;
}

QString MeteoDataDialog::CurrentFileType()
{
	return m_CurrentFileType;
}


void MeteoDataDialog::InitDrawType(QList<int> lstProType)
{
	CProductTypeManager* m_ProTypeMgr = CProductTypeManager::GetInstance();
	foreach(int key , lstProType)
	{
		QString data = m_ProTypeMgr->ChineseName(key);
		m_htDrawType.insert(key , data);
	}
}

void MeteoDataDialog::Initata(QHash<long, QDateTime> m_htDate , QHash<int, float > m_htLevel)
{
	//m_CmbDrawType->clear();
	QHash<int, QString>::const_iterator moditDrawType;
	//循环m_htDate得到符合该节点名称的数据结构体初始化对话框
	for( moditDrawType=m_htDrawType.constBegin(); moditDrawType!=m_htDrawType.constEnd(); ++moditDrawType)
	{
		QIcon tempIcon;
		switch (moditDrawType.key())
		{
		case 1:
		case 7:
		case 14:
			tempIcon = QIcon(":/images/Layer_contor.png");//等值线
			break;
		case 2:
			tempIcon = QIcon(":/images/Layer_shaded1.png");//色斑图
			break;
		case 8:
		case 15:
			tempIcon = QIcon(":/images/Layer_shaded2.png");//色斑图
			break;
		case 3:
		case 18:
			tempIcon = QIcon(":/images/Layer_windVector.png");//风矢图
			break;
		case 4:
		case 19:
			tempIcon = QIcon(":/images/Layer_windFeather.png");//风羽图
			break;
		case 5:
		case 20:
			tempIcon = QIcon(":/images/Layer_streamline.png");//流线图
			break;
		case 11:
		case 22:
			tempIcon = QIcon(":/images/Layer_grid.png");//格点图
			break;
		case 23:
			tempIcon = QIcon(":/images/Layer_brokenline.png");//折线图
			break;
		case 6:
		case 9:
		case 10:
			tempIcon = QIcon(":/images/Layer_stations.png");//站点图
			break;
		case 16:
			tempIcon = QIcon(":/images/Layer_surface.png");//地面填图
			break;
		case 17:
			tempIcon = QIcon(":/images/Layer_upperAir.png");//高空填图
			break;
		case 21:
			tempIcon = QIcon(":/images/Layer_typhoonPath.png");//台风路径
			break;
		case 12:
		case 13:
			tempIcon = QIcon(":/images/Layer_bisect.png");//剖面图
			break;
		case 24:
			tempIcon = QIcon(":/images/Layer_shaded2.png");//云图
			break;
		default:
			tempIcon = QIcon(":/images/tabMap.png");
			break;
		}
		this->m_CmbDrawType->addItem(tempIcon,moditDrawType.value(),moditDrawType.key());
	}
	m_CmbDrawType->setCurrentIndex(0);

	//高空填图无法选择元素，不方便选择元素
	if (tr("高空填图") == m_CmbDrawType->currentText())
	{
		if (m_CmbDrawType->count() > 2)
		{
			m_CmbDrawType->setCurrentIndex(1);
		}
	}

	QHash<long, QDateTime>::const_iterator modiDate;
	//循环m_htDate得到符合该节点名称的数据结构体初始化对话框
	//m_CmbTime->clear();
	QStringList strlist;
	for( modiDate=m_htDate.constBegin(); modiDate!=m_htDate.constEnd(); ++modiDate)
	{
		QString qs=modiDate.value().toString("yyyy'-'MM'-'dd'-'hh:mm:ss");
		//防止时间信息重复,add by zhanglei
		if (!strlist.contains(qs))
		{
			strlist.append(qs);
			this->m_CmbTime->addItem(qs,modiDate.key());
		}		
	}

	QHash<int, float>::const_iterator modiLevel;
	//m_CmbLevel->clear();
	//循环m_htLevel得到符合该节点名称的数据结构体初始化对话框
	for( modiLevel=m_htLevel.constBegin(); modiLevel!=m_htLevel.constEnd(); ++modiLevel)
	{
		this->m_CmbLevel->addItem(QString::number(modiLevel.value()),modiLevel.key());
	}
}

void MeteoDataDialog::InitNetCDF2Grads( QString filePath )
{
	CElement* element1 = NULL;
	element1 = m_pDPMgr->GetElementByChineseName(m_CmbVar->currentText(), filePath);// m_pDPMgr->GetElementByName(m_CmbVar->currentText());
	if(element1 == NULL)
	{
		return;
	}

	element1->SelectedLon().m_count = element1->ActualLon().m_count;
	element1->SelectedLat().m_count = element1->ActualLat().m_count;

	element1->SelectedLon().m_begin = element1->ActualLon().m_begin;
	element1->SelectedLat().m_begin = element1->ActualLat().m_begin;

	element1->SelectedLon().m_end = element1->ActualLon().m_end;
	element1->SelectedLat().m_end = element1->ActualLat().m_end;

	element1->SelectedPeriod().m_begin = QDateTime::fromString(m_CmbTime->currentText(),"yyyy'-'MM'-'dd'-'hh:mm:ss");
	element1->SelectedPeriod().m_end = QDateTime::fromString(m_CmbTime->currentText(),"yyyy'-'MM'-'dd'-'hh:mm:ss");

	element1->SelectedPeriod().m_hashDate.clear();
	element1->SelectedPeriod().m_hashDate.insert(m_CmbTime->currentIndex(), element1->SelectedPeriod().m_begin);
	element1->SelectedPeriod().m_count = 1;

	element1->Level().m_selected.clear();
	element1->Level().m_selected.insert(m_CmbLevel->currentIndex(),m_CmbLevel->currentText().toFloat());

	if(!(m_pDPMgr->ReadDataFromFile(element1)))
	{
		//CDiskFileLog::WriteLog(tr("警告"), tr("MeteoDataDialog::InitNetCDF2Grads 读元素文件数据失败！"));//写日志--renxue
		setCursor(Qt::ArrowCursor);
		return;
	}

	size_t iLonNCCount = element1->ActualLon().m_count; 
	size_t iLatNCCount = element1->ActualLat().m_count;
	size_t iTimeNCCount = 1; 
	size_t iLevelNCCount = 1;
	float fLonBegin = element1->ActualLon().m_begin;
	float fLonEnd = element1->ActualLon().m_end;

	float fLonDelta = 1;
	if (element1->ActualLon().m_count == 1)
	{
		fLonDelta = 1;
	}
	else
	{
		fLonDelta = (element1->ActualLon().m_end - element1->ActualLon().m_begin)/(element1->ActualLon().m_count-1);
	}
	float fLatBegin = element1->ActualLat().m_begin;
	float fLatEnd = element1->ActualLat().m_end;
	float fLatDelta = 1;
	if (element1->ActualLat().m_count == 1)
	{
		fLatDelta = 1;
	}
	else
	{
		fLatDelta = (element1->ActualLat().m_end - element1->ActualLat().m_begin)/(element1->ActualLat().m_count-1);
	}

	float fMissingValue = element1->MissingValue();
	float fLevelBegin = 0;
	float fLevelEnd = 0;
	float fLevelDelta = 0;

	std::vector<float> fLevelVector;
	//fLevelVector.push_back(m_CmbLevel->currentText().toFloat());

	std::string strTimeBegin = /*QDateTime::fromString(*/m_CmbTime->currentText().toStdString();//,"yyyy'-'MM'-'dd'-'hh:mm:ss");
	std::string strTimeEnd = /*QDateTime::fromString(*/m_CmbTime->currentText().toStdString();//,"yyyy'-'MM'-'dd'-'hh:mm:ss");
	int iTimeDelta = 1;
	int fDataSize = element1->Data().GetBasicData(0).TotalSize();
	float* fData = element1->GetDataArray(0);
	double* dData = new double[fDataSize]; 
	for (int i = 0 ;i< fDataSize ; i++)
	{
		dData[i] =fData[i];
	}
	
	//typedef goto_gis::DataProvider* (*DataProviderPluginMain)();
	//goto_gis::DataProvider* radarDProvider = 0;
	//QLibrary lib("GradsProvider.dll");
	goto_gis::GeosOnTheOsg* pGeosOnTheosg = goto_gis::GeosOnTheOsg::Instantiate();
	goto_gis::VectorLayer* pLayer = new goto_gis::VectorLayer;
	pGeosOnTheosg->OpenDataAsLayer("display.grads",pLayer);
	DataInterface* pDI = pDI = (DataInterface*)(pLayer->GetDataProvider()->GetExtendInterface(0));
	if(pLayer && pDI)
	{
		//DataProviderPluginMain proMain = (DataProviderPluginMain)lib.resolve("DataProviderPluginMain");
		//radarDProvider = proMain();
		//初始化图层给数据提供器
		pLayer->LayerZ(-30000.0);
		pLayer->FontSize(18);
		int nModel = m_CmbDrawType->itemData(m_CmbDrawType->currentIndex(),Qt::UserRole).toInt();
		//0 等值线
		//1 色版图
		QHash<int,int> TypeHash;
		TypeHash.insert(1,0);
		TypeHash.insert(2,1);
		//TypeHash.insert(1,0); ProductTypeManager.cpp

		pDI->DataInput(iLonNCCount, iLatNCCount, iTimeNCCount, iLevelNCCount, fLonBegin, fLonEnd, fLonDelta, fLatBegin, fLatEnd, fLatDelta, fMissingValue, fLevelBegin, fLevelEnd, fLevelDelta,  fLevelVector, strTimeBegin, strTimeEnd, iTimeDelta, dData, 1, 0);
		int iType = TypeHash[nModel];
		pDI->Draw(iType);

		//添加图层
		MainWindow* mw = SysMainWindow::GetMainWindow();
		ViewManager* pViewManager = mw->GetViewManger();

		QWidget* viewerWindow  = 0;
		Map* pExtMap = 0;

		viewerWindow = mw->FromWidgetToQGLW();
		if (viewerWindow == 0)
		{
			viewerWindow = pViewManager->CreateViewport(NoData);
			pExtMap =  (ExtendMap*)viewerWindow->property("Map").toLongLong();
			pExtMap->CreateMapData();
			mw->CreateMDISubWindow(viewerWindow);
			viewerWindow->showMaximized();
		}
		pExtMap =  (ExtendMap*)viewerWindow->property("Map").toLongLong();
		pLayer->CoordTrans(pExtMap->GetCoordinateTransform());
		std::string treeTitle = strTimeBegin+"-"+strTimeEnd+"_"+m_CmbDrawType->currentText().toStdString();
		pExtMap->AddLayer(treeTitle, pLayer);
		pLayer->CreateLayerData();

		mw->GetCentralMdiArea()->Slot_SubWindowActivated(mw->GetCentralMdiArea()->activeSubWindow());

		delete dData;
		dData = 0;
	}
	
}

