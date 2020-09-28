/************************************************************************/
/* Grads��Ʒ�������öԻ���ͷ�ļ�                                          */
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
	MeteoDataDialog(QWidget *parent = 0);//��ʼ���Ի���
	~MeteoDataDialog(void);

	void EmitOpenSigal() {Slots_ActionTriggered(m_ActionOpenData); }
	void OpenFileOrData( QString filename);
	void setExtraNames(QStringList sl){ m_EleNames_ = sl;}
private:
	void InitDialog();//��ʼ���Ի���ؼ�
	void InitSlots();
	void ClearData();
	

	void CurrentFileType(QString fileType);				//���õ�ǰ�򿪵��ļ�����	micaps	wuenping
	QString CurrentFileType();							//�õ���ǰ�򿪵��ļ�����	micaps	wep

	void InitDrawType(QList<int> lstProType);
	void Initata(QHash<long, QDateTime> m_htDate , QHash<int, float > m_htLevel);
	void DrawElement();
	void InitNetCDF2Micaps();
	void InitNetCDF2Grads(QString filePath);

public Q_SLOTS:
	void Slots_ActionTriggered(QAction* action);
	void On_CmbVar_CurrentIndexChanged(const QString& text); //Ԫ��ѡ��cmbbox�¼�
	

private:
	CDataProviderManager* m_pDPMgr;
	bool m_bFirstOpenFile;
	QHash<QString, CElement*>  m_HashElement;
	QHash<int, QString> m_htDrawType;//��Ʒ����
	QString m_CurrentFileType;
	float m_fMissVal;
private:
	QGridLayout* m_MGridLayout;

	QToolBar* m_TopToolBar; //��������
	QAction* m_ActionOpen; //������QAction

	QAction* m_ActionOpenData; //������
	QAction* m_ActionOpenM14; //��M14����
	QAction* m_ActionOpenTIF; //��TIF����


	QAction* m_ActionDataInfo;//��ʾ������Ϣ
	QAction* m_ActionDrawData;//����������Ϣ
	QAction* m_ActionViewData;//�鿴�����б�

	QAction* m_ActionSectionPlot;//����ͼ
	QAction* m_ActionDimensionPlot;//����ͼ

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