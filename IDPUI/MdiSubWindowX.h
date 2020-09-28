#ifndef MDISUBWINDOWX_H
#define MDISUBWINDOWX_H

#include <QtGui>

class MdiSubWindowX : public QMdiSubWindow
{
	Q_OBJECT

public:
	MdiSubWindowX(QWidget *parent = 0);
	~MdiSubWindowX();

protected:

public Q_SLOTS:
		void Slots_ActionTriggered();
};

#endif // MDISUBWINDOWX_H
