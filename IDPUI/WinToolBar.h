#ifndef WINTOOLBAR_H
#define WINTOOLBAR_H

#include <QtGui>
#include "ui_WinToolBar.h"

//#include "IDPdb.h"
#include "idpui_global.h"


//class CCipasComposer;
class IDPUI_EXPORT WinToolBar : public QToolBar
{
	Q_OBJECT

public:
	WinToolBar(QWidget *parent = 0);
	~WinToolBar();
public:
	void InitSlots();


protected Q_SLOTS:
	void Slots_ActionTriggered( QAction* action );
private:
	Ui::WinToolBar ui;
	QActionGroup m_WinActionGroup;
	//CCipasComposer* m_pCCipasComposer;
};

#endif // WINTOOLBAR_H
