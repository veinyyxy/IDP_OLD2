#include <QDesktopWidget>
#include "MonitorThread.h"
#include "MonitorDialog.h"

MonitorDialog::MonitorDialog(QWidget *parent, Qt::WFlags flags) : QDialog(parent, flags)
{
	ui.setupUi(this);
	InitConfig();
	CreateActions();
	CreateTrayIcon();
	mThread = new MonitorThread( m_sfsPath/*.split("#",QString::SkipEmptyParts)*/);
	//mThread->StartMonitorThread();
	startTimer(m_time.toInt()); // ���ʱ�䣬����
}

MonitorDialog::~MonitorDialog()
{
	if (mThread)
	{
		delete mThread;
		mThread = NULL;
	}
}

void MonitorDialog::InitConfig()
{
	QString iniPath = QCoreApplication::applicationDirPath()+tr("/Config/MonitorConfig.ini");
	QSettings *configIni = new QSettings(iniPath, QSettings::IniFormat);  
	//m_isTimer = configIni->value("/ISTIMER/SISTIMER").toString(); //�Ƿ�������ʱ��
	m_time = configIni->value("/TIMER/STIMER").toString();//����ˢ��ʱ��
	m_sfsPath = configIni->value("/SERVERFSPATH/SFSPATH").toString();//���Ŀ¼
	//��������ɺ�ɾ��ָ��   
	delete configIni;
}

void MonitorDialog::CreateActions()
{
	m_pMinimizeAction = new QAction(QString::fromLocal8Bit("&����ϵͳ����"), this);
	connect(m_pMinimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	m_pRestoreAction = new QAction(QString::fromLocal8Bit("&��ԭϵͳ����"), this);
	connect(m_pRestoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	m_pQuitAction = new QAction(QString::fromLocal8Bit("&�˳�ϵͳ����"), this);
	connect(m_pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MonitorDialog::CreateTrayIcon()
{
	m_pTrayIconMenu = new QMenu(QApplication::desktop());
	m_pTrayIconMenu->addAction(m_pMinimizeAction);
	m_pTrayIconMenu->addAction(m_pRestoreAction);
	m_pTrayIconMenu->addSeparator();
	m_pTrayIconMenu->addAction(m_pQuitAction);

	m_pTrayIcon = new QSystemTrayIcon(this);
	m_pTrayIcon->setIcon(QIcon(":/images/logo.png"));
	m_pTrayIcon->setContextMenu(m_pTrayIconMenu);
	m_pTrayIcon->show();
}

void MonitorDialog::timerEvent( QTimerEvent *event )
{
	QMutexLocker locker(&m_lock);
	mThread->StartMonitorThread();
}