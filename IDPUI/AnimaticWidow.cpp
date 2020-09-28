#include "IDPdb.h"
#include "AdapterWidget.h"
#include "mainwindow.h"
#include "sysmainwindow.h"
#include "AnimaticControlerInterface.h"
#include "MyActionInterface.h"
#include "AnimaticAction.h"
#include "AnimaticWidow.h"

AnimaticWidow::AnimaticWidow(osg::Node* node , QWidget *parent)
	: QMainWindow(parent), 
	m_pViewerQT(new ViewerQT(0,this)), 
	m_AnimaticControler(m_pViewerQT->getViewer())
{
	ui.setupUi(this);
	setWindowTitle(tr("�������Ŵ���"));
	ui.tableWidget_filelist->setAlternatingRowColors(true);
	ui.tableWidget_filelist->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_filelist->setSelectionBehavior (QAbstractItemView::SelectRows); //����ѡ����Ϊ������Ϊ��λ
	ui.tableWidget_filelist->setSelectionMode (QAbstractItemView::SingleSelection); //����ѡ��ģʽ��ѡ����
	ui.tableWidget_filelist->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	ui.tableWidget_filelist->setColumnWidth(0,200);
	m_pAnimaticToolBar = new QToolBar(tr("����������"),this);
	m_pPlayAction = new PlayAction(QIcon(":/images/statusToolBar/play.png"),tr("����"),m_pAnimaticToolBar);
	m_pPauseAction = new PauseAction(QIcon(":/images/statusToolBar/pause.png"),tr("��ͣ"),m_pAnimaticToolBar);
	m_pStopAction = new StopAction(QIcon(":/images/statusToolBar/stop.png"),tr("ֹͣ"),m_pAnimaticToolBar);
	m_pPre1Action = new PreAction(QIcon(":/images/statusToolBar/pre1.png"),tr("��һ֡"),m_pAnimaticToolBar);
	m_pPreAction = new speeddownAction(QIcon(":/images/statusToolBar/pre.png"),tr("����"),m_pAnimaticToolBar);
	m_pNextAction = new speedupAction(QIcon(":/images/statusToolBar/next.png"),tr("����"),m_pAnimaticToolBar);
	m_pNext1Action = new NextAction(QIcon(":/images/statusToolBar/next1.png"),tr("��һ֡"),m_pAnimaticToolBar);
	m_pRefreshAction = new LoopAction(QIcon(":/images/statusToolBar/refresh.png"),tr("ѭ��"),m_pAnimaticToolBar);

	m_pAnimaticToolBar->addAction(m_pPlayAction);
	m_pAnimaticToolBar->addAction(m_pPauseAction);
	m_pAnimaticToolBar->addAction(m_pStopAction);
	m_pAnimaticToolBar->addAction(m_pPre1Action);
	m_pAnimaticToolBar->addAction(m_pPreAction);
	m_pAnimaticToolBar->addAction(m_pNextAction);
	m_pAnimaticToolBar->addAction(m_pNext1Action);
	m_pAnimaticToolBar->addAction(m_pRefreshAction);
	connect(m_pAnimaticToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(Slot_AnimaticToolBarEvent(QAction*)));
	connect(ui.pushButtonSelectDir, SIGNAL(clicked()),this,  SLOT(Slot_ButtonClicked()));
	connect(ui.pushButtonClearList , SIGNAL(clicked()),this,  SLOT(Slot_ButtonClicked()));
	connect(ui.comboBox_fileforplay, SIGNAL(currentIndexChanged(int)), SLOT(Slot_ComboBoxIndex(int)));

	QGridLayout* gLayout = new QGridLayout(ui.centralWidget);
	gLayout->setContentsMargins(1,1,0,0);
	ui.centralWidget->setLayout(gLayout);
	
	m_pViewerQT->getViewer()->setThreadingModel(osgViewer::Viewer::SingleThreaded);
	//m_pViewerQT->getViewer()->setSceneData(node);
	m_pViewerQT->getViewer()->realize();
	m_pViewerQT->getViewer()->addEventHandler(new osgViewer::StatsHandler);
	m_pViewerQT->getViewer()->addEventHandler(new osgViewer::WindowSizeHandler);
	m_pViewerQT->getViewer()->addEventHandler(new osgGA::GUIEventHandler);

	DefaultManipulator* dm = new DefaultManipulator;
	m_pViewerQT->getViewer()->setCameraManipulator(dm);

	gLayout->addWidget(m_pViewerQT,0,0,1,1);
	gLayout->addWidget(m_pAnimaticToolBar,1,0,1,1,Qt::AlignHCenter);
}

AnimaticWidow::~AnimaticWidow()
{

}


void AnimaticWidow::closeEvent( QCloseEvent *event )
{
	int ret = QMessageBox::information(this,tr("��ʾ"),tr("���Ŷ����������ڽ��С�����\n�Ƿ�ȷ�Ϲرգ�"),QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == 1024)
	{
		event->accept();
		//qApp->quit();
	}
	else
	{
		event->ignore();
	}
}

void AnimaticWidow::Slot_AnimaticToolBarEvent( QAction* action )
{
	MyActionInterface* pAction = dynamic_cast<MyActionInterface*>(action);
	if(pAction)
		pAction->MyAction();

	/*try
	{
	MainWindow* mw = SysMainWindow::GetMainWindow();

	QWidget* cw	= mw->FromWidgetToQGLW();
	if (cw)
	{
	if (cw->property("DType").toInt() == RadarData)
	{

	}
	}
	}
	catch (...)
	{
	QMessageBox::critical(0,tr("��ʾ"),tr("�����״ﶯ���쳣��"));
	}*/
}

void AnimaticWidow::Slot_ButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (btn == ui.pushButtonSelectDir)
	{
		QString dir = QFileDialog::getExistingDirectory(0, tr("ѡ��·��"),"/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

		m_pAnimaticToolBar->setProperty("dir", dir);
		m_pAnimaticToolBar->setProperty("osg_Viewer", (qulonglong)m_pViewerQT->getViewer());
		m_pAnimaticToolBar->setProperty("m_AnimaticControler", (qulonglong)&m_AnimaticControler);
		if (dir == "")
		{
			return;
		}
		AddTableWidgetItem(dir);
	}
	else if(btn == ui.pushButtonClearList)
	{
		ui.tableWidget_filelist->clearContents();
		ui.tableWidget_filelist->setRowCount(0);
		ui.tableWidget_filelist->setColumnCount(1);

		m_AnimaticControler.SetLoadFileYes(false);

		m_pViewerQT->getViewer()->setSceneData(0);
	}
}

void AnimaticWidow::AddTableWidgetItem( QString strDir )
{
	ui.tableWidget_filelist->clearContents();

	QDir dir(strDir);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QStringList list = dir.entryList();

	ui.tableWidget_filelist->setRowCount(list.size());
	ui.tableWidget_filelist->setColumnCount(1);

	for (int i = 0; i < list.size(); ++i) 
	{
		QString strFile = list.at(i);
		//QTableWidgetItem*  item0 = new QTableWidgetItem;
		//item0->setText(QString::number(i));
		//ui.tableWidget_filelist->setItem(i,0,item0);
		QTableWidgetItem*  item1 = new QTableWidgetItem;
		item1->setText(strFile);
		ui.tableWidget_filelist->setItem(i,0,item1);
	}
	
}

void AnimaticWidow::Slot_ComboBoxIndex( int iIndex )
{
	AnimaticControlerInterface* pACI = m_AnimaticControler.GetAnimaticControlerInterface();
	if(!pACI) return;
	int FrameNum = -1;
	switch(iIndex)
	{
		case 0:
			FrameNum = -1;
			break;
		case 1:
			FrameNum = 5;
			break;
		case 2:
			FrameNum = 10;
			break;
		case 3:
			FrameNum = 15;
			break;
		case 4:
			FrameNum = 20;
			break;
		default:
			FrameNum = 25;
			break;
	}
	pACI->Stop();
	pACI->SetEndFrame(FrameNum);
	pACI->Loop();
}
