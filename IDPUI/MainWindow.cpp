#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtGui>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtOpenGL/QGLWidget>
#include <QDate>
#include "QtCore/QTimer"
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>

#include <.\QxtGlobalShortcut\qxtglobalshortcut.h>
#include "IDPApplication.h"
#include "GeosOnTheOsg.h"
#include "Proj4Transform.h"
#include "DataProvider.h"
#include "Layer.h"
#include "ViewManager.h"
#include "AdapterWidget.h"
#include "DefaultManipulator.h"
//#include "idpEarthManipulator.h"
#include "MyActionInterface.h"
#include "RadarGraphicsView.h"


//#include "DataProvider.h"
//#include "RadarVectorLayer.h"
//#include "GDALProvider.h"
//#include "VectorLayer.h"
//#include "GISMapManager.h"
//#include "OsgEarthBridge.h"
//#include "OsgEarthTransform.h"
//#include "LayerTreeControler.h"
//#include "EarthquakeFileDisplay.h"
#include "DataDockWidget.h"
#include "RadarControlDockWidget.h"

#include "CentralMdiArea.h"
#include "DefaultToolBar.h"
#include "WinToolBar.h"
#include "CZToolBar.h"
#include "RadarToolBar.h"
#include "MapToolBar.h"
#include "CommonToolBar.h"
#include "statustoolbar.h"
#include "RadarDataAccess.h"
#include "AnimaticControler.h"
#include "QRecentFilesMenu.h"
#include "MeteoDataDialog.h"
#include "sysmainwindow.h"
#include "CipasComposer.h"
//#include "ConfigDialog.h"
#include "IDPAbout.h"
#include "mainwindow.h"

using namespace goto_gis;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags): QMainWindow(parent, flags),/* m_pControlImp(new IDPUIControlImp),*/m_MenuActionGroup(0)
{
	GeosOnTheOsg::Instantiate()->LoadProviderPlugins("PrividerConfig.txt");
	GeosOnTheOsg::Instantiate()->LoadGisExtendPlugins("GisExtendPlugins");

	ui.setupUi(this);
	setAcceptDrops(true);
	ReadActionXML(IDPApplication::ConfigPath()+"/m_radar_action_config.xml");
	uint wid = (uint)winId();
	setMinimumSize(600,400);
	SysMainWindow::SetMainWindow(wid);
	m_wStyle = TabBar;

	//m_pStackedWidget = NULL;
	m_pCentralMdiArea = NULL;
	m_pTabToolBar = NULL;
	m_pTabWidget = NULL;

	m_pDataDockWidget = NULL;
	m_pRadarControlDockWidget = NULL;

	m_pDefaultToolBar = NULL;
	m_pWinToolBar = NULL;
	m_pRadarToolBar = NULL;
	m_pMapToolBar = NULL;
	m_pCZToolBar = NULL;
	m_pCommonToolBar = NULL;

	m_fullScreenParent = NULL;
	m_bF = true;
	m_bFullScreen = false;
	iWindowID = 0;
	InitConfig();

	InitMainWindow();
	initStatusToolBar();
	InitMap();
	m_MenuActionGroup.setExclusive(false);//
	m_MenuActionGroup.addAction(ui.actionLeftDockW);
	m_MenuActionGroup.addAction(ui.actionArrt);
	m_MenuActionGroup.addAction(ui.actionData);
	m_MenuActionGroup.addAction(ui.actionViewR);

	m_MenuActionGroup.addAction(ui.actionCascad);
	m_MenuActionGroup.addAction(ui.actionTile);
	m_MenuActionGroup.addAction(ui.actionOutMID);
	m_MenuActionGroup.addAction(ui.actionInMID);
	m_MenuActionGroup.addAction(ui.actionCloseAll);
	m_MenuActionGroup.addAction(ui.actionClose);
	m_MenuActionGroup.addAction(ui.actionFullScreen);
	m_MenuActionGroup.addAction(ui.actionAbout);
	m_MenuActionGroup.addAction(ui.actionConfig);
	m_MenuActionGroup.addAction(ui.actionHelp);
	m_MenuActionGroup.addAction(ui.actionThematic);
	m_MenuActionGroup.addAction(ui.actionCommonToolBar);
	m_MenuActionGroup.addAction(ui.actionDisplaySettings);
	m_MenuActionGroup.addAction(ui.actionMapSettings);
	m_MenuActionGroup.addAction(ui.m_pActionL);
	m_MenuActionGroup.addAction(ui.m_pActionM);
	m_MenuActionGroup.addAction(ui.m_pActionLonLat);
	m_MenuActionGroup.addAction(ui.m_pActionGrid);
	m_MenuActionGroup.addAction(ui.m_ActionProjCenter);
	m_MenuActionGroup.addAction(ui.action_RadarControl);
	m_MenuActionGroup.addAction(ui.actionWin4);
	m_MenuActionGroup.addAction(ui.action2D);
	m_MenuActionGroup.addAction(ui.action3D);
	m_MenuActionGroup.addAction(ui.action_VTK);
	m_MenuActionGroup.addAction(ui.actionArea);
	m_MenuActionGroup.addAction(ui.actionAnimatic);
	m_MenuActionGroup.addAction(ui.actionRadarData);
	//////////////////////////////////////////////////////////////////////////
	//m_SysAllActionGroup.setExclusive(false);//
	m_SysAllActionList.push_back(ui.actionLeftDockW);
	m_SysAllActionList.push_back(ui.actionArrt);
	m_SysAllActionList.push_back(ui.actionData);
	m_SysAllActionList.push_back(ui.actionViewR);

	m_SysAllActionList.push_back(ui.actionCascad);
	m_SysAllActionList.push_back(ui.actionTile);
	m_SysAllActionList.push_back(ui.actionOutMID);
	m_SysAllActionList.push_back(ui.actionInMID);
	m_SysAllActionList.push_back(ui.actionCloseAll);
	m_SysAllActionList.push_back(ui.actionClose);
	m_SysAllActionList.push_back(ui.actionFullScreen);
	m_SysAllActionList.push_back(ui.actionAbout);
	m_SysAllActionList.push_back(ui.actionConfig);
	m_SysAllActionList.push_back(ui.actionHelp);
	m_SysAllActionList.push_back(ui.actionThematic);
	m_SysAllActionList.push_back(ui.actionCommonToolBar);
	m_SysAllActionList.push_back(ui.actionDisplaySettings);
	m_SysAllActionList.push_back(ui.actionMapSettings);
	m_SysAllActionList.push_back(ui.m_pActionL);
	m_SysAllActionList.push_back(ui.m_pActionM);
	m_SysAllActionList.push_back(ui.m_pActionLonLat);
	m_SysAllActionList.push_back(ui.m_pActionGrid);
	m_SysAllActionList.push_back(ui.m_ActionProjCenter);
	m_SysAllActionList.push_back(ui.action_RadarControl);
	m_SysAllActionList.push_back(ui.actionWin4);
	m_SysAllActionList.push_back(ui.action2D);
	m_SysAllActionList.push_back(ui.action3D);
	m_SysAllActionList.push_back(ui.action_VTK);
	m_SysAllActionList.push_back(ui.actionArea);
	m_SysAllActionList.push_back(ui.actionAnimatic);
	m_SysAllActionList.push_back(ui.actionRadarData);

	//////////////////////////////////////////////////////////////////////////
	ui.actionViewR->setCheckable(false);
	ui.actionViewR->setChecked(false);
	QString skinPath = QCoreApplication::applicationDirPath()+ "/Skin/default.qss";
	SetSystemSkin(skinPath);
	testTimer = new QTimer(this);
	//����ʱ����ʱ�ź����(���ܺ���)��ϵ����
	connect( testTimer, SIGNAL(timeout()), this, SLOT(On_UpdateDateStatusBar()) );
	//��ʼ���ж�ʱ������ʱʱ����Ϊ1000ms
	testTimer->start(100);
	InitSlots();


	RadarControlDockWidgetGroupEnd(false);
}



bool MainWindow::ReadActionXML(const QString filename)
{
	//GeosOnTheOsg* pGeosOsg = GeosOnTheOsg::Instantiate();
	QFile file(filename);
	if(file.open(QFile::ReadOnly))
	{
		QDomDocument XmlReader;
		XmlReader.setContent(&file);

		QDomNodeList nodeList = XmlReader.elementsByTagName(tr("data_Info"));
		for(int i = 0 ; i < nodeList.count(); i++)
		{
			QDomNode domNode = nodeList.at(i);
			QDomElement Info_element =  domNode.toElement();

			QString product_code = Info_element.attribute("product_code");

			QMap<QString , QString> actionMap;
			QString action_move_zoom = Info_element.attribute("action_move_zoom");
			actionMap.insert("action_move_zoom",action_move_zoom);
			QString action_layer_up_down = Info_element.attribute("action_layer_up_down");
			actionMap.insert("action_layer_up_down",action_layer_up_down);
			QString actionWin4 = Info_element.attribute("actionWin4");
			actionMap.insert("actionWin4",actionWin4);
			QString action_commen_tool = Info_element.attribute("action_commen_tool");
			actionMap.insert("action_commen_tool",action_commen_tool);
			QString actionViewR = Info_element.attribute("actionViewR");
			actionMap.insert("actionViewR",actionViewR);
			QString action_color_table_filter = Info_element.attribute("action_color_table_filter");
			actionMap.insert("action_color_table_filter",action_color_table_filter);
			QString action_realtime_position = Info_element.attribute("action_realtime_position");
			actionMap.insert("action_realtime_position",action_realtime_position);
			QString action_realtime_value = Info_element.attribute("action_realtime_value");
			actionMap.insert("action_realtime_value",action_realtime_value);
			QString actionArrt = Info_element.attribute("actionArrt");
			actionMap.insert("actionArrt",actionArrt);
			QString action3D = Info_element.attribute("action3D");
			actionMap.insert("action3D",action3D);
			QString action_VTK = Info_element.attribute("action_VTK");
			actionMap.insert("action_VTK",action_VTK);
			QString actionDB2 = Info_element.attribute("actionDB2");
			actionMap.insert("actionDB2",actionDB2);
			QString actionClipPlane = Info_element.attribute("actionClipPlane");
			actionMap.insert("actionClipPlane",actionClipPlane);
			QString actionAnimatic = Info_element.attribute("actionAnimatic");
			actionMap.insert("actionAnimatic",actionAnimatic);
			QString action_3d_animate = Info_element.attribute("action_3d_animate");
			actionMap.insert("action_3d_animate",action_3d_animate);
			QString actionThematic = Info_element.attribute("actionThematic");
			actionMap.insert("actionThematic",actionThematic);
			QString action_base_layer = Info_element.attribute("action_base_layer");
			actionMap.insert("action_base_layer",action_base_layer);
			QString action_layer_projection = Info_element.attribute("action_layer_projection");
			actionMap.insert("action_layer_projection",action_layer_projection);
			QString action_layer_scroll = Info_element.attribute("action_layer_scroll");
			actionMap.insert("action_layer_scroll",action_layer_scroll);
			QString actionArea = Info_element.attribute("actionArea");
			actionMap.insert("actionArea",actionArea);
			QString m_pActionGrid = Info_element.attribute("m_pActionGrid");
			actionMap.insert("m_pActionGrid",m_pActionGrid);
			m_actionInfoMap.insert(product_code,actionMap);
		}
	}
	return true;
}

MainWindow::~MainWindow()
{

}

void MainWindow::SystemStyle(WinStyle wstyle)
{
	m_wStyle = wstyle;
}

WinStyle MainWindow::SystemStyle()
{
	return m_wStyle;
}

//��ʼ����ʷ�˵�
void MainWindow::InitRecentFilesMenu()
{
	m_pRecentFilesMenu = new QRecentFilesMenu(tr("��ʷ�ļ���¼"), ui.fileMenu);
	m_pRecentFilesMenu->loadRecent("IDP_RecentFiles.txt");

	QList<QAction*> actions = m_pDefaultToolBar->actions();
	foreach(QAction* action , actions)
	{
		if (QString::fromLocal8Bit("��") == action->text())
		{
			ui.fileMenu->addAction(action);
			ui.fileMenu->addSeparator();
			ui.fileMenu->addMenu(m_pRecentFilesMenu);
			ui.fileMenu->addSeparator();
		}
		else if (QString::fromLocal8Bit("�˳�") == action->text())
		{
			ui.fileMenu->addAction(action);
		}
	}
	connect(m_pRecentFilesMenu, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(LoadData(const QString &)));
}


/** 
  * @brief    ��ʼ��������   
  * @author   �׼���  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   MainWindow  ��ʼ��������
  * @note     �׼���
  * @date     2013��3��                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void MainWindow::InitMainWindow()
{
	//////////////////////////�Ϲ�����////////////////////////////////////////////////
	m_pViewManger = new ViewManager();				//�����һ��
	m_pRadarToolBar = new RadarToolBar(this);			//װ��IDP�в��������Ա�����DefaultToolBar ǰ ����������������Ҳ����DefaultToolBarǰ
	m_pRadarToolBar->setVisible(false);
	m_pDefaultToolBar = new DefaultToolBar(this);
	m_pWinToolBar = new WinToolBar(this);
	m_pWinToolBar->setVisible(false);
	m_pMapToolBar = new MapToolBar(this);
	m_pMapToolBar->setVisible(false);
	m_pCZToolBar = new CZToolBar(this);
	m_pCZToolBar->setVisible(false);

	m_pCommonToolBar = new CommonToolBar(this);
	if (m_wStyle == TabBar)
	{
		m_pTabToolBar = new QToolBar(this);
		m_pTabWidget = new QTabWidget(m_pTabToolBar);
		m_pTabToolBar->addWidget(m_pTabWidget);

		m_pTabWidget->addTab(m_pDefaultToolBar,QIcon(":/images/logo.png"),tr("�ļ�"));
		m_pDefaultToolBar->setProperty("title",tr("�ļ�"));

		m_pTabWidget->addTab(m_pRadarToolBar,QIcon(":/images/logo.png"),tr("�״�"));
		m_pRadarToolBar->setProperty("title",tr("�״�"));

		m_pTabWidget->addTab(m_pCZToolBar,QIcon(":/images/logo.png"),tr("����"));
		m_pCZToolBar->setProperty("title",tr("����"));

		m_pTabWidget->addTab(m_pMapToolBar,QIcon(":/images/logo.png"),tr("��ͼ"));
		m_pMapToolBar->setProperty("title",tr("��ͼ"));

		m_pTabWidget->addTab(m_pWinToolBar,QIcon(":/images/logo.png"),tr("����"));
		m_pWinToolBar->setProperty("title",tr("����"));

		m_pTabWidget->addTab(m_pCommonToolBar,QIcon(":/images/logo.png"),tr("ͨ��"));
		m_pCommonToolBar->setProperty("title","ͨ��");

		this->addToolBar(m_pTabToolBar);
	}
	this->setProperty("MapToolBar",(long long)m_pMapToolBar);
	this->addToolBar(m_pDefaultToolBar);
	this->setProperty("CommonToolBar",(long long)m_pCommonToolBar);
	this->addToolBar(m_pCommonToolBar);
	//this->AddIDPToolBar(NoData);
	InitRecentFilesMenu();

	m_pMeteoDataDialog = NULL;
	m_pMeteoDataDialog = new MeteoDataDialog(SysMainWindow::GetMainWindow());
	m_pMeteoDataDialog->hide();

	//////////////////////////////���////////////////////////////////////////////
	//m_pLeftDockWidget = new LeftDockWidget(this);
	//m_pLeftDockWidget->setProperty("GISMapManager", (long long)m_pViewManger->GetGISMapManager());

	//m_pLeftDockWidget->setMaximumWidth(288);
	//this->addDockWidget(Qt::LeftDockWidgetArea,m_pLeftDockWidget);
	//m_pLeftDockWidget->hide();
	/*m_pAttributeDockWidget = new AttributeDockWidget(this);
	this->addDockWidget(Qt::LeftDockWidgetArea,m_pAttributeDockWidget);*/
	//m_pAttributeDockWidget->hide();
	//m_pAttributeDockWidget->setMaximumWidth(288);

	m_pDataDockWidget = new DataDockWidget(this);
	this->addDockWidget(Qt::LeftDockWidgetArea,m_pDataDockWidget);
	//m_pDataDockWidget->setMaximumWidth(288);

	m_pRadarControlDockWidget = new RadarControlDockWidget(this);
	this->addDockWidget(Qt::LeftDockWidgetArea,m_pRadarControlDockWidget);

	//this->tabifyDockWidget(m_pLeftDockWidget,m_pAttributeDockWidget);
	//this->tabifyDockWidget(m_pAttributeDockWidget,m_pDataDockWidget);
	this->tabifyDockWidget(m_pDataDockWidget,m_pRadarControlDockWidget);

	//m_pDataDockWidget->hide();
	//m_pLeftDockWidget->raise();
	m_pCentralMdiArea = new CentralMdiArea(this);
	m_pCentralMdiArea->setObjectName("CentralMdiArea");
	this->setCentralWidget(m_pCentralMdiArea);
	this->setContextMenuPolicy(Qt::NoContextMenu);

	//m_pLeftDockWidget->setProperty("Action",(long long)ui.actionLeftDockW);
	m_pCommonToolBar->setProperty("Action",(long long)ui.actionCommonToolBar);
	//m_pAttributeDockWidget->setProperty("Action",(long long)ui.actionArrt);
	m_pDataDockWidget->setProperty("Action",(long long)ui.actionRadarData);

	m_pRadarControlDockWidget->setProperty("Action",(long long)ui.action_RadarControl);

	setProperty("Action",(long long)ui.actionViewR);

}

/************************************************************************
 ����˵������ʼ����״̬����
 ��    �ߣ��׼���
 ������ڣ�2013��05��2��
************************************************************************/
void MainWindow::initStatusToolBar()
{
	m_pStatusBar = this->statusBar();
	m_pStatusToolBar = new StatusToolBar(m_pStatusBar);
	m_pStatusBar->addPermanentWidget(m_pStatusToolBar);

	m_pPosition = new QLabel(m_pStatusBar);
	m_pPosition->setObjectName("m_pPosition");
	m_pPosition->setFrameShape(QFrame::Panel); //���ñ�ǩ��״
	m_pPosition->setFrameShadow(QFrame::Sunken); //���ñ�ǩ��Ӱ
	m_pPosition->setText(tr("��ǰλ�ã�      "));

	m_pValue = new QLabel(m_pStatusBar);
	m_pValue->setObjectName("m_pValue");
	m_pValue->setFrameShape(QFrame::Panel); //���ñ�ǩ��״
	m_pValue->setFrameShadow(QFrame::Sunken); //���ñ�ǩ��Ӱ
	m_pValue->setText(tr("��ǰ��ֵ��      "));

	m_pProjection = new QLabel(m_pStatusBar);
	m_pProjection->setObjectName("m_pProjection");
	m_pProjection->setFrameShape(QFrame::Panel); //���ñ�ǩ��״
	m_pProjection->setFrameShadow(QFrame::Sunken); //���ñ�ǩ��Ӱ
	m_pProjection->setText(tr("��ǰͶӰ��      "));

	m_pProjCenter = new QLabel(m_pStatusBar);
	m_pProjCenter->setObjectName("m_pProjCenter");
	m_pProjCenter->setFrameShape(QFrame::Panel); //���ñ�ǩ��״
	m_pProjCenter->setFrameShadow(QFrame::Sunken); //���ñ�ǩ��Ӱ
	m_pProjCenter->setText(tr("��ǰͶӰ���ģ�      "));

	m_pStatusBar->addWidget(m_pPosition,1);	//λ��
	m_pStatusBar->addWidget(m_pValue,2);	//λ��
	m_pStatusBar->addWidget(m_pProjection,3);	//λ��
	m_pStatusBar->addWidget(m_pProjCenter,4);	//λ��

}

void MainWindow::InitMap()
{
	m_pViewManger->Init2DBasicNode(QCoreApplication::applicationDirPath()+tr("/Config/Default.prj"));
	m_pViewManger->Init3DBasicNode(QCoreApplication::applicationDirPath() + tr("/Config/Default.earth"));
}

void MainWindow::InitConfig()
{
	QString iniPath = QCoreApplication::applicationDirPath()+tr("/Config/Config.ini");
	QSettings *configIni = new QSettings(iniPath, QSettings::IniFormat); 
	QString nDefaultMap = configIni->value("/DefaultMode/DefaultMap").toInt();
	this->setProperty("nDefaultMap",nDefaultMap);
	
	QString FullScreen = configIni->value("/DefaultMode/FullScreen").toString();
	QString NoFullScreen = configIni->value("/DefaultMode/NoFullScreen").toString();
	bool bISFullScreen = configIni->value("/DefaultMode/ISFullScreen").toBool();

	this->setProperty("bISFullScreen",bISFullScreen);

	QxtGlobalShortcut * scFullScreen = new QxtGlobalShortcut(QKeySequence(FullScreen), this);
	QxtGlobalShortcut * scEscFullScreen = new QxtGlobalShortcut(QKeySequence(NoFullScreen), this);

	connect(scFullScreen, SIGNAL(activated()),this, SLOT(Slot_FullScreen()));
	connect(scEscFullScreen, SIGNAL(activated()),this, SLOT(Slot_EscFullScreen()));

	delete configIni;
	configIni = NULL;
}


void MainWindow::InitSlots()
{
	connect(&m_MenuActionGroup, SIGNAL(triggered(QAction*)),this,  SLOT(Slot_ActionTriggered(QAction*)));
}

//////////////////////////////////////////////////////////////////////////
//ȫ�����˳�ȫ��
void MainWindow::Slot_FullScreen()
{
	try
	{
		if (m_pCentralMdiArea)
		{
			//if (!m_bFullScreen)
			{
				QMdiSubWindow* currentSubW = m_pCentralMdiArea->activeSubWindow();
				if(!currentSubW) return;
				QWidget *currentW = currentSubW->widget();
				if (currentW)
				{
					currentSubW->layout()->removeWidget(currentW);
					currentSubW->setWidget(0);
					currentSubW->hide();


					currentW->setParent(this);
					currentW->setWindowFlags(Qt::Window);
					currentW->showFullScreen();
					currentW->setProperty("FullScreen",true);
				}
	
				//if(currentW && !currentW->property("FullScreen").toBool())
				//{
				//	if (property("bISFullScreen").toBool())
				//	{
				//		currentW->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
				//		currentW->showFullScreen();
				//	}
				//	else
				//	{
				//		currentW->setWindowFlags(Qt::Window);
				//		currentW->showMaximized();
				//	}
				//	currentW->setProperty("FullScreen",true);
				//}
				
			}
			
		}
		
	}
	
	catch (...)
	{
	}
	
}

void MainWindow::Slot_EscFullScreen()
{
	try
	{
		if (m_pCentralMdiArea)
		{
			QWidget* dw = m_pCentralMdiArea->CurrentWidget();// (QWidget*)subW->property("Widget").toLongLong();
			if (dw)
			{
				QMdiSubWindow* subW = (QMdiSubWindow*)dw->property("ParentWidget").toLongLong();
				if (subW)
				{
					dw->setWindowFlags(Qt::Widget);
					subW->setWidget(dw);
					dw->showMaximized();
					dw->setProperty("FullScreen",false);
				}
			}

			//QMdiSubWindow* pSubWin = m_pCentralMdiArea->activeSubWindow();
			//if(!pSubWin) return;
			//QWidget *activew = pSubWin->widget();

			//if(activew && activew->property("FullScreen").toBool() )
			//{
			//	activew->setWindowFlags(Qt::Widget);
			//	activew->showMaximized();
			//	activew->setProperty("FullScreen",false);
			//}
		}
	}
	
	catch (...)
	{

	}
	
}

//////////////////////////////////////////////////////////////////////////
//������ʾ/���ز���
void MainWindow::Slot_ActionTriggered( QAction* action )
{
	bool visible;
	QList<QMdiSubWindow *> subWinList = m_pCentralMdiArea->subWindowList();
	MyActionInterface* pMyAction = dynamic_cast<MyActionInterface*>(action);
	if(pMyAction) pMyAction->MyAction();
	if (subWinList.count() > 0)
	{
		if (action == ui.actionThematic)//ר��ͼ���� OK
		{
			QWidget* viewerWindow = FromWidgetToQGLW();
			int wdt = viewerWindow->property("DType").toInt();
			if (wdt != RadarData)
			{
				QMessageBox::information(this,"��ʾ","��ȡ�ӿ�����ʧ�ܣ�");
				return;
			}
			ViewerQT* vW = dynamic_cast<ViewerQT*>(viewerWindow);
			if (vW)
			{
				m_pCipasComposer = new CCipasComposer(this);
				m_pCipasComposer->setOsgView(vW);
				m_pCipasComposer->setWindowModality(Qt::ApplicationModal);

				m_pCipasComposer->setExtent(0,-90,180,90);
				//m_pCipasComposer->setExtent(x_min,y_mim,x_max,y_max);

				m_pCipasComposer->statusBar()->showMessage(tr("����"));
				QString templateString =IDPApplication::TemplatePath()+ "/ThemeTemplate/ServerGrid.cpt";
				m_pCipasComposer->LoadTemplatefromfile(templateString);
				m_pCipasComposer->open();
				m_pCipasComposer->showMaximized();
			}
		}
		else if (action  == ui.actionOutMID)//���ڸ��� OK
		{
				/*QMdiSubWindow *currentSubW = m_pCentralMdiArea->activeSubWindow();
				if (currentSubW)
				{
					QWidget* currentW = currentSubW->widget();

					if (currentW)
					{
						currentSubW->layout()->removeWidget(currentW);
						currentSubW->setWidget(0);
						currentSubW->hide();

				
						currentW->setParent(this);
						currentW->setWindowFlags(Qt::Window);
						currentW->showMaximized();
					}
				}*/
			}
			else if (action == ui.actionInMID)//�������� OK
			{
				//QMdiSubWindow* subW = m_pCentralMdiArea->activeSubWindow();
				//QWidget* dw = m_pCentralMdiArea->CurrentWidget();// (QWidget*)subW->property("Widget").toLongLong();
				//if (dw)
				//{
				//	QMdiSubWindow* subW = (QMdiSubWindow*)dw->property("ParentWidget").toLongLong();
				//	if (subW)
				//	{
				//		subW->setWidget(dw);
				//		dw->showMaximized();
				//	}
				//}
				//else
				//{
				//	QMessageBox::information(this,tr("��ʾ"),tr("������ڴ������������У��Ǳ�������"));
				//}
			}
			//              ������������                 �״��ά��ʾ                 �״���ά��ʾ               ��ά���з���                           ���ͳ��
			else if ((action == ui.actionWin4) || (action == ui.action2D) || (action == ui.action3D) || (action == ui.action_VTK)  || (action == ui.actionArea)/* || (action == ui.actionAnimatic)*/)
			{
				if (m_pRadarToolBar)
				{
					m_pRadarToolBar->ActionTrigger(action);
				}
			}
	}
	//�״ﶯ��
	if(action == ui.actionAnimatic)
	{
		if (m_pRadarToolBar)
		{
			m_pRadarToolBar->ActionTrigger(action);
		}
	}
	//ͼ�����
	if (action == ui.actionLeftDockW)
	{
		/*if (m_pLeftDockWidget)
		{
			m_pLeftDockWidget->setVisible(!m_pLeftDockWidget->isVisible());
		}*/
	}
	//�״�����ݴ���
	else if (action == ui.actionRadarData)
	{
		m_pDataDockWidget->setVisible(!m_pDataDockWidget->isVisible());
	}
	//ͨ�ù�����
	else if (action == ui.actionCommonToolBar)
	{
		if (m_pCommonToolBar)
		{
			visible = !m_pCommonToolBar->isVisible();
			m_pCommonToolBar->setVisible(visible);
		}
	}
	//ͼ�����
	/*else if (action == ui.actionArrt)
	{
	if (m_pAttributeDockWidget)
	{
	m_pAttributeDockWidget->setVisible(!m_pAttributeDockWidget->isVisible());
	}
	}*/
	//�����б�
	else if (action == ui.actionData)
	{
		if (m_pDataDockWidget)
		{
			m_pDataDockWidget->setVisible(!m_pDataDockWidget->isVisible());

		}

	}
	//�״�����ӿ�
	else if (action == ui.action_RadarControl)
	{
		if (m_pRadarControlDockWidget)
		{
			m_pRadarControlDockWidget->setVisible(!m_pRadarControlDockWidget->isVisible());

		}

	}

	//�״��Ҳ������ӿ�
	else if (action == ui.actionViewR)
	{
			QWidget* bw = FromWidgetToQGLW();
			if (bw)
			{
				int wdt = bw->property("DType").toInt();
				if (wdt == RadarData)
				{
					RadarGraphicsView* gv = bw->findChild<RadarGraphicsView*>();
					if (gv)
					{
						gv->setVisible(!gv->isVisible());
						//action->setCheckable(!action->isCheckable());
						//action->setChecked(!action->isChecked());
					}
				}
			}
	}
	//����ȫ��
	else if (action == ui.actionFullScreen)
	{
		this->Slot_FullScreen();
	}
	//���ڲ��
	else if (action == ui.actionCascad)
	{
		/*QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(this->centralWidget());
		if (mdiArea)
		{
			foreach(QMdiSubWindow* subWin , mdiArea->subWindowList())
			{
				subWin->showNormal();
			}
			mdiArea->cascadeSubWindows();
		}*/
	}
	//����ƽ��
	else if (action == ui.actionTile)
	{
		/*QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(this->centralWidget());
		if (mdiArea)
		{
			foreach(QMdiSubWindow* subWin , mdiArea->subWindowList())
			{
				subWin->showNormal();
			}
			mdiArea->tileSubWindows();
		}*/

	}
	//������ͶӰ OK
	else if (action == ui.m_pActionL)
	{
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
		QSettings setting2(IDPApplication::ConfigPath()+"ProjectionType.ini", QSettings::IniFormat);
		setting2.setIniCodec(QTextCodec::codecForName("UTF-8"));	
		if (viewerWindow)
		{
			int wdt = viewerWindow->property("DType").toInt();
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();;
			if (pMap == NULL)
			{
				//QMessageBox::information(this,"��ʾ","��ȡ�ӿ�Mapʧ�ܣ�");
				return;
			}
			SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
			pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
			viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);

			m_pCommonToolBar->CreateImage();
			pMap->GetCoordinateTransform()->setSourceCRS(setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString().toStdString());
			pMap->GetCoordinateTransform()->setDestCRS(setting2.value("/ProjectionCoordinate_CONFIG/LAMBERT").toString().toStdString());
			pMap->GetCoordinateTransform()->setDesProjName("������ͶӰ");
			pMap->TramsformMap();
			pMap->ScaleToLayer("����Ȧ");
		}
	}
	//Ħ����ͶӰ  OK
	else if (action == ui.m_pActionM)
	{
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
		QSettings setting2(IDPApplication::ConfigPath()+"ProjectionType.ini", QSettings::IniFormat);
		setting2.setIniCodec(QTextCodec::codecForName("UTF-8"));	
		if (viewerWindow)
		{
			int wdt = viewerWindow->property("DType").toInt();
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();;
			if (pMap == NULL)
			{
				//QMessageBox::information(this,"��ʾ","��ȡ�ӿ�Mapʧ�ܣ�");
				return;
			}
			SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
			pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
			viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);

			m_pCommonToolBar->CreateImage();
			pMap->GetCoordinateTransform()->setSourceCRS(setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString().toStdString());
			pMap->GetCoordinateTransform()->setDestCRS(setting2.value("/ProjectionCoordinate_CONFIG/MERCATOR").toString().toStdString());
			pMap->GetCoordinateTransform()->setDesProjName("ī����ͶӰ");
			pMap->TramsformMap();
			pMap->ScaleToLayer("����Ȧ");
		}
	}
	//�Ⱦ�γͶӰ  OK
	else if (action == ui.m_pActionLonLat)
	{
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
		QSettings setting2(IDPApplication::ConfigPath()+"ProjectionType.ini", QSettings::IniFormat);
		setting2.setIniCodec(QTextCodec::codecForName("UTF-8"));	
		if (viewerWindow)
		{
			int wdt = viewerWindow->property("DType").toInt();
			goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();;
			if (pMap == NULL)
			{
				//QMessageBox::information(this,"��ʾ","��ȡ�ӿ�Mapʧ�ܣ�");
				return;
			}
			SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
			pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
			viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);

			m_pCommonToolBar->CreateImage();
			pMap->GetCoordinateTransform()->setSourceCRS(setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString().toStdString());
			pMap->GetCoordinateTransform()->setDestCRS(setting2.value("/ProjectionCoordinate_CONFIG/LONGLAT").toString().toStdString());
			pMap->GetCoordinateTransform()->setDesProjName("�Ⱦ�γͶӰ");
			pMap->TramsformMap();
			pMap->ScaleToLayer("����Ȧ");
		}
	}
	//��Ӿ�γ�� OK
	else if (action == ui.m_pActionGrid)
	{
		/*m_pGridAddDlg = new GridSetting(this);
		m_pGridAddDlg->exec();*/

	}
	//ͶӰ�������� OK
	else if (action == ui.m_ActionProjCenter)
	{
		/*m_pProjCenterDlg = new ProjCenterSetting(this);
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
		if (viewerWindow)
		{
			SnapImageDrawCallback* pImageDrawCallback = new SnapImageDrawCallback;
			pImageDrawCallback->setFileName((IDPApplication::TempPath()+"Thematic.png").toStdString());
			viewerWindow->getViewer()->getCamera()->setFinalDrawCallback(pImageDrawCallback);
			m_pCommonToolBar->CreateImage();
		}
		m_pProjCenterDlg->exec();*/

	}
	//���ڹر�
	else if (action == ui.actionClose)
	{
		QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(this->centralWidget());
		if (mdiArea)
		{
			mdiArea->closeActiveSubWindow();
		}

	}
	//�ر����д���
	else if (action == ui.actionCloseAll)
	{
		/*QMdiArea* mdiArea = dynamic_cast<QMdiArea*>(this->centralWidget());
		if (mdiArea)
		{
		mdiArea->closeAllSubWindows();
		}*/
	}
	//ϵͳ����
	else if (action == ui.actionConfig)
	{
		/*m_pConfigDialog = new ConfigDialog(this);
		m_pConfigDialog->exec();*/
	}
	//��ʾ����
	else if (action == ui.actionDisplaySettings)
	{
		/*DisplaySettingsDialog* pDisplaySettingsDialog = new DisplaySettingsDialog(this);
		pDisplaySettingsDialog->exec();*/
	}
	//��ͼ���� OK
	else if (action == ui.actionMapSettings)
	{
		/*DisplaySettingsDialog* pDisplaySettingsDialog = new DisplaySettingsDialog(this);
		pDisplaySettingsDialog->setModal(true);
		pDisplaySettingsDialog->showNormal();
		pDisplaySettingsDialog->StackedWidgetCurrentIndex(1);*/
	}
	else 	if (action == ui.actionAbout)
	{
		IDPAbout* about = new IDPAbout(this);
		about->setIcon(QIcon(tr(":/images/about.png")));
		about->setSize(QSize(428,178));
		QStringList contents;
		QString cunrentDate = QDate::currentDate().toString("yyyy/mm/dd");
		contents<<tr("  �汾 %1 beta").arg("1.0")
			<<tr("  ����ʱ�� %2").arg(/*"2014/4/25"*/cunrentDate)
			<<tr("  ��Ȩ %3").arg("�����ѧ��ϵͳ")
			<<tr("  ��ϵ��ʽ %4").arg("www.css.com.cn")
			<<tr("  email %5").arg("yijx@css.com.cn");
		about->setContentList(contents);
		about->setWindowTitle(tr("���� �ؿ����ݽ�������ϵͳ"));
		about->initUI();
		about->exec();
	}
	else 	if (action == ui.actionHelp)
	{
		QDesktopServices desktopServices;
		QString strUrl=IDPApplication::ExePath()+QString("/help.chm");
		desktopServices.openUrl(QUrl(strUrl));
	}
}

//////////////////////////////////////////////////////////////////////////
//ϵͳ���
void MainWindow::SetSystemSkin( const QString& skinPath)
{
	QFile file(skinPath);
	file.open(QFile::ReadOnly);
	QTextStream stream(&file);
	stream.setCodec(QTextCodec::codecForName("UTF-8"));
	QString styleSheet = 	stream.readAll();
	qApp->setStyleSheet(styleSheet);
	file.close();
}


void MainWindow::closeEvent( QCloseEvent *event )
{
	int ret = QMessageBox::information(this,tr("��ʾ"),tr("����ҵ�����ݽ�������ϵͳ��IDP���������У�\n�Ƿ�ȷ�Ϲرգ�"),QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == 1024)
	{
		m_pRecentFilesMenu->saveRecent(IDPApplication::ExePath()+"/IDP_RecentFiles.txt");
		//event->accept();
		qApp->quit();
	}
	else
	{
		event->ignore();
	}
}

//////////////////////////////////////////////////////////////////////////
//���ӹ�����
void MainWindow::AddIDPToolBar( int dType )
{
	try
	{
		
		QWidget* cw = FromWidgetToQGLW();
		if (cw)
		{
			Map* map = (Map*)cw->property("Map").toLongLong();
			if (map)
			{
				m_pRadarControlDockWidget->AddLayersToCmb(map);
				Layer* layer = map->CurrentLayer();
				if (layer)
				{
					DataProvider* dp = layer->GetDataProvider();
					if (dp)
					{
						int key = dp->GetProductCode();
						SetSysGroupEnabled(QString::number(key));
					}
				}
			}
			
		}
		else
		{
			m_pRadarControlDockWidget->AddLayersToCmb(0);
		}

		if (cw)
		{

			int wdt = cw->property("DType").toInt();
			if (wdt == RadarData)
			{
				RadarGraphicsView* gv = cw->findChild<RadarGraphicsView*>();
				if (gv)
				{
					gv->setProperty("Action",(long long)ui.actionViewR);
					gv->setVisible(gv->isVisible());
				}
			}
			else
			{
				ui.actionViewR->setCheckable(false);
				ui.actionViewR->setChecked(false);
			}

		}
		else
		{
			ui.actionViewR->setCheckable(false);
			ui.actionViewR->setChecked(false);
		}

		m_pCommonToolBar->setEnabled(true);

		if (dType == RadarData)
		{
			//GetStatusToolBar()->GetSpinBox()->setEnabled(true);
			if (m_wStyle == ToolBar)
			{
				//this->removeToolBar(m_pMapToolBar);
				this->removeToolBar(m_pCZToolBar);

				this->addToolBar(m_pRadarToolBar);
				m_pRadarToolBar->show();

				//this->addToolBar(m_pMapToolBar);
				//m_pMapToolBar->show();

				//this->addToolBar(m_pCommonToolBar);
				//m_pCommonToolBar->show();

				//this->addToolBar(m_pWinToolBar);
				//m_pWinToolBar->show();
			}
			else
			{
				RemoveTabToolBar(QStringList()<<tr("��ͼ")<<tr("����"));
				m_pTabWidget->insertTab(1,m_pRadarToolBar,QIcon(":/images/logo.png"),tr("�״�"));
				m_pTabWidget->setCurrentWidget(m_pRadarToolBar);
				//m_pRadarToolBar->show();
			}

			foreach(QAction* action , m_pCommonToolBar->actions())
			{
				if (action->text().contains("ͶӰ"))
				{
					action->setVisible(false);
					break;
				}
			}

			QWidget* bw = FromWidgetToQGLW();
			if (NULL == bw)
			{
				return;
			}

			if (bw->objectName().contains("CompositeViewerQT"))
			{
				QStringList list;
				list<<tr("��ֵ������")<<tr("������")<<tr("��ͼ");
				//m_pRadarToolBar->SetEnabledList(list,false);
			}
			else
			{
				QStringList list;
				list<<tr("��ֵ������")<<tr("������")<<tr("��ͼ");
				m_pRadarToolBar->SetEnabledList(list,true);
			}
		}
		else if (dType == GISData)
		{
			if (m_wStyle == ToolBar)
			{
				this->removeToolBar(m_pRadarToolBar);
				this->removeToolBar(m_pCZToolBar);


				//this->addToolBar(m_pCommonToolBar);
				//m_pCommonToolBar->show();

				//this->addToolBar(m_pWinToolBar);
				//m_pWinToolBar->show();

				//this->addToolBar(m_pMapToolBar);
				//m_pMapToolBar->show();
			}
			else
			{
				RemoveTabToolBar(QStringList()<<tr("�״�")<<tr("����"));

				//m_pTabWidget->insertTab(1,m_pMapToolBar,QIcon(":/images/logo.png"),tr("��ͼ"));
				//m_pTabWidget->setCurrentWidget(m_pMapToolBar);
				//m_pMapToolBar->show();
			}
			foreach(QAction* action , m_pWinToolBar->actions())
			{
				if (action->text().contains("ͶӰ"))
				{
					action->setVisible(true);
					break;
				}
			}
		}
		else if (dType == CZData)
		{
			//GetStatusToolBar()->GetSpinBox()->setEnabled(false);
			if (m_wStyle == ToolBar)
			{
				this->removeToolBar(m_pRadarToolBar);

				this->addToolBar(m_pCZToolBar);
				m_pCZToolBar->show();

				//this->addToolBar(m_pMapToolBar);
				//m_pMapToolBar->show();

				//this->addToolBar(m_pCommonToolBar);
				//m_pCommonToolBar->show();

				//this->addToolBar(m_pWinToolBar);
				//m_pWinToolBar->show();
			}
			else
			{
				RemoveTabToolBar(QStringList()<<tr("�״�"));
			}
			foreach(QAction* action , m_pCommonToolBar->actions())
			{
				if (action->text().contains("ͶӰ"))
				{
					action->setVisible(true);
					break;
				}
			}
		}
		else if (dType == NoData || dType == Win4)
		{
			if (m_wStyle == ToolBar)
			{
				this->removeToolBar(m_pRadarToolBar);
				this->removeToolBar(m_pMapToolBar);
				this->removeToolBar(m_pCZToolBar);
				this->removeToolBar(m_pWinToolBar);
				//this->removeToolBar(m_pCommonToolBar);
				//m_pCommonToolBar->setEnabled(false);
				//this->addToolBar(m_pDefaultToolBar);
				//m_pDefaultToolBar->show();
			}
			else
			{
				RemoveTabToolBar(QStringList()<<tr("��ͼ")<<tr("�״�")<<tr("����")<<tr("����")<<tr("ͨ��"));
				//m_pTabWidget->addTab(m_pDefaultToolBar,QIcon(":/images/logo.png"),tr("�ļ�"));
				//m_pTabWidget->setCurrentWidget(m_pDefaultToolBar);
				//m_pDefaultToolBar->show();
			}
			/*if (dType == NoData && m_pLeftDockWidget)
			{
				m_pLeftDockWidget->ClearTreeWidgetItem();
			}*/

		}
		ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(FromWidgetToQGLW());
		if (viewerWindow)
		{
			QString winType = viewerWindow->property("Win2_3").toString();
			if (QString("2D").contains(winType))
			{
				m_pMapToolBar->setEnabled(true);

				foreach(QAction* action , m_pRadarToolBar->actions())
				{
					if (action->text().contains("��ά"))
					{
						action->setEnabled(false);
					}
					else if (action->text().contains("��ά"))
					{
						action->setEnabled(true);
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			else if (QString("3D").contains(winType))
			{
				m_pMapToolBar->setEnabled(false);

				foreach(QAction* action , m_pRadarToolBar->actions())
				{
					if (action->text().contains("��ά"))
					{
						action->setEnabled(true);
					}
					else if (action->text().contains("��ά"))
					{
						action->setEnabled(false);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			

		}
		

	}
	
	catch (...)
	{
	}
	
	
}

void MainWindow::RemoveTabToolBar( QStringList list )
{
	foreach(QString  title, list)
	{
		int count = m_pTabWidget->count();
		for (int i = 0 ; i<count ; i++)
		{
			QWidget* w = m_pTabWidget->widget(i);
			if (w)
			{
				if (w->property("title") == title)
				{
					m_pTabWidget->removeTab(i);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//�����״ﴰ�ڼ���layer
void MainWindow::AddLeftDockW( QWidget* currentWidget )
{
	try
	{
		//m_pLeftDockWidget->AddTreeWidgetItem(currentWidget);
	}
	
	catch (...)
	{
	}
	
}


QWidget* MainWindow::FromWidgetToQGLW(int i)
{
	QWidget* glw = 0;
	try
	{
		if (m_pCentralMdiArea)
		{
			QMdiSubWindow *subWin = m_pCentralMdiArea->activeSubWindow();
			if (subWin)
			{
				QWidget* activeW = subWin->widget();
				activeW = (QWidget*)subWin->property("Widget").toLongLong();
				if (activeW)
				{
					QTabWidget* tabW	= dynamic_cast<QTabWidget*>(activeW);
					if (tabW)
					{
						QWidget* w = tabW->widget(i);
						if (w)
						{
							glw = w;
						}
					}
					else
					{
						glw = activeW;
					}
				}
			}
		}
		return glw;
	}

	catch (...)
	{
		return 0;
	}
	return glw;
}

QMdiSubWindow*  MainWindow::CreateMDISubWindow( QWidget* widget , Qt::WindowFlags flags /*= 0*/ )
{
	if(m_pCentralMdiArea)
	{
		return m_pCentralMdiArea->CreateSubWindow(widget,flags);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//װ�����ݵ��ø������ݹ�����
void MainWindow::LoadData( QString fileName )
{
	if (!QFile::exists(fileName))
	{
		QMessageBox::information(this,tr("��ʾ"),tr("�ļ�������"));
		return;
	}
	
	QString suffix = QFileInfo(fileName).suffix().toLower();

	if ("bz2" == suffix ||"bin" == suffix )//�״�
	{
		m_pRadarToolBar->LoadData(fileName);
	}
	else if ("nc" == suffix)//NC
	{
		if (m_pMeteoDataDialog)
		{
			m_pMeteoDataDialog->OpenFileOrData(fileName);
			if (!m_pMeteoDataDialog->isVisible())
			{
				m_pMeteoDataDialog->show();
			}
		}
	}
	else if("meca" == suffix || "in" == suffix || "tif" == suffix)
	{
		m_pCZToolBar->LoadData(fileName);
	}
	setWindowFilePath(fileName);
	m_pRecentFilesMenu->addRecentFile(fileName);
}

//////////////////////////////////////////////////////////////////////////
//��ʾ�״��ұ�����
void MainWindow::Slot_ShowRaderAttributeInfo( QWidget* viewer , Layer* layer )
{
	ui.actionViewR->setCheckable(true);
	ui.actionViewR->setChecked(true);
	m_pViewManger->ShowRaderAttributeInfo(layer,viewer);
}

//////////////////////////////////////////////////////////////////////////
//��������б�
//void MainWindow::Slot_ShowAttributePropertyBrowser( Layer* layer , bool readonly )
//{
//	m_pAttributeDockWidget->ShowAttributePropertyBrowser(layer,readonly);
//}

////////////////////////////////////////////////////////////////////////////
//����MainWindow��װ������ �ݲ����״�����bz2
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	QStringList lsit = mimeData->formats();
	QStringList dataPathList;
	foreach (QString format, mimeData->formats()) 
	{
		if (format == "text/uri-list")
		{
			QList<QUrl> urlList = mimeData->urls();
			for (int i = 0; i < urlList.size(); ++i)
			{
				dataPathList.append(urlList[i].toString().remove("file:///"));
			}
			break;
		}
	}
	if (dataPathList.size() > 0)
	{
		QFileInfo fileInfo(dataPathList.at(0));
		if ("bz2" == fileInfo.suffix().toLower())
		{
			LoadData(dataPathList.at(0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//��ʾ״̬������
void MainWindow::On_UpdateDateStatusBar()
{
	try
	{
		QWidget* bw = FromWidgetToQGLW();
		if(bw)
		{
			int wdt = bw->property("DType").toInt();
			if (wdt == RadarData)
			{
				ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(bw);
				QString winType = viewerWindow->property("Win2_3").toString();
				if ("2D" ==winType)
				{
					//DefaultManipulator* dm = dynamic_cast<DefaultManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
					//if (dm)
					//{
						goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();
						if (pMap)
						{
							//dm->CoordTrans(pMap->GetCoordinateTransform());
							std::string s_proj = pMap->GetCoordinateTransform()->getDesProjName();
							double lat = 0.0,lon = 0.0;
							pMap->GetCoordinateTransform()->getCenter(lon,lat);
							m_pProjection->setText(tr("��ǰͶӰ��") + QString(s_proj.c_str()) );
							m_pProjCenter->setText(tr("��ǰͶӰ���ģ�")+ QString::number(lon)+ QString(",")+QString::number(lat));
							osg::Vec3 temp = pMap->GetCurrentLocation();
							std::string currentvalue = pMap->GetCurrentValue();
							if(abs(temp.x())<=360 && abs(temp.y())<=90) 
							{
								m_pPosition->setText(tr("��ǰλ�ã�")+ QString::number(temp.x())+ QString(",")+QString::number(temp.y()));
								m_pValue->setText(tr("��ǰ��ֵ��") + QString::fromStdString(currentvalue));

							}
						}
					//}
				}
				else if ("3D" == winType)
				{
					/*idpEarthManipulator *iem = dynamic_cast<idpEarthManipulator *>(viewerWindow->getViewer()->getCameraManipulator());
					if (!iem)
					{
					return;
					}*/

					goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();

					osg::Vec3 temp2 = pMap->GetCurrentLocation();
					m_pProjection->setText(tr("��ǰͶӰ����ͶӰ"));
					m_pProjCenter->setText(tr("��ǰͶӰ���ģ�")+ QString::number(0.0)+ QString(",")+QString::number(0.0));
					if(abs(temp2.x())<=360 && abs(temp2.y())<=90) 
					{
						m_pPosition->setText(tr("��ǰλ�ã�")+ QString::number(temp2.x())+ QString(",")+QString::number(temp2.y()));


					}
				}

				return;
			}
			else if (wdt == CZData)
			{
				ViewerQT* viewerWindow =dynamic_cast<ViewerQT*>(bw);
				if (viewerWindow)
				{
					//DefaultManipulator* dm = 0;
					//osgGA::KeySwitchMatrixManipulator* pKeyMapnipulator = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(viewerWindow->getViewer()->getCameraManipulator());
					//if(!pKeyMapnipulator) return;
					//osgGA::CameraManipulator* pCamMaintor = pKeyMapnipulator->getMatrixManipulatorWithKey('1');
					//dm = dynamic_cast<DefaultManipulator*>(pCamMaintor);
					//if(dm)
					//{
						goto_gis::Map* pMap = (goto_gis::Map*)viewerWindow->property("Map").toLongLong();
						if (pMap)
						{
							//dm->CoordTrans(pMap->GetCoordinateTransform());
							std::string s_proj = pMap->GetCoordinateTransform()->getDesProjName();
							double lat = 0.0,lon = 0.0;
							pMap->GetCoordinateTransform()->getCenter(lon,lat);
							m_pProjection->setText(tr("��ǰͶӰ��") + QString(s_proj.c_str()) );
							m_pProjCenter->setText(tr("��ǰͶӰ���ģ�")+ QString::number(lon)+ QString(",")+QString::number(lat));
							osg::Vec3 temp3 = pMap->GetCurrentLocation();
							if(abs(temp3.x())<=360 && abs(temp3.y())<=90) 
							{
								m_pPosition->setText(tr("��ǰλ�ã�")+ QString::number(temp3.x())+ QString(",")+QString::number(temp3.y()));


							}
						}
					//}
				}
			}
		}
	}

	catch (...)
	{
	}

}

void MainWindow::ShowMeteoDataDialog()
{
	if (m_pMeteoDataDialog == 0)
	{
		m_pMeteoDataDialog = new MeteoDataDialog(this);
	}
	m_pMeteoDataDialog->show();
}

QRecentFilesMenu * MainWindow::GetRecentFilesMenu()
{return m_pRecentFilesMenu; }

CentralMdiArea* MainWindow::GetCentralMdiArea()
{ return m_pCentralMdiArea;}

void MainWindow::ShowLeftDockWidget( bool isVisible )
{  
	 //m_pLeftDockWidget->setVisible(isVisible);
}

//void MainWindow::ShowAttributeDockWidget( bool isVisible )
//{ m_pAttributeDockWidget->setVisible(isVisible);}

ViewManager* MainWindow::GetViewManger()
{return m_pViewManger;}

StatusToolBar* MainWindow::GetStatusToolBar()
{ return m_pStatusToolBar;}

goto_gis::Map* MainWindow::GetCurrentMap()
{
	goto_gis::Map* pMap = 0;
	QWidget* cw = FromWidgetToQGLW();
	if (cw)
	{
		pMap = (goto_gis::Map*)(cw->property("Map").toLongLong());
	}
	return pMap;
}

void MainWindow::SetSysGroupEnabled(QString productCode )
{
	QMap<QString , QString> actionMap = m_actionInfoMap.value(productCode);
	if (actionMap.size() <= 0 )//δ���ò���XML���ݣ����һ����Բ���
	{
		//QMessageBox::critical(this,tr("����"),tr("δ���ò���XML���ݣ����һ����Բ���"));
		foreach(QAction* action , m_SysAllActionList)
		{										
			action->setEnabled(false);
		}
		return;
	}

	foreach(QAction* action , m_SysAllActionList)
	{
		if (actionMap.contains(action->objectName()))//����
		{

			int value = actionMap.value(action->objectName()).toInt();
			action->setEnabled(value);
			if ("action3D" == action->objectName())
			{
				m_pRadarControlDockWidget->setEnabled(value);
			}
		}
		else//������ �һ� false��ʹ�� true
		{
			action->setEnabled(true);
			if ("action3D" == action->objectName())
			{
				m_pRadarControlDockWidget->setEnabled(true);
			}
		}
	}
}

void MainWindow::RadarControlDockWidgetGroupEnd( bool bEnd )
{
	m_pRadarControlDockWidget->GroupEnd(bEnd);
}


QWidget* GetActiveWindow(QMdiArea* m_pMdiArea)
{
	if(!m_pMdiArea) return 0;

	//QWidget* pWidget = 0;
	QMdiSubWindow *subWin = m_pMdiArea->activeSubWindow();
	return subWin;
	/*if (subWin)
	{
		QWidget* activeW = subWin->widget();
		activeW = (QWidget*)subWin->property("Widget").toLongLong();
		if (activeW)
		{
			QTabWidget* tabW	= dynamic_cast<QTabWidget*>(activeW);
			if (tabW)
			{
				QWidget* w = tabW->widget(0);
				if (w)
				{
					pWidget = w;
				}
			}
			else
			{
				pWidget = activeW;
			}
		}
	}*/
}