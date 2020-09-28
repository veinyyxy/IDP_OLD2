/************************************************************************/
/* Grads��Ʒ�������öԻ���ͷ�ļ�                                          */
/************************************************************************/
#pragma once

#include <QtGui/QWidget>
#include <QDir>
#include <QFileInfo>
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
#include <QFileDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>

#include "../IDataProvider/IDataProvider.h"
#include "../CDataProviderManager/cdataprovidermanager.h"
#include "../CDataElement/CElement.h"

struct STATION_ITEM
{
   int id;
   float flon;
   float flat;
};

class GridDataDialog : public QDialog
{
	Q_OBJECT
public:
	GridDataDialog( const QString& filePath, CDataProviderManager* dPMgr , QString var, int nTime ,QString time , int nLevel ,QString level , QStringList EleNames, QWidget *parent = 0/*, QString EleTypeName=""*/);//��ʼ���Ի���
	~GridDataDialog(void);
private:
	void InitDialog();//��ʼ���Ի���ؼ�
	void InitUISlots();	//��ʼ��UI�Ͽؼ��¼�	
	void ClearData();	//�������
	void InitializeModel(CDataProviderManager* dPMgr , QString var, int nTime ,QString time , int nLevel ,QString level);//װ�����ݵ�model
signals:
	void SendStationItems(QStringList);
	void SendZoomStation(int,float,float);
public slots:
	void DataToolBarEvent(QAction* action);//ToolBar�¼�
	void modifyItemData(QStandardItem *);
	void modifyCipasr(QStandardItem *);
	void FocusStations();
	void ZoomToStation(const QModelIndex& index);
private:
	QStandardItemModel* m_ItemModel;//model����
	static QString pBasePath;		//�����·��
	
private:   // ��ʾmicaps����
	void InitializeModelM1(CDataProviderManager* dPMgr ,int nTime ,QString time ,int nLevel ,QString level);
	void InitializeModelM2(CDataProviderManager* dPMgr );
	void InitializeModelM3(CDataProviderManager* dPMgr );
	void InitializeModelM5(CDataProviderManager* dPMgr );
	void InitializeModelM7(CDataProviderManager* dPMgr );
	void InitializeModelM8(CDataProviderManager* dPMgr );
	void InitializeModelM11(CDataProviderManager* dPMgr , QString var, int nTime ,QString time , int nLevel ,QString level);//װ�����ݵ�model
	void InitializeModelCIPASM(CDataProviderManager* dPMgr ,int nTime ,QString time ,int nLevel ,QString level);
	void InitializeModelSTADA(CDataProviderManager* dPMgr );
	void InitializeModelTXT(CDataProviderManager* dPMgr );
	void InitializeModelStaion(CDataProviderManager* dPMgr );
	void InitializeModelEXPONENT(QString var ,CDataProviderManager* dPMgr );
private:
	void writeGridData(QString& fileName);
	void writeNCData(QString& fileName);
	void writeMICAPS3(QString& fileName);
	void writeMICAPS4(QString& fileName);
	void writeCipasGridData(QString& fileName);

private:
	QGridLayout* m_MGridLayout;
	QToolBar* m_TopToolBar; //��������
	QAction* m_ActionSave; //��������QAction
	QTableView* m_TbView;
	QString m_sEleTypeName_;
	CDataProviderManager* m_pDPMgr;
	CElement* m_pElement;
	QList<CElement*> m_pLstElement;
	QString m_pFilePath;
	QString m_fileType;
	QStringList m_HeaderItemList;
	QStringList m_EleTypeNames_;
	int m_nNumData,m_nLevel;            //�洢�ж���������,��ȡ�ڼ������ݿ�
	QString m_elename;					//���ݿ�����
	QHash<int,float> m_stationname;

	QString m_VarName; // ��������
	QString m_time; // ʱ��
	
};