#include "StdAfx.h"
#include "FramTooBoxesActionFactory.h"
#include "OpenDataAction.h"
#include "MapActions.h"
#include "WindowsSettingActions.h"

FramTooBoxesActionFactory::FramTooBoxesActionFactory(void)
{
	MyActionInterface* pOpenAction = new OpenDataAction(QIcon(":/images/standardTools/open.png")
		, QString::fromLocal8Bit("���ļ�"), 0);
	
	MyActionInterface* pCloseAction = new CloseSystemAction(QIcon(":/images/standardTools/close.png")
		, QString::fromLocal8Bit("�ر�ϵͳ"), 0);

	m_ActionMap.insert("OpenData", pOpenAction);
	m_ActionMap.insert("CloseSystem", pCloseAction);
	
	MyActionInterface* pDisplaySettingsAction = new DisplaySettingsAction(
		QIcon(":/images/mapTools/displaySettings.png"), QString::fromLocal8Bit("��ʾ����"), 0);
	MyActionInterface* pMapSettingsAction = new MapSettingsAction(
		QIcon(":/images/mapTools/mapSetting.png"), QString::fromLocal8Bit("��ͼ����"), 0);
	MyActionInterface* pAddLatLonNetAction = new AddLatLonNetAction(
		QIcon(":/images/mapTools/netnet.png"), QString::fromLocal8Bit("��Ӿ�γ��"), 0);
	MyActionInterface* pCenterProjectionAction = new CenterProjectionAction(
		QIcon(":/images/mapTools/mapCenter.png"), QString::fromLocal8Bit("��ͼ��������"), 0);
	MyActionInterface* pMakeProfessionalChartAction = new MakeProfessionalChartAction(
		QIcon(":/images/mapTools/chart.png"), QString::fromLocal8Bit("ר��ͼ����"), 0);
	MyActionInterface* pMakeProfessionalReportAction = new MakeProfessionalReportAction(
		QIcon(":/images/mapTools/report.png"), QString::fromLocal8Bit("ר�ⱨ������"), 0);

	m_ActionMap.insert("DisplaySettings", pDisplaySettingsAction);
	m_ActionMap.insert("MapSettings", pMapSettingsAction);
	m_ActionMap.insert("AddLatLonNet", pAddLatLonNetAction);
	m_ActionMap.insert("CenterProjection", pCenterProjectionAction);
	m_ActionMap.insert("MakeProfessionalChart", pMakeProfessionalChartAction);
	m_ActionMap.insert("MakeProfessionalReport", pMakeProfessionalReportAction);


	MyActionInterface* pFloatingWindowsAction = new FloatingWindowsAction(
		QIcon(":/images/windowControl/float.png"), QString::fromLocal8Bit("��������"), 0);
	MyActionInterface* pDockingWindowsAction = new DockingWindowsAction(
		QIcon(":/images/windowControl/dock.png"), QString::fromLocal8Bit("ͣ������"), 0);
	MyActionInterface* pOverlapAction = new OverlapAction(
		QIcon(":/images/windowControl/Overlap.png"), QString::fromLocal8Bit("�������"), 0);
	MyActionInterface* pTileAction = new TileAction(
		QIcon(":/images/windowControl/tile.png"), QString::fromLocal8Bit("ƽ�̴���"), 0);
	MyActionInterface* pCloseAllAction = new CloseAllAction(
		QIcon(":/images/windowControl/close.png"), QString::fromLocal8Bit("�ر����д���"), 0);
	MyActionInterface* pSystemSettingAction = new SystemSettingAction(
		QIcon(":/images/windowControl/settings.png"), QString::fromLocal8Bit("ϵͳ����"), 0);
	MyActionInterface* pFullScreenWindowAction = new FullScreenWindowAction(
		QIcon(":/images/windowControl/fullscreen.png"), QString::fromLocal8Bit("����ȫ��"), 0);

	m_ActionMap.insert("FloatWindow", pFloatingWindowsAction);
	m_ActionMap.insert("DockWindow", pDockingWindowsAction);
	m_ActionMap.insert("OverlapWindow", pOverlapAction);
	m_ActionMap.insert("TileWindow", pTileAction);
	m_ActionMap.insert("CloseAllWindow", pCloseAllAction);
	m_ActionMap.insert("SystemSettings", pSystemSettingAction);
	m_ActionMap.insert("FullScreen", pFullScreenWindowAction);
}


FramTooBoxesActionFactory::~FramTooBoxesActionFactory(void)
{
}

MyActionInterface* FramTooBoxesActionFactory::GetActionFromName( const QString& name )
{
	QMap<QString, MyActionInterface*>::iterator iterFinded = m_ActionMap.find(name);
	if(iterFinded != m_ActionMap.end())
	{
		return iterFinded.value();
	}
	else
	{
		return 0;
	}
}
