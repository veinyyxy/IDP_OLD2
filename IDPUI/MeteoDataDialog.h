/************************************************************************/
/* Grads产品生产设置对话框头文件                                          */
/************************************************************************/
#pragma once
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QIcon>
#include <QtGui/QToolBar>
#include <QtGui/QLayout>
#include <QtGui/QFileDialog>
#include <QtCore/QObject>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QToolButton>
#include <QMenu>
#include <QComboBox>
#include <QSize>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QHash>
#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QFile>
#include <QCoreApplication>
#include <QSharedPointer>
#include <QtGui>

#include "../CDataProviderManager/cdataprovidermanager.h"
#include "../CDataElement/ProductTypeManager.h"
#include "../CDataElement/CElement.h"

#include "idpui_global.h"

class GridDataDialog;

class IDPUI_EXPORT MeteoDataDialog : public QDialog
{
	Q_OBJECT
public:
	MeteoDataDialog(QWidget *parent = 0);//初始化对话框
	~MeteoDataDialog(void);

	void EmitOpenSigal() {Slots_ActionTriggered(m_ActionOpenData); }
	void OpenFileOrData( QString filename);
	void setExtraNames(QStringList sl){ m_EleNames_ = sl;}
private:
	void InitDialog();//初始化对话框控件
	void InitSlots();
	void ClearData();
	

	void CurrentFileType(QString fileType);				//设置当前打开的文件类型	micaps	wuenping
	QString CurrentFileType();							//得到当前打开的文件类型	micaps	wep

	void InitDrawType(QList<int> lstProType);
	void Initata(QHash<long, QDateTime> m_htDate , QHash<int, float > m_htLevel);
	void DrawElement();
	void InitNetCDF2Micaps();
	void InitNetCDF2Grads(QString filePath);

public Q_SLOTS:
	void Slots_ActionTriggered(QAction* action);
	void On_CmbVar_CurrentIndexChanged(const QString& text); //元素选择cmbbox事件
	

private:
	CDataProviderManager* m_pDPMgr;
	bool m_bFirstOpenFile;
	QHash<QString, CElement*>  m_HashElement;
	QHash<int, QString> m_htDrawType;//产品类型
	QString m_CurrentFileType;
	float m_fMissVal;
private:
	QGridLayout* m_MGridLayout;

	QToolBar* m_TopToolBar; //主工具栏
	QAction* m_ActionOpen; //打开数据QAction

	QAction* m_ActionOpenData; //打开数据
	QAction* m_ActionOpenM14; //打开M14数据
	QAction* m_ActionOpenTIF; //打开TIF数据


	QAction* m_ActionDataInfo;//显示数据信息
	QAction* m_ActionDrawData;//绘制数据信息
	QAction* m_ActionViewData;//查看数据列表

	QAction* m_ActionSectionPlot;//剖面图
	QAction* m_ActionDimensionPlot;//折线图

	QGroupBox* m_GroupBox;
	QGridLayout* m_GridLayout;

	QLabel* m_LblTime;
	QComboBox* m_CmbTime;

	QLabel* m_LblLevel;
	QComboBox* m_CmbLevel;

	QLabel* m_LblVar;
	QComboBox* m_CmbVar;
	
	QLabel* m_LblDrawType;
	QComboBox* m_CmbDrawType;

	QListWidget *m_pFileListWidget;

	QHBoxLayout *mainLayout;
	GridDataDialog* m_pGridDataDialog;
	QStringList m_EleNames_;	
	//////////////////////////////////////////////////////////////////////////
	
};