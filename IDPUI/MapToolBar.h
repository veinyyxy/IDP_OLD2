#ifndef MAPTOOLBAR_H
#define MAPTOOLBAR_H

#include <QToolBar>
#include "ui_MapToolBar.h"

#include "idpui_global.h"

class MainWindow;

class IDPUI_EXPORT MapToolBar : public QToolBar
{
	Q_OBJECT

public:
	MapToolBar(QWidget *parent = 0);
	~MapToolBar();
public:
	void InitSlots();

	public Q_SLOTS:
		void Slots_ActionTriggered( QAction* action );
private:
	Ui::MapToolBar ui;
	QActionGroup m_DefaultActionGroup;
	MainWindow* m_pMW;
};

#endif // MAPTOOLBAR_H
