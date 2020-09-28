#ifndef CZTOOLBAR_H
#define CZTOOLBAR_H

#include <QtGui>
#include "ui_CZToolBar.h"
#include "idpui_global.h"
#include "IDPdb.h"

class MainWindow;

class IDPUI_EXPORT CZToolBar : public QToolBar
{
	Q_OBJECT

public:
	CZToolBar(QWidget *parent = 0);
	~CZToolBar();
public:
	void LoadData(QString fileName);
private:
	void InitSlots();
public Q_SLOTS:
		void Slots_ActionTriggered( QAction* action );
private:
	Ui::CZToolBar ui;
	QActionGroup m_CZActionGroup;
	MainWindow* _mw;
};

#endif // CZTOOLBAR_H
