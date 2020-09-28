#include "GridSetting.h"
#include <qvalidator.h>
#include "mainwindow.h"
#include "sysmainwindow.h"
#include "AdapterWidget.h"
#include <QMessageBox>


GridSetting::GridSetting( QWidget *parent /*= 0*/) 
	: QDialog(parent),m_GSActionGroup(0),latstart(55),latend(0),lonstart(75),lonend(145),latreso(5),lonreso(5)
{
	ui.setupUi(this);
	_mw = SysMainWindow::GetMainWindow();
	foreach(QAction* action,this->actions())
	{
		m_GSActionGroup.addAction(action);
	}
	InitDialog();
	InitSlots();
}


GridSetting::~GridSetting(void)
{
}

void GridSetting::InitSlots()
{
	connect(ui.pushButton_addGrid, SIGNAL(clicked()),this,  SLOT(Slots_OnOKCliked()));
	connect(ui.pushButton_quit,SIGNAL(clicked()),this,  SLOT(Slots_OnQuitCliked()));

}

void GridSetting::InitDialog()
{
	QValidator *validatorlat = new QIntValidator(-90,90);
	ui.lineEditUP->setText("55.0");
	ui.lineEditUP->setValidator(validatorlat);
	ui.lineEditDown->setText("0.0");
	ui.lineEditDown->setValidator(validatorlat);
	QValidator *validatorlon = new QIntValidator(-180,180);

	ui.lineEditLeft->setText("70.0");
	ui.lineEditRight->setText("145.0");
	ui.lineEditLeft->setValidator(validatorlon);
	ui.lineEditRight->setValidator(validatorlon);

	ui.lineEditLatResolution->setText("5.0");
	ui.lineEditLonResolution->setText("5.0");

}

void GridSetting::Slots_OnOKCliked()
{
	if (ValidityCheck())
	{
		goto_gis::Map* pMap = _mw->GetCurrentMap();
		if (!pMap)
		{
			return;
		}
		if (0!= pMap->GetLayer("地理网格"))
		{
			pMap->RemoveLayer("地理网格");
		}
		pMap->AddFishNet(lonstart,latstart,lonend,latend,latreso,lonreso,.5);

		_mw->AddLeftDockW(_mw->FromWidgetToQGLW());
	}
	this->close();
	return;
}

bool GridSetting::ValidityCheck()
{
	latstart = ui.lineEditUP->text().toFloat();
	latend = ui.lineEditDown->text().toFloat();
	lonstart = ui.lineEditLeft->text().toFloat();
	lonend = ui.lineEditRight->text().toFloat();
	latreso = ui.lineEditLatResolution->text().toFloat();
	lonreso = ui.lineEditLonResolution->text().toFloat();

	if (latstart <=latend ||lonstart >=lonend ||latreso <=0 ||lonreso <=0 )
	{
		QMessageBox::information(0,QObject::tr("提示"),QObject::tr("您输入的范围参数或经纬网格分辨率有误，请重新输入"));
		return false;
	}
	return true;
}

void GridSetting::Slots_OnQuitCliked()
{
	this->close();
}

ProjCenterSetting::ProjCenterSetting( QWidget *parent /*= 0*/ ): QDialog(parent),m_lat(0.0),m_lon(0.0),m_PCActionGroup(0)
{
	ui.setupUi(this);
	_mw = SysMainWindow::GetMainWindow();
	foreach(QAction* action,this->actions())
	{
		m_PCActionGroup.addAction(action);
	}
	InitDialog();
	InitSlots();
}

ProjCenterSetting::~ProjCenterSetting( void )
{

}

void ProjCenterSetting::InitSlots()
{
	connect(ui.pushButton_OK, SIGNAL(clicked()),this,  SLOT(Slots_OnOKCliked()));

}

void ProjCenterSetting::InitDialog()
{
	QValidator *validatorlat = new QIntValidator(-90,90);
	ui.lineEdit_Lat->setText("0.0");
	ui.lineEdit_Lat->setValidator(validatorlat);
	QValidator *validatorlon = new QIntValidator(-180,180);
	ui.lineEdit_Lon->setText("0.0");
	ui.lineEdit_Lon->setValidator(validatorlon);

}

bool ProjCenterSetting::ValidityCheck()
{
	m_lat = ui.lineEdit_Lat->text().toFloat();
	m_lon = ui.lineEdit_Lon->text().toFloat();
	return true;
}

void ProjCenterSetting::Slots_OnOKCliked()
{
	if (ValidityCheck())
	{
		goto_gis::Map* pMap = _mw->GetCurrentMap();
		if (pMap == NULL)
		{
			return;
		}

		pMap->GetCoordinateTransform()->setCenter(m_lon,m_lat);
		pMap->TramsformMap();

		ViewerQT* viewerQT = dynamic_cast<ViewerQT*>(_mw->FromWidgetToQGLW());
		if (viewerQT)
		{
			DefaultManipulator* dm = dynamic_cast<DefaultManipulator*>(viewerQT->getViewer()->getCameraManipulator()) ;
			if (dm)
			{
				dm->MapALLMap();
			}
		}
		

	}
	this->close();
}
