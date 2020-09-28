#include "AdapterWidget.h"
#include "LegendWidget.h"

#define IDM_INMDI_Q 0x0011

LegendWidget::LegendWidget(int flag , QWidget *parent): QTabWidget(parent)
{
	setTabPosition(South);
	setProperty("DType",flag);
}


void LegendWidget::AddMenuItem()
{
	QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
	if (subW)
	{
		HMENU hMenu = ::GetSystemMenu(winId(), FALSE);
		if (hMenu != NULL)
		{
			::AppendMenuA(hMenu, MF_SEPARATOR, 0, 0);
			::AppendMenuA(hMenu, MF_STRING, IDM_INMDI_Q, "窗口锁定");
		}
	}
}

void LegendWidget::setVisible( bool visible )
{
	if (visible)
	{
		AddMenuItem();
	}
	QTabWidget::setVisible(visible);
}

bool LegendWidget::winEvent(MSG *m, long *result)
{
	if (m->message == WM_SYSCOMMAND)
	{
		if ((m->wParam) == IDM_INMDI_Q)
		{
			*result = 0;
			QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
			if (subW)
			{
				this->setWindowFlags(Qt::Widget);
				subW->setWidget(this);
				subW->showMaximized();
			}
			return (true);
		}
	}
	return (false);
}


LegendWidget::~LegendWidget()
{

}


void LegendWidget::closeEvent( QCloseEvent *event )
{
	int ret = QMessageBox::information(0,tr("提示"),tr("正在运行！\n是否确认关闭当前视口？"),QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == 1024)
	{
		QMdiSubWindow* pw = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
		if(pw)
		{
			pw->close();
		}
	}
	else
	{
		QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
		if (subW)
		{
			this->setWindowFlags(Qt::Widget);
			subW->setWidget(this);
			subW->showMaximized();
		}

		event->ignore();
	}
	//QTabWidget::closeEvent(event);
}

void LegendWidget::mouseReleaseEvent( QMouseEvent* event )
{
		emit SetCurrentWidget(this);
}
