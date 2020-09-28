#include "mainwindow.h"
#include "sysmainwindow.h"
#include "MdiSubWindowX.h"
#include "CentralMdiArea.h"

CentralMdiArea::CentralMdiArea(QWidget *parent)	: QMdiArea(parent)
{
	ui.setupUi(this);
	m_pMW = SysMainWindow::GetMainWindow();
	InitSlots();
}

CentralMdiArea::~CentralMdiArea()
{

}

void CentralMdiArea::InitSlots()
{
	connect(this,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(Slot_SubWindowActivated(QMdiSubWindow*)));
}

/** 
  * @brief    客户区槽   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   CentralMdiArea  客户区槽
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void CentralMdiArea::Slot_SubWindowActivated( QMdiSubWindow * window )
{
	static QWidget* oldW = 0;
	if (window)
	{
		QWidget* activateW = dynamic_cast<QWidget*>(window->widget());
		if (activateW)
		{
			QWidget* viewer = 0;
			QTabWidget* tabW	= dynamic_cast<QTabWidget*>(activateW);
			if (tabW)
			{
				viewer = tabW->widget(0);
			}
			else
			{
				viewer = activateW;
			} 
			//if (bw && bw != oldW)
			/*{
				int wdt = viewer->property("DType").toInt();
				m_pMW->AddIDPToolBar(wdt);
				m_pMW->AddLeftDockW(viewer);
				oldW = viewer;
			}*/
		}
		
	}
	/*if (this->subWindowList().count() <=0)
	{
		m_pMW->ShowLeftDockWidget(false);
		m_pMW->ShowAttributeDockWidget(false);
		m_pMW->AddIDPToolBar(0);
		m_pMW->RadarControlDockWidgetGroupEnd(false);
	}
	else
	{

	}*/
}

QMdiSubWindow* CentralMdiArea::CreateSubWindow( QWidget* widget , Qt::WindowFlags flags /*= 0*/ )
{
	return addSubWindow(widget,flags);
}

QMdiSubWindow * CentralMdiArea::addSubWindow( QWidget *widget, Qt::WindowFlags flags /*= 0*/ )
{
	MdiSubWindowX *subWindow1 = new MdiSubWindowX;
	widget->setAttribute(Qt::WA_DeleteOnClose);
	widget->setProperty("ParentWidget",(long long)subWindow1);
	connect(widget, SIGNAL(SetCurrentWidget(QWidget*)),this,  SLOT(Slot_ActivateWindow(QWidget*)));
	subWindow1->setWidget(widget);
	subWindow1->setProperty("Widget",(long long)widget);
	subWindow1->setProperty("MdiArea",(long long)this);
	subWindow1->setAttribute(Qt::WA_DeleteOnClose);
	CurrentWidget(widget);
	return QMdiArea::addSubWindow(subWindow1,flags);
}

void CentralMdiArea::Slot_ActivateWindow( QWidget* currentWidget )
{
	_currentWidget = currentWidget;
}

