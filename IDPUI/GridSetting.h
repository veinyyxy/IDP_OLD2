#ifndef GRIDSETTING_H
#define GRIDSETTING_H
#include <QtGui>
#include <QDialog>
#include "ui_GridSetting.h"
#include "ui_ProjCenterDialog.h"
#include "idpui_global.h"
#include "IDPdb.h"

class MainWindow;

class IDPUI_EXPORT GridSetting :
	public QDialog
{
	Q_OBJECT
public:
	GridSetting(QWidget *parent = 0);
	~GridSetting(void);
public:
	//void LoadData(QString fileName);
private:
	void InitSlots();
	void InitDialog();
	bool ValidityCheck();//合法性检查
public Q_SLOTS:
		//void Slots_ActionTriggered( QAction* action );
	void Slots_OnOKCliked();
	void Slots_OnQuitCliked();
private:
	Ui::GridSettingDialog ui;
	QActionGroup m_GSActionGroup;
	MainWindow* _mw;
	float latstart,latend,lonstart,lonend,latreso,lonreso;
};



class IDPUI_EXPORT ProjCenterSetting :
	public QDialog
{
	Q_OBJECT
public:
	ProjCenterSetting(QWidget *parent = 0);
	~ProjCenterSetting(void);
private:
	void InitSlots();
	void InitDialog();
	bool ValidityCheck();//合法性检查
public Q_SLOTS:
		void Slots_OnOKCliked();
private:
	Ui::ProjCenterDialog ui;
	QActionGroup m_PCActionGroup;
	MainWindow* _mw;
	float m_lat,m_lon;
};
#endif // GRIDSETTING_H