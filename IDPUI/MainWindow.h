#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/** @file 
* @brief   ϵͳ������
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "ui_MainWindow.h"
#include "CentralMdiArea.h"
#include "Map.h"
#include "Layer.h"
#include "idpui_global.h"

 using namespace goto_gis;

class LeftDockWidget;
class DataDockWidget;
class RadarControlDockWidget;

class DefaultToolBar;
class WinToolBar;
class RadarToolBar;
class MapToolBar;
class StatusToolBar;		//״̬������
class CZToolBar;
class CommonToolBar;

class GISMapManager;

class ViewManager;
class IDPUIControlImp;
class ViewerQT;
class QTimer;
class MeteoDataDialog;
class QRecentFilesMenu;
class CCipasComposer;
class ConfigDialog;

class ProjCenterSetting;
enum WinStyle
{
	ToolBar,//_MdiArea,
	//ToolBar_StackedWidget,

	TabBar//_MdiArea
	//TabBar_StackedWidget
};
 /** @class   MainWindow
    @brief   ϵͳ������
    @author  �׼���
    @note   ϵͳ�����ڵĶ���ӿ�
*/ 
class IDPUI_EXPORT MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

protected:
	void SystemStyle(WinStyle wstyle);
	WinStyle SystemStyle();
	void InitMainWindow();
	void InitRecentFilesMenu();
	void initStatusToolBar();		//��ʼ��״̬��
	void InitConfig();
	void InitSlots();
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);
public:
	/** @brief ����ϵͳ��� */
	void SetSystemSkin(const QString& skinPath);
	/** @brief �õ��ļ��˵�ָ�� */
	QRecentFilesMenu * GetRecentFilesMenu();
	//��ʾһ����Ƭ
	void ShowMeteoDataDialog();
	inline QList<QAction*>& GetSysGroup() { return m_SysAllActionList;}
	void SetSysGroupEnabled(QString productCode);

	/** @brief �õ��ͻ���ָ�� */
	CentralMdiArea* GetCentralMdiArea();
	/** @brief �õ������ָ�� */
	void ShowLeftDockWidget(bool isVisible);
	/** @brief �õ����Կؼ�ָ�� */
    void ShowAttributeDockWidget(bool isVisible);
	/** @brief �õ���ͼ����ָ�� */
	ViewManager* GetViewManger();
	/** @brief �õ�APPUI����ָ�� */
	/*inline IDPUIControlImp* GetUIControlImp(){return m_pControlImp.get();}*/
	/** @brief װ�ع����� */
	void AddIDPToolBar(int dType);
	/** @brief װ������� */
	void AddLeftDockW(QWidget* currentWidget);
	StatusToolBar* GetStatusToolBar();
	QWidget* FromWidgetToQGLW(int i = 0);
	QMdiSubWindow*  CreateMDISubWindow(QWidget* subWindow , Qt::WindowFlags flags = 0);
	goto_gis::Map* GetCurrentMap();
	bool ReadActionXML(const QString filename);
	void RadarControlDockWidgetGroupEnd(bool bEnd);
	//friend QWidget* GetActiveWindow();
protected:
	void closeEvent( QCloseEvent *event );
	void RemoveTabToolBar(QStringList  list);
	void InitMap();
	Q_SIGNAL

public Q_SLOTS:
	void LoadData( QString fileName );

	void Slot_ActionTriggered( QAction* action );
	void Slot_FullScreen();
	void Slot_EscFullScreen();

	void Slot_ShowRaderAttributeInfo(QWidget* viewer , Layer* layer);
	//void Slot_ShowAttributePropertyBrowser(Layer* layer , bool readonly);

	void On_UpdateDateStatusBar();

private:
	Ui::MainWindowClass ui;

	CentralMdiArea* m_pCentralMdiArea;

	QToolBar* m_pTabToolBar;
	QTabWidget* m_pTabWidget;

	DataDockWidget* m_pDataDockWidget;
	RadarControlDockWidget *m_pRadarControlDockWidget;

	DefaultToolBar* m_pDefaultToolBar;
	WinToolBar* m_pWinToolBar;
	RadarToolBar* m_pRadarToolBar;
	MapToolBar* m_pMapToolBar;
	QStatusBar*		m_pStatusBar;				//״̬��
	StatusToolBar*	m_pStatusToolBar;			//״̬������
	CZToolBar* m_pCZToolBar;
	CommonToolBar* m_pCommonToolBar;
	QLabel* m_pPosition;
	QLabel* m_pValue;
	QLabel* m_pProjection;
	QLabel* m_pProjCenter;
	ViewManager* m_pViewManger;
	//std::auto_ptr<IDPUIControlImp> m_pControlImp;
	WinStyle m_wStyle;
	int iWindowID;
	QActionGroup m_MenuActionGroup;
	QTimer* testTimer;
	//goto_gis::Map* m_pMap;
	//GISMapManager* m_pGM;
	bool m_bF;
	bool m_bFullScreen;
	Qt::WindowFlags m_fullScreenFlags;
	QWidget* m_fullScreenParent;
	
	QRecentFilesMenu* m_pRecentFilesMenu;
	MeteoDataDialog* m_pMeteoDataDialog;
	CCipasComposer* m_pCipasComposer;
	ConfigDialog* m_pConfigDialog;
	//GridSetting * m_pGridAddDlg;
	ProjCenterSetting *m_pProjCenterDlg;

	QList<QAction*> m_SysAllActionList;
	QMap<QString,QMap<QString , QString>> m_actionInfoMap;
};

#endif // MAINWINDOW_H
