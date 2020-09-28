#include <QDebug>
#include "MdiSubWindowX.h"

MdiSubWindowX::MdiSubWindowX(QWidget *parent): QMdiSubWindow(parent)
{
	setMinimumSize(100,50);
	QMenu *_systemMenu =  systemMenu();
	_systemMenu->addSeparator();
	QAction *addAction = _systemMenu->addAction(QString::fromLocal8Bit("´°¿Ú¸¡¶¯"));
	connect(addAction, SIGNAL(triggered()),this,  SLOT(Slots_ActionTriggered()));
}

MdiSubWindowX::~MdiSubWindowX()
{

}

void MdiSubWindowX::Slots_ActionTriggered()
{
	QMdiArea *pMdiArea = dynamic_cast<QMdiArea*>(this->parentWidget()->parentWidget());//(QMdiArea*)this->property("MdiArea").toLongLong();;
	if (pMdiArea)
	{
		//////////////////////////////////////////////////////////////////////////

		QWidget* currentW = dynamic_cast<QWidget*>(this->widget());
		if (currentW)
		{
			this->setWidget(0);
			this->layout()->removeWidget(currentW);

			QWidget* pW = pMdiArea->parentWidget();
			currentW->setParent(QApplication::desktop());
			currentW->setWindowFlags(Qt::Window);
			currentW->showNormal();
		}

		foreach(QMdiSubWindow* subW , pMdiArea->subWindowList())
		{
			bool b = subW->isVisible();
			if (b)
			{
				subW->showNormal();
			}
			else
			{
				subW->hide();
			}
		}
	}

	this->hide();
}

//////////////////////////////////////////////////////////////////////////
